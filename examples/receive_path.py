#!/usr/bin/env python

from gnuradio import blocks,gr,filter,analog,digital
import scfde,packet
from timing_sync import timing_sync
import math

class receive_path(gr.hier_block2):
    def __init__(self,samples_per_symbol,demod_type,rx_callback,SNR,equal="HLS_LS"):
	
	gr.hier_block2.__init__(self, "receive_path",
				gr.io_signature(1,1,gr.sizeof_gr_complex),
				gr.io_signature(0,0,0))
	
	self.samples_per_symbol = samples_per_symbol
	self.demod_type = demod_type
	if(self.demod_type == "bpsk"):
	    self.bits = 1
	else:
	    self.bits = 2

	self.block_size = 128
	self.cp_size = 32
	self.db_num = 4 
	self.eb_num = 1

	sw_decim = 1
	self.chbw_factor = 1.0
	
	self.excess_bw = 0.35
	self.timing_bw = 2*math.pi/100
	self.timing_max_dev = 1.5
	
	self.thresh = 0.5
	self.k = 50
	
	self.equal_method = equal # MMSE or LS
	
        chan_coeffs = filter.firdes.low_pass(1.0,                  # gain
                                             sw_decim * self.samples_per_symbol,  #sampling rate
                                             self.chbw_factor,    # midpoint of trans. band
                                             0.5,                  # width of trans. band
                                             filter.firdes.WIN_HANN)   # filter type
	self.channel_filter = filter.fft_filter_ccc(sw_decim, chan_coeffs)
	self.agc = analog.agc2_cc(0.6e-1, 1e-3, 1, 1)
	
	#pulse shaping
	nfilts = 32
	ntaps = nfilts*11*int(self.samples_per_symbol)
	taps = filter.firdes.root_raised_cosine(nfilts, nfilts*self.samples_per_symbol,
                                                1.0, self.excess_bw, ntaps)
	
	self.time_recov = digital.pfb_clock_sync_ccf(self.samples_per_symbol,self.timing_bw, taps,
							nfilts, nfilts//2, self.timing_max_dev)
	self.time_sync = timing_sync(self.block_size,self.cp_size,self.thresh,self.k)
	#### for perfect timing synchromization
	#self.time_sync = scfde.simulation_perfect_timing_cb(self.block_size,self.cp_size,6)
	
	self.block_acq = scfde.block_acquization_cbcb(block_size=self.block_size,cp_size=self.cp_size,
							   eb_num=self.eb_num,db_num=self.db_num)
	self.freq_sync = scfde.frequency_sync_cbcb(block_size=self.block_size,cp_size=self.cp_size)
	self.fde = scfde.frequency_domain_equalize_cbcc(block_size=self.block_size,
							    cp_size=self.cp_size,
							    eb_num=self.eb_num,
							    equal_method=self.equal_method,
							    SNR=SNR)
	self.demod = scfde.demod_mapper_cbb(demod_type=self.demod_type,data_size=self.block_size)
	self.pack = scfde.pack_k_bits_bbbb(self.bits)
	self.rcvd_pktq = gr.msg_queue()
	self.sink = scfde.message_acquization_bb(self.rcvd_pktq,frame_size=self.block_size/8*self.bits)
	self.watcher = queue_watcher_thread(self.rcvd_pktq,rx_callback)
	
	self.connect(self,self.channel_filter,self.agc,self.time_recov)
	self.connect(self.time_recov,self.time_sync)
	
	self.connect(self.time_recov,(self.block_acq,0))
	
	#self.connect(self,self.time_sync)
	#self.connect(self,(self.block_acq,0))
	
	self.connect(self.time_sync,(self.block_acq,1))
	
	self.connect((self.block_acq,0),(self.freq_sync,0))
	self.connect((self.block_acq,1),(self.freq_sync,1))
	
	self.connect((self.freq_sync,0),(self.fde,0))
	self.connect((self.freq_sync,1),(self.fde,1))
		
	self.connect((self.fde,0),self.demod)
	
	self.connect((self.demod,0),(self.pack,0))
	self.connect((self.demod,1),(self.pack,1))
	
	self.connect((self.pack,0),(self.sink,0))
	self.connect((self.pack,1),(self.sink,1))
	
	self.write_data()

    def write_data(self):
	self.connect(self.time_recov,blocks.file_sink(gr.sizeof_gr_complex,"Data/time_recov_dat.32fc"))
	self.connect((self.block_acq,0),blocks.file_sink(self.block_size*gr.sizeof_gr_complex,"Data/acq_dat.32fc"))
	self.connect((self.block_acq,1),blocks.file_sink(gr.sizeof_char,"Data/acq_flag_dat.8b"))
	self.connect((self.freq_sync,0),blocks.file_sink(self.block_size*gr.sizeof_gr_complex,"Data/freq_dat.32fc"))
	self.connect((self.freq_sync,1),blocks.file_sink(gr.sizeof_char,"Data/freq_flag_dat.8b"))
	self.connect((self.freq_sync,2),blocks.file_sink(gr.sizeof_short,"Data/ifo_dat.16s"))
	self.connect((self.fde,0),blocks.file_sink(self.block_size*gr.sizeof_gr_complex,"Data/equalized_dat.32fc"))
	self.connect((self.fde,1),blocks.file_sink(self.block_size*gr.sizeof_gr_complex,"Data/taps_dat.32fc"))

class queue_watcher_thread(gr.gr_threading.Thread):
    def __init__(self, rcvd_pktq, callback):
	gr.gr_threading.Thread.__init__(self)
	
	self.setDaemon(1)
	self.rcvd_pktq = rcvd_pktq
	self.callback = callback
	self.keep_running = True
	self.start()

    def run(self):
	while self.keep_running:
	    msg = self.rcvd_pktq.delete_head()
	    pkt = msg.to_string()
	    ok,payload = packet.unmake_packet(pkt)
	    if self.callback:
		self.callback(ok,payload)
