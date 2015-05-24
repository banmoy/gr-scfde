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
#include "simulation_equalize_cc_impl.h"

namespace gr {
  namespace scfde {

    simulation_equalize_cc::sptr
    simulation_equalize_cc::make(int block_size,int cp_size,std::string equal_method,
				 std::vector<gr_complex> taps,float snr)
    {
      return gnuradio::get_initial_sptr
        (new simulation_equalize_cc_impl(block_size,cp_size,equal_method,taps,snr));
    }

    /*
     * The private constructor
     */
    simulation_equalize_cc_impl::simulation_equalize_cc_impl(int block_size,int cp_size,
			std::string equal_method,std::vector<gr_complex> taps,float snr)
      : gr::sync_block("simulation_equalize_cc",
              gr::io_signature::make(1, 1, (block_size+cp_size)*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, block_size*sizeof(gr_complex))),
        d_block_size(block_size),
        d_cp_size(cp_size),
        d_snr(snr),
        d_equal_method(equal_method)
    {
	d_taps = new gr_complex[d_block_size];
	d_taps_fft = new gr_complex[d_block_size];
	
	d_fft = new spinal::fft::fft_complex(d_block_size,true);
	d_ifft = new spinal::fft::fft_complex(d_block_size,false);
	
	for(int i=0;i<d_block_size;i++)
	    d_taps[i] = gr_complex(0,0);
	for(int i=0;i<taps.size();i++)
	    d_taps[i] = taps[i];
	init_taps_fft();
	
	d_block_num = 100000;
	d_block_count = 0;
    }

    /*
     * Our virtual destructor.
     */
    simulation_equalize_cc_impl::~simulation_equalize_cc_impl()
    {
	delete d_taps;
	delete d_taps_fft;
	delete d_fft;
	delete d_ifft;
    }

    void simulation_equalize_cc_impl::init_taps_fft()
    {
	memcpy((char*)d_fft->get_inbuf(), (char*)d_taps, d_block_size*sizeof(gr_complex));
	d_fft->execute();
	memcpy((char*)d_taps_fft, (char*)d_fft->get_outbuf(), d_block_size*sizeof(gr_complex));
    }

    void simulation_equalize_cc_impl::equalize(gr_complex* data_fft)
    {
	if(d_equal_method == "LS")
	    ls_equalize(data_fft);
	if(d_equal_method == "MMSE")
	    mmse_equalize(data_fft);
	if(d_equal_method == "NO")
	    ;
    }

    void simulation_equalize_cc_impl::ls_equalize(gr_complex* data_fft)
    {
	for(int i=0;i<d_block_size;i++)
	    data_fft[i] /= d_taps_fft[i];
    }

    void simulation_equalize_cc_impl::mmse_equalize(gr_complex* data_fft)
    {
	for(int i=0;i<d_block_size;i++){
	    float abs_n = norm(d_taps_fft[i]);
	    data_fft[i] *= conj(d_taps_fft[i])/(abs_n+1/d_snr);
	}
    }

    int
    simulation_equalize_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

	if(d_block_count >= d_block_num)
	    return -1;
	
        // Do <+signal processing+>
	int no=0;
	while(no<noutput_items){
	    gr_complex tmp[d_block_size];
	    gr_complex tmp_fft[d_block_size];
	    memcpy((char*)tmp, (char*)&in[no*(d_block_size+d_cp_size)+d_cp_size],
						d_block_size*sizeof(gr_complex));
	    
	    memcpy((char*)d_fft->get_inbuf(), (char*)tmp, d_block_size*sizeof(gr_complex));
	    d_fft->execute();
	    memcpy((char*)tmp_fft, (char*)d_fft->get_outbuf(), d_block_size*sizeof(gr_complex));
	    
	    equalize(tmp_fft);
	    
	    memcpy((char*)d_ifft->get_inbuf(), (char*)tmp_fft, d_block_size*sizeof(gr_complex));
	    d_ifft->execute();
	    memcpy((char*)tmp, (char*)d_ifft->get_outbuf(), d_block_size*sizeof(gr_complex));
	    for(int i=0;i<d_block_size;i++)
		tmp[i] /= (float)d_block_size;
	    
	    memcpy((char*)&out[no*d_block_size], (char*)tmp, d_block_size*sizeof(gr_complex));
	    
	    no++;
	    d_block_count++;
	    if(d_block_count >= d_block_num)
		break;
	}
	
        // Tell runtime system how many output items we produced.
        return no;
    }

  } /* namespace scfde */
} /* namespace gr */

