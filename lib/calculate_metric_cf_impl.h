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

#ifndef INCLUDED_SCFDE_CALCULATE_METRIC_CF_IMPL_H
#define INCLUDED_SCFDE_CALCULATE_METRIC_CF_IMPL_H

#include <scfde/calculate_metric_cf.h>
#include <vector>

namespace gr {
  namespace scfde {

    class calculate_metric_cf_impl : public calculate_metric_cf
    {
     private:
	int d_block_size;
	int d_count;
	gr_complex d_p;
	float d_r;
	bool d_first;
	std::vector<gr_complex> d_temp_seq;
	
	float cal_first_metric();
	float cal_metric(gr_complex data);
	
     public:
      calculate_metric_cf_impl(int block_size);
      ~calculate_metric_cf_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_CALCULATE_METRIC_CF_IMPL_H */

