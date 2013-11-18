/* -*- c++ -*- */

#define DIGIMODES_API

%include <gnuradio.i>			// the common stuff

//load generated python docstrings
%include "digimodes_swig_doc.i"

%{
#include "digimodes/psk31_encoder.h"
#include "digimodes/psk31_decoder.h"
%}


%include "digimodes/psk31_encoder.h"
%include "digimodes/psk31_decoder.h"



GR_SWIG_BLOCK_MAGIC2(digimodes, psk31_encoder);
GR_SWIG_BLOCK_MAGIC2(digimodes, psk31_decoder);


