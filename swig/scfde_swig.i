/* -*- c++ -*- */

#define SCFDE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "scfde_swig_doc.i"

%{
#include "scfde/modulate_message_c.h"
#include "scfde/insert_esti_block_ccb.h"
#include "scfde/gen_uw.h"
#include "scfde/insert_sync_block_cbc.h"
#include "scfde/insert_cp_cc.h"
#include "scfde/parallel_to_serial_cc.h"
#include "scfde/serial_to_parallel_cc.h"
#include "scfde/calculate_metric_cf.h"
#include "scfde/detect_peak_fb.h"
#include "scfde/k_offset_xx.h"
#include "scfde/block_acquization_cbcb.h"
#include "scfde/frequency_sync_cbcb.h"
#include "scfde/frequency_domain_equalize_cbcc.h"
#include "scfde/demod_mapper_cbb.h"
#include "scfde/pack_k_bits_bbbb.h"
#include "scfde/message_acquization_bb.h"
#include "scfde/awgn_cc.h"
#include "scfde/simulation_cfo_source_c.h"
#include "scfde/simulation_cfo_cf.h"
#include "scfde/block_source_c.h"
#include "scfde/simulation_equalize_cc.h"
#include "scfde/simulation_timing_source_c.h"
#include "scfde/simulation_calculate_metric_cf.h"
#include "scfde/simulation_perfect_timing_cb.h"
%}


%include "scfde/modulate_message_c.h"
GR_SWIG_BLOCK_MAGIC2(scfde, modulate_message_c);
%include "scfde/insert_esti_block_ccb.h"
GR_SWIG_BLOCK_MAGIC2(scfde, insert_esti_block_ccb);
%include "scfde/gen_uw.h"
%include "scfde/insert_sync_block_cbc.h"
GR_SWIG_BLOCK_MAGIC2(scfde, insert_sync_block_cbc);
%include "scfde/insert_cp_cc.h"
GR_SWIG_BLOCK_MAGIC2(scfde, insert_cp_cc);
%include "scfde/parallel_to_serial_cc.h"
GR_SWIG_BLOCK_MAGIC2(scfde, parallel_to_serial_cc);
%include "scfde/serial_to_parallel_cc.h"
GR_SWIG_BLOCK_MAGIC2(scfde, serial_to_parallel_cc);
%include "scfde/calculate_metric_cf.h"
GR_SWIG_BLOCK_MAGIC2(scfde, calculate_metric_cf);
%include "scfde/detect_peak_fb.h"
GR_SWIG_BLOCK_MAGIC2(scfde, detect_peak_fb);
%include "scfde/k_offset_xx.h"
GR_SWIG_BLOCK_MAGIC2(scfde, k_offset_xx);
%include "scfde/block_acquization_cbcb.h"
GR_SWIG_BLOCK_MAGIC2(scfde, block_acquization_cbcb);
%include "scfde/frequency_sync_cbcb.h"
GR_SWIG_BLOCK_MAGIC2(scfde, frequency_sync_cbcb);
%include "scfde/frequency_domain_equalize_cbcc.h"
GR_SWIG_BLOCK_MAGIC2(scfde, frequency_domain_equalize_cbcc);
%include "scfde/demod_mapper_cbb.h"
GR_SWIG_BLOCK_MAGIC2(scfde, demod_mapper_cbb);
%include "scfde/pack_k_bits_bbbb.h"
GR_SWIG_BLOCK_MAGIC2(scfde, pack_k_bits_bbbb);
%include "scfde/message_acquization_bb.h"
GR_SWIG_BLOCK_MAGIC2(scfde, message_acquization_bb);
%include "scfde/awgn_cc.h"
GR_SWIG_BLOCK_MAGIC2(scfde, awgn_cc);
%include "scfde/simulation_cfo_source_c.h"
GR_SWIG_BLOCK_MAGIC2(scfde, simulation_cfo_source_c);
%include "scfde/simulation_cfo_cf.h"
GR_SWIG_BLOCK_MAGIC2(scfde, simulation_cfo_cf);
%include "scfde/block_source_c.h"
GR_SWIG_BLOCK_MAGIC2(scfde, block_source_c);
%include "scfde/simulation_equalize_cc.h"
GR_SWIG_BLOCK_MAGIC2(scfde, simulation_equalize_cc);
%include "scfde/simulation_timing_source_c.h"
GR_SWIG_BLOCK_MAGIC2(scfde, simulation_timing_source_c);
%include "scfde/simulation_calculate_metric_cf.h"
GR_SWIG_BLOCK_MAGIC2(scfde, simulation_calculate_metric_cf);
%include "scfde/simulation_perfect_timing_cb.h"
GR_SWIG_BLOCK_MAGIC2(scfde, simulation_perfect_timing_cb);
