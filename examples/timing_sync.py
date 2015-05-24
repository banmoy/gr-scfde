#!/usr/bin/env python

from gnuradio import blocks,gr,filter,analog,digital
import scfde,packet
import math

class timing_sync(gr.hier_block2):
    def __init__(self,block_size,cp_size,thresh,k):
	
	gr.hier_block2.__init__(self, "timing_sync",
				gr.io_signature(1,1,gr.sizeof_gr_complex),
				gr.io_signature(1,1,gr.sizeof_char))
	
	self.block_size = block_size
	self.cp_size = cp_size
	
	self.thresh = thresh
	self.k = k
	
	self.cal_metric = scfde.calculate_metric_cf(block_size=self.block_size)
	
	matched_filter_taps = [1.0/(self.cp_size) for i in range(self.cp_size)]
        self.matched_filter = filter.fir_filter_fff(1,matched_filter_taps)
        
        self.detect_peak = scfde.detect_peak_fb(thresh=self.thresh,k=self.k)
        self.peak_offset = scfde.k_offset_xx(gr.sizeof_char,10)
        
        self.connect(self,self.cal_metric,self.matched_filter)
        self.connect(self.matched_filter,self.detect_peak,self.peak_offset,self)
    
	self.write_data()

    def write_data(self):
	self.connect(self.cal_metric,blocks.file_sink(gr.sizeof_float,"Data/metric_dat.32f"))
	self.connect(self.matched_filter,blocks.file_sink(gr.sizeof_float,"Data/mf_dat.32f"))
	self.connect(self.detect_peak,blocks.file_sink(gr.sizeof_char,"Data/peak_dat.8b"))
	self.connect(self.peak_offset,blocks.file_sink(gr.sizeof_char,"Data/offset_dat.8b"))
