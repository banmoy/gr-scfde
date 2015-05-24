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
#include "simulation_cfo_source_c_impl.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

namespace gr {
  namespace scfde {

    simulation_cfo_source_c::sptr
    simulation_cfo_source_c::make(int block_size,int sync_num,int interval,std::string method)
    {
      return gnuradio::get_initial_sptr
        (new simulation_cfo_source_c_impl(block_size,sync_num,interval,method));
    }

    /*
     * The private constructor
     */
    simulation_cfo_source_c_impl::simulation_cfo_source_c_impl(int block_size,int sync_num,int interval,std::string method)
      : gr::sync_block("simulation_cfo_source_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, block_size*sizeof(gr_complex))),
        d_block_size(block_size),
        d_interval(interval),
        d_method(method),
        d_sync_num(sync_num),
        d_sync_count(0),
        d_count(0)
    {
	d_uw_generator = new gen_uw("PN");
	d_ifft = new spinal::fft::fft_complex(d_block_size,false);
	
	d_constellation_point = new gr_complex[2];
	d_constellation_point[0] = gr_complex(-1,0);
	d_constellation_point[1] = gr_complex(1,0);
	
	d_block_data = new gr_complex[d_block_size];
	
	init_uw();
    }

    /*
     * Our virtual destructor.
     */
    simulation_cfo_source_c_impl::~simulation_cfo_source_c_impl()
    {
	delete d_uw_generator;
	delete d_ifft;
	delete d_uw;
	delete d_block_data;
    }

    void simulation_cfo_source_c_impl::init_uw()
    {
	if(d_method == "Lim")
	    init_lim();
	if(d_method == "SC")
	    init_sc();
    }

    void simulation_cfo_source_c_impl::init_lim()
    {
	d_uw_num = 1;
	d_uw = new gr_complex[d_uw_num*d_block_size];
	
	d_uw_generator->set_len(d_block_size/2);
	d_uw_generator->set_bgn(0);
	d_uw_generator->gen_seq();
	for(int i=0;i<d_block_size;i+=2){
	    d_uw[i] = (d_uw_generator->get_seq())[i/2];
	    d_uw[i+1] = gr_complex(0,0);
	}
	
	memcpy((char*)d_ifft->get_inbuf(),(char*)d_uw,d_block_size*sizeof(gr_complex));
	d_ifft->execute();
	memcpy((char*)d_uw,(char*)d_ifft->get_outbuf(),d_block_size*sizeof(gr_complex));
	
	float sum=0;
	for(int i=0;i<d_block_size;i++)
	    sum += norm(d_uw[i]);	
	sum = sqrt(sum);
	
	for(int i=0;i<d_block_size;i++){
	   d_uw[i] = d_uw[i]*(float)sqrt(d_block_size)/(float)sum;
	}
    }

    void simulation_cfo_source_c_impl::init_sc()
    {
	d_uw_num = 2;
	d_uw = new gr_complex[d_uw_num*d_block_size];
	
	d_uw_generator->set_len(d_block_size);
	d_uw_generator->set_bgn(0);
	d_uw_generator->gen_seq();
	for(int i=0;i<d_block_size*d_uw_num;i+=2){
	    d_uw[i] = (d_uw_generator->get_seq())[i/2];
	    d_uw[i+1] = gr_complex(0,0);
	}
	
	memcpy((char*)d_ifft->get_inbuf(),(char*)d_uw,d_block_size*sizeof(gr_complex));
	d_ifft->execute();
	memcpy((char*)d_uw,(char*)d_ifft->get_outbuf(),d_block_size*sizeof(gr_complex));
	
	memcpy((char*)d_ifft->get_inbuf(),(char*)&d_uw[d_block_size],d_block_size*sizeof(gr_complex));
	d_ifft->execute();
	memcpy((char*)&d_uw[d_block_size],(char*)d_ifft->get_outbuf(),d_block_size*sizeof(gr_complex));
	
	float sum=0;
	for(int i=0;i<d_block_size;i++)
	    sum += norm(d_uw[i]);	
	sum = sqrt(sum);
	for(int i=0;i<d_block_size;i++){
	   d_uw[i] = d_uw[i]*(float)sqrt(d_block_size)/(float)sum;
	}
	
	sum=0;
	for(int i=d_block_size;i<d_block_size*d_uw_num;i++)
	    sum += norm(d_uw[i]);	
	sum = sqrt(sum);
	
	for(int i=d_block_size;i<d_block_size*d_uw_num;i++){
	   d_uw[i] = d_uw[i]*(float)sqrt(d_block_size)/(float)sum;
	}
    }

    void simulation_cfo_source_c_impl::gen_data()
    {
	srand((int)time(NULL));
	for(int i=0;i<d_block_size;i++){
	    int ran = rand()%2;
	    d_block_data[i] = d_constellation_point[ran];
	}
    }
    
    int
    simulation_cfo_source_c_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
         gr_complex *out = (gr_complex *) output_items[0];

	if(d_sync_count >= d_sync_num)
	    return -1;
	
        // Do <+signal processing+>
	int no=0;
	while(no<noutput_items){
	    if(d_count <d_uw_num ){
		memcpy((char*)&out[no*d_block_size], (char*)&d_uw[d_block_size*d_count],
				d_block_size*sizeof(gr_complex));
		d_count++;
	    }else{
		gen_data();
		memcpy((char*)&out[no*d_block_size], (char*)d_block_data,
				d_block_size*sizeof(gr_complex));
		d_count++;
		if(d_count == d_interval){
		    d_count = 0;
		    d_sync_count++;
		}
	    }
	    no++;
	    if(d_sync_count >= d_sync_num)
		break;
	}
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace scfde */
} /* namespace gr */

