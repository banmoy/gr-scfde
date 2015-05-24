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
#include "k_offset_xx_impl.h"

namespace gr {
  namespace scfde {

    k_offset_xx::sptr
    k_offset_xx::make(size_t size,int offset)
    {
      return gnuradio::get_initial_sptr
        (new k_offset_xx_impl(size,offset));
    }

    /*
     * The private constructor
     */
    k_offset_xx_impl::k_offset_xx_impl(size_t size,int offset)
      : gr::block("k_offset_xx",
              gr::io_signature::make(1, 1, size),
              gr::io_signature::make(1, 1, size)),
    d_size(size),
    d_offset(offset),
    d_count(0)
    {}

    /*
     * Our virtual destructor.
     */
    k_offset_xx_impl::~k_offset_xx_impl()
    {
    }

    void
    k_offset_xx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }

    int
    k_offset_xx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const char *in = (const char *) input_items[0];
        char *out = (char *) output_items[0];

        // Do <+signal processing+>
	int min_items = std::min(ninput_items[0],noutput_items);
	int no=0;
	int ni=0;
	if(d_count<d_offset){
	    d_count++;
	    ni++;
	}else{
	    memcpy(out,in,min_items*d_size);
	    ni += min_items;
	    no += min_items;
	}
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (ni);

        // Tell runtime system how many output items we produced.
        return no;
    }

  } /* namespace scfde */
} /* namespace gr */

