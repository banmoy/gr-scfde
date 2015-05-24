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
#include "block_source_c_impl.h"
#include <stdlib.h>
#include <time.h>

namespace gr {
  namespace scfde {

    block_source_c::sptr
    block_source_c::make(int block_size,int blk_num)
    {
      return gnuradio::get_initial_sptr
        (new block_source_c_impl(block_size,blk_num));
    }

    /*
     * The private constructor
     */
    block_source_c_impl::block_source_c_impl(int block_size,int blk_num)
      : gr::sync_block("block_source_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, block_size*sizeof(gr_complex))),
        d_block_size(block_size),
        d_block_num(blk_num),
        d_block_count(0)
    {
	d_constellation_point = new gr_complex[2];
	d_block_data = new gr_complex[d_block_size];
	
	d_constellation_point[0] = gr_complex(-1,0);
	d_constellation_point[1] = gr_complex(1,0);
    }

    /*
     * Our virtual destructor.
     */
    block_source_c_impl::~block_source_c_impl()
    {
	delete d_constellation_point;
	delete d_block_data;
    }
    
    void block_source_c_impl::gen_data()
    {
	srand((int)time(NULL));
	for(int i=0;i<d_block_size;i++){
	    int ran = rand()%2;
	    d_block_data[i] = d_constellation_point[ran];
	}
    }

    int
    block_source_c_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        gr_complex *out = (gr_complex *) output_items[0];

	if(d_block_count >= d_block_num)
	    return -1;
        // Do <+signal processing+>
	int no=0;
	while(no<noutput_items){
	    gen_data();
	    memcpy((char*)&out[no*d_block_size], (char*)d_block_data, d_block_size*sizeof(gr_complex));
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

