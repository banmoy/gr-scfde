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
#include "frequency_sync_cbcb_impl.h"
#include <gnuradio/math.h>
#include <gnuradio/fxpt.h>
#include <vector>

namespace gr {
  namespace scfde {

     #define PI 3.141592657

    frequency_sync_cbcb::sptr
    frequency_sync_cbcb::make(int block_size,int cp_size)
    {
      return gnuradio::get_initial_sptr
        (new frequency_sync_cbcb_impl(block_size,cp_size));
    }

    /*
     * The private constructor
     */
    frequency_sync_cbcb_impl::frequency_sync_cbcb_impl(int block_size,int cp_size)
      : gr::block("frequency_sync_cbcb",
              gr::io_signature::make2(1, 2, block_size*sizeof(gr_complex),sizeof(char)),
              gr::io_signature::make3(1, 3, block_size*sizeof(gr_complex), sizeof(char), sizeof(short))),
      d_block_size(block_size),
      d_cp_size(cp_size),
      d_max_ifo(d_block_size/2),
      d_first(false),
      d_offset(0),
      d_fraction(0),
      d_interger(0),
      d_phase_erro(0),
      d_phase(0)
    {
	d_uw_generator = new gen_uw("PN",d_block_size/2);
	d_fft = new spinal::fft::fft_complex(d_block_size,true);
	if(1)
	    init_freq_sync_fft();
	else
	    init_time_sync_fft();
	init_phase_diff();
    }

    /*
     * Our virtual destructor.
     */
    frequency_sync_cbcb_impl::~frequency_sync_cbcb_impl()
    {
	delete d_uw_generator;
	delete d_fft;
	delete d_sync_fft;
	delete d_phase_diff;
    }

    void
    frequency_sync_cbcb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        for(int i=0;i<ninput_items_required.size();i++)
	    ninput_items_required[i] = noutput_items;
    }
    
    // init freq sync
    void frequency_sync_cbcb_impl::init_freq_sync_fft()
    {
	d_sync_fft = new gr_complex[d_block_size];
	std::vector<gr_complex> seq = d_uw_generator->get_seq();
	for(int i=0;i<d_block_size;i+=2){
	    d_sync_fft[i] = seq[i/2];
	    d_sync_fft[i+1] = gr_complex(0,0);
// 	    std::cout<<seq[i/2].real()<<"  "<<seq[i/2].imag()<<std::endl;
	}  
    }
    
    //init time sync
    void frequency_sync_cbcb_impl::init_time_sync_fft()
    {
	d_sync_fft = new gr_complex[d_block_size];
	std::vector<gr_complex> seq = d_uw_generator->get_seq();
	for(int i=0;i<d_block_size/2;i++)
	    d_sync_fft[i] = d_sync_fft[i+d_block_size/2] = seq[i];
	memcpy((char*)d_fft->get_inbuf(),(char*)d_sync_fft,
			d_block_size*sizeof(gr_complex));
	d_fft->execute();
	memcpy((char*)d_sync_fft,(char*)d_fft->get_outbuf(),
			d_block_size*sizeof(gr_complex));
    }
    
    void frequency_sync_cbcb_impl::init_phase_diff()
    {
	d_phase_diff = new gr_complex[d_block_size];
	
	for(int i=0;i<d_block_size;i+=2){
	    d_phase_diff[i] = conj(d_sync_fft[i])*d_sync_fft[(i+2)%d_block_size];
	    d_phase_diff[i] /= (float)sqrt(norm(d_phase_diff[i]));
	    d_phase_diff[i+1] = gr_complex(0,0);
	}
    }
    
    void frequency_sync_cbcb_impl::compute_ffo(gr_complex *dptr)
    {
	gr_complex sum(0,0);
	for(int i=0;i<d_block_size/2;i++)
	    sum += conj(dptr[i])*dptr[i+d_block_size/2];
 	d_fraction = gr::fast_atan2f(sum)/(2*PI)/(d_block_size/2);
// 	std::cout<<d_fraction<<std::endl;
    }
    
    void frequency_sync_cbcb_impl::compute_ifo(gr_complex* dptr)
    {	
	gr_complex dfft[d_block_size];
	gr_complex phase_diff[d_block_size];
	
	memcpy((char*)d_fft->get_inbuf(),(char*)dptr,
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
		d_offset = -i; 	// d_offset>0 raw seq left shift; d_offset<0 raw seq right shift
		max = metric;
	    }
	}
		
 	d_interger = (float)d_offset/d_block_size;
	//FIXME:d_interger = 0;
    }
    
    void frequency_sync_cbcb_impl::correct_sync_ffo(gr_complex *dptr)
    {
	float phase=0;
	float phase_erro = 2*PI*d_fraction;
	for(int i=0;i<d_block_size;i++){
	    phase = phase - phase_erro;
	    while(phase > (float)(PI))
		phase -= (float)(2*PI);
	    while(phase < (float)(-PI))
		phase += (float)(2*PI);

	    float oi, oq;
	    gr_int32 angle = gr::fxpt::float_to_fixed (phase);
	    gr::fxpt::sincos(angle, &oq, &oi);
	    dptr[i] *= gr_complex(oi, oq);   
	}
    }
    
    void frequency_sync_cbcb_impl::correct_data_cfo(gr_complex* dptr)
    {
	d_phase -= d_phase_erro*d_cp_size;
	for(int i=0;i<d_block_size;i++){
	    d_phase -= d_phase_erro;
	    while(d_phase > (float)(PI))
		d_phase -= (float)(2*PI);
	    while(d_phase < (float)(-PI))
		d_phase += (float)(2*PI);

	    float oi, oq;
	    gr_int32 angle = gr::fxpt::float_to_fixed(d_phase);
	    gr::fxpt::sincos(angle, &oq, &oi);
	    dptr[i] *= gr_complex(oi, oq);   
	}
    }

    int
    frequency_sync_cbcb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in_data = (const gr_complex *) input_items[0];
	const unsigned char *in_flag = (const unsigned char *) input_items[1];
        gr_complex *out_data = (gr_complex *) output_items[0];
	unsigned char *out_flag = (unsigned char *) output_items[1];
	short *out_ifo = (short *) output_items[2];

        // Do <+signal processing+>
	int ni=0;
	int no=0;
	int in_items = std::min(ninput_items[0],ninput_items[1]);
	while(no<noutput_items&&ni<in_items){
	    if(in_flag[ni]&0x01){
		//compute ifo and ffo
		gr_complex sync_data[d_block_size];
		memcpy((char*)sync_data,(char*)&in_data[ni*d_block_size],
				d_block_size*sizeof(gr_complex));
		compute_ffo(sync_data);
		correct_sync_ffo(sync_data);
		compute_ifo(sync_data);
		d_phase = 0;
		d_phase_erro = 2*PI*(d_fraction+d_interger);
				
		d_first = true;
	    }else{
		gr_complex data[d_block_size];
		memcpy((char*)data,(char*)&in_data[ni*d_block_size],
				d_block_size*sizeof(gr_complex));
		correct_data_cfo(data);
		memcpy((char*)&out_data[no*d_block_size],(char*)data,
				d_block_size*sizeof(gr_complex));
		
		if(d_first){
		    out_flag[no] = 1;
		    d_first = false;
		}
		else
		    out_flag[no] = 0;
		
		out_ifo[no] = d_offset;
		
		no++;
	    }
	    ni++;
	}
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (ni);

        // Tell runtime system how many output items we produced.
        return no;
    }

  } /* namespace scfde */
} /* namespace gr */

