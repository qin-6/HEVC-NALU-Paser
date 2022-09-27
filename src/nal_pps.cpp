#include <stdio.h>
#include "types.h"
#include "bio.h"
#include "nal_common.h"

extern FILE * outfile;

const uint8 SEI_MSG[34] = {
0x00, 0x00, 0x00, 0x01, 0x4E, 0x01, 0x89, 0x18, //0
0x33, 0xC2, 0x86, 0xC4, 0x1D, 0x4C, 0x0B, 0xB8, //8
0x84, 0xD0, 0x3E, 0x80, 0x3D, 0x13, 0x40, 0x42, //15
0x00, 0x98, 0x96, 0x80, 0x00, 0x00, 0x03, 0x00, //24
0x32, 0x80										//32
};

void nal_pps_init() {
}

void nal_pps_parse(nal_buffer_t * pnal_buffer)
{
	//write PPS_NAL unit to file
	copy_nal_to_file(pnal_buffer, outfile);
	//write SEI_MSG
	fwrite(&SEI_MSG, sizeof(SEI_MSG), 1, outfile);
}

void print_nal_pps_parse(nal_buffer_t *pInBuffer) {
	dump_nal_buffer(pInBuffer);
	nal_buffer_t out_buf = {0};
	decode_nul_rbsp(pInBuffer, &out_buf, pInBuffer->posmax);
	out_buf.posmax = out_buf.pos;
	out_buf.pos = 0;
	out_buf.bitpos = 8;
	
    nal_buffer_t * pnal_buffer = &out_buf;
	pnal_buffer->pos += 2;
	fprintf(stdout, "\tpps_pic_parameter_set_id=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tpps_seq_parameter_set_id=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tdependent_slice_segments_enabled_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\toutput_flag_present_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tnum_extra_slice_header_bits=%d\n", read_bits(pnal_buffer, 3));
	fprintf(stdout, "\tsign_data_hiding_enabled_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tcabac_init_present_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tnum_ref_idx_l0_default_active_minus1=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tnum_ref_idx_l1_default_active_minus1=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tinit_qp_minus26=%d\n", read_sev(pnal_buffer)); //se(v)
	fprintf(stdout, "\tconstrained_intra_pred_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\ttransform_skip_enabled_flag=%d\n", read_bit(pnal_buffer));
	uint8 cu_qp_delta_enabled_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tcu_qp_delta_enabled_flag=%d\n", cu_qp_delta_enabled_flag);
	if (cu_qp_delta_enabled_flag) {
		fprintf(stdout, "\tdiff_cu_qp_delta_depth=%d\n", read_uev(pnal_buffer));
	}
	fprintf(stdout, "\tpps_cb_qp_offset=%d\n", read_sev(pnal_buffer)); //se(v)
	fprintf(stdout, "\tpps_cr_qp_offset=%d\n", read_sev(pnal_buffer)); //se(v)
	fprintf(stdout, "\tpps_slice_chroma_qp_offsets_present_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tweighted_pred_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tweighted_bipred_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\ttransquant_bypass_enabled_flag=%d\n", read_bit(pnal_buffer));
	uint8 tiles_enabled_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\ttiles_enabled_flag=%d\n", tiles_enabled_flag);
	fprintf(stdout, "\tentropy_coding_sync_enabled_flag=%d\n", read_bit(pnal_buffer));
	if (tiles_enabled_flag) {
		uint32 num_tile_columns_minus1 = read_uev(pnal_buffer);
		fprintf(stdout, "\tnum_tile_columns_minus1=%d\n", num_tile_columns_minus1);
		uint32 num_tile_rows_minus1 = read_uev(pnal_buffer);
		fprintf(stdout, "\tnum_tile_rows_minus1=%d\n", num_tile_rows_minus1);
		uint8 uniform_spacing_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tuniform_spacing_flag=%d\n", uniform_spacing_flag);
		if (!uniform_spacing_flag) {
			for (int i = 0; i < num_tile_columns_minus1; i++) {
				fprintf(stdout, "\tcolumn_width_minus1[i]=%d\n", read_uev(pnal_buffer));
			}
			for (int i = 0; i < num_tile_rows_minus1; i++) {
				fprintf(stdout, "\trow_height_minus1[i]=%d\n", read_uev(pnal_buffer));
			}
		}
		fprintf(stdout, "\tloop_filter_across_tiles_enabled_flag=%d\n", read_bit(pnal_buffer));
	}
	fprintf(stdout, "\tpps_loop_filter_across_slices_enabled_flag=%d\n", read_bit(pnal_buffer));
	uint8 deblocking_filter_control_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tdeblocking_filter_control_present_flag=%d\n", deblocking_filter_control_present_flag);
	if (deblocking_filter_control_present_flag) {
		fprintf(stdout, "\tdeblocking_filter_override_enabled_flag=%d\n", read_bit(pnal_buffer));
		uint8 pps_deblocking_filter_disabled_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tpps_deblocking_filter_disabled_flag=%d\n", pps_deblocking_filter_disabled_flag);
		if (!pps_deblocking_filter_disabled_flag) {
			fprintf(stdout, "\tpps_beta_offset_div2=%d\n", read_sev(pnal_buffer)); //se(v)
			fprintf(stdout, "\tpps_tc_offset_div2=%d\n", read_sev(pnal_buffer)); //se(v)
		}
	}

	uint8 pps_scaling_list_data_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tpps_scaling_list_data_present_flag=%d\n", pps_scaling_list_data_present_flag);
	if (pps_scaling_list_data_present_flag) {
		print_scaling_list_data(pnal_buffer);
	}

	fprintf(stdout, "\tlists_modification_present_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tlog2_parallel_merge_level_minus2=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tslice_segment_header_extension_present_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tpps_extension_flag=%d\n", read_bit(pnal_buffer));
}
