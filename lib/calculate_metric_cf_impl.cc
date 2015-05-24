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
#include "calculate_metric_cf_impl.h"

namespace gr {
  namespace scfde {

   calculate_metric_cf::sptr
    calculate_metric_cf::make(int block_size)
    {
      return gnuradio::get_initial_sptr
        (new calculate_metric_cf_impl(block_size));
    }

    /*
     * The private constructor
     */
    calculate_metric_cf_impl::calculate_metric_cf_impl(int block_size)
      : gr::block("calculate_metric_cf",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(float))),
      d_block_size(block_size),
      d_count(0),
      d_p(gr_complex(0,0)),
      d_r(0),
      d_first(true)
    {
	d_temp_seq.resize(0);
    }

    /*
     * Our virtual destructor.
     */
    calculate_metric_cf_impl::~calculate_metric_cf_impl()
    {
    }

    void
    calculate_metric_cf_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }
    
    float calculate_metric_cf_impl::cal_first_metric()
    {
	for(int i=0;i<d_block_size/2;i++){
	    d_p += conj(d_temp_seq[i])*d_temp_seq[i+d_block_size/2];
	    d_r += norm(d_temp_seq[i+d_block_size/2]);
	}
	
	return norm(d_p)/d_r/d_r;
    }
    
    float calculate_metric_cf_impl::cal_metric(gr_complex data)
    {
	d_p += conj(d_temp_seq[d_block_size/2])*data - conj(d_temp_seq[0])*d_temp_seq[d_block_size/2];
	d_r += norm(data) - norm(d_temp_seq[d_block_size/2]);
	
	return norm(d_p)/d_r/d_r;
    }

    int
    calculate_metric_cf_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        float *out = (float *) output_items[0];

        // Do <+signal processing+>
	int min_items = std::min(ninput_items[0],noutput_items);
	int ni=0;
	int no=0; 
	if(d_count<d_block_size){
	    while(ni<min_items){
		d_temp_seq.push_back(in[ni]);
		ni++;
		d_count++;
		if(d_count == d_block_size){
		    out[no] = cal_first_metric();
		    no++;
		    break;
		}
	    }
	}else{
	    while(no<min_items){
		out[no] = cal_metric(in[ni]);
		d_temp_seq.erase(d_temp_seq.begin());
		d_temp_seq.push_back(in[ni]);
		ni++;
		no++;
	    }
	}
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (ni);

        // Tell runtime system how many output items we produced.
        return no;
    }
  } /* namespace scfde */
} /* namespace gr */

