#ifndef INCLUDED_GR_PSK31_ENCODER_IMPL_H
#define INCLUDED_GR_PSK31_ENCODER_IMPL_H

#include <bats/psk31_encoder.h>
#include "varicodes.h"
#include <string>


namespace gr {
namespace bats {
	class psk31_encoder_impl : public psk31_encoder {
		private:
			std::string d_tx_string;
			pmt::pmt_t d_tx_time;
			int d_max_tx;
			int d_pad;
			bool d_idle;

			void handle_msg(pmt::pmt_t msg);
			void encode_string(std::string &str);
			void add_sob(uint64_t item);
			void add_eob(uint64_t item);
		public:
			psk31_encoder_impl();

			int work(int noutput, gr_vector_const_void_star& input_items,
					gr_vector_void_star& output_items);
	};
}
}

#endif /* INCLUDED_GR_PSK31_ENCODER_IMPL_H */
