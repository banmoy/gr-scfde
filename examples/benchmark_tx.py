#!/usr/bin/env python

from gnuradio import blocks,gr
from transmit_path import transmit_path
from uhd_interface import uhd_transmitter
import struct,random,time,sys

class my_block(gr.top_block):
    def __init__(self,byte_size,bit_rate=100000):
	gr.top_block.__init__(self)
	
	self.mod_type = "bpsk"	# bpsk or qpsk
	if(self.mod_type == "bpsk"):
		self.bits = 1
	else:
		self.bits = 2
	self.samples_per_symbol = 4 
	self.bit_rate = bit_rate
	self.symbol_rate = self.bit_rate / self.bits
	self.freq = 1000000000.0	
	
	self.trans = transmit_path(byte_size,self.mod_type,self.samples_per_symbol)
	self.sink = uhd_transmitter(sym_rate=self.symbol_rate,
				    sps=self.samples_per_symbol,
				    freq=self.freq,
				    verbose=True,
				    gain=19.5)
	
	self.connect(self.trans,self.sink)

    def send_pkt(self,payload):
	self.trans.send_pkt(payload)
	
def main(bit_rate=100000):
    
    byte_size = 7	# 128 : bpsk 7 qpsk 23 256 : bpsk 23 qpsk 55 512 : bpsk 53 qpsk 119
    
    blk = my_block(byte_size,bit_rate)
    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: failed to enable realtime scheduling"
    blk.start()
    
    pkt_no = 0
    charset = "abcdefghijklmnopqrstyvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    k = 0
    is_sleep = False
 
    while(1):
	data = ''
	i = 0
	while(i<byte_size):
	    tmp = random.randint(0,61)
	    data += charset[tmp]
	    i += 1
	payload = struct.pack('!HH',pkt_no,byte_size)
	payload += data
	blk.send_pkt(payload)
	sys.stderr.write('.')
	k += 1
	if(k == 4):
	    k = 0
	    if(is_sleep):
		time.sleep(1)
	pkt_no += 1
	if(pkt_no >= 65535):
	    pkt_no = 0
        
    blk.wait()
    
if __name__ == '__main__':
    try:
	if(len(sys.argv)>1):
		main(float(sys.argv[1]))
	else:
		main()
    except KeyboardInterrupt:
	pass
