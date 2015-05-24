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

#ifndef INCLUDED_SCFDE_BLOCK_ACQUIZATION_CBCB_IMPL_H
#define INCLUDED_SCFDE_BLOCK_ACQUIZATION_CBCB_IMPL_H

#include <scfde/block_acquization_cbcb.h>
#include <vector>

namespace gr {
  namespace scfde {

    class block_acquization_cbcb_impl : public block_acquization_cbcb
    {
     private:
	int d_block_size;
	int d_cp_size;
	int d_db_num;
	int d_eb_num;
	int d_total_size;
	int d_count;
	int d_full;
	std::vector< std::vector<gr_complex> > d_data_list;
	
	int d_in; 
	int d_out;

     public:
      block_acquization_cbcb_impl(int block_size,int cp_size,int db_num,int eb_num);
      ~block_acquization_cbcb_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_BLOCK_ACQUIZATION_CBCB_IMPL_H */

