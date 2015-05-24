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


#ifndef INCLUDED_SCFDE_FREQUENCY_DOMAIN_EQUALIZE_CBCC_H
#define INCLUDED_SCFDE_FREQUENCY_DOMAIN_EQUALIZE_CBCC_H

#include <scfde/api.h>
#include <gnuradio/block.h>
#include <string>

namespace gr {
  namespace scfde {

    /*!
     * \brief channel estimation and equalizer
     * \ingroup scfde
     *
     */
    class SCFDE_API frequency_domain_equalize_cbcc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<frequency_domain_equalize_cbcc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::frequency_domain_equalize_cbcc.
       *
       * \param block_size the size of block
       * \param cp_size the size of cp
       * \param eb_num the number of uw 
       * \param equal_method MMSE or LS
       * \param SNR init the snr in dB
       */
      static sptr make(int block_size,int cp_size,int eb_num,std::string equal_method,float SNR);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_FREQUENCY_DOMAIN_EQUALIZE_CBCC_H */

