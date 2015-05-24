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
#include "modulate_message_c_impl.h"
#include <cmath>

namespace gr {
  namespace scfde {

   #define BYTE 8     
    #define PI 3.141592657
      
    modulate_message_c::sptr
    modulate_message_c::make(int block_size,std::string mod_type,unsigned msgq_limit,int block_num)
    {
      return gnuradio::get_initial_sptr
        (new modulate_message_c_impl(block_size,mod_type,msgq_limit,block_num));
    }

    /*
     * The private constructor
     */
    modulate_message_c_impl::modulate_message_c_impl(int block_size,
						     std::string mod_type,
						     unsigned msgq_limit,
						     int block_num)
      : gr::sync_block("modulate_message_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, block_size*sizeof(gr_complex))),
        d_block_size(block_size),
	d_mod_type(mod_type),
	d_msgq(msg_queue::make(msgq_limit)),
	d_n_block(block_num),
	d_count(0)
    {
	init_constellation();
	d_bit_num = log2(d_constellation.size());
    }

    /*
     * Our virtual destructor.
     */
    modulate_message_c_impl::~modulate_message_c_impl()
    {
    }

    //       :			 :
    //    10 : 11   		 :
    //.................   ...1.......0...
    //    00 : 01  		 :
    //       :			 :
    void modulate_message_c_impl::init_constellation()
    {
	if(d_mod_type == "qpsk"){
	    d_constellation.resize(4);
	    float c= std::cos(PI/4);
	    d_constellation[0] = gr_complex(-c,-c);
	    d_constellation[1] = gr_complex(c,-c);
	    d_constellation[2] = gr_complex(-c,c);
	    d_constellation[3] = gr_complex(c,c);
	}
	if(d_mod_type == "bpsk"){
	    d_constellation.resize(2);
	    d_constellation[0] = gr_complex(1,0);
	    d_constellation[1] = gr_complex(-1,0);
	}	
    }
    
    //GR_MSB_FIRST
    bool modulate_message_c_impl::modulate_message(std::vector< gr_complex >& modulated_data)
    {
	if(d_msgq->empty_p())
	  return false;
	
	d_msg = d_msgq->delete_head();
	
	unsigned int byte_offset = 0;
	unsigned int bit_offset = 0;
	int index=0;
	
	while(byte_offset < d_msg->length()){
	  unsigned char byte = *(d_msg->msg() + byte_offset);
	  //std::cout<<"byte offset: "<<byte_offset<<"  data: "<<std::hex<<(int)byte<<std::endl;
	  while(bit_offset < BYTE){
	    unsigned char tmp = byte;
	    tmp <<= bit_offset;
	    tmp >>= (BYTE - d_bit_num);
	    modulated_data[index] = d_constellation[tmp];
	    //std::cout<<"bit offset: "<<bit_offset<<"  byte: "<<(int)tmp<<std::endl;
	    bit_offset += d_bit_num;
	    index++;
	  }
	  byte_offset++;
	  bit_offset = 0;
	}
	
	return true;
    }

    int
    modulate_message_c_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        gr_complex *out = (gr_complex *) output_items[0];
	
	if(d_n_block!=-1 && d_count>=d_n_block){
	    return -1;
	}
	
        // Do <+signal processing+>
	int no = 0;
	int oo = 0;
	while(no<noutput_items){
	  std::vector<gr_complex> tmp(d_block_size,gr_complex(0,0));
	  if(!modulate_message(tmp))
	    break;
	  for(int i=0;i<d_block_size;i++){
	    out[oo] = tmp[i];
	    oo++;
	  }
	  
	  if(d_n_block!=-1)
	      d_count++;
// 	  std::cout<<d_count<<std::endl;
	  no++;
	}
        // Tell runtime system how many output items we produced.
        return no;
    }

  } /* namespace scfde */
} /* namespace gr */

