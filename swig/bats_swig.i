/* -*- c++ -*- */

#define BATS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "bats_swig_doc.i"

%{
#include "bats/timing_controller.h"
#include "bats/chip_decoder.h"
%}


%include "bats/timing_controller.h"
GR_SWIG_BLOCK_MAGIC2(bats, timing_controller);
%include "bats/chip_decoder.h"
GR_SWIG_BLOCK_MAGIC2(bats, chip_decoder);
