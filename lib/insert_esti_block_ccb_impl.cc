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
#include "insert_esti_block_ccb_impl.h"

namespace gr {
  namespace scfde {

    insert_esti_block_ccb::sptr
    insert_esti_block_ccb::make(int block_size,int db_num,int eb_num)
    {
      return gnuradio::get_initial_sptr
        (new insert_esti_block_ccb_impl(block_size,db_num,eb_num));
    }

    /*
     * The private constructor
     */
    insert_esti_block_ccb_impl::insert_esti_block_ccb_impl(int block_size,int db_num,int eb_num)
      : gr::block("insert_esti_block_ccb",
              gr::io_signature::make(1, 1, block_size*sizeof(gr_complex)),
              gr::io_signature::make2(1, 2, block_size*sizeof(gr_complex),sizeof(char))),
        d_block_size(block_size),
        d_db_num(db_num),
        d_eb_num(eb_num),
	d_eb_count(0),
	d_db_count(0)
    {
	d_uw_generator = new gen_uw("Chu",block_size);
	
	d_esti_uw.resize(0);
	d_esti_uw = d_uw_generator->get_seq();
    }

    /*
     * Our virtual destructor.
     */
    insert_esti_block_ccb_impl::~insert_esti_block_ccb_impl()
    {
	delete d_uw_generator;
    }

    void
    insert_esti_block_ccb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        for(int i=0;i<ninput_items_required.size();i++)
	    ninput_items_required[i] = noutput_items;
    }

    int
    insert_esti_block_ccb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
	gr_complex *out_data = (gr_complex *) output_items[0];
	unsigned char * out_flag = (unsigned char *) output_items[1];

        // Do <+signal processing+>
	int no=0;
	int ni=0;
	while(no<noutput_items){
	    if(d_db_count==0){
		for(int i=0;i<d_block_size;i++)
		    out_data[no*d_block_size+i] = d_esti_uw[i];
		
		if(d_eb_count == 0)
		    out_flag[no] = 1;
		else
		    out_flag[no] = 0;
		
		d_eb_count++;
		if(d_eb_count == d_eb_num){
		    d_eb_count = 0;
		    d_db_count++;
		}
	    }else{
		
		memcpy((char*)&out_data[no*d_block_size],(char*)&in[ni*d_block_size],
						d_block_size*sizeof(gr_complex));
		out_flag[no] = 0;
		
		d_db_count++;
		if(d_db_count > d_db_num)
		    d_db_count = 0;
		
		ni++;
	    }
	    
	    no++;
	}
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (ni);

        // Tell runtime system how many output items we produced.
        return no;
    }

  } /* namespace scfde */
} /* namespace gr */

