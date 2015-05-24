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


#ifndef INCLUDED_SCFDE_SERIAL_TO_PARALLEL_CC_H
#define INCLUDED_SCFDE_SERIAL_TO_PARALLEL_CC_H

#include <scfde/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief <+description of block+>
     * \ingroup scfde
     *
     */
    class SCFDE_API serial_to_parallel_cc : virtual public gr::sync_decimator
    {
     public:
      typedef boost::shared_ptr<serial_to_parallel_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::serial_to_parallel_cc.
       *
       * To avoid accidental use of raw pointers, scfde::serial_to_parallel_cc's
       * constructor is in a private implementation
       * class. scfde::serial_to_parallel_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int ratio);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_SERIAL_TO_PARALLEL_CC_H */

