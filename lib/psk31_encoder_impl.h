#ifndef INCLUDED_GR_PSK31_ENCODER_IMPL_H
#define INCLUDED_GR_PSK31_ENCODER_IMPL_H

#include <digimodes/psk31_encoder.h>
#include "varicodes.h"
#include <string>
#include <unordered_map>


namespace gr {
namespace digimodes {
	class psk31_encoder_impl : public psk31_encoder {
		private:
			std::unordered_map<char, std::string> d_varimap;
			std::string d_tx_string;
			void handle_msg(pmt::pmt_t msg);
			void encode_string(std::string &str);
			void fill_varimap();

		public:
			psk31_encoder_impl();
			int work(int noutput,
					gr_vector_const_void_star& input_items,
					gr_vector_void_star& output_items);
	};
}
}

#endif /* INCLUDED_GR_PSK31_ENCODER_IMPL_H */
