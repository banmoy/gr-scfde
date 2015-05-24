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


#ifndef INCLUDED_SCFDE_GEN_UW_H
#define INCLUDED_SCFDE_GEN_UW_H

#include <scfde/api.h>
#include <gnuradio/gr_complex.h>
#include <vector>
#include <string>

namespace gr {
  namespace scfde {

    /*!
     * \brief generate a specific training sequence
     * \param uw_type	the type of the sequence: "PN","Frank","Chu"
     * \param len	the length of the sequence
     */
    class SCFDE_API gen_uw
    {
	public:
	    gen_uw(std::string uw_type="Chu",int len=256);
	    ~gen_uw();

 	    std::vector<gr_complex> get_seq();
	    std::vector<gr_complex> gen_seq();
	    void set_bgn(int bgn);
	    void set_len(int len);
	    void set_type(std::string uw_type);
	    
	private:
	    int d_len;
	    int d_M;		//useful for chu sequence, d_M and d_len are relatively prime
	    int d_P;		//useful for frank sequence, d_P and  sqrt(d_len) are relatively prime
	    int d_bgn;		//useful for pn sequence, use the data after d_bgn in the pn_array
	    std::string d_uw_type;
	    std::vector<gr_complex> d_seq;
	    
	    void gen_chu_seq();
	    void gen_frank_seq();
	    void gen_pn_seq();
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_GEN_UW_H */

