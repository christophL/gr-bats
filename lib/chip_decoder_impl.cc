/* -*- c++ -*- */
/* 
 * Copyright 2013 <c.leitner@student.uibk.ac.at>.
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
#define DEBUG 1
#define DBG DEBUG && std::cout

namespace gr {
namespace bats {
	chip_decoder_impl::chip_decoder_impl(unsigned chips_per_sym)
			: sync_decimator("chip_decoder",
					io_signature::make(1, 1, sizeof(char)),
					io_signature::make(1, 1, sizeof(char)),
					chips_per_sym),
			d_prev_samp(23), d_prev_out(1), d_chips_per_sym(chips_per_sym),
			d_written(false)
	{
		if(chips_per_sym == 0)
			throw std::out_of_range("chips per symbol must be > 0");
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
		for(int i = 0; i < noutput_items; i++){
			d_written = false;
			int base = i*d_chips_per_sym;
			for(int j = 0; j < d_chips_per_sym; j++){
				DBG << boost::to_string((int)in[base+j]);
				if(d_prev_samp > 1){
					d_prev_samp = in[base+j];
					continue;
				}
				if(d_prev_samp == in[base+j]){
					out[i] = d_prev_out = !d_prev_samp;
					d_written = true;
					//DBG << "==" << boost::to_string(d_prev_out) << " ";
					break;
				}
			}
			if(!d_written){
				d_prev_samp = in[base+d_chips_per_sym-1];
				out[i] = d_prev_out;
				//DBG << "!=" << boost::to_string(d_prev_out) << " ";
			}
		}
		DBG << std::endl;

		return noutput_items;
	}

	chip_decoder::sptr
	chip_decoder::make(unsigned chips_per_sym)
	{
		return gnuradio::get_initial_sptr(new chip_decoder_impl(chips_per_sym));
	}

} /* namespace bats */
} /* namespace gr */

