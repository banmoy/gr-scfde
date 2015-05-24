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


#ifndef INCLUDED_SCFDE_SIMULATION_CFO_CF_H
#define INCLUDED_SCFDE_SIMULATION_CFO_CF_H

#include <scfde/api.h>
#include <gnuradio/sync_block.h>
#include <string>

namespace gr {
  namespace scfde {

    /*!
     * \brief simulation for frequence synchronization
     * \ingroup scfde
     *
     */
    class SCFDE_API simulation_cfo_cf : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<simulation_cfo_cf> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::simulation_cfo_cf.
       *
       * \param block_size the size of block_size
       * \param cp_size the size of cp
       * \param interval the number of blocks between sync blocks including sync block itself
       * \param method the method used for synchronization : "SC","Lim"
       */
      static sptr make(int block_size,int cp_size,int interval,std::string method);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_SIMULATION_CFO_CF_H */

