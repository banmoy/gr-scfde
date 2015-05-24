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


#ifndef INCLUDED_SCFDE_INSERT_ESTI_BLOCK_CCB_H
#define INCLUDED_SCFDE_INSERT_ESTI_BLOCK_CCB_H

#include <scfde/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief insert uw for channel estimation
     * \ingroup scfde
     *
     */
    class SCFDE_API insert_esti_block_ccb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<insert_esti_block_ccb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::insert_esti_block_ccb.
       *
       * \param block_size    the size of a block
       * \param db_num	      insert esti blocks every db_num data blocks
       * \param eb_num	      insert eb_num esti blocks every time   
       */
      static sptr make(int block_size=256,int db_num=5,int eb_num=1);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_INSERT_ESTI_BLOCK_CCB_H */

