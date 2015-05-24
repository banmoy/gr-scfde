# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Include python install macros
########################################################################
include(GrPython)
if(NOT PYTHONINTERP_FOUND)
    return()
endif()

########################################################################
# Install python sources
########################################################################
GR_PYTHON_INSTALL(
    FILES
    __init__.py
    DESTINATION ${GR_PYTHON_DIR}/scfde
)

########################################################################
# Handle the unit tests
########################################################################
include(GrTest)

set(GR_TEST_TARGET_DEPS gnuradio-scfde)
set(GR_TEST_PYTHON_DIRS ${CMAKE_BINARY_DIR}/swig)
GR_ADD_TEST(qa_modulate_message_c ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_modulate_message_c.py)
GR_ADD_TEST(qa_insert_esti_block_ccb ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_insert_esti_block_ccb.py)
GR_ADD_TEST(qa_insert_sync_block_cbc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_insert_sync_block_cbc.py)
GR_ADD_TEST(qa_insert_cp_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_insert_cp_cc.py)
GR_ADD_TEST(qa_parallel_to_serial_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_parallel_to_serial_cc.py)
GR_ADD_TEST(qa_serial_to_parallel_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_serial_to_parallel_cc.py)
GR_ADD_TEST(qa_calculate_metric_cf ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_calculate_metric_cf.py)
GR_ADD_TEST(qa_detect_peak_fb ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_detect_peak_fb.py)
GR_ADD_TEST(qa_k_offset_xx ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_k_offset_xx.py)
GR_ADD_TEST(qa_block_acquization_cbcb ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_block_acquization_cbcb.py)
GR_ADD_TEST(qa_frequency_sync_cbcb ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_frequency_sync_cbcb.py)
GR_ADD_TEST(qa_frequency_domain_equalize_cbcc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_frequency_domain_equalize_cbcc.py)
GR_ADD_TEST(qa_demod_mapper_cbb ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_demod_mapper_cbb.py)
GR_ADD_TEST(qa_pack_k_bits_bbbb ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_pack_k_bits_bbbb.py)
GR_ADD_TEST(qa_message_acquization_bb ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_message_acquization_bb.py)
GR_ADD_TEST(qa_awgn_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_awgn_cc.py)
GR_ADD_TEST(qa_simulation_cfo_source_c ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_simulation_cfo_source_c.py)
GR_ADD_TEST(qa_simulation_cfo_cf ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_simulation_cfo_cf.py)
GR_ADD_TEST(qa_block_source_c ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_block_source_c.py)
GR_ADD_TEST(qa_simulation_equalize_cc ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_simulation_equalize_cc.py)
GR_ADD_TEST(qa_simulation_timing_source_c ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_simulation_timing_source_c.py)
GR_ADD_TEST(qa_simulation_calculate_metric_cf ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_simulation_calculate_metric_cf.py)
GR_ADD_TEST(qa_simulation_perfect_timing_cb ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_simulation_perfect_timing_cb.py)
