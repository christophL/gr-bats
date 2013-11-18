#ifndef INCLUDED_GR_PSK31_DECODER_IMPL_H
#define INCLUDED_GR_PSK31_DECODER_IMPL_H

#include <digimodes/psk31_decoder.h>
#include "varicodes.h"
#include <unordered_map>

namespace gr {
namespace digimodes {
	class psk31_decoder_impl : public psk31_decoder {
		private:
			std::unordered_map<std::string, char> d_rev_varimap;
			void fill_rev_varimap();
			int d_num_zeroes;
			std::string d_curr;
			std::string d_out_string;
		public:
			psk31_decoder_impl();
			int work(int noutput,
					gr_vector_const_void_star& input_items,
					gr_vector_void_star& output_items);
	};
}
}

#endif /* INCLUDED_GR_PSK31_DECODER_IMPL_H */
