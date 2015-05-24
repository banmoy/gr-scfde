#!/usr/bin/env python

from gnuradio import blocks,gr,filter
import scfde,packet

class transmit_path(gr.hier_block2):
    def __init__(self,byte_size,mod_type,samples_per_symbol,blk_num=-1):

	gr.hier_block2.__init__(self, "transmit_path",
				gr.io_signature(0,0,0),
				gr.io_signature(1,1,gr.sizeof_gr_complex))

	self.mod_type = mod_type	# bpsk or qpsk
	if(self.mod_type == "bpsk"):
		self.block_size = (byte_size+9)*8
	else:
		self.block_size = (byte_size+9)*4
	
	self.cp_size = 32
	
	self.db_num = 1
	self.eb_num = 1
	
	self.samples_per_symbol = samples_per_symbol
	self.excess_bw = 0.35
	
	self.amp = 2.0
	
	self.src = scfde.modulate_message_c(block_size=self.block_size,
						mod_type=self.mod_type,
						msgq_limit=4,
						block_num=blk_num)
	self.insert_esti = scfde.insert_esti_block_ccb( block_size=self.block_size,
							    db_num=self.db_num,
							    eb_num=self.eb_num)
	self.insert_sync = scfde.insert_sync_block_cbc(block_size=self.block_size)
	self.insert_cp = scfde.insert_cp_cc(block_size=self.block_size,
						cp_size=self.cp_size)
	self.p_to_s = scfde.parallel_to_serial_cc(ratio=self.block_size+self.cp_size)
	#pulse shaping
	nfilts = 32
	ntaps = nfilts*11*int(self.samples_per_symbol)
	self.rrc_taps = filter.firdes.root_raised_cosine(
			nfilts,nfilts,1.0,self.excess_bw,ntaps)
	self.rrc_filter = filter.pfb_arb_resampler_ccf(self.samples_per_symbol,
								self.rrc_taps)
	self.amplifier = blocks.multiply_const_cc(self.amp)
	
	self.connect(self.src,self.insert_esti)
	self.connect((self.insert_esti,0),(self.insert_sync,0))
	self.connect((self.insert_esti,1),(self.insert_sync,1))
	self.connect(self.insert_sync,self.insert_cp)
	self.connect(self.insert_cp,self.p_to_s,self.rrc_filter,self.amplifier,self)
		
#	self.write_data()

    def send_pkt(self,payload):
	pkt = packet.make_packet(payload)
	msg = gr.message_from_string(pkt)
	self.src.msgq().insert_tail(msg)
	
    def write_data(self):
	self.connect(self.src,blocks.file_sink(self.block_size*gr.sizeof_gr_complex,"Data/src_dat.32fc"))
	self.connect((self.insert_esti,0),blocks.file_sink(self.block_size*gr.sizeof_gr_complex,"Data/esti_dat.32fc"))
	self.connect(self.insert_sync,blocks.file_sink(self.block_size*gr.sizeof_gr_complex,"Data/sync_dat.32fc"))
	self.connect(self.insert_cp,blocks.file_sink((self.block_size+self.cp_size)*gr.sizeof_gr_complex,"Data/cp_dat.32fc"))
	self.connect(self.p_to_s,blocks.file_sink(gr.sizeof_gr_complex,"Data/ps_dat.32fc"))
	self.connect(self.rrc_filter,blocks.file_sink(gr.sizeof_gr_complex,"Data/rrc_dat.32fc"))
