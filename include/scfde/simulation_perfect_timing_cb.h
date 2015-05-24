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


#ifndef INCLUDED_SCFDE_SIMULATION_PERFECT_TIMING_CB_H
#define INCLUDED_SCFDE_SIMULATION_PERFECT_TIMING_CB_H

#include <scfde/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief <+description of block+>
     * \ingroup scfde
     *
     */
    class SCFDE_API simulation_perfect_timing_cb : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<simulation_perfect_timing_cb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::simulation_perfect_timing_cb.
       *
       * \param num the number of blocks in a frame
       */
      static sptr make(int block_size,int cp_size,int num);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_SIMULATION_PERFECT_TIMING_CB_H */

