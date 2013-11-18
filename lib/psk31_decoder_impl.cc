#include "psk31_decoder_impl.h"
#include <gnuradio/io_signature.h>

using namespace gr::digimodes;

psk31_decoder_impl::psk31_decoder_impl()
		: sync_block("psk31_decoder",
				gr::io_signature::make(1, 1, sizeof(uint8_t)),
				gr::io_signature::make(0,0,0)),
		d_num_zeroes(0)
{
	
	message_port_register_out(pmt::mp("out"));

}

void
psk31_decoder_impl::fill_rev_varimap(){
	for(int i = 0; i < varicodes::varicode_num_entries; i++){
		std::pair<std::string, char> to_insert(varicodes::varicode_vals[i], varicodes::varicode_keys[i]);
		d_rev_varimap.insert(to_insert);
	}
}

int
psk31_decoder_impl::work(int noutput,
		gr_vector_const_void_star& input_items,
		gr_vector_void_star& output_items){
	const unsigned char *in = static_cast<const unsigned char *>(input_items[0]);

	for(int i = 0; i < noutput; i++){
		unsigned char bit = in[i];
		if(bit == 0) bit = 1;
		else bit = 0;
		
		if(bit == 0){
			d_num_zeroes += 1;
			if(d_num_zeroes == 2){
				if(d_rev_varimap.count(d_curr) > 0){
					unsigned char c = d_rev_varimap[d_curr];
					d_out_string =+ c;
					if(c == '\n'){
						pmt::pmt_t payload = pmt::make_blob(d_out_string.data(), d_out_string.length());	
						message_port_pub(pmt::mp("out"), pmt::cons(pmt::PMT_NIL, payload));
						std::cout << "decoder sent pdu" << std::endl;
						d_out_string.erase();
					}				
				}
			d_num_zeroes = 0;
			d_curr.erase();			
			}
		} else {
			if(d_num_zeroes == 1 && d_curr.length() != 0){
				d_curr += "0";			
			}
			d_curr += "1";
			d_num_zeroes = 0;
		}
	}

	return noutput;
}

psk31_decoder::sptr
psk31_decoder::make(){
	return gnuradio::get_initial_sptr(new psk31_decoder_impl());
}
