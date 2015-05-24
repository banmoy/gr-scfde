# Copyright 2011,2012 Free Software Foundation, Inc.
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
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND scfde_sources
    modulate_message_c_impl.cc
    insert_esti_block_ccb_impl.cc
    gen_uw.cc
    fft.cc
    insert_sync_block_cbc_impl.cc
    insert_cp_cc_impl.cc
    parallel_to_serial_cc_impl.cc
    serial_to_parallel_cc_impl.cc
    calculate_metric_cf_impl.cc
    detect_peak_fb_impl.cc
    k_offset_xx_impl.cc
    block_acquization_cbcb_impl.cc
    frequency_sync_cbcb_impl.cc
    frequency_domain_equalize_cbcc_impl.cc
    demod_mapper_cbb_impl.cc
    pack_k_bits_bbbb_impl.cc
    message_acquization_bb_impl.cc
    awgn_cc_impl.cc
    simulation_cfo_source_c_impl.cc
    simulation_cfo_cf_impl.cc
    block_source_c_impl.cc
    simulation_equalize_cc_impl.cc
    simulation_timing_source_c_impl.cc
    simulation_calculate_metric_cf_impl.cc
    simulation_perfect_timing_cb_impl.cc
)

set(scfde_sources "${scfde_sources}" PARENT_SCOPE)
if(NOT scfde_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT scfde_sources)

add_library(gnuradio-scfde SHARED ${scfde_sources})
target_link_libraries(gnuradio-scfde ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} fftw3f)
set_target_properties(gnuradio-scfde PROPERTIES DEFINE_SYMBOL "gnuradio_scfde_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-scfde PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-scfde
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_scfde_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_scfde.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_scfde.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_gen_uw.cc
)

add_executable(test-scfde ${test_scfde_sources})

target_link_libraries(
  test-scfde
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-scfde
)

GR_ADD_TEST(test_scfde test-scfde)
