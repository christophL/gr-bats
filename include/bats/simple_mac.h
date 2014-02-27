/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_BATS_SIMPLE_MAC_H
#define INCLUDED_BATS_SIMPLE_MAC_H

#include <bats/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace bats {

	class BATS_API simple_mac : virtual public gr::block
	{
		public:
			typedef boost::shared_ptr<simple_mac> sptr;

			static sptr make(double cycle_dur = 1.0, double data_slot_dur = 250e-6, 
					double location_slot_dur = 250e-6, int bandwidth = 200000);
	};

} // namespace bats
} // namespace gr

#endif /* INCLUDED_BATS_SIMPLE_MAC_H */

