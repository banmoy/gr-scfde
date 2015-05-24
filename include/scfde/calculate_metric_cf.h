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


#ifndef INCLUDED_SCFDE_CALCULATE_METRIC_CF_H
#define INCLUDED_SCFDE_CALCULATE_METRIC_CF_H

#include <scfde/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief calculate time metric
     * \ingroup scfde
     * \brief refer to Schmidl&Cox
     */
    class SCFDE_API calculate_metric_cf : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<calculate_metric_cf> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::calculate_metric_cf.
       *
       */
      static sptr make(int block_size);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_CALCULATE_METRIC_CF_H */

