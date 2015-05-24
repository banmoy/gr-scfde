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
#include "simulation_cfo_cf_impl.h"
#include <vector>
#include <gnuradio/math.h>
#include <gnuradio/fxpt.h>

namespace gr {
  namespace scfde {

  #define PI 3.141592657
    
    simulation_cfo_cf::sptr
    simulation_cfo_cf::make(int block_size,int cp_size,int interval,std::string method)
    {
      return gnuradio::get_initial_sptr
        (new simulation_cfo_cf_impl(block_size,cp_size,interval,method));
    }

    /*
     * The private constructor
     */
    simulation_cfo_cf_impl::simulation_cfo_cf_impl(int block_size,int cp_size,int interval,std::string method)
      : gr::sync_block("simulation_cfo_cf",
              gr::io_signature::make(1, 1, (block_size+cp_size)*interval*sizeof(gr_complex)),
              gr::io_signature::make3(1, 3, sizeof(float),sizeof(float),sizeof(float))),
        d_block_size(block_size),
        d_cp_size(cp_size),
        d_interval(interval),
        d_interval_size((block_size+cp_size)*d_interval),
        d_method(method),
        d_ffo(0),
        d_ifo(0),
        d_cfo(0),
        d_max_ifo(block_size/2)
    {
	d_uw_generator = new gen_uw("PN");
	d_fft = new spinal::fft::fft_complex(d_block_size,true);
	d_data = new gr_complex[d_interval_size];
	
	init_uw();
	
    }

    /*
     * Our virtual destructor.
     */
    simulation_cfo_cf_impl::~simulation_cfo_cf_impl()
    {
	delete d_uw_generator;
	delete d_fft;
	delete d_phase_diff;
	delete d_data;
    }

    void simulation_cfo_cf_impl::init_uw()
    {
	if(d_method == "Lim")
	    init_lim();
	if(d_method == "SC")
	    init_sc();
    }
    
    void simulation_cfo_cf_impl::init_lim()
    {
	d_uw_num = 1;
	d_phase_diff = new gr_complex[d_block_size];
	std::vector<gr_complex> tmp;
	
	d_uw_generator->set_len(d_block_size/2);
	d_uw_generator->set_bgn(0);
	d_uw_generator->gen_seq();
	tmp = d_uw_generator->get_seq();
	
	for(int i=0;i<d_block_size;i+=2){
	    d_phase_diff[i] = conj(tmp[i/2])*tmp[(i/2+1)%(d_block_size/2)];
	    d_phase_diff[i] /= (float)sqrt(norm(d_phase_diff[i]));
	    d_phase_diff[i+1] = gr_complex(0,0);
	}
    }
    
    void simulation_cfo_cf_impl::init_sc()
    {
	d_uw_num = 2;
	d_phase_diff = new gr_complex[d_block_size];
	std::vector<gr_complex> tmp;
	
	d_uw_generator->set_len(d_block_size);
	d_uw_generator->set_bgn(0);
	d_uw_generator->gen_seq();
	tmp = d_uw_generator->get_seq();
	
	for(int i=0;i<d_block_size;i+=2){
	    d_phase_diff[i] = conj(tmp[i/2])*tmp[d_block_size/2+i/2];
	    d_phase_diff[i] /= (float)sqrt(norm(d_phase_diff[i]));
	    d_phase_diff[i+1] = gr_complex(0,0);
	}
    }
    
    void simulation_cfo_cf_impl::cal_ffo()
    {
	gr_complex sum(0,0);
	for(int i=0;i<d_block_size/2;i++)
	    sum += conj(d_data[i+d_cp_size])*d_data[i+d_block_size/2+d_cp_size];
 	d_ffo = gr::fast_atan2f(sum)/(2*PI)*2;
    }	

    void simulation_cfo_cf_impl::cor_ffo()
    {
	float phase=0;
	float phase_erro = 2*PI*d_ffo/d_block_size;
	for(int i=0;i<(d_block_size+d_cp_size)*d_uw_num;i++){
	    phase = phase - phase_erro;
	    while(phase > (float)(PI))
		phase -= (float)(2*PI);
	    while(phase < (float)(-PI))
		phase += (float)(2*PI);

	    float oi, oq;
	    gr_int32 angle = gr::fxpt::float_to_fixed (phase);
	    gr::fxpt::sincos(angle, &oq, &oi);
	    d_data[i] *= gr_complex(oi, oq);   
	}
    }

    void simulation_cfo_cf_impl::cal_ifo()
    {
	if(d_method == "Lim")
	    cal_lim_ifo();
	if(d_method == "SC")
	    cal_sc_ifo();
    }

    void simulation_cfo_cf_impl::cal_lim_ifo()
    {
	gr_complex dfft[d_block_size];
	gr_complex phase_diff[d_block_size];
	
	memcpy((char*)d_fft->get_inbuf(),(char*)&d_data[d_cp_size],
			d_block_size*sizeof(gr_complex));
	d_fft->execute();
	memcpy((char*)dfft,(char*)d_fft->get_outbuf(),
			d_block_size*sizeof(gr_complex));
	
	for(int i=0;i<d_block_size;i+=2){
	    phase_diff[i] = conj(dfft[i])*dfft[(i+2)%d_block_size];
	    phase_diff[i] /= (float)sqrt(norm(phase_diff[i]));
	}
	
	float max=-1;
	float norm_factor = 4.0/d_block_size/d_block_size;
	for(int i=-d_max_ifo+2;i<=d_max_ifo;i+=2){
	    gr_complex tmp(0,0);
	    for(int j=0;j<d_block_size;j+=2){
		int k = j+i;
		while(k<0)
		    k += d_block_size;
		while(k>=d_block_size)
		    k -= d_block_size;
		tmp += conj(d_phase_diff[k])*phase_diff[j];
	    }
	    float metric = norm(tmp)*norm_factor;
	    if(metric>max){
		d_ifo = -i; 	// d_offset>0 raw seq left shift; d_offset<0 raw seq right shift
		max = metric;
	    }
	}
    }

    void simulation_cfo_cf_impl::cal_sc_ifo()
    {
	gr_complex dfft1[d_block_size];
	gr_complex dfft2[d_block_size];
	
	memcpy((char*)d_fft->get_inbuf(),(char*)&d_data[d_cp_size],
			d_block_size*sizeof(gr_complex));
	d_fft->execute();
	memcpy((char*)dfft1,(char*)d_fft->get_outbuf(),
			d_block_size*sizeof(gr_complex));
	
	memcpy((char*)d_fft->get_inbuf(),(char*)&d_data[2*d_cp_size+d_block_size],
			d_block_size*sizeof(gr_complex));
	d_fft->execute();
	memcpy((char*)dfft2,(char*)d_fft->get_outbuf(),
			d_block_size*sizeof(gr_complex));
	
	float max=-1;
	float norm_factor = 0;
	for(int i=0;i<d_block_size;i+=2)
	    norm_factor += norm(dfft2[i]);
	norm_factor = 2*norm_factor*norm_factor;
	
	for(int i=-d_max_ifo+2;i<=d_max_ifo;i+=2){
	    gr_complex tmp(0,0);
	    for(int j=0;j<d_block_size;j+=2){
		int k = j+i;
		while(k<0)
		    k += d_block_size;
		while(k>=d_block_size)
		    k -= d_block_size;
		tmp += conj(dfft1[k]*d_phase_diff[j])*dfft2[k];
	    }
	    float metric = norm(tmp)/norm_factor;
	    if(metric>max){
		d_ifo = i; 	
		max = metric;
	    }
	}
    }

    void simulation_cfo_cf_impl::cal_cfo()
    {
	d_cfo = d_ffo + d_ifo;
    }

    int
    simulation_cfo_cf_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        float *cfo = (float *) output_items[0];
	float *ifo = (float *) output_items[1];
	float *ffo = (float *) output_items[2];
	
        // Do <+signal processing+>
	int no=0;
	while(no<noutput_items){
	    memcpy((char*)d_data, (char*)&in[no*d_interval_size],
				d_interval_size*sizeof(gr_complex));
	    
	    cal_ffo();
	    cor_ffo();
	    cal_ifo();
	    cal_cfo();
	    
	    cfo[no] = d_cfo;
	    ifo[no] = d_ifo;
	    ffo[no] = d_ffo;
	    
	    no++;
	}
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace scfde */
} /* namespace gr */

