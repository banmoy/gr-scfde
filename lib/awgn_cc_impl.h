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

#ifndef INCLUDED_SCFDE_AWGN_CC_IMPL_H
#define INCLUDED_SCFDE_AWGN_CC_IMPL_H

#include <scfde/awgn_cc.h>
#include <gnuradio/random.h>
#include <vector>

namespace gr {
  namespace scfde {

    class awgn_cc_impl : public awgn_cc
    {
     private:
	float d_snr;
	std::string d_method;
	int d_samples;
	float d_sig_pow;
	float d_ampl;
	int d_count;
	
	random d_rng;
	
	void update_ampl();
		
     public:
      awgn_cc_impl(float snr,std::string method,int samples,float sig_pow);
      ~awgn_cc_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_AWGN_CC_IMPL_H */

