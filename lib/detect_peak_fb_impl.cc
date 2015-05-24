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
#include "detect_peak_fb_impl.h"

namespace gr {
  namespace scfde {

   detect_peak_fb::sptr
    detect_peak_fb::make(float thresh,int k)
    {
      return gnuradio::get_initial_sptr
        (new detect_peak_fb_impl(thresh,k));
    }

    /*
     * The private constructor
     */
    detect_peak_fb_impl::detect_peak_fb_impl(float thresh,int k)
      : gr::block("detect_peak_fb",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make2(1, 2, sizeof(char),sizeof(float))),
       d_thresh(thresh),
       d_k(k),
       d_count(0)
    {
	d_tmp_metric.resize(2*d_k+1);
	std::fill(d_tmp_metric.begin(),d_tmp_metric.end(),0);
    }

    /*
     * Our virtual destructor.
     */
    detect_peak_fb_impl::~detect_peak_fb_impl()
    {
    }

    void
    detect_peak_fb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }
    
    int detect_peak_fb_impl::compare()
    {
	int flag = 0;
	if(d_tmp_metric[d_k]>d_thresh){
	    int i;
	    for(i=0;i<d_k*2+1;i++)
		if(d_tmp_metric[i]>d_tmp_metric[d_k])
		    break;
	    if(i == d_k*2+1)
		flag = 1;
	}
	
	return flag;
    }

    int
    detect_peak_fb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];
	
	float *out_f = 0;
	if(output_items.size() == 2)
	    out_f = (float *) output_items[1];

        // Do <+signal processing+>
	int min_items = std::min(ninput_items[0],noutput_items);
	int ni=0;
	int no=0;
	if(d_count<d_k+1){
	    while(ni<min_items){
		d_tmp_metric.push_back(in[ni]);
		d_tmp_metric.erase(d_tmp_metric.begin());
		ni++;
		d_count++;
		if(d_count == d_k+1){
		    out[no] = compare();
		    if(output_items.size() == 2)
			out_f[no] = d_tmp_metric[d_k];
		    no++;
		    break;
		}
	    }
	}else{
	    while(no<min_items){
		d_tmp_metric.erase(d_tmp_metric.begin());
		d_tmp_metric.push_back(in[ni]);
		out[no] = compare();
		if(output_items.size() == 2)
			out_f[no] = d_tmp_metric[d_k];
		no++;
		ni++;
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

