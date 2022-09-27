#include <stdio.h>
#include "types.h"
#include "bio.h"
#include "cio.h"
#include "nal_common.h"
#include "nal_vps.h"

void nal_vps_init() {

}

void print_nal_vps_parse(nal_buffer_t *pInBuffer) {
	
	nal_buffer_t out_buf = {0};
	decode_nul_rbsp(pInBuffer, &out_buf, pInBuffer->posmax);
	out_buf.posmax = out_buf.pos;
	out_buf.pos = 0;
	out_buf.bitpos = 8;
	
	nal_vps_t nal_vps;
	nal_buffer_t *pnal_buffer = &out_buf;
	pnal_buffer->pos += 2; // skip header
	nal_vps.vps_video_parameter_set_id = read_bits(pnal_buffer, 4);	// u(4)
	nal_vps.vps_reserved_three_2bits = read_bits(pnal_buffer, 2);	// u(2)
	nal_vps.vps_max_layers_minus1 = read_bits(pnal_buffer, 6);		// u(6)
	nal_vps.vps_max_sub_layers_minus1 = read_bits(pnal_buffer, 3);	// u(3)
	nal_vps.vps_temporal_id_nesting_flag = read_bits(pnal_buffer, 1);// u(1)
	nal_vps.vps_reserved_0xffff_16bits = read_bits(pnal_buffer, 16);	// u(16)

	fprintf(stdout, "\tvps_video_parameter_set_id = %d\n", nal_vps.vps_video_parameter_set_id);
	fprintf(stdout, "\tvps_reserved_three_2bits = %d\n", nal_vps.vps_reserved_three_2bits);
	fprintf(stdout, "\tvps_max_layers_minus1 = %d\n", nal_vps.vps_max_layers_minus1);
	fprintf(stdout, "\tvps_max_sub_layers_minus1 = %d\n", nal_vps.vps_max_sub_layers_minus1);
	fprintf(stdout, "\tvps_temporal_id_nesting_flag = %d\n", nal_vps.vps_temporal_id_nesting_flag);
	fprintf(stdout, "\tvps_reserved_0xffff_16bits = %d\n", nal_vps.vps_reserved_0xffff_16bits);

	
	uint8 vps_max_layers_minus1 = nal_vps.vps_max_layers_minus1;
    uint8 vps_max_sub_layers_minus1 = vps_max_sub_layers_minus1;
	profile_tier_level_data_t profile_tier_level_data;
	profile_tier_level(pnal_buffer, vps_max_sub_layers_minus1, &profile_tier_level_data);
	uint8 vps_sub_layer_ordering_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tvps_sub_layer_ordering_info_present_flag = %d\n", vps_sub_layer_ordering_info_present_flag);

	for (int i = (vps_sub_layer_ordering_info_present_flag ? 0 : vps_max_sub_layers_minus1); i <= vps_max_sub_layers_minus1; i++) {
		fprintf(stdout, "\tvps_max_dec_pic_buffering_minus1[%d] = %d\n", i, read_uev(pnal_buffer));
		fprintf(stdout, "\tvps_max_num_reorder_pics[%d] = %d\n", i, read_uev(pnal_buffer));
		fprintf(stdout, "\tvps_max_latency_increase_plus1[%d] = %d\n", i, read_uev(pnal_buffer));
	}

	uint8 vps_max_layer_id = read_bits(pnal_buffer, 6);
	fprintf(stdout, "\tvps_max_layer_id = %d\n", vps_max_layer_id);
	uint32 vps_num_layer_sets_minus1 = read_uev(pnal_buffer);
	fprintf(stdout, "\tvps_num_layer_sets_minus1 = %d\n", vps_num_layer_sets_minus1);

	for (int i = 1; i <= vps_num_layer_sets_minus1; i++) {
		for (int j = 0; j <= vps_max_layer_id; j++) {
			fprintf(stdout, "\tlayer_id_included_flag[%d][%d] = %d\n", i, j, read_bit(pnal_buffer));
		}
	}

	uint8 vps_timing_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tvps_timing_info_present_flag = %d\n", vps_timing_info_present_flag);

	if (vps_timing_info_present_flag) {
		fprintf(stdout, "\tvps_num_units_in_tick = %d\n", read_bits(pnal_buffer, 32));
		fprintf(stdout, "\tvps_time_scale = %d\n", read_bits(pnal_buffer, 32));
		uint8 vps_poc_proportional_to_timing_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tvps_poc_proportional_to_timing_flag = %d\n", vps_poc_proportional_to_timing_flag);
		if (vps_poc_proportional_to_timing_flag) {
			fprintf(stdout, "\tvps_num_ticks_poc_diff_one_minus1 = %d\n", read_uev(pnal_buffer));
		}
		uint32 vps_num_hrd_parameters = read_uev(pnal_buffer);
		fprintf(stdout, "\tvps_num_hrd_parameters = %d\n", vps_num_hrd_parameters);
		for (int i = 0; i < vps_num_hrd_parameters; i++) {
			fprintf(stdout, "\thrd_layer_set_idx[%d] = %d\n", i, read_uev(pnal_buffer));
			uint8 cprms_present_flag_i = 0;
			if (i > 0) {
				cprms_present_flag_i = read_bit(pnal_buffer);
				fprintf(stdout, "\tcprms_present_flag[%d] = %d\n", read_bit(pnal_buffer));
			}
	//		hrd_parameters(cprms_present_flag_i, vps_max_sub_layers_minus1);
		}
	}

	uint8 vps_extension_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tvps_extension_flag = %d\n", vps_extension_flag);

}
