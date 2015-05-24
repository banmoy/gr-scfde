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

#ifndef INCLUDED_SCFDE_MODULATE_MESSAGE_C_IMPL_H
#define INCLUDED_SCFDE_MODULATE_MESSAGE_C_IMPL_H

#include <scfde/modulate_message_c.h>
#include <gnuradio/message.h>
#include <vector>

namespace gr {
  namespace scfde {

    class modulate_message_c_impl : public modulate_message_c
    {
      private:
	 int d_block_size;
	 std::string d_mod_type;
	 std::vector<gr_complex> d_constellation;
	 msg_queue::sptr d_msgq;
	 message::sptr d_msg;
	 int d_bit_num;
	 
	 ////////for simulation///////
	 int d_n_block;
	 int d_count;
	  
	 void init_constellation();
	 bool modulate_message(std::vector<gr_complex> &modulated_data);

     public:
      modulate_message_c_impl(int block_size,std::string mod_type,unsigned msgq_limit,int block_num);
      ~modulate_message_c_impl();

      msg_queue::sptr msgq() const { return d_msgq;}
      
      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_MODULATE_MESSAGE_C_IMPL_H */

