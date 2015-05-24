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

#ifndef INCLUDED_SCFDE_DEMOD_MAPPER_CBB_IMPL_H
#define INCLUDED_SCFDE_DEMOD_MAPPER_CBB_IMPL_H

#include <scfde/demod_mapper_cbb.h>
#include <vector>

namespace gr {
  namespace scfde {

    class demod_mapper_cbb_impl : public demod_mapper_cbb
    {
      private:
	std::string d_demod_type;
	int d_data_size;
	int d_count;
	std::vector<gr_complex> d_constellation;
	
	void init_constellation();
	
	int demod(gr_complex data);
	int bpsk_demod(gr_complex data);
	int qpsk_demod(gr_complex data);

     public:
      demod_mapper_cbb_impl(std::string demod_type,int data_size);
      ~demod_mapper_cbb_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_DEMOD_MAPPER_CBB_IMPL_H */

