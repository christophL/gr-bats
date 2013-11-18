/*
 * Copyright (C) 2013 Christoph Leitner <c.leitner@student.uibk.ac.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDED_GR_PSK31_ENCODER_H
#define INCLUDED_GR_PSK31_ENCODER_H

#include <digimodes/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
	namespace digimodes {

		class DIGIMODES_API psk31_encoder : virtual public sync_block
		{
		public:

			typedef boost::shared_ptr<psk31_encoder> sptr;
			static sptr make();
		};
	}  // namespace digimodes
}  // namespace gr

#endif /* INCLUDED_GR_PSK31_ENCODER_H */
