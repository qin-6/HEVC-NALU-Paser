#ifndef __NAL_COMMON_H
#define __NAL_COMMON_H

#define EXTENDED_SAR 255
#include <cstdint>

typedef struct _profile_tier_level_data_t
{
	uint8 general_profile_space;//	u(2)
	uint8 general_tier_flag;//	u(1)
	uint8 general_profile_idc;//	u(5)
	//for( i = 0; i < 32; i++ )	
	uint8 general_profile_compatibility_flag[32];//	u(1)
	uint8 general_progressive_source_flag;//	u(1)
	uint8 general_interlaced_source_flag;//	u(1)
	uint8 general_non_packed_constraint_flag;//	u(1)
	uint8 general_frame_only_constraint_flag;//	u(1)
	//uint64 general_reserved_zero_44bits;  //[Ed. (GJS): Adjust semantics accordingly.]	u(44)
//	if( general_profile_idc  = =  4  | |  general_profile_compatibility_flag[ 4 ]  | |
//			general_profile_idc  = =  5  | |  general_profile_compatibility_flag[ 5 ]  | |
//			general_profile_idc  = =  6  | |  general_profile_compatibility_flag[ 6 ]  | |
//			general_profile_idc  = =  7  | |  general_profile_compatibility_flag[ 7 ] ) {
//			/* The number of bits in this syntax structure is not affected by this condition */	
	uint8 general_max_12bit_constraint_flag;//	u(1)
	uint8 general_max_10bit_constraint_flag;//	u(1)
	uint8 general_max_8bit_constraint_flag;//	u(1)
	uint8 general_max_422chroma_constraint_flag;//	u(1)
	uint8 general_max_420chroma_constraint_flag;//	u(1)
	uint8 general_max_monochrome_constraint_flag;//u(1)
	uint8 general_intra_constraint_flag;//	u(1)
	uint8 general_one_picture_only_constraint_flag;//	u(1)
	uint8 general_lower_bit_rate_constraint_flag;//	u(1)
	uint64 general_reserved_zero_34bits;//	u(34)
//		} else	
	uint64	general_reserved_zero_43bits;//	u(43)
//		if( ( general_profile_idc  >=  1  &&  general_profile_idc  <=  5 )  | |
//			 general_profile_compatibility_flag[ 1 ]  | |  general_profile_compatibility_flag[ 2 ]  | |
//			 general_profile_compatibility_flag[ 3 ]  | |  general_profile_compatibility_flag[ 4 ]  | |
//			 general_profile_compatibility_flag[ 5 ] )
//			/* The number of bits in this syntax structure is not affected by this condition */	
	uint8 general_inbld_flag;//	u(1)
//		else	
	uint8 general_reserved_zero_bit;//	u(1)

	uint8 general_level_idc;//	u(8)
	//for( i = 0; i < maxNumSubLayersMinus1; i++ ) value of sps_max_sub_layers_minus1 shall be in the range of 0 to 6, inclusive
	uint8 sub_layer_profile_present_flag[8];//	u(1)
	uint8 sub_layer_level_present_flag[8];//	u(1)
	//if( maxNumSubLayersMinus1 > 0 )	
	//	for( i = maxNumSubLayersMinus1; i < 8; i++ )	
	uint8 reserved_zero_2bits[2];//	u(2)
	//for( i = 0; i < maxNumSubLayersMinus1; i++ ) {	
		//if( sub_layer_profile_present_flag[ i ] ) {	
	uint8 sub_layer_profile_space[7];//	u(2)
	uint8 sub_layer_tier_flag[7];//	u(1)
	uint8 sub_layer_profile_idc[7];//	u(5)
		    //for( j = 0; j < 32; j++ )	
	uint8 sub_layer_profile_compatibility_flag[7][32];//	u(1)
	uint8 sub_layer_progressive_source_flag[7];//	u(1)
	uint8 sub_layer_interlaced_source_flag[7];//	u(1)
	uint8 sub_layer_non_packed_constraint_flag[7];//	u(1)
	uint8 sub_layer_frame_only_constraint_flag[7];//	u(1)
//	uint64 sub_layer_reserved_zero_44bits[7];//	u(44)
//				if( sub_layer_profile_idc[ i ]  = =  4  | |  sub_layer_profile_compatibility_flag[ i ][ 4 ]  | |
//				sub_layer_profile_idc[ i ]  = =  5  | |  sub_layer_profile_compatibility_flag[ i ][ 5 ]  | |
//				sub_layer_profile_idc[ i ]  = =  6  | |  sub_layer_profile_compatibility_flag[ i ][ 6 ]  | |
//				sub_layer_profile_idc[ i ]  = =  7  | |  sub_layer_profile_compatibility_flag[ i ][ 7 ] ) {
//				/* The number of bits in this syntax structure is not affected by this condition */	
	uint8 sub_layer_max_12bit_constraint_flag[7];//	u(1)
	uint8 sub_layer_max_10bit_constraint_flag[7];//	u(1)
	uint8 sub_layer_max_8bit_constraint_flag[7];//	u(1)
    uint8 sub_layer_max_422chroma_constraint_flag[7];//	u(1)
	uint8 sub_layer_max_420chroma_constraint_flag[7];//	u(1)
	uint8 sub_layer_max_monochrome_constraint_flag[7];//	u(1)
	uint8 sub_layer_intra_constraint_flag[7];//	u(1)
	uint8 sub_layer_one_picture_only_constraint_flag[7];//	u(1)
	uint8 sub_layer_lower_bit_rate_constraint_flag[7];//	u(1)
	uint64 sub_layer_reserved_zero_34bits[7];//	u(34)
//	} else	
	uint64 sub_layer_reserved_zero_43bits[7];//	u(43)
//	if( ( sub_layer_profile_idc[ i ]  >=  1  &&  sub_layer_profile_idc[ i ]  <=  5 )  | |
//	 sub_layer_profile_compatibility_flag[ 1 ]  | |
//	 sub_layer_profile_compatibility_flag[ 2 ]  | |
//	 sub_layer_profile_compatibility_flag[ 3 ]  | |
//	 sub_layer_profile_compatibility_flag[ 4 ]  | |
//	 sub_layer_profile_compatibility_flag[ 5 ] )
	/* The number of bits in this syntax structure is not affected by this condition */	
	uint8 sub_layer_inbld_flag[7];//	u(1)
//	else	
	uint8 sub_layer_reserved_zero_bit[7];//	u(1)
	//if( sub_layer_level_present_flag[ i ] )	
	uint8 sub_layer_level_idc[7];//	u(8)
} profile_tier_level_data_t;

typedef struct _scaling_list_data_data_t
{
//	for( sizeId = 0; sizeId < 4; sizeId++ )	
//		for( matrixId = 0; matrixId < ( ( sizeId = = 3 ) ? 2 : 6 ); matrixId++ ) {	
	uint8 scaling_list_pred_mode_flag[4][6];//	u(1)
//			if( !scaling_list_pred_mode_flag[ sizeId ][ matrixId ] )	
	uint32 scaling_list_pred_matrix_id_delta[4][6];//	ue(v)
//			else {	
//				nextCoef = 8	
//				coefNum = Min( 64, ( 1 << ( 4 + ( sizeId << 1) ) ) )	
//				if( sizeId > 1 ) {	
    sint32 scaling_list_dc_coef_minus8[4][6];//	se(v)
//					nextCoef =
//							scaling_list_dc_coef_minus8[ sizeId − 2 ][ matrixId ] + 8	
//				}	
//				for( i = 0; i < coefNum; i++) {	
	sint32	scaling_list_delta_coef[64];//	se(v)
//					nextCoef = ( nextCoef + scaling_list_delta_coef + 256 ) % 256	
//					ScalingList[ sizeId ][ matrixId ][ i ] = nextCoef	
//	uint8 scaling_list[4][6][64];
//				}	
//			}	
//		}	
//} 	

	 
} scaling_list_data_data_t;

typedef struct _short_term_ref_pic_set_data_t {
//		if( idxRps != 0 )	
		uint8 inter_ref_pic_set_prediction_flag;//	u(1)
//	if( inter_ref_pic_set_prediction_flag ) {	
//		if( idxRps  = =  num_short_term_ref_pic_sets )	
		uint32 delta_idx_minus1;//	ue(v) The value of delta_idx_minus1 shall be in the range of 0 to idxRps − 1, inclusive
		uint8 delta_rps_sign;//	u(1)
		uint16 abs_delta_rps_minus1;//	ue(v) value of abs_delta_rps_minus1 shall be in the range of 0 to 2^15 − 1, inclusive
//		for( j = 0; j <= NumDeltaPocs[ RIdx ]; j++ ) {	NumDeltaPocs <= sps_max_dec_pic_buffering_minus1 + 1 <= MaxDpbSize <= 16
		uint8 used_by_curr_pic_flag[15];//	u(1)
//			if( !used_by_curr_pic_flag[ j ] )	
		uint8 use_delta_flag[15];//	u(1)
//		}	
//	}	
//	else {	
		uint32 num_negative_pics[15];//	ue(v)
		uint32 num_positive_pics[15];//	ue(v)
//		for( i = 0; i < num_negative_pics; i++ ) {	num_negative_pics <= NumDeltaPocs
		uint32	delta_poc_s0_minus1[15];//	ue(v)
		uint8	used_by_curr_pic_s0_flag[15];//	u(1)
//		}	
//		for( i = 0; i < num_positive_pics; i++ ) {	
		uint32	delta_poc_s1_minus1[15];//	ue(v)
		uint8	used_by_curr_pic_s1_flag[15];//	u(1)
//		}	
//	}	
//}	

} short_term_ref_pic_set_data_t;

typedef struct _sub_layer_hrd_parameters_data_t{
//	for( i = 0; i <= CpbCnt; i++ ) {	
		uint32 bit_rate_value_minus1[8];//	ue(v) shall be in the range of 0 to 2^32 − 2, inclusive.
		uint32 cpb_size_value_minus1[8];//	ue(v) in the range of 0 to 2^32 − 2, inclusive
//		if( sub_pic_cpb_params_present_flag ) {	
		uint32 cpb_size_du_value_minus1[8];//	ue(v) shall be in the range of 0 to 2^32 − 2, inclusive. 
		uint32 bit_rate_du_value_minus1[8];//	ue(v) shall be in the range of 0 to 2^32 − 2, inclusive
//		}	
		uint8 cbr_flag[8];//	u(1)
//	}	
} sub_layer_hrd_parameters_data_t;

typedef struct _hrd_parameters_data_t {
//	if( commonInfPresentFlag ) {	
		uint8 nal_hrd_parameters_present_flag;//	u(1)
		uint8 vcl_hrd_parameters_present_flag;//	u(1)
//		if( nal_hrd_parameters_present_flag | | vcl_hrd_parameters_present_flag ){	
		uint8 sub_pic_cpb_params_present_flag;//	u(1)
//			if( sub_pic_cpb_params_present_flag ) {	
		uint8 tick_divisor_minus2;//	u(8)
		uint8 du_cpb_removal_delay_increment_length_minus1;//	u(5)
		uint8 sub_pic_cpb_params_in_pic_timing_sei_flag;//	u(1)
		uint8 dpb_output_delay_du_length_minus1;//	u(5)
//			}	
		uint8 bit_rate_scale;//		u(4)
		uint8 cpb_size_scale;//		u(4)
//			if( sub_pic_cpb_params_present_flag )	
		uint8 cpb_size_du_scale;//		u(4)
		uint8 initial_cpb_removal_delay_length_minus1;//		u(5)
		uint8 au_cpb_removal_delay_length_minus1;//		u(5)
		uint8 dpb_output_delay_length_minus1;//		u(5)
//		}	
//	}	
//	for( i = 0; i  <=  maxNumSubLayersMinus1; i++ ) {	//sps_max_sub_layers_minus1 0..6 inclusev
		uint8 fixed_pic_rate_general_flag[8];//	u(1)
//		if( !fixed_pic_rate_general_flag[ i ] )	
		uint8 fixed_pic_rate_within_cvs_flag[8];//	u(1)
//		if( fixed_pic_rate_within_cvs_flag[ i ] )	
		uint16 elemental_duration_in_tc_minus1[8];//	ue(v) shall be in the range of 0 to 2047, inclusive.
//		else	
		uint8 low_delay_hrd_flag[8];//	u(1)
//		if( !low_delay_hrd_flag[ i ] )	
		uint8 cpb_cnt_minus1[8];//	ue(v) shall be in the range of 0 to 31, inclusive
/*		if( nal_hrd_parameters_present_flag )*/	
		sub_layer_hrd_parameters_data_t nal_sublayer_hrd_parameters_data[8];
//		if( vcl_hrd_parameters_present_flag )	
		sub_layer_hrd_parameters_data_t vcl_sublayer_hrd_parameters_data[8];
//	}	
//}	

} hrd_parameters_data_t;

typedef struct _vui_parameters_data_t {
	uint8 aspect_ratio_info_present_flag;//	u(1)
//	if( aspect_ratio_info_present_flag ) {	
	uint8 aspect_ratio_idc;//	u(8)
//		if( aspect_ratio_idc  = =  Extended_SAR ) {	
	uint16 sar_width;//	u(16)
	uint16 sar_height;// u(16)
//		}	
//	}	
	uint8 overscan_info_present_flag;//	u(1)
//	if( overscan_info_present_flag )	
	uint8	overscan_appropriate_flag;//	u(1)
	uint8 video_signal_type_present_flag;//	u(1)
//	if( video_signal_type_present_flag ) {	
	uint8 video_format;//	u(3)
	uint8 video_full_range_flag;//	u(1)
	uint8 colour_description_present_flag;//	u(1)
//		if( colour_description_present_flag ) {	
	uint8 colour_primaries;//	u(8)
	uint8 transfer_characteristics;//	u(8)
	uint8 matrix_coefficients;//	u(8)
//		}	
//	}	
	uint8 chroma_loc_info_present_flag;//	u(1)
//	if( chroma_loc_info_present_flag ) {	
	uint8	chroma_sample_loc_type_top_field;//	ue(v) 	The value of chroma_sample_loc_type_top_field and 
	uint8	chroma_sample_loc_type_bottom_field;//	ue(v)    chroma_sample_loc_type_bottom_field shall be in the range of 0 to 5, inclusive
//	}	
	uint8 neutral_chroma_indication_flag;//	u(1)
	uint8 field_seq_flag;//	u(1)
	uint8 frame_field_info_present_flag;//	u(1)
	uint8 default_display_window_flag;//	u(1)
//	if ( default_display_window_flag ) {	
	uint32 def_disp_win_left_offset;//	ue(v)
	uint32 def_disp_win_right_offset;//	ue(v)
	uint32 def_disp_win_top_offset;//	ue(v)
	uint32 def_disp_win_bottom_offset;//	ue(v)
//	}	
	uint8 vui_timing_info_present_flag;//	u(1)
//	if( vui_timing_info_present_flag ) {	
	uint32 vui_num_units_in_tick;//	u(32)
	uint32 vui_time_scale;//	u(32)
	uint8 vui_poc_proportional_to_timing_flag;// u(1)
//		if( vui_poc_proportional_to_timing_flag )	
	uint32 vui_num_ticks_poc_diff_one_minus1;//	ue(v)
	uint8 vui_hrd_parameters_present_flag;//	u(1)
//		if( vui_hrd_parameters_present_flag )	
	hrd_parameters_data_t hrd_parameters; //( 1, sps_max_sub_layers_minus1 ) [Ed. (BB): Syntax element naming convention violation.]	
//	}	
	uint8 bitstream_restriction_flag;//	u(1)
//	if( bitstream_restriction_flag ) {	
	uint8 tiles_fixed_structure_flag;//	u(1)
	uint8 motion_vectors_over_pic_boundaries_flag;//	u(1)
	uint8 restricted_ref_pic_lists_flag;//	u(1)
	uint16 min_spatial_segmentation_idc;//	ue(v) The value of min_spatial_segmentation_idc shall be in the range of 0 to 4095, inclusive
	uint8 max_bytes_per_pic_denom;//	ue(v) The value of max_bytes_per_pic_denom shall be in the range of 0 to 16, inclusive.
	uint8 max_bits_per_mincu_denom;//	ue(v)The value of max_bits_per_mincu_denom shall be in the range of 0 to 16, inclusive
	uint8 log2_max_mv_length_horizontal;//	ue(v) The value of log2_max_mv_length_horizontal shall be in the range of 0 to 16, inclusive.
	uint8 log2_max_mv_length_vertical;//	ue(v)
//	}	
//}	

} vui_parameters_data_t;

typedef struct _nal_unit_header_t {
	uint16 nuh_temporal_id_plus1 : 3; // u(3)
	uint16 nuh_layer_id : 6; // u(6)
	uint16 nal_unit_type : 6; // u(6)
	uint16 forbidden_zero_bit : 1; // f(1)
} nal_unit_header_t;

void profile_tier_level(nal_buffer_t * pnal_buffer, int maxNumSubLayersMinus1, profile_tier_level_data_t *profile_tier_level_data);
void short_term_ref_pic_set(nal_buffer_t * pnal_buffer, int stRpsIdx, int num_short_term_ref_pic_sets,  short_term_ref_pic_set_data_t * short_term_ref_pic_set_data);
void sub_layer_hrd_parameters(nal_buffer_t * pnal_buffer, uint32 CpbCnt, uint8 sub_pic_hrd_params_present_flag, sub_layer_hrd_parameters_data_t * sub_layer_hrd_parameters_data);
void hrd_parameters(nal_buffer_t * pnal_buffer, uint8 commonInfPresentFlag, uint32 maxNumSubLayersMinus1, hrd_parameters_data_t *hrd_parameters_data);
void vui_parameters(nal_buffer_t * pnal_buffer, uint8 sps_max_sub_layers_minus1, vui_parameters_data_t * vui_parameters_data);
void scaling_list_data(nal_buffer_t * pnal_buffer, scaling_list_data_data_t* scaling_list_data);
void print_profile_tier_level(nal_buffer_t * pnal_buffer, int maxNumSubLayersMinus1);
void print_short_term_ref_pic_set(nal_buffer_t * pnal_buffer, int stRpsIdx, int num_short_term_ref_pic_sets);
void print_sub_layer_hrd_parameters(nal_buffer_t * pnal_buffer, uint32 CpbCnt, uint8 sub_pic_hrd_params_present_flag);
void print_hrd_parameters(nal_buffer_t * pnal_buffer, uint8 commonInfPresentFlag, uint32 maxNumSubLayersMinus1);
void print_vui_parameters(nal_buffer_t * pnal_buffer, uint8 sps_max_sub_layers_minus1);
void print_scaling_list_data(nal_buffer_t * pnal_buffer);
void write_profile_tier_level(nal_buffer_t * pnal_buffer, int maxNumSubLayersMinus1, profile_tier_level_data_t *profile_tier_level_data);
void write_short_term_ref_pic_set(nal_buffer_t * pnal_buffer, int stRpsIdx, int num_short_term_ref_pic_sets,  short_term_ref_pic_set_data_t * short_term_ref_pic_set_data);
void write_vui_parameters(nal_buffer_t * pnal_buffer, uint8 sps_max_sub_layers_minus1, vui_parameters_data_t * vui_parameters_data);
void write_hrd_parameters(nal_buffer_t * pnal_buffer, uint8 commonInfPresentFlag, uint32 maxNumSubLayersMinus1, hrd_parameters_data_t *hrd_parameters_data);
void write_sub_layer_hrd_parameters(nal_buffer_t * pnal_buffer, uint32 CpbCnt, uint8 sub_pic_hrd_params_present_flag, sub_layer_hrd_parameters_data_t * sub_layer_hrd_parameters_data);
void write_scaling_list_data(nal_buffer_t * pnal_buffer, scaling_list_data_data_t* scaling_list_data);
#endif