#include "psk31_encoder_impl.h"
#include <gnuradio/io_signature.h>

#define DEBUG 0
#define DBG DEBUG && std::cout

using namespace gr::bats;

psk31_encoder_impl::psk31_encoder_impl()
		: sync_block("psk31_encoder",
				gr::io_signature::make(0, 0, 0),
				gr::io_signature::make(1, 1, sizeof(uint8_t)))
{
	
	message_port_register_in(pmt::mp("in"));
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
		std::string s(static_cast<const char *>(pmt::blob_data(blob)), pmt::blob_length(blob));
		encode_string(s);
	} else if(pmt::is_symbol(msg)){
		std::string s(pmt::symbol_to_string(msg));
		encode_string(s);
	} else {
		std::cout << "unrecognized msg format - expected PDU or symbol" << std::endl;
	}
            
}

int
psk31_encoder_impl::work(int noutput,
		gr_vector_const_void_star& input_items,
		gr_vector_void_star& output_items){

	char *out = static_cast<char *>(output_items[0]);
	int items = 0;

	pmt::pmt_t msg(delete_head_nowait(pmt::mp("in")));
	if(!msg.get()){
		return 0;
	}
	
	handle_msg(msg);

	for(int i = 0; i < noutput; i++){
		if(d_tx_string.length() == 0){
			break;
		}
		std::string to_convert = d_tx_string.substr(0,8);
		unsigned char u8 = ~std::stoul(to_convert, nullptr, 2);
		out[i] = u8;
		d_tx_string = d_tx_string.erase(0,8);

		items += 1;
		DBG << "remaining string: " + d_tx_string << std::endl;
	}
	
	return items;
}

psk31_encoder::sptr
psk31_encoder::make(){
	return gnuradio::get_initial_sptr(new psk31_encoder_impl());
}
