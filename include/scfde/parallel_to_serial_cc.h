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


#ifndef INCLUDED_SCFDE_PARALLEL_TO_SERIAL_CC_H
#define INCLUDED_SCFDE_PARALLEL_TO_SERIAL_CC_H

#include <scfde/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief 
     * \ingroup scfde
     *
     */
    class SCFDE_API parallel_to_serial_cc : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<parallel_to_serial_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::parallel_to_serial_cc.
       *
       * To avoid accidental use of raw pointers, scfde::parallel_to_serial_cc's
       * constructor is in a private implementation
       * class. scfde::parallel_to_serial_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int ratio);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_PARALLEL_TO_SERIAL_CC_H */

