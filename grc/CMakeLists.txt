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

install(FILES
    scfde_modulate_message_c.xml
    scfde_insert_esti_block_ccb.xml
    scfde_gen_uw.xml
    scfde_insert_sync_block_cbc.xml
    scfde_insert_cp_cc.xml
    scfde_parallel_to_serial_cc.xml
    scfde_serial_to_parallel_cc.xml
    scfde_calculate_metric_cf.xml
    scfde_detect_peak_fb.xml
    scfde_k_offset_xx.xml
    scfde_block_acquization_cbcb.xml
    scfde_frequency_sync_cbcb.xml
    scfde_frequency_domain_equalize_cbcc.xml
    scfde_demod_mapper_cbb.xml
    scfde_pack_k_bits_bbbb.xml
    scfde_message_acquization_bb.xml
    scfde_awgn_cc.xml
    scfde_simulation_cfo_source_c.xml
    scfde_simulation_cfo_cf.xml
    scfde_block_source_c.xml
    scfde_simulation_equalize_cc.xml
    scfde_simulation_timing_source_c.xml
    scfde_simulation_calculate_metric_cf.xml
    scfde_simulation_perfect_timing_cb.xml DESTINATION share/gnuradio/grc/blocks
)
