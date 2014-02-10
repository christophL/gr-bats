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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "simple_mac_impl.h"

namespace gr {
  namespace bats {

    simple_mac::sptr
    simple_mac::make(double cycle_dur, double data_slot_dur, double location_slot_dur)
    {
      return gnuradio::get_initial_sptr
        (new simple_mac_impl(cycle_dur, data_slot_dur, location_slot_dur));
    }

    simple_mac_impl::simple_mac_impl(double cycle_dur, double data_slot_dur, double location_slot_dur)
			: gr::block("simple_mac",
					gr::io_signature::make(0, 0, 0),
					gr::io_signature::make(0, 0, 0)),
			d_thread(&simple_mac_impl::run, this), d_cycle_time(cycle_dur), 
			d_data_slot_time(data_slot_dur), d_loc_slot_time(location_slot_dur),
			d_stop(false)
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
			uhd::time_spec_t now = uhd::time_spec_t(d_cycle_start.tv_sec + d_cycle_start.tv_usec / 1e6);
			pmt::pmt_t time_value = pmt::make_tuple(
					pmt::from_uint64(now.get_full_secs()),
					pmt::from_double(now.get_frac_secs()));

			//TODO: calc if msg fits in time slot	
			if(pmt::is_pair(msg)){
				pmt::pmt_t dict = pmt::car(msg);
				if(!pmt::is_dict(dict)){
					dict = pmt::make_dict();
				}
				pmt::pmt_t payload = pmt::cdr(msg);
				
				dict = pmt::dict_add(dict, pmt::mp("tx_time"), time_value);
				message_port_pub(pmt::mp("out"), pmt::cons(dict, payload));
			} else if(pmt::is_symbol(msg)){
				pmt::pmt_t dict = pmt::make_dict();
				dict = pmt::dict_add(dict, pmt::mp("tx_time"), time_value);
				message_port_pub(pmt::mp("out"), pmt::cons(dict, msg));
			} else {
				std::cerr << "Bats Simple MAC: invalid message format, expected PDU or PMT Symbol" << std::endl;
				continue;
			}	
			
			update_cycle_start();
			boost::this_thread::sleep(boost::posix_time::milliseconds(d_cycle_time*1e3 - 150));
		}			
	}

	void
	simple_mac_impl::handle_message(pmt::pmt_t msg){
		if(d_msg_queue.size() <= 10000){
			q_push(msg);
		} else {
			std::cerr << "Bats simple MAC: msg queue is full, incoming msg dropped" << std::endl;
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

