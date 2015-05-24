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


#ifndef INCLUDED_SCFDE_BLOCK_SOURCE_C_H
#define INCLUDED_SCFDE_BLOCK_SOURCE_C_H

#include <scfde/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief generate source data for equalize simulation
     * \ingroup scfde
     *
     */
    class SCFDE_API block_source_c : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<block_source_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::block_source_c.
       *
       * \param block_size the size of block_size
       * \param blk_num the number of blocks
       */
      static sptr make(int block_size,int blk_num);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_BLOCK_SOURCE_C_H */

