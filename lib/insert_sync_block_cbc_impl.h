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

#ifndef INCLUDED_SCFDE_INSERT_SYNC_BLOCK_CBC_IMPL_H
#define INCLUDED_SCFDE_INSERT_SYNC_BLOCK_CBC_IMPL_H

#include <scfde/insert_sync_block_cbc.h>
#include <scfde/gen_uw.h>
#include "fft.h"
#include <vector>

namespace gr {
  namespace scfde {

    class insert_sync_block_cbc_impl : public insert_sync_block_cbc
    {
      private:
	int d_block_size;
	int d_first;
	std::vector<gr_complex> d_sync_seq;
	
	gen_uw *d_uw_generator;
	spinal::fft::fft_complex *d_ifft;
	
	void init_freq_sync_seq();
	void init_time_sync_seq();

     public:
      insert_sync_block_cbc_impl(int block_size);
      ~insert_sync_block_cbc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_INSERT_SYNC_BLOCK_CBC_IMPL_H */

