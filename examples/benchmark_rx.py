#!/usr/bin/env python

from gnuradio import blocks,gr
from uhd_interface import uhd_receiver
from receive_path import receive_path
import struct

class my_block(gr.top_block):
    def __init__(self,rx_callback):
	gr.top_block.__init__(self)
	
	self.mod_type = "bpsk"	# bpsk or qpsk
	if(self.mod_type == "bpsk"):
		self.bits = 1
	else:
		self.bits = 2
	self.samples_per_symbol = 4
	self.bit_rate = 100000
	self.symbol_rate = self.bit_rate / self.bits
	
	self.freq = 1000000000.0	
	
	self.source = uhd_receiver(sym_rate=self.symbol_rate,
				   sps=self.samples_per_symbol,
				   freq=self.freq, 
				   verbose=True)
	
	self.recv = receive_path(self.samples_per_symbol,self.mod_type,rx_callback,30.0,"HLS_LS")

	self.connect(self.source,self.recv)
	
	self.write_data()
	
    def write_data(self):
	self.connect(self.source,blocks.file_sink(gr.sizeof_gr_complex,"Data/rx_dat.32fc"))
	
	
global n_rcvd, n_right

def main():
    
    global n_rcvd, n_right
    
    n_rcvd = 0
    n_right = 0
    
    def rx_callback(ok,payload):
	global n_rcvd, n_right
	n_rcvd += 1
	(pkt_no, data_len) = struct.unpack('!HH',payload[0:4])
	data = payload[4:]
	if(ok):
	    n_right += 1
	    print "received: ",n_rcvd,"  right: ",n_right,"  pkt_no: ",pkt_no,"  ok: ",ok
    
    blk = my_block(rx_callback)
    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: failed to enable realtime scheduling"
    blk.start()         
    blk.wait()
    
if __name__ == '__main__':
    try:
	main()
    except KeyboardInterrupt:
	pass
