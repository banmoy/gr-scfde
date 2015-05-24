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
#include "insert_sync_block_cbc_impl.h"

namespace gr {
  namespace scfde {

   insert_sync_block_cbc::sptr
    insert_sync_block_cbc::make(int block_size)
    {
      return gnuradio::get_initial_sptr
        (new insert_sync_block_cbc_impl(block_size));
    }

    /*
     * The private constructor
     */
    insert_sync_block_cbc_impl::insert_sync_block_cbc_impl(int block_size)
      : gr::block("insert_sync_block_cbc",
             gr::io_signature::make2(1, 2, block_size*sizeof(gr_complex),sizeof(char)),
              gr::io_signature::make(1, 1, block_size*sizeof(gr_complex))),
	d_block_size(block_size),
	d_first(true)
    {
	d_ifft = new spinal::fft::fft_complex(d_block_size,false);
	d_uw_generator = new gen_uw("PN",d_block_size/2);
	
	if(1)
	    init_freq_sync_seq();
	else
	    init_time_sync_seq();
    }

    /*
     * Our virtual destructor.
     */
    insert_sync_block_cbc_impl::~insert_sync_block_cbc_impl()
    {
	delete d_ifft;
	delete d_uw_generator;
    }

    void
    insert_sync_block_cbc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        for(int i=0;i<ninput_items_required.size();i++)
	    ninput_items_required[0] = noutput_items;
    }

    // init freq sync
    void insert_sync_block_cbc_impl::init_freq_sync_seq()
    {
	d_sync_seq.resize(d_block_size);
	
	std::vector<gr_complex> pn_seq = d_uw_generator->get_seq();
	gr_complex tmp[d_block_size];
	for(int i=0;i<d_block_size;i+=2){
	    tmp[i] = pn_seq[i/2];
	    tmp[i+1] = gr_complex(0,0);
	}
	
	memcpy((char*)d_ifft->get_inbuf(),(char*)tmp,d_block_size*sizeof(gr_complex));
	d_ifft->execute();
	memcpy((char*)tmp,(char*)d_ifft->get_outbuf(),d_block_size*sizeof(gr_complex));
	
	float sum=0;
	for(int i=0;i<d_block_size;i++)
	    sum += norm(tmp[i]);	
	sum = sqrt(sum);
	
	for(int i=0;i<d_block_size;i++){
	   d_sync_seq[i] = tmp[i]*(float)sqrt(d_block_size)/(float)sum;
	}
    }
    
    //init time sync
    void insert_sync_block_cbc_impl::init_time_sync_seq()
    {
	d_sync_seq.resize(d_block_size);
	std::vector<gr_complex> tmp_seq = d_uw_generator->get_seq();
	for(int i=0;i<d_block_size/2;i++)
	    d_sync_seq[i] = d_sync_seq[i+d_block_size/2] = tmp_seq[i];
     }

    int
    insert_sync_block_cbc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in_data = (const gr_complex *) input_items[0];
	const unsigned char *in_flag = (const unsigned char *) input_items[1];
 	
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
	int no=0;
	int ni=0;
	while(no<noutput_items){
	    if((in_flag[ni]&0x01)){
		if(d_first){
		    for(int i=0;i<d_block_size;i++)
			out[no*d_block_size+i] = d_sync_seq[i];
		    d_first = false;
		}else{
		    memcpy((char*)&out[no*d_block_size],(char*)&in_data[ni*d_block_size],
				    d_block_size*sizeof(gr_complex));
		    d_first = true;
		    ni++;
		}
	    }else{
		 memcpy((char*)&out[no*d_block_size],(char*)&in_data[ni*d_block_size],
				    d_block_size*sizeof(gr_complex));
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

