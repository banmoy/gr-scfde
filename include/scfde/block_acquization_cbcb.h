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


#ifndef INCLUDED_SCFDE_BLOCK_ACQUIZATION_CBCB_H
#define INCLUDED_SCFDE_BLOCK_ACQUIZATION_CBCB_H

#include <scfde/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief get frame from data stream
     * \ingroup scfde
     *
     */
    class SCFDE_API block_acquization_cbcb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<block_acquization_cbcb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::block_acquization_cbcb.
       *
       * \param block_size the size of block
       * \param cp_size the size of cp 
       * \param db_num the number of data blocks in a frame
       * \param eb_num the number of uw in a frame
       */
      static sptr make(int block_size,int cp_size,int db_num,int eb_num);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_BLOCK_ACQUIZATION_CBCB_H */

