/* -*- c++ -*- */

#define BATS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "bats_swig_doc.i"

%{
#include "bats/tdma_controller.h"
#include "bats/chip_decoder.h"
%}


%include "bats/tdma_controller.h"
GR_SWIG_BLOCK_MAGIC2(bats, tdma_controller);
%include "bats/chip_decoder.h"
GR_SWIG_BLOCK_MAGIC2(bats, chip_decoder);
