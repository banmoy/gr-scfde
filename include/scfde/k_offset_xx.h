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


#ifndef INCLUDED_SCFDE_K_OFFSET_XX_H
#define INCLUDED_SCFDE_K_OFFSET_XX_H

#include <scfde/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief 
     * \ingroup scfde
     *
     */
    class SCFDE_API k_offset_xx : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<k_offset_xx> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::k_offset_xx.
       *
       * To avoid accidental use of raw pointers, scfde::k_offset_xx's
       * constructor is in a private implementation
       * class. scfde::k_offset_xx::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t size,int offset=0);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_K_OFFSET_XX_H */

