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


#ifndef INCLUDED_SCFDE_DEMOD_MAPPER_CBB_H
#define INCLUDED_SCFDE_DEMOD_MAPPER_CBB_H

#include <scfde/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief demodulate the block to bit stream with flag
     * \ingroup scfde
     *
     */
    class SCFDE_API demod_mapper_cbb : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<demod_mapper_cbb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::demod_mapper_cbb.
       *
       * \param demod_type the type of demodulation
       * \param data_size the size of block
       */
      static sptr make(std::string demod_type,int data_size);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_DEMOD_MAPPER_CBB_H */

