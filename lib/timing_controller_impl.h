/* -*- c++ -*- */
/* 
 * Copyright 2013 Christoph Leitner <c.leitner@student.uibk.ac.at>.
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

#ifndef INCLUDED_BATS_TIMING_CONTROLLER_IMPL_H
#define INCLUDED_BATS_TIMING_CONTROLLER_IMPL_H

#include <bats/timing_controller.h>

namespace gr {
namespace bats {

	class timing_controller_impl : public timing_controller
	{
		private:
			double d_cycle_time;
			double d_slot_time;
			double d_startup_time;
			struct timeval d_cycle_start;

			void update_cycle_start();
			void add_sob(uint64_t item);
			void add_eob(uint64_t item);
		public:
			timing_controller_impl(double cycle_dur, double slot_dur, double startup_dur);
			~timing_controller_impl();

			int work(int noutput_items,
					gr_vector_const_void_star &input_items,
					gr_vector_void_star &output_items);
	};

} // namespace bats
} // namespace gr

#endif /* INCLUDED_BATS_TIMING_CONTROLLER_IMPL_H */

