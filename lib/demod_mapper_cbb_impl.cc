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
#include "demod_mapper_cbb_impl.h"
#include <gnuradio/math.h>
#include <gnuradio/expj.h>

namespace gr {
  namespace scfde {

   #define PI 3.141592657

    demod_mapper_cbb::sptr
    demod_mapper_cbb::make(std::string demod_type,int data_size)
    {
      return gnuradio::get_initial_sptr
        (new demod_mapper_cbb_impl(demod_type,data_size));
    }

    /*
     * The private constructor
     */
    demod_mapper_cbb_impl::demod_mapper_cbb_impl(std::string demod_type,int data_size)
      : gr::sync_interpolator("demod_mapper_cbb",
              gr::io_signature::make(1, 1, data_size*sizeof(gr_complex)),
              gr::io_signature::make2(1, 2, sizeof(char),sizeof(char)), data_size),
        d_demod_type(demod_type),
        d_data_size(data_size),
        d_count(0)
    {	
	init_constellation();
    }

    /*
     * Our virtual destructor.
     */
    demod_mapper_cbb_impl::~demod_mapper_cbb_impl()
    {

    }
    
    //       :			 :
    //    10 : 11   		 :
    //.................   ...1.......0...
    //    00 : 01  		 :
    //       :			 :
    void demod_mapper_cbb_impl::init_constellation()
    {
	if(d_demod_type == "qpsk"){
	    d_constellation.resize(4);
	    float c= std::cos(PI/4);
	    d_constellation[0] = gr_complex(-c,-c);
	    d_constellation[1] = gr_complex(c,-c);
	    d_constellation[2] = gr_complex(-c,c);
	    d_constellation[3] = gr_complex(c,c);
	}
	if(d_demod_type == "bpsk"){
	    d_constellation.resize(2);
	    d_constellation[0] = gr_complex(1,0);
	    d_constellation[1] = gr_complex(-1,0);
	}	
    }
    
    //       :			 :
    //    10 : 11   		 :
    //.................   ...1.......0...
    //    00 : 01  		 :
    //       :			 :
    int demod_mapper_cbb_impl::demod(gr_complex data)
    {
	if(d_demod_type == "qpsk")
	  return qpsk_demod(data);
	if(d_demod_type == "bpsk")
	  return bpsk_demod(data);
    }
    
    int demod_mapper_cbb_impl::bpsk_demod(gr_complex data)
    {
	return data.real()<0;
    }
    
    int demod_mapper_cbb_impl::qpsk_demod(gr_complex data)
    {
	return ((data.real()>0)&0x01) | (((data.imag()>0)&0x01)<<1);
    }

    int
    demod_mapper_cbb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
	
        unsigned char *out = (unsigned char *) output_items[0];
	unsigned char *out_flag = (unsigned char *) output_items[1];
	
        // Do <+signal processing+>
	int no=0;
	while(no<noutput_items){
	   int index = demod(in[no]);
	   out[no] = index;
	   
	   if(d_count == 0)
		out_flag[no] = 1;
	   else
		out_flag[no] = 0;
	   
	    d_count++;
	    if(d_count == d_data_size){
	      d_count = 0;
	    }
	    no++;
	}

        // Tell runtime system how many output items we produced.
        return no;
    }

  } /* namespace scfde */
} /* namespace gr */

