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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "block_acquization_cbcb_impl.h"

namespace gr {
  namespace scfde {

    block_acquization_cbcb::sptr
    block_acquization_cbcb::make(int block_size,int cp_size,int db_num,int eb_num)
    {
      return gnuradio::get_initial_sptr
        (new block_acquization_cbcb_impl(block_size,cp_size,db_num,eb_num));
    }

    /*
     * The private constructor
     */
    block_acquization_cbcb_impl::block_acquization_cbcb_impl(int block_size,int cp_size,int db_num,int eb_num)
      : gr::block("block_acquization_cbcb",
              gr::io_signature::make2(1, 2, sizeof(gr_complex),sizeof(char)),
              gr::io_signature::make2(1, 2, block_size*sizeof(gr_complex),sizeof(char))),
      d_block_size(block_size),
      d_cp_size(cp_size),
      d_db_num(db_num),
      d_eb_num(eb_num),
      d_full(false)
    {
	d_total_size = d_block_size+(d_eb_num+d_db_num)*(d_block_size+d_cp_size);
	d_data_list.resize(0);
	
	d_count = 0;
    }

    /*
     * Our virtual destructor.
     */
    block_acquization_cbcb_impl::~block_acquization_cbcb_impl()
    {
// 	std::cout<<d_in<<"  "<<d_out<<std::endl;
    }

    void
    block_acquization_cbcb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
	for(int i=0;i<ninput_items_required.size();i++)
	    ninput_items_required[i] = d_block_size*noutput_items;
    }

    int
    block_acquization_cbcb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in_data = (const gr_complex *) input_items[0];
	const unsigned char *in_flag = (const unsigned char *) input_items[1];
        gr_complex *out_data = (gr_complex *) output_items[0];
	unsigned char *out_flag = (unsigned char *) output_items[1];
    
        // Do <+signal processing+>
	int ni=0;
	int no=0;
	int oo=0;
	int in_items = std::min(ninput_items[0],ninput_items[1]);
// 	std::cout<<"acq in1 "<<d_full<<"  "<<in_items<<std::endl;
	while((!d_full)&&(ni<in_items)){
	    if(in_flag[ni]&0x01){
		d_data_list.push_back(std::vector<gr_complex>(0));
	    }
	    for(int i=0;i<d_data_list.size();i++)
		if(d_data_list[i].size() < d_total_size)
		    d_data_list[i].push_back(in_data[ni]);
	    ni++;
	    if((!d_data_list.empty())&&(d_data_list[0].size()==d_total_size))
		d_full = true;
	}
// 	std::cout<<"acq out1"<<std::endl;
	int out_items = std::min(noutput_items,(d_db_num+d_eb_num+1)-d_count);
// 	std::cout<<"acq in2 "<<d_full<<"  "<<out_items<<std::endl;

	while(d_full && (no<out_items)){
	    for(int i=0;i<d_block_size;i++){
		out_data[oo] = d_data_list[0][(d_block_size+d_cp_size)*d_count+i];
		oo++;
	    }
	    if(d_count == 0)
		out_flag[no] = 1;
	    else
		out_flag[no] = 0;
	    d_count++;
	    if(d_count == d_db_num+d_eb_num+1){
		d_count = 0;
		d_data_list.erase(d_data_list.begin());
		d_full = false;
	    }
	    no++;
	}
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (ni);
// 	std::cout<<"acq out2"<<std::endl;

        // Tell runtime system how many output items we produced.
        return no;
    }

  } /* namespace scfde */
} /* namespace gr */

