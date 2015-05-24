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
#include "simulation_perfect_timing_cb_impl.h"

namespace gr {
  namespace scfde {

   simulation_perfect_timing_cb::sptr
    simulation_perfect_timing_cb::make(int block_size,int cp_size,int num)
    {
      return gnuradio::get_initial_sptr
        (new simulation_perfect_timing_cb_impl(block_size,cp_size,num));
    }

    /*
     * The private constructor
     */
    simulation_perfect_timing_cb_impl::simulation_perfect_timing_cb_impl(int block_size,int cp_size,int num)
      : gr::sync_block("simulation_perfect_timing_cb",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(char))),
       d_block_size(block_size),
       d_cp_size(cp_size),
       d_num(num),
       d_count(0),
       d_first(true)
    {
	d_total_size = (block_size+cp_size)*num;
    }

    /*
     * Our virtual destructor.
     */
    simulation_perfect_timing_cb_impl::~simulation_perfect_timing_cb_impl()
    {
    }

    int
    simulation_perfect_timing_cb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        char *out = (char *) output_items[0];

        // Do <+signal processing+>
	int no=0;
	while(no<noutput_items){
	    if(d_first){
		if(d_count == d_cp_size)
		    out[no] = 1;
		else
		    out[no] = 0;
		d_count++;
		if(d_count-1 == d_cp_size){
		    d_count = 0;
		    d_first = false;
		}	    
	    }else{
		if(d_count == d_total_size-1)
		    out[no] = 1;
		else
		    out[no] = 0;
		d_count++;
		if(d_count == d_total_size){
		    d_count = 0;
		}	
	    }
	    no++;
	}
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace scfde */
} /* namespace gr */

