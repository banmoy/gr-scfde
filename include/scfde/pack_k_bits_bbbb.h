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


#ifndef INCLUDED_SCFDE_PACK_K_BITS_BBBB_H
#define INCLUDED_SCFDE_PACK_K_BITS_BBBB_H

#include <scfde/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief packe item with k bits to bytes
     * \ingroup scfde
     *
     */
    class SCFDE_API pack_k_bits_bbbb : virtual public gr::sync_decimator
    {
     public:
      typedef boost::shared_ptr<pack_k_bits_bbbb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::pack_k_bits_bbbb.
       *
       * \param k one item represents k bits
       */
      static sptr make(unsigned k);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_PACK_K_BITS_BBBB_H */

