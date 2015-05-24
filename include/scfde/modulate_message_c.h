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


#ifndef INCLUDED_SCFDE_MODULATE_MESSAGE_C_H
#define INCLUDED_SCFDE_MODULATE_MESSAGE_C_H

#include <scfde/api.h>
#include <gnuradio/sync_block.h>
#include <gnuradio/msg_queue.h>
#include <string>

namespace gr {
  namespace scfde {

    /*!
     * \brief modulate messages from msg_queue
     * \ingroup scfde
     *
     */
    class SCFDE_API modulate_message_c : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<modulate_message_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::modulate_message_c.
       *
       * \param block_size the size of the modulated message
       * \param mod_type   the type of modulation
       * \param msgq_limit refer to msg_queue.h  
       * \param block_num  if not -1, return -1 from work() until the number of modulated 
       * 		   messages is block_num
       */
      static sptr make(int block_size,std::string mod_type,int unsigned msgq_limit = 2,int block_num=-1);
       virtual gr::msg_queue::sptr msgq() const = 0;
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_MODULATE_MESSAGE_C_H */

