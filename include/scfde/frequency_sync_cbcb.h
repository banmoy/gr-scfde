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


#ifndef INCLUDED_SCFDE_FREQUENCY_SYNC_CBCB_H
#define INCLUDED_SCFDE_FREQUENCY_SYNC_CBCB_H

#include <scfde/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief synchronization for frequence
     * \ingroup scfde
     * \brief refer to Schmidl&Cox and Lim
     */
    class SCFDE_API frequency_sync_cbcb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<frequency_sync_cbcb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::frequency_sync_cbcb.
       *
       * To avoid accidental use of raw pointers, scfde::frequency_sync_cbcb's
       * constructor is in a private implementation
       * class. scfde::frequency_sync_cbcb::make is the public interface for
       * creating new instances.
       */
      static sptr make(int block_size,int cp_size);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_FREQUENCY_SYNC_CBCB_H */

