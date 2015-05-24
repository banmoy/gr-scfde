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
#include "simulation_calculate_metric_cf_impl.h"

namespace gr {
  namespace scfde {

    simulation_calculate_metric_cf::sptr
    simulation_calculate_metric_cf::make(int block_size,std::string method)
    {
      return gnuradio::get_initial_sptr
        (new simulation_calculate_metric_cf_impl(block_size,method));
    }

    /*
     * The private constructor
     */
    simulation_calculate_metric_cf_impl::simulation_calculate_metric_cf_impl(int block_size,std::string method)
      : gr::block("simulation_calculate_metric_cf",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(float))),
        d_block_size(block_size),
        d_method(method),
        d_count(0),
        d_st(ST_FILL),
        d_p(gr_complex(0,0)),
        d_r(0),
        d_m(0)
    {
	d_tmp.resize(0);
	init_num();
    }

    /*
     * Our virtual destructor.
     */
    simulation_calculate_metric_cf_impl::~simulation_calculate_metric_cf_impl()
    {
    }

    void
    simulation_calculate_metric_cf_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }

    void simulation_calculate_metric_cf_impl::init_num()
    {
	if(d_method == "SC")
	    d_num = d_block_size;
	if(d_method == "Minn")
	    d_num = d_block_size;
	if(d_method == "Park")
	    d_num = d_block_size+1;
    }

    void simulation_calculate_metric_cf_impl::init_metric()
    {
	if(d_method == "SC")
	    init_sc_metric();
	if(d_method == "Minn")
	    init_minn_metric();
    }
    
    void simulation_calculate_metric_cf_impl::init_sc_metric()
    {
	int L = d_block_size/2;
	for(int i=0;i<L;i++){
	    d_p += conj(d_tmp[i])*d_tmp[i+L];
	    d_r += norm(d_tmp[i+L]);
	}
	d_m = norm(d_p)/(d_r*d_r);
    }
    
    void simulation_calculate_metric_cf_impl::init_minn_metric()
    {
	int L = d_block_size/4;
	for(int i=0;i<2;i++){
	    for(int j=0;j<L;j++){
		d_p += conj(d_tmp[i*2*L+j])*d_tmp[i*2*L+j+L];
		d_r += norm(d_tmp[i*2*L+j+L]);
	    }
	}
	d_m = norm(d_p)/(d_r*d_r);
    }

    void simulation_calculate_metric_cf_impl::cal_metric(gr_complex nw)
    {
	if(d_method == "SC")
	    cal_sc_metric(nw);
	if(d_method == "Minn")
	    cal_minn_metric(nw);
	if(d_method == "Park")
	    cal_park_metric(nw);
    }

    void simulation_calculate_metric_cf_impl::cal_sc_metric(gr_complex nw)
    {
	int L = d_block_size/2;
	d_p += conj(d_tmp[L])*nw - conj(d_tmp[0])*d_tmp[L];
	d_r += norm(nw) - norm(d_tmp[L]);
	d_m = norm(d_p)/(d_r*d_r);
	
	d_tmp.erase(d_tmp.begin());
	d_tmp.push_back(nw);
    }

    void simulation_calculate_metric_cf_impl::cal_minn_metric(gr_complex nw)
    {
	int L = d_block_size/4;
	d_p +=  conj(d_tmp[L])*d_tmp[2*L]
	       -conj(d_tmp[0])*d_tmp[L]
	       +conj(d_tmp[3*L])*nw
	       -conj(d_tmp[2*L])*d_tmp[3*L];
	d_r +=  norm(d_tmp[2*L])
	       -norm(d_tmp[L])
	       +norm(nw)
	       -norm(d_tmp[3*L]);
	d_m = norm(d_p)/(d_r*d_r);       
	
	d_tmp.erase(d_tmp.begin());
	d_tmp.push_back(nw);
    }

    void simulation_calculate_metric_cf_impl::cal_park_metric(gr_complex nw)
    {
	int L = d_block_size/2;
	d_p = gr_complex(0,0);
	d_r = 0;
	for(int i=0,left=L-1,right=L;i<L;i++){
	    d_p += d_tmp[left-i]*d_tmp[right+i];
	    d_r += norm(d_tmp[right+i]);	    
	}
	d_m = norm(d_p)/(d_r*d_r);       
	
	d_tmp.erase(d_tmp.begin());
	d_tmp.push_back(nw);
    }

    int
    simulation_calculate_metric_cf_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        float *out = (float *) output_items[0];

        // Do <+signal processing+>
	int ni=0;
	int no=0;
	while(ni<ninput_items[0] && no<noutput_items){
	    switch(d_st){
		case ST_FILL:	if(d_count == d_num){
				    if(d_method != "Park")
					d_st = ST_FIRST_RUN;
				    else
					d_st = ST_RUN;
				    break;
				}
				d_tmp.push_back(in[ni]);
				d_count++;
				ni++;
				break;
		case ST_FIRST_RUN:  init_metric();
				    out[no] = d_m;
				    no++;
				    d_st = ST_RUN;
				    break;
		case ST_RUN:	cal_metric(in[ni]);
				out[no] = d_m;
				no++;
				ni++;
				break;
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

