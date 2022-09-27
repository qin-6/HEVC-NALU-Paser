#include <stdio.h>

#include "types.h"
#include "bio.h"
#include "cio.h"
#include "nal_common.h"
#include "nal_sps.h"
#include <cstring>
#include <cstdlib>



extern FILE * outfile;

void nal_sps_init() {

}

/** 
 * Removes 0x03 byte from stream
 */

void print_nal_sps_parse(nal_buffer_t *pInBuffer) {
	dump_nal_buffer(pInBuffer);
	
	nal_buffer_t out_buf = {0};
	decode_nul_rbsp(pInBuffer, &out_buf, pInBuffer->posmax);
	out_buf.posmax = out_buf.pos;
	out_buf.pos = 0;
	out_buf.bitpos = 8;

	nal_buffer_t *pnal_buffer = pInBuffer;
	pnal_buffer->pos += 2;
	fprintf(stdout, "\tsps_video_parameter_set_id=%d\n", read_bits(pnal_buffer, 4));
	uint8 sps_max_sub_layers_minus1 = read_bits(pnal_buffer, 3);
	fprintf(stdout, "\tsps_max_sub_layers_minus1=%d\n", sps_max_sub_layers_minus1);
	fprintf(stdout, "\tsps_temporal_id_nesting_flag=%d\n", read_bit(pnal_buffer));
	print_profile_tier_level(pnal_buffer, sps_max_sub_layers_minus1);
	fprintf(stdout, "\tsps_seq_parameter_set_id=%d\n", read_uev(pnal_buffer));
	uint8 chroma_format_idc = read_uev(pnal_buffer);														read_uev(pnal_buffer); read_uev(pnal_buffer); read_uev(pnal_buffer); read_bit(pnal_buffer); // somewhere lost 3 uev + 1 bit, does not work w/o this
	fprintf(stdout, "\tchroma_format_idc=%d\n", chroma_format_idc);
	if (chroma_format_idc == 3) {
		fprintf(stdout, "\tseparate_colour_plane_flag=%d\n", chroma_format_idc);
	}
	fprintf(stdout, "\tpic_width_in_luma_samples=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tpic_height_in_luma_samples=%d\n", read_uev(pnal_buffer));
	uint8 conformance_window_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tconformance_window_flag=%d\n", conformance_window_flag);
	if (conformance_window_flag) {
		fprintf(stdout, "\tconf_win_left_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tconf_win_right_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tconf_win_top_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tconf_win_bottom_offset=%d\n", read_uev(pnal_buffer));
	}
	fprintf(stdout, "\tbit_depth_luma_minus8=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tbit_depth_chroma_minus8=%d\n", read_uev(pnal_buffer));
	uint8 log2_max_pic_order_cnt_lsb_minus4 = read_uev(pnal_buffer);
	fprintf(stdout, "\tlog2_max_pic_order_cnt_lsb_minus4=%d\n", log2_max_pic_order_cnt_lsb_minus4);

	uint8 sps_sub_layer_ordering_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tsps_sub_layer_ordering_info_present_flag=%d\n", sps_sub_layer_ordering_info_present_flag);

	for (int i = (sps_sub_layer_ordering_info_present_flag ? 0 : sps_max_sub_layers_minus1);
		i <= sps_max_sub_layers_minus1; i++) {
		fprintf(stdout, "\tsps_max_dec_pic_buffering_minus1[i]=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tsps_max_num_reorder_pics[i]=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tsps_max_latency_increase_plus1[i]=%d\n", read_uev(pnal_buffer));
	}
	fprintf(stdout, "\tlog2_min_luma_coding_block_size_minus3=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tlog2_diff_max_min_luma_coding_block_size=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tlog2_min_transform_block_size_minus2=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tlog2_diff_max_min_transform_block_size=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tmax_transform_hierarchy_depth_inter=%d\n", read_uev(pnal_buffer));
	fprintf(stdout, "\tmax_transform_hierarchy_depth_intra=%d\n", read_uev(pnal_buffer));
	uint8 scaling_list_enabled_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tscaling_list_enabled_flag=%d\n", scaling_list_enabled_flag);
	if (scaling_list_enabled_flag) {
		uint8 sps_scaling_list_data_present_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tsps_scaling_list_data_present_flag=%d\n", sps_scaling_list_data_present_flag);
		if (sps_scaling_list_data_present_flag) {
			print_scaling_list_data(pnal_buffer);
		}
	}
	fprintf(stdout, "\tamp_enabled_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tsample_adaptive_offset_enabled_flag=%d\n", read_bit(pnal_buffer));

	uint8 pcm_enabled_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tpcm_enabled_flag=%d\n", pcm_enabled_flag);
	if (pcm_enabled_flag) {
		fprintf(stdout, "\tpcm_sample_bit_depth_luma_minus1=%d\n", read_bits(pnal_buffer, 4));
		fprintf(stdout, "\tpcm_sample_bit_depth_chroma_minus1=%d\n", read_bits(pnal_buffer, 4));
		fprintf(stdout, "\tlog2_min_pcm_luma_coding_block_size_minus3=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tlog2_diff_max_min_pcm_luma_coding_block_size=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tpcm_loop_filter_disabled_flag=%d\n", read_bit(pnal_buffer));
	}
	uint32 num_short_term_ref_pic_sets = read_uev(pnal_buffer);
	fprintf(stdout, "\tnum_short_term_ref_pic_sets=%d\n", num_short_term_ref_pic_sets);
	for (int i = 0; i < num_short_term_ref_pic_sets; i++) {
		print_short_term_ref_pic_set(pnal_buffer, i, num_short_term_ref_pic_sets);
	}
	uint8 long_term_ref_pics_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tlong_term_ref_pics_present_flag=%d\n", long_term_ref_pics_present_flag);
	if (long_term_ref_pics_present_flag) {
		uint32 num_long_term_ref_pics_sps = read_uev(pnal_buffer);
		fprintf(stdout, "\tnum_long_term_ref_pics_sps=%d\n", num_long_term_ref_pics_sps);
		for (int i = 0; i < num_long_term_ref_pics_sps; i++) {
			fprintf(stdout, "\tlt_ref_pic_poc_lsb_sps[%d]=%d\n", i, read_bits(pnal_buffer, log2_max_pic_order_cnt_lsb_minus4)); // u(v)
			fprintf(stdout, "\tused_by_curr_pic_lt_sps_flag[i]=%d\n", read_bit(pnal_buffer));
		}
	}
	fprintf(stdout, "\tsps_temporal_mvp_enabled_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tstrong_intra_smoothing_enabled_flag=%d\n", read_bit(pnal_buffer));
	uint8 vui_parameters_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tvui_parameters_present_flag=%d\n", vui_parameters_present_flag);
	if (vui_parameters_present_flag) {
		print_vui_parameters(pnal_buffer, sps_max_sub_layers_minus1);
	}
	fprintf(stdout, "\tsps_extension_flag=%d\n", read_bit(pnal_buffer));
}

/**
 * Process data from pnal_buffer
 */
void nal_sps_parse(nal_buffer_t * pnal_buffer) {
	//Decode SPS NAL rbsp data (remove 0x03 stuff)
	nal_buffer_t nal_buffer_decoded = {0};
	decode_nul_rbsp(pnal_buffer, &nal_buffer_decoded, pnal_buffer->posmax);
	nal_buffer_decoded.posmax = nal_buffer_decoded.pos;
	nal_buffer_decoded.pos = 0;
	nal_buffer_decoded.bitpos = 8;

	//parse SPS NAL to structure
	nal_sps_data_t* parset_sps_data;
	parset_sps_data = (nal_sps_data_t *)malloc(sizeof(nal_sps_data_t));
	memset(parset_sps_data, 0, sizeof(nal_sps_data_t));
	nal_sps_parse_data(&nal_buffer_decoded, parset_sps_data);
	
	//change data
	parset_sps_data->vui_parameters.video_signal_type_present_flag |= 1;
	parset_sps_data->vui_parameters.video_format = 5;
	parset_sps_data->vui_parameters.video_full_range_flag |= 1;
	parset_sps_data->vui_parameters.colour_description_present_flag |= 1;
	parset_sps_data->vui_parameters.colour_primaries = 9;
	parset_sps_data->vui_parameters.transfer_characteristics = 16;
	parset_sps_data->vui_parameters.matrix_coefficients = 9;

	//write data to byte buffer
	nal_buffer_t buffer_to_write;
	memset(&buffer_to_write, 0, sizeof(nal_buffer_t));
	buffer_to_write.pos = -1;
	nal_sps_write(&buffer_to_write, parset_sps_data);
	buffer_to_write.posmax = buffer_to_write.pos;
	buffer_to_write.pos = 0;
	buffer_to_write.bitpos = 8;

	//write byte buffer to output 
	write_nal_data_to_file(&buffer_to_write, outfile);
	free(parset_sps_data);
}

/**
 * Parses data as specified in HEVC spec
 */
void nal_sps_parse_data(nal_buffer_t * pnal_buffer, nal_sps_data_t * nal_sps_data) {
	
	nal_sps_data->nal_unit_header.forbidden_zero_bit |=  (uint16)read_bits(pnal_buffer, 1);
	nal_sps_data->nal_unit_header.nal_unit_type |=  (uint16)read_bits(pnal_buffer, 6);
	nal_sps_data->nal_unit_header.nuh_layer_id |=  (uint16)read_bits(pnal_buffer, 6);
	nal_sps_data->nal_unit_header.nuh_temporal_id_plus1 |=  (uint16)read_bits(pnal_buffer, 3);
	nal_sps_data->sps_video_parameter_set_id = (uint8)read_bits(pnal_buffer, 4);
	nal_sps_data->sps_max_sub_layers_minus1 = (uint8)read_bits(pnal_buffer, 3);
	nal_sps_data->sps_temporal_id_nesting_flag = (uint8)read_bit(pnal_buffer);
	profile_tier_level(pnal_buffer, nal_sps_data->sps_max_sub_layers_minus1, &nal_sps_data->profile_tier_level);
	nal_sps_data->sps_seq_parameter_set_id = read_uev(pnal_buffer);

	nal_sps_data->chroma_format_idc = read_uev(pnal_buffer);
	if (nal_sps_data->chroma_format_idc == 3) {
		nal_sps_data->separate_colour_plane_flag = (uint8)read_bit(pnal_buffer);
	}
	nal_sps_data->pic_width_in_luma_samples = read_uev(pnal_buffer);
	nal_sps_data->pic_height_in_luma_samples= read_uev(pnal_buffer);
	nal_sps_data->conformance_window_flag = (uint8)read_bit(pnal_buffer);
	if (nal_sps_data->conformance_window_flag) {
		nal_sps_data->conf_win_left_offset = read_uev(pnal_buffer);
		nal_sps_data->conf_win_right_offset = read_uev(pnal_buffer);
		nal_sps_data->conf_win_top_offset = read_uev(pnal_buffer);
		nal_sps_data->conf_win_bottom_offset = read_uev(pnal_buffer);
	}
	nal_sps_data->bit_depth_luma_minus8 = read_uev(pnal_buffer);
	nal_sps_data->bit_depth_chroma_minus8 = read_uev(pnal_buffer);
	nal_sps_data->log2_max_pic_order_cnt_lsb_minus4 = read_uev(pnal_buffer);

	nal_sps_data->sps_sub_layer_ordering_info_present_flag = read_bit(pnal_buffer);

	for (int i = (nal_sps_data->sps_sub_layer_ordering_info_present_flag ? 0 : nal_sps_data->sps_max_sub_layers_minus1);
		i <= nal_sps_data->sps_max_sub_layers_minus1; i++) {
		nal_sps_data->sps_max_dec_pic_buffering_minus1[i] = read_uev(pnal_buffer);
		nal_sps_data->sps_max_num_reorder_pics[i] = read_uev(pnal_buffer);
		nal_sps_data->sps_max_latency_increase_plus1[i] = read_uev(pnal_buffer);
	}
	nal_sps_data->log2_min_luma_coding_block_size_minus3 = read_uev(pnal_buffer);
	nal_sps_data->log2_diff_max_min_luma_coding_block_size = read_uev(pnal_buffer);
	nal_sps_data->log2_min_transform_block_size_minus2 = read_uev(pnal_buffer);
	nal_sps_data->log2_diff_max_min_transform_block_size = read_uev(pnal_buffer);
	nal_sps_data->max_transform_hierarchy_depth_inter = read_uev(pnal_buffer);
	nal_sps_data->max_transform_hierarchy_depth_intra = read_uev(pnal_buffer);
	nal_sps_data->scaling_list_enabled_flag = (uint8)read_bit(pnal_buffer);
	if (nal_sps_data->scaling_list_enabled_flag) {
		
		nal_sps_data->sps_scaling_list_data_present_flag = (uint8)read_bit(pnal_buffer);
		if (nal_sps_data->sps_scaling_list_data_present_flag) {
			scaling_list_data(pnal_buffer, &nal_sps_data->scaling_list_data);
		}
	}
	nal_sps_data->amp_enabled_flag = read_bit(pnal_buffer);
	nal_sps_data->sample_adaptive_offset_enabled_flag = read_bit(pnal_buffer);

	nal_sps_data->pcm_enabled_flag = (uint8)read_bit(pnal_buffer);
	if (nal_sps_data->pcm_enabled_flag) {
		nal_sps_data->pcm_sample_bit_depth_luma_minus1 = read_bits(pnal_buffer, 4);
		nal_sps_data->pcm_sample_bit_depth_chroma_minus1 = read_bits(pnal_buffer, 4);
		nal_sps_data->log2_min_pcm_luma_coding_block_size_minus3 = read_uev(pnal_buffer);
		nal_sps_data->log2_diff_max_min_pcm_luma_coding_block_size = read_uev(pnal_buffer);
		nal_sps_data->pcm_loop_filter_disable_flag = read_bit(pnal_buffer);
	}
	nal_sps_data->num_short_term_ref_pic_sets = read_uev(pnal_buffer);
	for (int i = 0; i < nal_sps_data->num_short_term_ref_pic_sets; i++) {
		short_term_ref_pic_set(pnal_buffer, i, nal_sps_data->num_short_term_ref_pic_sets, nal_sps_data->short_term_ref_pic_set);
	}
	nal_sps_data->long_term_ref_pics_present_flag = (uint8)read_bit(pnal_buffer);
	if (nal_sps_data->long_term_ref_pics_present_flag) {
		nal_sps_data->num_long_term_ref_pics_sps =  read_uev(pnal_buffer);
		for (int i = 0; i < nal_sps_data->num_long_term_ref_pics_sps; i++) {
			nal_sps_data->lt_ref_pic_poc_lsb_sps[i] = read_bits(pnal_buffer, nal_sps_data->log2_max_pic_order_cnt_lsb_minus4); // u(v)
			nal_sps_data->used_by_curr_pic_lt_sps_flag[i] = read_bit(pnal_buffer);
		}
	}
	nal_sps_data->sps_temporal_mvp_enabled_flag = read_bit(pnal_buffer);
	nal_sps_data->strong_intra_smoothing_enabled_flag = read_bit(pnal_buffer);
	
	nal_sps_data->vui_parameters_present_flag = (uint8)read_bit(pnal_buffer);
	if (nal_sps_data->vui_parameters_present_flag) {
		vui_parameters(pnal_buffer, nal_sps_data->sps_max_sub_layers_minus1, &nal_sps_data->vui_parameters);
	}
	nal_sps_data->sps_extension_flag = (uint8)read_bit(pnal_buffer);
	nal_sps_data->sps_extension_flag = 0; //Decoders conforming to this version of this Specification shall ignore 
	                                      //(remove from the bitstream and discard) all sps_extension_data_flag syntax elements 
}

/**
 * Writes SPS struct data to byte stream buffer
 */
void nal_sps_write(nal_buffer_t* pnal_buffer, nal_sps_data_t *nal_sps_data)
{
	write_bits(pnal_buffer, &nal_sps_data->nal_unit_header, 16);
	write_bits(pnal_buffer, &nal_sps_data->sps_video_parameter_set_id, 4);
	write_bits(pnal_buffer, &nal_sps_data->sps_max_sub_layers_minus1, 3);
	write_bit(pnal_buffer, nal_sps_data->sps_temporal_id_nesting_flag);

	write_profile_tier_level(pnal_buffer, nal_sps_data->sps_max_sub_layers_minus1, &nal_sps_data->profile_tier_level);
	write_uev(pnal_buffer, nal_sps_data->sps_seq_parameter_set_id);

	write_uev(pnal_buffer, nal_sps_data->chroma_format_idc);
	if (nal_sps_data->chroma_format_idc == 3)
	{
		write_bit(pnal_buffer, nal_sps_data->separate_colour_plane_flag);
	}
	write_uev(pnal_buffer, nal_sps_data->pic_width_in_luma_samples);
	write_uev(pnal_buffer, nal_sps_data->pic_height_in_luma_samples);
	write_bit(pnal_buffer, nal_sps_data->conformance_window_flag);
	if (nal_sps_data->conformance_window_flag)
	{
		write_uev(pnal_buffer, nal_sps_data->conf_win_left_offset);
		write_uev(pnal_buffer, nal_sps_data->conf_win_right_offset);
		write_uev(pnal_buffer, nal_sps_data->conf_win_top_offset);
		write_uev(pnal_buffer, nal_sps_data->conf_win_bottom_offset);
	}
	write_uev(pnal_buffer, nal_sps_data->bit_depth_luma_minus8);
	write_uev(pnal_buffer, nal_sps_data->bit_depth_chroma_minus8);
	write_uev(pnal_buffer, nal_sps_data->log2_max_pic_order_cnt_lsb_minus4);

	write_bit(pnal_buffer, nal_sps_data->sps_sub_layer_ordering_info_present_flag);

	for (int i = (nal_sps_data->sps_sub_layer_ordering_info_present_flag ? 0 : nal_sps_data->sps_max_sub_layers_minus1);
	     i <= nal_sps_data->sps_max_sub_layers_minus1; i++)
	{
		write_uev(pnal_buffer, nal_sps_data->sps_max_dec_pic_buffering_minus1[i]);
		write_uev(pnal_buffer, nal_sps_data->sps_max_num_reorder_pics[i]);
		write_uev(pnal_buffer, nal_sps_data->sps_max_latency_increase_plus1[i]);
	}
	write_uev(pnal_buffer, nal_sps_data->log2_min_luma_coding_block_size_minus3);
	write_uev(pnal_buffer, nal_sps_data->log2_diff_max_min_luma_coding_block_size);
	write_uev(pnal_buffer, nal_sps_data->log2_min_transform_block_size_minus2);
	write_uev(pnal_buffer, nal_sps_data->log2_diff_max_min_transform_block_size);
	write_uev(pnal_buffer, nal_sps_data->max_transform_hierarchy_depth_inter);
	write_uev(pnal_buffer, nal_sps_data->max_transform_hierarchy_depth_intra);
	write_bit(pnal_buffer, nal_sps_data->scaling_list_enabled_flag);
	if (nal_sps_data->scaling_list_enabled_flag)
	{
		write_bit(pnal_buffer, nal_sps_data->sps_scaling_list_data_present_flag);
		if (nal_sps_data->sps_scaling_list_data_present_flag)
		{
			write_scaling_list_data(pnal_buffer, &nal_sps_data->scaling_list_data);
		}
	}
	write_bit(pnal_buffer, nal_sps_data->amp_enabled_flag);
	write_bit(pnal_buffer, nal_sps_data->sample_adaptive_offset_enabled_flag);

	write_bit(pnal_buffer, nal_sps_data->pcm_enabled_flag);
	if (nal_sps_data->pcm_enabled_flag)
	{
		write_bits(pnal_buffer, &nal_sps_data->pcm_sample_bit_depth_luma_minus1, 4);
		write_bits(pnal_buffer, &nal_sps_data->pcm_sample_bit_depth_chroma_minus1, 4);
		write_uev(pnal_buffer, nal_sps_data->log2_min_pcm_luma_coding_block_size_minus3);
		write_uev(pnal_buffer, nal_sps_data->log2_diff_max_min_pcm_luma_coding_block_size);
		write_bit(pnal_buffer, nal_sps_data->pcm_loop_filter_disable_flag);
	}
	write_uev(pnal_buffer, nal_sps_data->num_short_term_ref_pic_sets);
	for (int i = 0; i < nal_sps_data->num_short_term_ref_pic_sets; i++)
	{
		write_short_term_ref_pic_set(pnal_buffer, i, nal_sps_data->num_short_term_ref_pic_sets, nal_sps_data->short_term_ref_pic_set);
	}
	write_bit(pnal_buffer, nal_sps_data->long_term_ref_pics_present_flag);
	if (nal_sps_data->long_term_ref_pics_present_flag)
	{
		write_uev(pnal_buffer, nal_sps_data->num_long_term_ref_pics_sps);
		for (int i = 0; i < nal_sps_data->num_long_term_ref_pics_sps; i++)
		{
			write_bits(pnal_buffer, &nal_sps_data->lt_ref_pic_poc_lsb_sps[i], nal_sps_data->log2_max_pic_order_cnt_lsb_minus4); // u(v)
			write_bit(pnal_buffer, nal_sps_data->used_by_curr_pic_lt_sps_flag[i]);
		}
	}
	write_bit(pnal_buffer, nal_sps_data->sps_temporal_mvp_enabled_flag);
	write_bit(pnal_buffer, nal_sps_data->strong_intra_smoothing_enabled_flag);

	write_bit(pnal_buffer, nal_sps_data->vui_parameters_present_flag);
	if (nal_sps_data->vui_parameters_present_flag)
	{
		write_vui_parameters(pnal_buffer, nal_sps_data->sps_max_sub_layers_minus1, &nal_sps_data->vui_parameters);
	}
	write_bit(pnal_buffer, nal_sps_data->sps_extension_flag);

//	7.3.2.11	RBSP trailing bits syntax
//
//		rbsp_trailing_bits( ) {	Descriptor
//			rbsp_stop_one_bit  /* equal to 1 */	f(1)
//			while( !byte_aligned( ) )	
//				rbsp_alignment_zero_bit  /* equal to 0 */	f(1)
//		}		

	if (pnal_buffer->bitpos != 0)
	{
		write_bit(pnal_buffer, 1);
	}
	while (pnal_buffer->bitpos != 0)
	{
		write_bit(pnal_buffer, 0);
	}
}

