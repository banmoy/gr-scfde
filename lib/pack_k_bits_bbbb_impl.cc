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
#include "pack_k_bits_bbbb_impl.h"

namespace gr {
  namespace scfde {

   #define BYTE 8

    pack_k_bits_bbbb::sptr
    pack_k_bits_bbbb::make(unsigned k)
    {
      return gnuradio::get_initial_sptr
        (new pack_k_bits_bbbb_impl(k));
    }

    /*
     * The private constructor
     */
    pack_k_bits_bbbb_impl::pack_k_bits_bbbb_impl(unsigned k)
      : gr::sync_decimator("pack_k_bits_bbbb",
              gr::io_signature::make2(1, 2, sizeof(char),sizeof(char)),
              gr::io_signature::make2(1, 2, sizeof(char),sizeof(char)), BYTE/k),
	d_k(k),
	d_decimator(BYTE/k),
	d_flag_num(0)
    {}

    /*
     * Our virtual destructor.
     */
    pack_k_bits_bbbb_impl::~pack_k_bits_bbbb_impl()
    {
    }

    int
    pack_k_bits_bbbb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
	const unsigned char *in_flag = (const unsigned char*) input_items[1];
	
	unsigned char *out = (unsigned char *) output_items[0];
	unsigned char *out_flag = (unsigned char *) output_items[1];

        // Do <+signal processing+>
	int no=0;
	int ni=0;
	while(no<noutput_items){
	    unsigned char byte = 0;
	    unsigned char flag = 0;
	    if(in_flag[ni]&0x01){
		flag = 1;
		d_flag_num++;
	    }
	    for(int i=0;i<d_decimator;i++){
		byte <<= d_k;
		byte = byte ^ in[ni];
		ni++;
	    }
	    out[no] = byte;
	    out_flag[no] = flag;
	    no++;
	}
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace scfde */
} /* namespace gr */

