#ifndef __NAL_VPS_H
#define __NAL_VPS_H

#include "types.h"
#include "cio.h"

typedef struct _nal_vps_t {
	uint8 vps_video_parameter_set_id;	// u(4)
	uint8 vps_reserved_three_2bits;		// u(2)
	uint8 vps_max_layers_minus1;		// u(6)
	uint8 vps_max_sub_layers_minus1;	// u(3)
	uint8 vps_temporal_id_nesting_flag;	// u(1)
	uint16 vps_reserved_0xffff_16bits;	// u(16)

	uint8 vps_sub_layer_ordering_info_present_flag; // u(1)

	uint32 vps_max_dec_pic_buffering_minus1[8];		// ue(v)
	uint32 vps_max_num_reorder_pics[8];				// ue(v)
	uint32 vps_max_latency_increase_plus1[8];		// ue(v)

	uint8 vps_max_layer_id;	// u(6)
	uint32 vps_num_layer_sets_minus1; // ue(v)

	/*
	uint8 layer_id_included_flag[i][j] u(1)

		vps_timing_info_present_flag u(1)

	if
		vps_num_units_in_tick u(32)
		vps_time_scale u(32)
		vps_poc_proportional_to_timing_flag u(1)

		vps_num_ticks_poc_diff_one_minus1 ue(v)

		vps_num_hrd_parameters ue(v)

		arr

		cprms_present_flag[i] u(1)

	while (more data)
		vps_extension_flag u(1)
		hrd_layer_set_idx[i] ue(v)
		*/
} nal_vps_t;

void nal_vps_init();
void print_nal_vps_parse(nal_buffer_t *pInBuffer);

#endif
