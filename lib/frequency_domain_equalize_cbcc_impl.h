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

#ifndef INCLUDED_SCFDE_FREQUENCY_DOMAIN_EQUALIZE_CBCC_IMPL_H
#define INCLUDED_SCFDE_FREQUENCY_DOMAIN_EQUALIZE_CBCC_IMPL_H

#include <scfde/frequency_domain_equalize_cbcc.h>
#include <scfde//gen_uw.h>
#include <pmt/pmt.h>
#include <gnuradio/thread/thread.h>
#include "fft.h"

namespace gr {
  namespace scfde {

    class frequency_domain_equalize_cbcc_impl : public frequency_domain_equalize_cbcc
    {
    private:
	int d_block_size;
	int d_cp_size;
	int d_eb_num;
	int d_count;
	gr_complex *d_uw_fft;
	gr_complex *d_taps_fft;
	gr_complex *d_taps;
	float d_SNR;
	std::string d_equal_method;
	std::string d_ce_method;
		
	gr::thread::mutex d_mutex;
	
	gen_uw *d_uw_generator;
	spinal::fft::fft_complex *d_fft;
	spinal::fft::fft_complex *d_ifft;
	
	int d_block_count;
	
	void set_snr(pmt::pmt_t msg);
	
	void init_uw_fft();
	void reset_taps_fft();
	
	void ls_ce(gr_complex *tmp);
	
	void equalize(gr_complex *tmp);
	void mmse_equalize(gr_complex *tmp);
	void ls_equalize(gr_complex *tmp);
	
     public:
      frequency_domain_equalize_cbcc_impl(int block_size,int cp_size,int eb_num,std::string equal_method,float SNR);
      ~frequency_domain_equalize_cbcc_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_FREQUENCY_DOMAIN_EQUALIZE_CBCC_IMPL_H */

