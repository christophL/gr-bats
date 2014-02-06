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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "timing_controller_impl.h"

namespace gr {
namespace bats {

	timing_controller::sptr
	timing_controller::make(double cycle_dur, double slot_dur, double startup_dur)
	{
		return gnuradio::get_initial_sptr
				(new timing_controller_impl(cycle_dur, slot_dur, startup_dur));
	}

	timing_controller_impl::timing_controller_impl(double cycle_dur, double slot_dur, double startup_dur)
			: gr::sync_block("timing_controller",
					gr::io_signature::make(1, 1, sizeof(gr_complex)),
					gr::io_signature::make(1, 1, sizeof(gr_complex))),
			d_cycle_time(cycle_dur), d_slot_time(slot_dur), d_startup_time(startup_dur)
    {
		gettimeofday(&d_cycle_start, NULL);
		update_cycle_start();
	}

	timing_controller_impl::~timing_controller_impl()
	{
	}

	void
	timing_controller_impl::update_cycle_start(){
		d_cycle_start.tv_sec += int(d_cycle_time);
		d_cycle_start.tv_usec += int(fmod(d_cycle_time,1)*1e6);
		if(d_cycle_start.tv_usec >= int(1e6)){
			d_cycle_start.tv_usec -= int(1e6);
			d_cycle_start.tv_sec++;
		}
	}

	void
	timing_controller_impl::add_sob(uint64_t item){
		static const pmt::pmt_t sob_key = pmt::string_to_symbol("tx_sob");
		static const pmt::pmt_t sob_val = pmt::PMT_T;
		static const pmt::pmt_t srcid = pmt::string_to_symbol(alias());
		
		add_item_tag(0, item, sob_key, sob_val, srcid);
	}

	void
	timing_controller_impl::add_eob(uint64_t item){
		static const pmt::pmt_t eob_key = pmt::string_to_symbol("tx_eob");
		static const pmt::pmt_t eob_val = pmt::PMT_T;
		static const pmt::pmt_t srcid = pmt::string_to_symbol(alias());
		
		add_item_tag(0, item, eob_key, eob_val, srcid);
	}

	int
	timing_controller_impl::work(int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
	{
		//std::vector<const gr_complex *> in = static_cast<const std::vector<const gr_complex *> >(input_items);
		//std::vector<gr_complex *> out = static_cast<const std::vector<gr_complex *> >(output_items);

		/*
		 * Ideas:
		 * - put the timing controller directly after the PSK31 encoder in the flowgraph
		 * - add length tags to stream coming from PSK31 encoder block?
		 * - buffer "packets"
		 * - calculate max number of samples that can be sent in one slot
		 * 		- don't split "packets" to different slots?
		 * - use gettimeofday to determine when to send the data 
		 */
		struct timeval t;
		gettimeofday(&t, NULL);

		int ninput = noutput_items;
		const uint64_t nread = nitems_read(0);
		std::vector<gr::tag_t> tags;
		get_tags_in_range(tags, 0, nread, nread + ninput);
		std::sort(tags.begin(), tags.end(), tag_t::offset_compare);

		// Tell runtime system how many output items we produced.
		return noutput_items;
	}

} /* namespace bats */
} /* namespace gr */

