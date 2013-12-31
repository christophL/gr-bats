/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_BATS_TDMA_CONTROLLER_H
#define INCLUDED_BATS_TDMA_CONTROLLER_H

#include <bats/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace bats {

    /*!
     * \brief <+description of block+>
     * \ingroup bats
     *
     */
    class BATS_API tdma_controller : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<tdma_controller> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of bats::tdma_controller.
       *
       * To avoid accidental use of raw pointers, bats::tdma_controller's
       * constructor is in a private implementation
       * class. bats::tdma_controller::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace bats
} // namespace gr

#endif /* INCLUDED_BATS_TDMA_CONTROLLER_H */

