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
#include "insert_cp_cc_impl.h"

namespace gr {
  namespace scfde {
 insert_cp_cc::sptr
    insert_cp_cc::make(int block_size,int cp_size)
    {
      return gnuradio::get_initial_sptr
        (new insert_cp_cc_impl(block_size,cp_size));
    }

    /*
     * The private constructor
     */
    insert_cp_cc_impl::insert_cp_cc_impl(int block_size,int cp_size)
      : gr::sync_block("insert_cp_cc",
              gr::io_signature::make(1, 1, block_size*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, (block_size+cp_size)*sizeof(gr_complex))),
       d_block_size(block_size),
       d_cp_size(cp_size),
       d_total_size(block_size+cp_size)
    { }

    /*
     * Our virtual destructor.
     */
    insert_cp_cc_impl::~insert_cp_cc_impl()
    {
    }

    int
    insert_cp_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
	int no=0;
	while(no<noutput_items){
	    memcpy((char*)&out[no*d_total_size],(char*)&in[(no+1)*d_block_size-d_cp_size],
						    d_cp_size*sizeof(gr_complex));
	    memcpy((char*)&out[no*d_total_size+d_cp_size],(char*)&in[no*d_block_size],
						    d_block_size*sizeof(gr_complex));
	    
	    no++;
	}
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }
  } /* namespace scfde */
} /* namespace gr */

