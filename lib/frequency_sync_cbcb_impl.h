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

#ifndef INCLUDED_SCFDE_FREQUENCY_SYNC_CBCB_IMPL_H
#define INCLUDED_SCFDE_FREQUENCY_SYNC_CBCB_IMPL_H

#include <scfde/frequency_sync_cbcb.h>
#include <scfde//gen_uw.h>
#include "fft.h"

namespace gr {
  namespace scfde {

    class frequency_sync_cbcb_impl : public frequency_sync_cbcb
    {
     private:
	int d_block_size;
	int d_cp_size;
	float d_fraction;
	float d_interger;
	short d_offset;
	float d_phase_erro;
	float d_phase;
	int d_max_ifo;			//relative to d_block_size
	gr_complex *d_sync_fft;
	gr_complex *d_phase_diff;
	bool d_first;	
	
	gen_uw *d_uw_generator;
	spinal::fft::fft_complex *d_fft;
	
	void init_freq_sync_fft();
	void init_time_sync_fft();
	void init_phase_diff();
	
	void compute_ffo(gr_complex *dptr);
	void compute_ifo(gr_complex *dptr);
	void correct_sync_ffo(gr_complex *dptr);
	void correct_data_cfo(gr_complex *dptr);
	
     public:
      frequency_sync_cbcb_impl(int block_size,int cp_size);
      ~frequency_sync_cbcb_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_FREQUENCY_SYNC_CBCB_IMPL_H */

