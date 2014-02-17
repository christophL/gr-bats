/* -*- c++ -*- */

#define BATS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "bats_swig_doc.i"

%{
#include "bats/chip_decoder.h"
#include "bats/psk31_encoder.h"
#include "bats/psk31_decoder.h"
#include "bats/simple_mac.h"
%}

%include "bats/psk31_encoder.h"
%include "bats/psk31_decoder.h"
%include "bats/chip_decoder.h"
%include "bats/simple_mac.h"

GR_SWIG_BLOCK_MAGIC2(bats, psk31_encoder);
GR_SWIG_BLOCK_MAGIC2(bats, psk31_decoder);
GR_SWIG_BLOCK_MAGIC2(bats, chip_decoder);
GR_SWIG_BLOCK_MAGIC2(bats, simple_mac);
