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
#include "simulation_timing_source_c_impl.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

namespace gr {
  namespace scfde {

   simulation_timing_source_c::sptr
    simulation_timing_source_c::make(int block_size,int sync_num,int interval,std::string method)
    {
      return gnuradio::get_initial_sptr
        (new simulation_timing_source_c_impl(block_size,sync_num,interval,method));
    }

    /*
     * The private constructor
     */
    simulation_timing_source_c_impl::simulation_timing_source_c_impl(int block_size,int sync_num,int interval,std::string method)
      : gr::sync_block("simulation_timing_source_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, block_size*sizeof(gr_complex))),
        d_block_size(block_size),
        d_interval(interval),
        d_count(0),
        d_method(method),
        d_sync_num(sync_num),
        d_sync_count(0)
    {
	d_uw = new gr_complex[d_block_size];
	d_constellation_point = new gr_complex[2];
	d_block_data = new gr_complex[d_block_size];
	
	d_constellation_point[0] = gr_complex(-1,0);
	d_constellation_point[1] = gr_complex(1,0);
	
	init_uw();
    }

    /*
     * Our virtual destructor.
     */
    simulation_timing_source_c_impl::~simulation_timing_source_c_impl()
    {
	delete d_uw;
	delete d_uw_generator;
	delete d_ifft;
	delete d_constellation_point;
	delete d_block_data;
    }

    void simulation_timing_source_c_impl::init_uw()
    {
	if(d_method == "SC")
	    sc_uw();
	if(d_method == "Minn")
	    minn_uw();
	if(d_method == "Park")
	    park_uw();
    }

    void simulation_timing_source_c_impl::sc_uw()
    {
	d_ifft = new spinal::fft::fft_complex(d_block_size,false);
	d_uw_generator = new gen_uw("PN",d_block_size/2);
	
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

    void simulation_timing_source_c_impl::minn_uw()
    {
	d_ifft = new spinal::fft::fft_complex(d_block_size/2,false);
	d_uw_generator = new gen_uw("PN",d_block_size/4);
	
	for(int i=0;i<d_block_size/2;i+=2){
	    d_uw[i] = (d_uw_generator->get_seq())[i/2];
	    d_uw[i+1] = gr_complex(0,0);
	}
	
	memcpy((char*)d_ifft->get_inbuf(),(char*)d_uw,d_block_size/2*sizeof(gr_complex));
	d_ifft->execute();
	memcpy((char*)d_uw,(char*)d_ifft->get_outbuf(),d_block_size/2*sizeof(gr_complex));
	
	float sum=0;
	for(int i=0;i<d_block_size/2;i++)
	    sum += norm(d_uw[i]);	
	sum = sqrt(sum);
	
	for(int i=0;i<d_block_size/2;i++){
	   d_uw[i] = d_uw[i]*(float)sqrt(d_block_size/2)/(float)sum;
	}
	
	for(int i=0;i<d_block_size/2;i++){
	   d_uw[i+d_block_size/2] = -d_uw[i];
	}
    }

    void simulation_timing_source_c_impl::park_uw()
    {
	d_ifft = new spinal::fft::fft_complex(d_block_size/4,false);
	d_uw_generator = new gen_uw("PN",d_block_size/4);
	
	for(int i=0;i<d_block_size/4;i++){
	    d_uw[i] = (d_uw_generator->get_seq())[i];
	}
	
	memcpy((char*)d_ifft->get_inbuf(),(char*)d_uw,d_block_size/4*sizeof(gr_complex));
	d_ifft->execute();
	memcpy((char*)d_uw,(char*)d_ifft->get_outbuf(),d_block_size/4*sizeof(gr_complex));
	
	float sum=0;
	for(int i=0;i<d_block_size/4;i++)
	    sum += norm(d_uw[i]);	
	sum = sqrt(sum);
	
	for(int i=0;i<d_block_size/4;i++){
	   d_uw[i] = d_uw[i]*(float)sqrt(d_block_size/4)/(float)sum;
	}
	
	for(int i=0;i<d_block_size/4;i++){
	   d_uw[i+d_block_size/4] = d_uw[d_block_size/4-1-i];
	}
	
	for(int i=0;i<d_block_size/2;i++)
	    d_uw[i+d_block_size/2] = conj(d_uw[i]);
    }

    void simulation_timing_source_c_impl::gen_data()
    {
	srand((int)time(NULL));
	for(int i=0;i<d_block_size;i++){
	    int ran = rand()%2;
	    d_block_data[i] = d_constellation_point[ran];
	}
    }
    
    int
    simulation_timing_source_c_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        gr_complex *out = (gr_complex *) output_items[0];

	if(d_sync_count >= d_sync_num)
	    return -1;
	
        // Do <+signal processing+>
	int no=0;
	while(no<noutput_items){
	    if(d_count == 0){
		memcpy((char*)&out[no*d_block_size], (char*)d_uw,
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

