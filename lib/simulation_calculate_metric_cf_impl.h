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

#ifndef INCLUDED_SCFDE_SIMULATION_CALCULATE_METRIC_CF_IMPL_H
#define INCLUDED_SCFDE_SIMULATION_CALCULATE_METRIC_CF_IMPL_H

#include <scfde/simulation_calculate_metric_cf.h>
#include <vector>

namespace gr {
  namespace scfde {

    class simulation_calculate_metric_cf_impl : public simulation_calculate_metric_cf
    {
      private:
	enum state{
	  ST_FILL,
	  ST_FIRST_RUN,
	  ST_RUN
	};
	state d_st;
	int d_block_size;
	int d_num;
	int d_count;
	std::string d_method;
	std::vector<gr_complex> d_tmp;

	gr_complex d_p;
	float d_r;
	float d_m;
	
	void init_num();
	
	void init_metric();
	void init_sc_metric();
	void init_minn_metric();
	
	void cal_metric(gr_complex nw);
	void cal_sc_metric(gr_complex nw);
	void cal_minn_metric(gr_complex nw);
	void cal_park_metric(gr_complex nw);
	
     public:
      simulation_calculate_metric_cf_impl(int block_size,std::string method);
      ~simulation_calculate_metric_cf_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_SIMULATION_CALCULATE_METRIC_CF_IMPL_H */

