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

#ifndef INCLUDED_SCFDE_SIMULATION_CFO_CF_IMPL_H
#define INCLUDED_SCFDE_SIMULATION_CFO_CF_IMPL_H

#include <scfde/simulation_cfo_cf.h>
#include <scfde/gen_uw.h>
#include "fft.h"

namespace gr {
  namespace scfde {

    class simulation_cfo_cf_impl : public simulation_cfo_cf
    {
     private:
	int d_block_size;
	int d_cp_size;
	int d_interval;
	int d_interval_size;
	std::string d_method;
	int d_uw_num;
	gr_complex *d_phase_diff;
	gr_complex *d_data;
	
	float d_ffo;
	float d_ifo;
	float d_cfo;
	float d_max_ifo;

	gen_uw *d_uw_generator;
	
	spinal::fft::fft_complex *d_fft;
	
	void init_uw();
	void init_lim();
	void init_sc();
	
	void cal_ffo();
	void cor_ffo();
	void cal_ifo();
	void cal_lim_ifo();
	void cal_sc_ifo();
	void cal_cfo();
	
     public:
      simulation_cfo_cf_impl(int block_size,int cp_size,int interval,std::string method);
      ~simulation_cfo_cf_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_SIMULATION_CFO_CF_IMPL_H */

