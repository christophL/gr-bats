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

#include <gnuradio/io_signature.h>
#include "chip_decoder_impl.h"
#define DEBUG 0
#define DBG DEBUG && std::cout

namespace gr {
namespace bats {
	chip_decoder_impl::chip_decoder_impl()
			: sync_block("chip_decoder",
					io_signature::make(1, 1, sizeof(char)),
					io_signature::make(1, 1, sizeof(char)))
	{
	}

	chip_decoder_impl::~chip_decoder_impl()
	{}

	int
	chip_decoder_impl::work(int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
	{
		const char *in = static_cast<const char *>(input_items[0]);
		char *out = static_cast<char *>(output_items[0]);
		uint64_t read = nitems_read(0);
		DBG << "output items: " << noutput_items << std::endl;
		for(int i = 0; i < noutput_items; i++){
			if((i+ read)%2 && !in[i] || !((i + read) % 2) && in[i]){
				out[i] = -1;
			} else {
				out[i] = 1;
			}
		}

		return noutput_items;
	}

	chip_decoder::sptr
	chip_decoder::make()
	{
		return gnuradio::get_initial_sptr(new chip_decoder_impl());
	}

} /* namespace bats */
} /* namespace gr */

