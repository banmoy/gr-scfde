/* -*- c++ -*- */
/* 
 * Copyright 2015 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "awgn_cc_impl.h"
#include <time.h>

namespace gr {
  namespace scfde {

    awgn_cc::sptr
    awgn_cc::make(float snr,std::string method,int samples,float sig_pow)
    {
      return gnuradio::get_initial_sptr
        (new awgn_cc_impl(snr,method,samples,sig_pow));
    }

    /*
     * The private constructor
     */
    awgn_cc_impl::awgn_cc_impl(float snr,std::string method,int samples,float sig_pow)
      : gr::sync_block("awgn_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
        d_method(method),
        d_samples(samples),
        d_sig_pow(0),
        d_count(0)
    {
	d_snr = std::pow(10.0,snr/10);
	d_ampl = std::sqrt(sig_pow/d_snr); 
	
	srand((int)time(NULL));
	d_rng.reseed(-rand()%100000);
    }

    /*
     * Our virtual destructor.
     */
    awgn_cc_impl::~awgn_cc_impl()
    {
    }

    void awgn_cc_impl::update_ampl()
    {
	d_sig_pow /= d_samples;
	d_ampl = std::sqrt(d_sig_pow/d_snr);
    }

    int
    awgn_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
	int no=0;
	while(no<noutput_items){
	    out[no] = in[no]+d_ampl*d_rng.rayleigh_complex();
	    if(d_method == "measured"){
		d_sig_pow += norm(in[no]);
		d_count++;
		if(d_count == d_samples){
		    update_ampl();
		    d_count = 0;
		    d_sig_pow = 0;
		}
	    }
	    
	    no++;
	}
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace scfde */
} /* namespace gr */

