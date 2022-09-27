#ifndef __NAL_SPS_H
#define __NAL_SPS_H
#include "nal_common.h"

typedef struct _nal_sps_data_t
{
	_nal_unit_header_t nal_unit_header; //u(16)
	uint8 sps_video_parameter_set_id; //u(4)
	uint8 sps_max_sub_layers_minus1; //u(3)
	uint8 sps_temporal_id_nesting_flag; //u(1)
	profile_tier_level_data_t profile_tier_level; // ( sps_max_sub_layers_minus1 )	
	uint8 sps_seq_parameter_set_id;//	ue(v) The value of sps_seq_parameter_set_id shall be in the range of 0 to 15, inclusive
	uint8 chroma_format_idc; //	ue(v) value of chroma_format_idc shall be in the range of 0 to 3, inclusive
	//if( chroma_format_idc  = =  3 )	
	uint8 separate_colour_plane_flag;//	u(1)
	uint32 pic_width_in_luma_samples;//	ue(v)
	uint32 pic_height_in_luma_samples;//	ue(v)
	uint8 conformance_window_flag;//	u(1)
	//if( conformance_window_flag ) 	
	uint32 conf_win_left_offset; //ue(v)
	uint32 conf_win_right_offset;//	ue(v)
	uint32 conf_win_top_offset;//	ue(v)
	uint32 conf_win_bottom_offset;//	ue(v)
	uint8 bit_depth_luma_minus8;//	ue(v) bit_depth_luma_minus8 shall be in the range of 0 to 6, inclusive.
	uint8 bit_depth_chroma_minus8;//	ue(v) bit_depth_chroma_minus8 shall be in the range of 0 to 6, inclusive.
	uint8 log2_max_pic_order_cnt_lsb_minus4;//	ue(v) shall be in the range of 0 to 12, inclusive.
	uint8 sps_sub_layer_ordering_info_present_flag;//	u(1)
	//	for( i = ( sps_sub_layer_ordering_info_present_flag ?
	//			0 : sps_max_sub_layers_minus1 );
	//			i <= sps_max_sub_layers_minus1; i++ ) {	sps_max_sub_layers_minus1 0..6
	uint32 sps_max_dec_pic_buffering_minus1[8]; //	ue(v) shall be in the range of 0 to MaxDpbSize − 1 
	uint32 sps_max_num_reorder_pics[8]; //	ue(v)
	uint32 sps_max_latency_increase_plus1[8]; //	ue(v)

	uint32 log2_min_luma_coding_block_size_minus3; //	ue(v)
	uint32 log2_diff_max_min_luma_coding_block_size; //	ue(v)
	uint32 log2_min_transform_block_size_minus2; //	ue(v)
	uint32 log2_diff_max_min_transform_block_size; //	ue(v)
	uint32 max_transform_hierarchy_depth_inter; //	ue(v)
	uint32 max_transform_hierarchy_depth_intra; //	ue(v)
	uint8 scaling_list_enabled_flag;//	u(1)
	//if( scaling_list_enabled_flag ) {	
	uint8 sps_scaling_list_data_present_flag;//	u(1)
	//if( sps_scaling_list_data_present_flag )	
	scaling_list_data_data_t scaling_list_data; //( )	
	uint8 amp_enabled_flag;//	u(1)
	uint8 sample_adaptive_offset_enabled_flag;//	u(1)
	uint8 pcm_enabled_flag;//	u(1)
	//if( pcm_enabled_flag ) {	
	uint8 pcm_sample_bit_depth_luma_minus1;//	u(4)
	uint8 pcm_sample_bit_depth_chroma_minus1;//	u(4)
	uint32 log2_min_pcm_luma_coding_block_size_minus3;//	ue(v)
	uint32 log2_diff_max_min_pcm_luma_coding_block_size;//	ue(v)
	uint8 pcm_loop_filter_disable_flag;//	u(1)
	uint32 num_short_term_ref_pic_sets;//	ue(v) value of num_short_term_ref_pic_sets shall be in the range of 0 to 64, inclusive
	//for( i = 0; i < num_short_term_ref_pic_sets; i++)	
	short_term_ref_pic_set_data_t short_term_ref_pic_set[65];
	uint8 long_term_ref_pics_present_flag;//	u(1)
	//if( long_term_ref_pics_present_flag ) {	
	uint32 num_long_term_ref_pics_sps;//	ue(v) value of num_long_term_ref_pics_sps shall be in the range of 0 to 32, inclusive
	//for( i = 0; i < num_long_term_ref_pics_sps; i++ ) {	
	uint32 lt_ref_pic_poc_lsb_sps[33];//	u(v)
	uint32 used_by_curr_pic_lt_sps_flag[33];//	u(1)
	uint8 sps_temporal_mvp_enabled_flag;//	u(1)
	uint8 strong_intra_smoothing_enabled_flag;//	u(1)
	uint8 vui_parameters_present_flag;//	u(1)
	//if( vui_parameters_present_flag )	
	vui_parameters_data_t vui_parameters; //( )	
	uint8 sps_extension_flag;//	u(1)
	//if( sps_extension_flag )	Decoders conforming to this version of this Specification shall ignore 
	//(remove from the bitstream and discard) all sps_extension_data_flag syntax elements
	//	     while( more_rbsp_data( ) )	
	//			sps_extension_data_flag	u(1)
	//	rbsp_trailing_bits( )	
} nal_sps_data_t;

void nal_sps_init();
void nal_sps_parse(nal_buffer_t* pnal_buffer);
void nal_sps_parse_data(nal_buffer_t* pnal_buffer, nal_sps_data_t *nal_sps_data);
void nal_sps_write(nal_buffer_t* pnal_buffer, nal_sps_data_t *nal_sps_data);
void print_nal_sps_parse(nal_buffer_t * pnal_buffer);


#endif
