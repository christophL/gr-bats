#include "psk31_encoder_impl.h"
#include <gnuradio/io_signature.h>

#define DEBUG 1
#define DBG DEBUG && std::cout

using namespace gr::bats;

psk31_encoder_impl::psk31_encoder_impl()
		: sync_block("psk31_encoder",
				gr::io_signature::make(0, 0, 0),
				gr::io_signature::make(1, 1, sizeof(uint8_t))),
		d_pad(0), d_idle(true)
{
	
	message_port_register_in(pmt::mp("in"));
}

void
psk31_encoder_impl::add_sob(uint64_t item){
	DBG << "inserted sob/tx_time at: " << item << std::endl;
	static const pmt::pmt_t sob_key = pmt::string_to_symbol("tx_sob");
	static const pmt::pmt_t time_key = pmt::string_to_symbol("tx_time");
	static const pmt::pmt_t sob_val = pmt::PMT_T;
	static const pmt::pmt_t srcid = pmt::string_to_symbol(alias());

	DBG << "tx_time: " << (long)pmt::to_uint64(pmt::tuple_ref(d_tx_time, 0)) << "," << pmt::to_double(pmt::tuple_ref(d_tx_time, 1)) << std::endl;
	add_item_tag(0, item, sob_key, sob_val, srcid);
	add_item_tag(0, item, time_key, d_tx_time, srcid);
}

void
psk31_encoder_impl::add_eob(uint64_t item){
	DBG << "inserted eob at: " << item << std::endl;
	static const pmt::pmt_t eob_key = pmt::string_to_symbol("tx_eob");
	static const pmt::pmt_t eob_val = pmt::PMT_T;
	static const pmt::pmt_t srcid = pmt::string_to_symbol(alias());

	add_item_tag(0, item, eob_key, eob_val, srcid);
}

void
psk31_encoder_impl::encode_string(std::string &str){
	std::string tmp("00000000");

	if(str.length() <= 1) return;

	for(char c : str){
		if(c == '\n'){
			break;
		}
		tmp += varicodes::varicode_vals[c];
		tmp += "00";
	}
	tmp += varicodes::varicode_vals['\n'];
	tmp += "00";

	while(tmp.length() % 8 != 0)
            tmp += "0";

	d_tx_string += tmp;
	d_tx_string += "00000000";
	DBG << "tx_string:" + d_tx_string << std::endl;
}

void
psk31_encoder_impl::handle_msg(pmt::pmt_t msg){
	if(pmt::is_pair(msg)){
		pmt::pmt_t blob = pmt::cdr(msg);
		pmt::pmt_t dict = pmt::car(msg);

		if(pmt::dict_has_key(dict, pmt::mp("tx_time"))){
			d_tx_time = pmt::dict_ref(dict, pmt::mp("tx_time"), pmt::PMT_F);
		}
		if(pmt::dict_has_key(dict, pmt::mp("max_tx"))){
			d_max_tx = pmt::to_long(pmt::dict_ref(dict, pmt::mp("max_tx"), pmt::PMT_F));
		}
		std::string s(static_cast<const char *>(pmt::blob_data(blob)), pmt::blob_length(blob));
		encode_string(s);
		if(d_tx_string.length()){
			d_pad = d_max_tx - d_tx_string.length();
			d_pad += 8 - d_pad%8;
			DBG << "padding with " << d_pad << " 0s" << std::endl;
		}
	} else {
		std::cout << "unrecognized msg format - expected PDU" << std::endl;
	}
}

int
psk31_encoder_impl::work(int noutput, gr_vector_const_void_star& input_items,
		gr_vector_void_star& output_items){

	char *out = static_cast<char *>(output_items[0]);
	int items = 0;

	if(d_idle){
		pmt::pmt_t msg(delete_head_nowait(pmt::mp("in")));
		if(!msg.get()){
			return 0;
		}
		handle_msg(msg);
		if(!d_tx_string.length()){
			return 0;
		}
		add_sob(nitems_written(0));
		d_idle = false;
	}

	for(int i = 0; i < noutput; i++){
		if(d_tx_string.length() != 0){
			std::string to_convert = d_tx_string.substr(0,8);
			unsigned char u8 = ~std::stoul(to_convert, nullptr, 2);
			out[i] = u8;
			d_tx_string = d_tx_string.erase(0,8);
			items++;
			//DBG << "remaining string: " + d_tx_string << std::endl;
		} else if(d_pad){
			out[i] = 0;
			items++;
			d_pad-=8;
		} else {
			break;
		}
	}

	if(!d_pad && d_tx_string.length() == 0){
		add_eob(nitems_written(0) + items - 1);
		d_idle = true;
	}

	return items;
}

psk31_encoder::sptr
psk31_encoder::make(){
	return gnuradio::get_initial_sptr(new psk31_encoder_impl());
}

