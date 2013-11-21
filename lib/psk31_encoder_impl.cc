#include "psk31_encoder_impl.h"
#include <gnuradio/io_signature.h>

using namespace gr::digimodes;

psk31_encoder_impl::psk31_encoder_impl()
		: sync_block("psk31_encoder",
				gr::io_signature::make(0, 0, 0),
				gr::io_signature::make(1, 1, sizeof(uint8_t))),
		d_varimap(256){
	
	message_port_register_in(pmt::mp("in"));
	fill_varimap();
}

void
psk31_encoder_impl::fill_varimap(){
	for(int i = 0; i < varicodes::varicode_num_entries; i++){
		std::pair<char,std::string> to_insert(varicodes::varicode_keys[i], varicodes::varicode_vals[i]);
		d_varimap.insert(to_insert);
	}
}

void
psk31_encoder_impl::encode_string(std::string &str){
	std::string tmp("00000000");

	if(str.length() <= 1) return;
	
	for(char c : str){
		tmp += d_varimap[c];
		tmp += "00";
	}
	tmp += d_varimap['\n'];

	while(tmp.length() % 8 != 0)
            tmp += "1";
	
	d_tx_string += tmp;
//	std::cout << "tx_string:" + d_tx_string << std::endl;
}

void
psk31_encoder_impl::handle_msg(pmt::pmt_t msg){
	if(pmt::is_pair(msg)){
		pmt::pmt_t blob = pmt::cdr(msg);
		std::string s(static_cast<const char *>(pmt::blob_data(blob)), pmt::blob_length(blob));
		encode_string(s);
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
//		std::cout << "remaining string: " + d_tx_string << std::endl;
	}
	
	return items;
}

psk31_encoder::sptr
psk31_encoder::make(){
	return gnuradio::get_initial_sptr(new psk31_encoder_impl());
}
