/* -*- c++ -*- */
/* 
 * Copyright 2014 <c.leitner@student.uibk.ac.at>.
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

#ifndef INCLUDED_BATS_SIMPLE_MAC_IMPL_H
#define INCLUDED_BATS_SIMPLE_MAC_IMPL_H

#include <bats/simple_mac.h>
#include <queue>
#include <uhd/types/time_spec.hpp>
#include "varicodes.h"

namespace gr {
namespace bats {
	class simple_mac_impl : public simple_mac
	{
		private:
			const double d_cycle_time;
			const size_t d_max_data;
			const pmt::pmt_t d_max_total;
			struct timeval d_cycle_start;

			std::queue<pmt::pmt_t> d_msg_queue;
			gr::thread::thread d_thread;
			gr::thread::mutex d_mutex;
			gr::thread::condition_variable d_queue_not_empty;
			volatile bool d_stop;

			void q_push(pmt::pmt_t msg);
			pmt::pmt_t q_pop();
			void run();
			void handle_message(pmt::pmt_t msg);
			void update_cycle_start();
		public:
			simple_mac_impl(double cycle_dur, double data_slot_dur, 
					double location_slot_dur, int bandwidth, int chips_per_sym);
			~simple_mac_impl();
	};

} // namespace bats
} // namespace gr

#endif /* INCLUDED_BATS_SIMPLE_MAC_IMPL_H */

