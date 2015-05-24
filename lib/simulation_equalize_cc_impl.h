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

#ifndef INCLUDED_SCFDE_SIMULATION_EQUALIZE_CC_IMPL_H
#define INCLUDED_SCFDE_SIMULATION_EQUALIZE_CC_IMPL_H

#include <scfde/simulation_equalize_cc.h>
#include "fft.h"

namespace gr {
  namespace scfde {

    class simulation_equalize_cc_impl : public simulation_equalize_cc
    {
     private:
	int d_block_size;
	int d_cp_size;
	float d_snr;
	std::string d_equal_method;
	gr_complex* d_taps;
	gr_complex* d_taps_fft;

	spinal::fft::fft_complex* d_fft;
	spinal::fft::fft_complex* d_ifft;
	
	int d_block_num;
	int d_block_count;
	
	void init_taps_fft();
	void equalize(gr_complex* data_fft);
	void ls_equalize(gr_complex* data_fft);
	void mmse_equalize(gr_complex* data_fft);
	
     public:
      simulation_equalize_cc_impl(int block_size,int cp_size,std::string equal_method,
				  std::vector<gr_complex> taps,float snr);
      ~simulation_equalize_cc_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_SIMULATION_EQUALIZE_CC_IMPL_H */

