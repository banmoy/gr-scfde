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
# Install public header files
########################################################################
install(FILES
    api.h
    modulate_message_c.h
    insert_esti_block_ccb.h
    gen_uw.h
    insert_sync_block_cbc.h
    insert_cp_cc.h
    parallel_to_serial_cc.h
    serial_to_parallel_cc.h
    calculate_metric_cf.h
    detect_peak_fb.h
    k_offset_xx.h
    block_acquization_cbcb.h
    frequency_sync_cbcb.h
    frequency_domain_equalize_cbcc.h
    demod_mapper_cbb.h
    pack_k_bits_bbbb.h
    message_acquization_bb.h
    awgn_cc.h
    simulation_cfo_source_c.h
    simulation_cfo_cf.h
    block_source_c.h
    simulation_equalize_cc.h
    simulation_timing_source_c.h
    simulation_calculate_metric_cf.h
    simulation_perfect_timing_cb.h DESTINATION include/scfde
)
