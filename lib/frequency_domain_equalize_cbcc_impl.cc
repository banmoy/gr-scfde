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
#include "frequency_domain_equalize_cbcc_impl.h"
#include <gnuradio/gr_complex.h>
#include <gnuradio/expj.h>
#include <vector>

namespace gr {
  namespace scfde {

    #define PI 3.141592657

    frequency_domain_equalize_cbcc::sptr
    frequency_domain_equalize_cbcc::make(int block_size,int cp_size,int eb_num,std::string equal_method,float SNR)
    {
      return gnuradio::get_initial_sptr
        (new frequency_domain_equalize_cbcc_impl(block_size,cp_size,eb_num,equal_method,SNR));
    }

    /*
     * The private constructor
     */
    frequency_domain_equalize_cbcc_impl::frequency_domain_equalize_cbcc_impl(
				int block_size,int cp_size,int eb_num,std::string equal_method,float SNR)
      : gr::block("frequency_domain_equalize_cbcc",
              gr::io_signature::make2(1, 2, block_size*sizeof(gr_complex),sizeof(char)),
              gr::io_signature::make2(1, 2, block_size*sizeof(gr_complex),
					    block_size*sizeof(gr_complex))),
	d_block_size(block_size),
	d_cp_size(cp_size),
	d_eb_num(eb_num),
	d_count(eb_num)
    {
	d_uw_generator = new gen_uw("Chu",d_block_size);
	d_fft = new spinal::fft::fft_complex(d_block_size,true);
	d_ifft = new spinal::fft::fft_complex(d_block_size,false);
	d_taps = new gr_complex[d_block_size];
	d_taps_fft = new gr_complex[d_block_size];
	
	d_SNR = std::pow(10.0,SNR/10.0);
	
	int pos = equal_method.find("_");
	d_ce_method = equal_method.substr(0,pos);
	d_equal_method = equal_method.substr(pos+1);
	
	init_uw_fft();
	reset_taps_fft();
	
	d_block_count = 0;
		
// 	message_port_register_in(pmt::mp("snr_in_port"));
// 	set_msg_handler(pmt::mp("snr_in_port"),
// 			boost::bind(&frequency_domain_equalize_cbcc_impl::set_snr,this,_1));
    }

    /*
     * Our virtual destructor.
     */
    frequency_domain_equalize_cbcc_impl::~frequency_domain_equalize_cbcc_impl()
    {
	delete d_uw_generator;
	delete d_fft;
	delete d_ifft;
	delete d_taps;
	delete d_taps_fft;
	delete d_uw_fft;
// 	std::cout<<d_block_count<<std::endl;
    }

    void
    frequency_domain_equalize_cbcc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
	for(int i=0;i<ninput_items_required.size();i++)
	    ninput_items_required[i] = noutput_items;
    }
    
    void frequency_domain_equalize_cbcc_impl::set_snr(pmt::pmt_t msg)
    {
	gr::thread::scoped_lock guard(d_mutex);
	double SNR = pmt::to_double(msg);
	d_SNR = std::pow(10.0,SNR/10.0);
    }
    
    void frequency_domain_equalize_cbcc_impl::init_uw_fft()
    {
	d_uw_fft = new gr_complex[d_block_size];
	
	std::vector<gr_complex> uw = d_uw_generator->get_seq();
	for(int i=0;i<d_block_size;i++)
	    d_uw_fft[i] = uw[i];
	memcpy((char*)d_fft->get_inbuf(),(char*)d_uw_fft,
			d_block_size*sizeof(gr_complex));
	d_fft->execute();
	memcpy((char*)d_uw_fft,(char*)d_fft->get_outbuf(),
			d_block_size*sizeof(gr_complex));
    }
    
    void frequency_domain_equalize_cbcc_impl::reset_taps_fft()
    {
	for(int i=0;i<d_block_size;i++)
	    d_taps_fft[i] = gr_complex(0,0);
	
	for(int i=0;i<d_block_size;i++)
	    d_taps[i] = gr_complex(0,0);
    }
    
    void frequency_domain_equalize_cbcc_impl::ls_ce(gr_complex* tmp)
    {
	for(int i=0;i<d_block_size;i++)
	    d_taps_fft[i] += tmp[i]/d_uw_fft[i];
    }
    
    
    void frequency_domain_equalize_cbcc_impl::equalize(gr_complex* tmp)
    {
	if(d_equal_method=="MMSE")
	    mmse_equalize(tmp);
	if(d_equal_method=="LS")
	    ls_equalize(tmp);
	if(d_equal_method == "None")
	    ;
    }
    
    void frequency_domain_equalize_cbcc_impl::mmse_equalize(gr_complex* tmp)
    {
	for(int i=0;i<d_block_size;i++){
	    float abs_n = norm(d_taps_fft[i]);
	    tmp[i] *= conj(d_taps_fft[i])/(abs_n+1/d_SNR);
	}
    }
    
    void frequency_domain_equalize_cbcc_impl::ls_equalize(gr_complex* tmp)
    {
	for(int i=0;i<d_block_size;i++)
	    tmp[i] /= d_taps_fft[i];
    }

    int
    frequency_domain_equalize_cbcc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in_data = (const gr_complex *) input_items[0];
	const unsigned char *in_flag = (const unsigned char *) input_items[1];
        gr_complex *out_data = (gr_complex *) output_items[0];
	gr_complex *out_tap = (gr_complex *) output_items[1];

	// Do <+signal processing+>
	int ni=0;
	int no=0;
	int in_items = std::min(ninput_items[0],ninput_items[1]);
	while(ni<in_items && no<noutput_items){
	    if((in_flag[ni]&0x01) || d_count<d_eb_num){
		if(in_flag[ni]&0x01){
		    reset_taps_fft();
		    d_count = 0;
		    d_block_count++;
		}
		gr_complex tmp[d_block_size];
		memcpy((char*)d_fft->get_inbuf(),(char*)&in_data[ni*d_block_size],
			    d_block_size*sizeof(gr_complex));
		d_fft->execute();
		memcpy((char*)tmp,(char*)d_fft->get_outbuf(),
			    d_block_size*sizeof(gr_complex));
		ls_ce(tmp);
		d_count++;
		
		if(d_count == d_eb_num){
		    for(int i=0;i<d_block_size;i++)
			d_taps_fft[i] /= (float)d_eb_num;
		    
		    if(d_ce_method == "HLS"){
			memcpy((char*)d_ifft->get_inbuf(),(char*)d_taps_fft,
					d_block_size*sizeof(gr_complex));
			d_ifft->execute();
			memcpy((char*)d_taps,(char*)d_ifft->get_outbuf(),
					d_block_size*sizeof(gr_complex));
			for(int i=0;i<d_block_size;i++)
			    d_taps[i] /= (float)d_block_size;
		    
			for(int i=d_cp_size;i<d_block_size;i++)
			    d_taps[i] = gr_complex(0,0);
		    
			memcpy((char*)d_fft->get_inbuf(),(char*)d_taps,
					d_block_size*sizeof(gr_complex));
			d_fft->execute();
			memcpy((char*)d_taps_fft,(char*)d_fft->get_outbuf(),
					d_block_size*sizeof(gr_complex)); 
		    } 
		}
		ni++;
	    }else{
		gr_complex tmp[d_block_size];
		memcpy((char*)d_fft->get_inbuf(),(char*)&in_data[ni*d_block_size],
			    d_block_size*sizeof(gr_complex));
		d_fft->execute();
		memcpy((char*)tmp,(char*)d_fft->get_outbuf(),
			    d_block_size*sizeof(gr_complex));
		
		equalize(tmp);
		
		memcpy((char*)d_ifft->get_inbuf(),(char*)tmp,
			    d_block_size*sizeof(gr_complex));
		d_ifft->execute();
		memcpy((char*)tmp,(char*)d_ifft->get_outbuf(),
			    d_block_size*sizeof(gr_complex));
		for(int i=0;i<d_block_size;i++)
			tmp[i] /= (float)d_block_size;
		memcpy((char*)&out_data[no*d_block_size],(char*)tmp,
			    d_block_size*sizeof(gr_complex));
		
		memcpy((char*)&out_tap[no*d_block_size],(char*)d_taps,
			    d_block_size*sizeof(gr_complex));
		
		ni++;
		no++;
	    }
	}
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (ni);

        // Tell runtime system how many output items we produced.
        return no;
    }


  } /* namespace scfde */
} /* namespace gr */

