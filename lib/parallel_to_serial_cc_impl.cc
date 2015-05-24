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
#include "parallel_to_serial_cc_impl.h"

namespace gr {
  namespace scfde {

     parallel_to_serial_cc::sptr
    parallel_to_serial_cc::make(int ratio)
    {
      return gnuradio::get_initial_sptr
        (new parallel_to_serial_cc_impl(ratio));
    }

    /*
     * The private constructor
     */
    parallel_to_serial_cc_impl::parallel_to_serial_cc_impl(int ratio)
      : gr::sync_interpolator("parallel_to_serial_cc",
              gr::io_signature::make(1, 1, ratio*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), ratio),
      d_ratio(ratio)
    {}

    /*
     * Our virtual destructor.
     */
    parallel_to_serial_cc_impl::~parallel_to_serial_cc_impl()
    {
    }

    int
    parallel_to_serial_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
	memcpy((char*)out,(char*)in,noutput_items*sizeof(gr_complex));
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace scfde */
} /* namespace gr */

