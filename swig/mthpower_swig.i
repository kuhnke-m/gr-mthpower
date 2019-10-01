/* -*- c++ -*- */

#define MTHPOWER_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "mthpower_swig_doc.i"

%{
#include "mthpower/m_th_power_fc_cc.h"
#include "mthpower/m_th_power_v1_cc.h"
%}


%include "mthpower/m_th_power_fc_cc.h"
GR_SWIG_BLOCK_MAGIC2(mthpower, m_th_power_fc_cc);
%include "mthpower/m_th_power_v1_cc.h"
GR_SWIG_BLOCK_MAGIC2(mthpower, m_th_power_v1_cc);
