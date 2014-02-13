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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "simple_mac_impl.h"

#define DEBUG 1
#define DBG DEBUG && std::cout

namespace gr {
namespace bats {

	simple_mac::sptr
	simple_mac::make(double cycle_dur, double data_slot_dur, 
			double location_slot_dur, int bandwidth, int chips_per_sym)
	{
		return gnuradio::get_initial_sptr
				(new simple_mac_impl(cycle_dur, data_slot_dur, location_slot_dur,
						bandwidth, chips_per_sym));
	}

    simple_mac_impl::simple_mac_impl(double cycle_dur, double data_slot_dur, double location_slot_dur,
			int bandwidth, int chips_per_sym)
			: gr::block("simple_mac",
					gr::io_signature::make(0, 0, 0),
					gr::io_signature::make(0, 0, 0)),
			d_thread(&simple_mac_impl::run, this), d_cycle_time(cycle_dur), d_stop(false),
			d_max_data((data_slot_dur * bandwidth) / (chips_per_sym/2)),
			d_max_total(pmt::from_long(((data_slot_dur + location_slot_dur) * bandwidth) / (chips_per_sym/2)))
    {
		message_port_register_in(pmt::mp("in"));
		set_msg_handler(pmt::mp("in"), boost::bind(&simple_mac_impl::handle_message, this, _1));
		message_port_register_out(pmt::mp("out"));
	}

	void
	simple_mac_impl::run(){
		//flowgraph startup delay
		boost::this_thread::sleep(boost::posix_time::milliseconds(500));
		//after startup: start sending in the next cycle
		gettimeofday(&d_cycle_start, NULL);
		update_cycle_start();
		boost::this_thread::sleep(boost::posix_time::milliseconds(d_cycle_time*1e3 - 150));
		while(!d_stop){
			pmt::pmt_t msg = q_pop();
		
			//time at which the lower layer should schedule the message	
			/*uhd::time_spec_t now = uhd::time_spec_t(d_cycle_start.tv_sec + d_cycle_start.tv_usec / 1e6);
			pmt::pmt_t time_value = pmt::make_tuple(
					pmt::from_uint64(now.get_full_secs()),
					pmt::from_double(now.get_frac_secs()));
			*/
			pmt::pmt_t time_value = pmt::make_tuple(
					pmt::from_uint64(d_cycle_start.tv_sec),
					pmt::from_double(d_cycle_start.tv_usec/1e6));
			
			//TODO: calc if msg fits in time slot	
			pmt::pmt_t dict, payload;
			std::string tx_string;

			if(pmt::is_pair(msg)){
				dict = pmt::car(msg);
				if(!pmt::is_dict(dict)){
					dict = pmt::make_dict();
				}
				payload = pmt::cdr(msg);
			} else if(pmt::is_symbol(msg)){
				tx_string = pmt::symbol_to_string(msg);
				payload = pmt::init_u8vector(tx_string.length(), (const uint8_t *)tx_string.c_str());
				dict = pmt::make_dict();
			} else {
				std::cerr << "MAC: invalid message format, expected PDU or PMT Symbol" << std::endl;
				continue;
			}	
			
			size_t tx_size = tx_string.length();
			size_t tx_size_encoded = 0;
			for(char c : tx_string){
				tx_size_encoded += ::strlen(varicodes::varicode_vals[c]);
				tx_size_encoded += 2; //character delimiter
			}
			
			tx_size_encoded += 8 - tx_size_encoded%8;
			tx_size_encoded += 16; //front + back padding

			DBG << "MAC: size of msg to send: " << (int)tx_size_encoded << std::endl;
			if(tx_size_encoded > d_max_data){
				std::cerr << "MAC: message will not fit into one tx slot, msg dropped (max allowed: " << d_max_data << ")" << std::endl;
				continue;
			}

			dict = pmt::dict_add(dict, pmt::mp("tx_time"), time_value);
			dict = pmt::dict_add(dict, pmt::mp("max_tx"), d_max_total);
			message_port_pub(pmt::mp("out"), pmt::cons(dict, payload));
			update_cycle_start();
			boost::this_thread::sleep(boost::posix_time::milliseconds(d_cycle_time*1e3 - 150));
		}			
	}

	void
	simple_mac_impl::handle_message(pmt::pmt_t msg){
		if(d_msg_queue.size() <= 10000){
			q_push(msg);
		} else {
			std::cerr << "MAC: msg queue is full, incoming msg dropped" << std::endl;
		}	
	}

	void
	simple_mac_impl::q_push(pmt::pmt_t msg){
		gr::thread::scoped_lock lock(d_mutex);
		d_msg_queue.push(msg);
		d_queue_not_empty.notify_one();
	}

	pmt::pmt_t
	simple_mac_impl::q_pop(){
		gr::thread::scoped_lock lock(d_mutex);

		while(d_msg_queue.size() == 0){
			d_queue_not_empty.wait(lock);
		}
		pmt::pmt_t msg = d_msg_queue.front();
		d_msg_queue.pop();
		return msg;
	}

	void
	simple_mac_impl::update_cycle_start(){
		d_cycle_start.tv_sec += int(d_cycle_time);
		d_cycle_start.tv_usec += int(fmod(d_cycle_time,1)*1e6);
		if(d_cycle_start.tv_usec >= int(1e6)){
			d_cycle_start.tv_usec -= int(1e6);
			d_cycle_start.tv_sec++;
		}
	}

    simple_mac_impl::~simple_mac_impl()
    {
		d_stop = true;
		d_thread.interrupt(); //if thread was sleeping
		d_thread.join();
    }

} /* namespace bats */
} /* namespace gr */

