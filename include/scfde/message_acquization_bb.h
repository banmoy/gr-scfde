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


#ifndef INCLUDED_SCFDE_MESSAGE_ACQUIZATION_BB_H
#define INCLUDED_SCFDE_MESSAGE_ACQUIZATION_BB_H

#include <scfde/api.h>
#include <gnuradio/sync_block.h>
#include <gnuradio/msg_queue.h>

namespace gr {
  namespace scfde {

    /*!
     * \brief pack bytes to message and insert to msg_queue
     * \ingroup scfde
     *
     */
    class SCFDE_API message_acquization_bb : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<message_acquization_bb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of scfde::message_acquization_bb.
       *
       * \param message_queue the queue of messages
       * \param frame_size the size of a message
       */
      static sptr make(msg_queue::sptr message_queue,int frame_size);
    };

  } // namespace scfde
} // namespace gr

#endif /* INCLUDED_SCFDE_MESSAGE_ACQUIZATION_BB_H */

