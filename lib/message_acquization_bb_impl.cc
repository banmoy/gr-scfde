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
#include "message_acquization_bb_impl.h"

namespace gr {
  namespace scfde {

     message_acquization_bb::sptr
    message_acquization_bb::make(msg_queue::sptr message_queue,int frame_size)
    {
      return gnuradio::get_initial_sptr
        (new message_acquization_bb_impl(message_queue,frame_size));
    }

    /*
     * The private constructor
     */
    message_acquization_bb_impl::message_acquization_bb_impl(msg_queue::sptr message_queue,int frame_size)
      : gr::sync_block("message_acquization_bb",
              gr::io_signature::make2(1, 2, sizeof(char),sizeof(char)),
              gr::io_signature::make(0,0,0)),
        d_message_queue(message_queue),
        d_frame_len(frame_size),
        d_flag(false)
    {
	d_str.clear();
	
	d_count = 0;
	d_idle = true;
    }

    /*
     * Our virtual destructor.
     */
    message_acquization_bb_impl::~message_acquization_bb_impl()
    {
    }

    int
    message_acquization_bb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *in_flag = (unsigned char *) input_items[1];
		
        // Do <+signal processing+>
	int no=0;
	while(d_idle && no<noutput_items){
	    if(in_flag[no]&0x01){
		d_flag = true;
	    }
	    if(d_flag){
		d_str.push_back(in[no]);
		if(d_str.size() == d_frame_len){
		   d_idle = false;
		}
	    }
	    no++;
	 }
	 
	 if(d_message_queue->empty_p() && !d_idle){
	    message::sptr msg = message::make_from_string(d_str);
	    d_message_queue->insert_tail(msg);
	    msg.reset();
	    d_str.clear();
	    d_flag = false;
	    d_idle = true;
	 }

        // Tell runtime system how many output items we produced.
        return no;
    }

  } /* namespace scfde */
} /* namespace gr */

