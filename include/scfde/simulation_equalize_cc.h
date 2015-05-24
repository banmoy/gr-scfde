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


#ifndef INCLUDED_SCFDE_SIMULATION_EQUALIZE_CC_H
#define INCLUDED_SCFDE_SIMULATION_EQUALIZE_CC_H

#include <scfde/api.h>
#include <gnuradio/sync_block.h>
#include <string>
#include <vector>

namespace gr {
  namespace scfde {

    /*!
     * \brief <+description of block+>
     * \ingroup scfde
     *
     */
    class SCFDE_API simulation_equalize_cc : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<simulation_equalize_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::simulation_equalize_cc.
       *
       * To avoid accidental use of raw pointers, scfde::simulation_equalize_cc's
       * constructor is in a private implementation
       * class. scfde::simulation_equalize_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int block_size,int cp_size,std::string equal_method,
		       std::vector<gr_complex> taps,float snr);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_SIMULATION_EQUALIZE_CC_H */

