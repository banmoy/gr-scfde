#!/usr/bin/env python
#
# Copyright 2010,2011 Free Software Foundation, Inc.
# 
# This file is part of GNU Radio
# 
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, uhd
from gnuradio import eng_notation
from gnuradio.eng_option import eng_option
from optparse import OptionParser

import sys

class uhd_interface:
    def __init__(self, istx, sym_rate, sps, freq=None,gain=None):
        
        if(istx):
            self.u = uhd.usrp_sink(device_addr="",stream_args=uhd.stream_args('fc32'))
        else:
            self.u = uhd.usrp_source(device_addr="",stream_args=uhd.stream_args('fc32'))

        self._gain = self.set_gain(gain)
        self._freq = self.set_freq(freq) 
        self._rate, self._sps = self.set_sample_rate(sym_rate, sps)

    def set_sample_rate(self, sym_rate, req_sps):
        start_sps = req_sps
        while(True):
            asked_samp_rate = sym_rate * req_sps
            self.u.set_samp_rate(asked_samp_rate)
            actual_samp_rate = self.u.get_samp_rate()

            sps = actual_samp_rate/sym_rate
            #FIXME
            #if(sps < 2):
                #req_sps +=1
            #else:
                #actual_sps = sps
                #break
	    actual_sps = sps
            break
        
        if(sps != req_sps):
            print "\nSymbol Rate:         %f" % (sym_rate)
            print "Requested sps:       %f" % (start_sps)
            print "Given sample rate:   %f" % (actual_samp_rate)
            print "Actual sps for rate: %f" % (actual_sps)

        if(actual_samp_rate != asked_samp_rate):
            print "\nRequested sample rate: %f" % (asked_samp_rate)
            print "Actual sample rate: %f" % (actual_samp_rate)

        return (actual_samp_rate, actual_sps)
    
    def set_gain(self, gain=None):
        if gain is None:
            # if no gain was specified, use the mid-point in dB
            g = self.u.get_gain_range()
            gain = float(g.start()+g.stop())/2
            print "\nNo gain specified."
            print "Setting gain to %f (from [%f, %f])" % \
                (gain, g.start(), g.stop())
        
        self.u.set_gain(gain, 0)
        return gain

    def set_freq(self, freq=None):
        if(freq is None):
            sys.stderr.write("You must specify -f FREQ or --freq FREQ\n")
            sys.exit(1)
        
        r = self.u.set_center_freq(uhd.tune_request(freq,0))
        if r:
            return freq
        else:
            frange = self.u.get_freq_range()
            sys.stderr.write(("\nRequested frequency (%f) out or range [%f, %f]\n") % \
                                 (freq, frange.start(), frange.stop()))
            sys.exit(1)

#-------------------------------------------------------------------#
#   TRANSMITTER
#-------------------------------------------------------------------#

class uhd_transmitter(uhd_interface, gr.hier_block2):
    def __init__(self, sym_rate, sps, freq=None,verbose=False, gain=None):
        gr.hier_block2.__init__(self, "uhd_transmitter",
                                gr.io_signature(1,1,gr.sizeof_gr_complex),
                                gr.io_signature(0,0,0))

        # Set up the UHD interface as a transmitter
        uhd_interface.__init__(self, True, sym_rate, sps, freq, gain)

        self.connect(self, self.u)

        if(verbose):
            self._print_verbage()
            
  
    def _print_verbage(self):
        """
        Prints information about the UHD transmitter
        """
        print "\nUHD Transmitter:"
        print "Freq:        %sHz"  % (eng_notation.num_to_str(self._freq))
        print "Gain:        %f dB" % (self._gain)
        print "Sample Rate: %ssps" % (eng_notation.num_to_str(self._rate))

#-------------------------------------------------------------------#
#   RECEIVER
#-------------------------------------------------------------------#


class uhd_receiver(uhd_interface, gr.hier_block2):
    def __init__(self, sym_rate, sps, freq=None, verbose=False, gain=None):
        gr.hier_block2.__init__(self, "uhd_receiver",
                                gr.io_signature(0,0,0),
                                gr.io_signature(1,1,gr.sizeof_gr_complex))
      
        # Set up the UHD interface as a receiver
        uhd_interface.__init__(self, False, sym_rate, sps, freq, gain)

        self.connect(self.u, self)

        if(verbose):
            self._print_verbage()

    def _print_verbage(self):
        """
        Prints information about the UHD transmitter
        """
        print "\nUHD Receiver:"
        print "Freq:         %sHz"  % (eng_notation.num_to_str(self._freq))
        print "Gain:         %f dB" % (self._gain)
        print "Sample Rate:  %ssps" % (eng_notation.num_to_str(self._rate))