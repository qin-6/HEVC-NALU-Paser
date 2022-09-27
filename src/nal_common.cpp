#include <stdio.h>

#include "types.h"
#include "cio.h"
#include "bio.h"
#include "nal_common.h"

void print_profile_tier_level(nal_buffer_t* pnal_buffer, int maxNumSubLayersMinus1)
{
	fprintf(stdout, "\t--- profile_tier(%d) ---\n", maxNumSubLayersMinus1);
	fprintf(stdout, "\tgeneral_profile_space=%d\n", read_bits(pnal_buffer, 2));
	fprintf(stdout, "\tgeneral_tier_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tgeneral_profile_idc=%d\n", read_bits(pnal_buffer, 5));
	for (int j = 0; j < 32; j++)
	{
		fprintf(stdout, "\tgeneral_profile_compatibility_flag[%d]=%d\n", j, read_bit(pnal_buffer));
	}
	fprintf(stdout, "\tgeneral_progressive_source_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tgeneral_interlaced_source_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tgeneral_non_packed_constraint_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tgeneral_frame_only_constraint_flag=%d\n", read_bit(pnal_buffer));
	//general_reserved_zero_44bits u(44)
	read_bits(pnal_buffer, 44);
	fprintf(stdout, "\tskipping 44 reserved bits...\n");

	fprintf(stdout, "\tgeneral_level_idc=%d\n", read_bits(pnal_buffer, 8));
	uint8 sub_layer_profile_present_flag[8];
	uint8 sub_layer_level_present_flag[8];
	for (int i = 0; i < maxNumSubLayersMinus1; i++)
	{
		sub_layer_profile_present_flag[i] = read_bit(pnal_buffer);
		fprintf(stdout, "\tsub_layer_profile_present_flag[%d]=%d\n", i, sub_layer_profile_present_flag[i]);
		sub_layer_level_present_flag[i] = read_bit(pnal_buffer);
		fprintf(stdout, "\tsub_layer_level_present_flag[%d]=%d\n", i, sub_layer_level_present_flag[i]);
	}

	if (maxNumSubLayersMinus1 > 0)
	{
		for (int i = maxNumSubLayersMinus1; i < 8; i++)
		{
			fprintf(stdout, "\treserved_zero_2bits[%d]=%d\n", i, read_bits(pnal_buffer, 2));
		}
	}

	for (int i = 0; i < maxNumSubLayersMinus1; i++)
	{
		if (sub_layer_profile_present_flag[i])
		{
			fprintf(stdout, "\tsub_layer_profile_space[%d]=%d\n", i, read_bits(pnal_buffer, 2));
			fprintf(stdout, "\tsub_layer_tier_flag[%d]=%d\n", i, read_bit(pnal_buffer));
			fprintf(stdout, "\tsub_layer_profile_idc[%d]=%d\n", i, read_bits(pnal_buffer, 5));
			for (int j = 0; j < 32; j++)
			{
				fprintf(stdout, "\tsub_layer_profile_compatibility_flag[%d][%d]=%d\n", i, j, read_bit(pnal_buffer));
			}
			fprintf(stdout, "\tsub_layer_progressive_source_flag[%d]=%d\n", i, read_bit(pnal_buffer));
			fprintf(stdout, "\tsub_layer_interlaced_source_flag[%d]=%d\n", i, read_bit(pnal_buffer));
			fprintf(stdout, "\tsub_layer_non_packed_constraint_flag[%d]=%d\n", i, read_bit(pnal_buffer));
			fprintf(stdout, "\tsub_layer_frame_only_constraint_flag[%d]=%d\n", i, read_bit(pnal_buffer));
			//sub_layer_reserved_zero_44bits[i] u(44)
			read_bits(pnal_buffer, 44);
			fprintf(stdout, "\tskipping sub_layer_reserved_zero_44bits[%d]\n", i);
		}
		if (sub_layer_level_present_flag[i])
		{
			fprintf(stdout, "\tsub_layer_level_idc[%d]=%d\n", i, read_bits(pnal_buffer, 8));
		}
	}
}

void profile_tier_level(nal_buffer_t* pnal_buffer, int maxNumSubLayersMinus1, profile_tier_level_data_t* profile_tier_level_data)
{
	profile_tier_level_data->general_profile_space = (uint8)read_bits(pnal_buffer, 2);
	profile_tier_level_data->general_tier_flag = (uint8)read_bit(pnal_buffer);
	profile_tier_level_data->general_profile_idc = (uint8)read_bits(pnal_buffer, 5);
	for (int j = 0; j < 32; j++)
	{
		profile_tier_level_data->general_profile_compatibility_flag[j] = (uint8)read_bit(pnal_buffer);
	}
	profile_tier_level_data->general_progressive_source_flag = (uint8)read_bit(pnal_buffer);
	profile_tier_level_data->general_interlaced_source_flag = (uint8)read_bit(pnal_buffer);
	profile_tier_level_data->general_non_packed_constraint_flag = (uint8)read_bit(pnal_buffer);
	profile_tier_level_data->general_frame_only_constraint_flag = (uint8)read_bit(pnal_buffer);
	//profile_tier_level_data->general_reserved_zero_44bits = (uint64)read_bits64(pnal_buffer, 44);
	if (profile_tier_level_data->general_profile_idc == 4 || profile_tier_level_data->general_profile_compatibility_flag[4] ||
		profile_tier_level_data->general_profile_idc == 5 || profile_tier_level_data->general_profile_compatibility_flag[5] ||
		profile_tier_level_data->general_profile_idc == 6 || profile_tier_level_data->general_profile_compatibility_flag[6] ||
		profile_tier_level_data->general_profile_idc == 7 || profile_tier_level_data->general_profile_compatibility_flag[7])
	/* The number of bits in this syntax structure is not affected by this condition */
	{
		profile_tier_level_data->general_max_12bit_constraint_flag = (uint8)read_bit(pnal_buffer);//	u(1)
		profile_tier_level_data->general_max_10bit_constraint_flag = (uint8)read_bit(pnal_buffer);//	u(1)
		profile_tier_level_data->general_max_8bit_constraint_flag = (uint8)read_bit(pnal_buffer);//	u(1)
		profile_tier_level_data->general_max_422chroma_constraint_flag = (uint8)read_bit(pnal_buffer);//	u(1)
		profile_tier_level_data->general_max_420chroma_constraint_flag = (uint8)read_bit(pnal_buffer);//	u(1)
		profile_tier_level_data->general_max_monochrome_constraint_flag = (uint8)read_bit(pnal_buffer);//	u(1)
		profile_tier_level_data->general_intra_constraint_flag = (uint8)read_bit(pnal_buffer);//	u(1)
		profile_tier_level_data->general_one_picture_only_constraint_flag = (uint8)read_bit(pnal_buffer);//	u(1)
		profile_tier_level_data->general_lower_bit_rate_constraint_flag = (uint8)read_bit(pnal_buffer);//	u(1)
		profile_tier_level_data->general_reserved_zero_34bits = (uint64)read_bits64(pnal_buffer, 34);//	u(34)
	}
	else
	{
		profile_tier_level_data->general_reserved_zero_43bits = (uint64)read_bits64(pnal_buffer, 43);//	u(43)
		if ((profile_tier_level_data->general_profile_idc >= 1 && profile_tier_level_data->general_profile_idc <= 5) ||
			profile_tier_level_data->general_profile_compatibility_flag[1] || profile_tier_level_data->general_profile_compatibility_flag[2] ||
			profile_tier_level_data->general_profile_compatibility_flag[3] || profile_tier_level_data->general_profile_compatibility_flag[4] ||
			profile_tier_level_data->general_profile_compatibility_flag[5])
		/* The number of bits in this syntax structure is not affected by this condition */
		{
			profile_tier_level_data->general_inbld_flag = (uint8)read_bit(pnal_buffer); //u(1)
		}
		else
		{
			profile_tier_level_data->general_reserved_zero_bit = (uint8)read_bit(pnal_buffer);//	u(1)
		}
	}
	profile_tier_level_data->general_level_idc = (uint8)read_bits(pnal_buffer, 8);
	for (int i = 0; i < maxNumSubLayersMinus1; i++)
	{
		profile_tier_level_data->sub_layer_profile_present_flag[i] = (uint8)read_bit(pnal_buffer);
		profile_tier_level_data->sub_layer_level_present_flag[i] = (uint8)read_bit(pnal_buffer);
	}

	if (maxNumSubLayersMinus1 > 0)
	{
		for (int i = maxNumSubLayersMinus1; i < 8; i++)
		{
			profile_tier_level_data->reserved_zero_2bits[i] = (uint8)read_bits(pnal_buffer, 2);
		}
	}

	for (int i = 0; i < maxNumSubLayersMinus1; i++)
	{
		if (profile_tier_level_data->sub_layer_profile_present_flag[i])
		{
			profile_tier_level_data->sub_layer_profile_space[i] = (uint8)read_bits(pnal_buffer, 2);
			profile_tier_level_data->sub_layer_tier_flag[i] = (uint8)read_bit(pnal_buffer);
			profile_tier_level_data->sub_layer_profile_idc[i] = (uint8)read_bits(pnal_buffer, 5);
			for (int j = 0; j < 32; j++)
			{
				profile_tier_level_data->sub_layer_profile_compatibility_flag[i][j] = (uint8)read_bit(pnal_buffer);
			}
			profile_tier_level_data->sub_layer_progressive_source_flag[i] = read_bit(pnal_buffer);
			profile_tier_level_data->sub_layer_interlaced_source_flag[i] = read_bit(pnal_buffer);
			profile_tier_level_data->sub_layer_non_packed_constraint_flag[i] = read_bit(pnal_buffer);
			profile_tier_level_data->sub_layer_frame_only_constraint_flag[i] = read_bit(pnal_buffer);
			//profile_tier_level_data->sub_layer_reserved_zero_44bits[i] = (uint64)read_bits64(pnal_buffer, 44);

			if (profile_tier_level_data->sub_layer_profile_idc[i] == 4 ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[i][4] ||
				profile_tier_level_data->sub_layer_profile_idc[i] == 5 ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[i][5] ||
				profile_tier_level_data->sub_layer_profile_idc[i] == 6 ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[i][6] ||
				profile_tier_level_data->sub_layer_profile_idc[i] == 7 ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[i][7])
			{
				/* The number of bits in this syntax structure is not affected by this condition */
				profile_tier_level_data->sub_layer_max_12bit_constraint_flag[i] = (uint8)read_bit(pnal_buffer);//	u(1)
				profile_tier_level_data->sub_layer_max_10bit_constraint_flag[i] = (uint8)read_bit(pnal_buffer);//	u(1)
				profile_tier_level_data->sub_layer_max_8bit_constraint_flag[i] = (uint8)read_bit(pnal_buffer);//	u(1)
				profile_tier_level_data->sub_layer_max_422chroma_constraint_flag[i] = (uint8)read_bit(pnal_buffer);//	u(1)
				profile_tier_level_data->sub_layer_max_420chroma_constraint_flag[i] = (uint8)read_bit(pnal_buffer);//	u(1)
				profile_tier_level_data->sub_layer_max_monochrome_constraint_flag[i] = (uint8)read_bit(pnal_buffer);//	u(1)
				profile_tier_level_data->sub_layer_intra_constraint_flag[i] = (uint8)read_bit(pnal_buffer);//	u(1)
				profile_tier_level_data->sub_layer_one_picture_only_constraint_flag[i] = (uint8)read_bit(pnal_buffer);//	u(1)
				profile_tier_level_data->sub_layer_lower_bit_rate_constraint_flag[i] = (uint8)read_bit(pnal_buffer);//	u(1)
				profile_tier_level_data->sub_layer_reserved_zero_34bits[i] = (uint64)read_bits(pnal_buffer, 34);//	u(34)
			}
			else
			{
				profile_tier_level_data->sub_layer_reserved_zero_43bits[i] = read_bits64(pnal_buffer, 43);//	u(43)
			}
			if ((profile_tier_level_data->sub_layer_profile_idc[i] >= 1 && profile_tier_level_data->sub_layer_profile_idc[i] <= 5) ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[1] ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[2] ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[3] ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[4] ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[5])
			/* The number of bits in this syntax structure is not affected by this condition */
			{
				profile_tier_level_data->sub_layer_inbld_flag[i] = (uint8)read_bit(pnal_buffer);//		u(1)
			}
			else
			{
				profile_tier_level_data->sub_layer_reserved_zero_bit[i] = (uint8)read_bit(pnal_buffer);//		u(1)
			}
		}
		if (profile_tier_level_data->sub_layer_level_present_flag[i])
		{
			profile_tier_level_data->sub_layer_level_idc[i] = read_bits(pnal_buffer, 8);
		}
	}
}

void print_short_term_ref_pic_set(nal_buffer_t* pnal_buffer, int stRpsIdx, int num_short_term_ref_pic_sets)
{
	fprintf(stdout, "\t--- short_term_ref_pic_set(%d, %d) ---\n", stRpsIdx, num_short_term_ref_pic_sets);
	uint8 inter_ref_pic_set_prediction_flag = 0;
	if (stRpsIdx != 0)
	{
		inter_ref_pic_set_prediction_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tinter_ref_pic_set_prediction_flag=%d\n", inter_ref_pic_set_prediction_flag);
	}

	if (inter_ref_pic_set_prediction_flag)
	{
		int delta_idx_minus1 = read_uev(pnal_buffer);
		if (stRpsIdx == num_short_term_ref_pic_sets)
		{
			fprintf(stdout, "\tdelta_idx_minus1=%d\n", delta_idx_minus1);
		}
		fprintf(stdout, "\tdelta_rps_sign=%d\n", read_bit(pnal_buffer));
		fprintf(stdout, "\tabs_delta_rps_minus1=%d\n", read_uev(pnal_buffer));
		int RefRpsIdx = stRpsIdx - (delta_idx_minus1 + 1);
		printf("need NuDeltaPocs here !!!\n");
		/*
		for (j = 0; j <= NumDeltaPocs[RefRpsIdx]; j++) {
		uint8 used_by_curr_pic_flag_j = read_bit(pnal_buffer);
		fprintf(stdout, "\tused_by_curr_pic_flag[j]=%d\n", used_by_curr_pic_flag_j);
		if (!used_by_curr_pic_flag_j)
		fprintf(stdout, "\tuse_delta_flag[j]=%d\n", read_bit(pnal_buffer));
		}
		*/
	}
	else
	{
		uint32 num_negative_pics = read_uev(pnal_buffer);
		fprintf(stdout, "\tnum_negative_pics=%d\n", num_negative_pics);
		uint32 num_positive_pics = read_uev(pnal_buffer);
		fprintf(stdout, "\tnum_positive_pics=%d\n", num_positive_pics);
		for (int i = 0; i < num_negative_pics; i++)
		{
			fprintf(stdout, "\tdelta_poc_s0_minus1[i]=%d\n", read_uev(pnal_buffer));
			fprintf(stdout, "\tused_by_curr_pic_s0_flag[i]=%d\n", read_bit(pnal_buffer));
		}
		for (int i = 0; i < num_positive_pics; i++)
		{
			fprintf(stdout, "\tdelta_poc_s1_minus1[i]=%d\n", read_uev(pnal_buffer));
			fprintf(stdout, "\tused_by_curr_pic_s1_flag[i]=%d\n", read_bit(pnal_buffer));
		}
	}
}

void short_term_ref_pic_set(nal_buffer_t* pnal_buffer, int stRpsIdx, int num_short_term_ref_pic_sets, short_term_ref_pic_set_data_t* short_term_ref_pic_set_data)
{
	if (stRpsIdx != 0)
	{
		short_term_ref_pic_set_data->inter_ref_pic_set_prediction_flag = (uint8)read_bit(pnal_buffer);
	}
	if (short_term_ref_pic_set_data->inter_ref_pic_set_prediction_flag)
	{
		if (stRpsIdx == num_short_term_ref_pic_sets)
		{
			short_term_ref_pic_set_data->delta_idx_minus1 = read_uev(pnal_buffer);
		}
		short_term_ref_pic_set_data->delta_rps_sign = (uint8)read_bit(pnal_buffer);
		short_term_ref_pic_set_data->abs_delta_rps_minus1 = (uint16)read_uev(pnal_buffer);
		int RefRpsIdx = stRpsIdx - (short_term_ref_pic_set_data->delta_idx_minus1 + 1);
		int NumDeltaPocs = short_term_ref_pic_set_data->num_negative_pics[RefRpsIdx] + short_term_ref_pic_set_data->num_positive_pics[RefRpsIdx];
		for (int j = 0; j <= NumDeltaPocs; j++)
		{
			short_term_ref_pic_set_data->used_by_curr_pic_flag[j] = (uint8)read_bit(pnal_buffer);
			if (!short_term_ref_pic_set_data->used_by_curr_pic_flag[j])
				short_term_ref_pic_set_data->use_delta_flag[j] = read_bit(pnal_buffer);
		}
	}
	else
	{
		short_term_ref_pic_set_data->num_negative_pics[stRpsIdx] = read_uev(pnal_buffer);
		short_term_ref_pic_set_data->num_positive_pics[stRpsIdx] = read_uev(pnal_buffer);
		for (int i = 0; i < short_term_ref_pic_set_data->num_negative_pics[stRpsIdx]; i++)
		{
			short_term_ref_pic_set_data->delta_poc_s0_minus1[i] = read_uev(pnal_buffer);
			short_term_ref_pic_set_data->used_by_curr_pic_s0_flag[i] = (uint8)read_bit(pnal_buffer);
		}
		for (int i = 0; i < short_term_ref_pic_set_data->num_positive_pics[stRpsIdx]; i++)
		{
			short_term_ref_pic_set_data->delta_poc_s1_minus1[i] = read_uev(pnal_buffer);
			short_term_ref_pic_set_data->used_by_curr_pic_s1_flag[i] = (uint8)read_bit(pnal_buffer);
		}
	}
}

void print_sub_layer_hrd_parameters(nal_buffer_t* pnal_buffer, uint32 CpbCnt, uint8 sub_pic_hrd_params_present_flag)
{
	fprintf(stdout, "\t--- sub_layer_hrd_parameters(%d, %d) ---\n", CpbCnt, sub_pic_hrd_params_present_flag);
	for (int i = 0; i <= CpbCnt; i++)
	{
		fprintf(stdout, "\tbit_rate_value_minus1[%d]=%d\n", i, read_uev(pnal_buffer));
		fprintf(stdout, "\tcpb_size_value_minus1[%d]=%d\n", i, read_uev(pnal_buffer));
		if (sub_pic_hrd_params_present_flag)
		{
			fprintf(stdout, "\tcpb_size_du_value_minus1[%d]=%d\n", i, read_uev(pnal_buffer));
			fprintf(stdout, "\tbit_rate_du_value_minus1[%d]=%d\n", i, read_uev(pnal_buffer));
		}
		fprintf(stdout, "\tcbr_flag[%d]=%d\n", i, read_bit(pnal_buffer));
	}
}

void sub_layer_hrd_parameters(nal_buffer_t* pnal_buffer, uint32 CpbCnt, uint8 sub_pic_hrd_params_present_flag, sub_layer_hrd_parameters_data_t* sub_layer_hrd_parameters_data)
{
	for (int i = 0; i <= CpbCnt; i++)
	{
		sub_layer_hrd_parameters_data->bit_rate_value_minus1[i] = read_uev(pnal_buffer);
		sub_layer_hrd_parameters_data->cpb_size_value_minus1[i] = read_uev(pnal_buffer);
		if (sub_pic_hrd_params_present_flag)
		{
			sub_layer_hrd_parameters_data->cpb_size_du_value_minus1[i] = read_uev(pnal_buffer);
			sub_layer_hrd_parameters_data->bit_rate_du_value_minus1[i] = read_uev(pnal_buffer);
		}
		sub_layer_hrd_parameters_data->cbr_flag[i] = read_bit(pnal_buffer);
	}
}

void print_hrd_parameters(nal_buffer_t* pnal_buffer, uint8 commonInfPresentFlag, uint32 maxNumSubLayersMinus1)
{
	fprintf(stdout, "\t--- hrd_parameters(%d, %d) ---\n", commonInfPresentFlag, maxNumSubLayersMinus1);
	uint8 nal_hrd_parameters_present_flag = 0;
	uint8 vcl_hrd_parameters_present_flag = 0;
	uint8 sub_pic_hrd_params_present_flag = 0;
	if (commonInfPresentFlag)
	{
		nal_hrd_parameters_present_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tnal_hrd_parameters_present_flag=%d\n", nal_hrd_parameters_present_flag);
		vcl_hrd_parameters_present_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tvcl_hrd_parameters_present_flag=%d\n", vcl_hrd_parameters_present_flag);
		if (nal_hrd_parameters_present_flag || vcl_hrd_parameters_present_flag)
		{
			sub_pic_hrd_params_present_flag = read_bit(pnal_buffer);
			fprintf(stdout, "\tsub_pic_hrd_params_present_flag=%d\n", sub_pic_hrd_params_present_flag);
			if (sub_pic_hrd_params_present_flag)
			{
				fprintf(stdout, "\ttick_divisor_minus2=%d\n", read_bits(pnal_buffer, 8));
				fprintf(stdout, "\tdu_cpb_removal_delay_increment_length_minus1=%d\n", read_bits(pnal_buffer, 5));
				fprintf(stdout, "\tsub_pic_cpb_params_in_pic_timing_sei_flag=%d\n", read_bit(pnal_buffer));
				fprintf(stdout, "\tdpb_output_delay_du_length_minus1=%d\n", read_bits(pnal_buffer, 5));
			}
			fprintf(stdout, "\tbit_rate_scale=%d\n", read_bits(pnal_buffer, 4));
			fprintf(stdout, "\tcpb_size_scale=%d\n", read_bits(pnal_buffer, 4));
			if (sub_pic_hrd_params_present_flag)
			{
				fprintf(stdout, "\tcpb_size_du_scale=%d\n", read_bits(pnal_buffer, 4));
			}
			fprintf(stdout, "\tinitial_cpb_removal_delay_length_minus1=%d\n", read_bits(pnal_buffer, 5));
			fprintf(stdout, "\tau_cpb_removal_delay_length_minus1=%d\n", read_bits(pnal_buffer, 5));
			fprintf(stdout, "\tdpb_output_delay_length_minus1=%d\n", read_bits(pnal_buffer, 5));
		}
	}

	for (int i = 0; i <= maxNumSubLayersMinus1; i++)
	{
		uint8 fixed_pic_rate_general_flag_i = read_bit(pnal_buffer);
		fprintf(stdout, "\tfixed_pic_rate_general_flag[%d]=%d\n", i, fixed_pic_rate_general_flag_i);
		uint8 fixed_pic_rate_within_cvs_flag_i = 0;
		if (!fixed_pic_rate_general_flag_i)
		{
			fixed_pic_rate_within_cvs_flag_i = read_bit(pnal_buffer);
		}
		fprintf(stdout, "\tfixed_pic_rate_within_cvs_flag[%d]=%d\n", i, read_bit(pnal_buffer));
		uint8 low_delay_hrd_flag_i = 0;
		if (fixed_pic_rate_within_cvs_flag_i)
		{
			fprintf(stdout, "\telemental_duration_in_tc_minus1[%d]=%d\n", i, read_uev(pnal_buffer));
		}
		else
		{
			low_delay_hrd_flag_i = read_bit(pnal_buffer);
		}
		fprintf(stdout, "\tlow_delay_hrd_flag[%d]=%d\n", i, low_delay_hrd_flag_i);
		uint8 cpb_cnt_minus1_i = 0;
		if (!low_delay_hrd_flag_i)
		{
			cpb_cnt_minus1_i = read_uev(pnal_buffer);
		}
		fprintf(stdout, "\tcpb_cnt_minus1[%d]=%d\n", i, cpb_cnt_minus1_i);
		if (nal_hrd_parameters_present_flag)
		{
			print_sub_layer_hrd_parameters(pnal_buffer, cpb_cnt_minus1_i, sub_pic_hrd_params_present_flag);
		}
		if (vcl_hrd_parameters_present_flag)
		{
			print_sub_layer_hrd_parameters(pnal_buffer, cpb_cnt_minus1_i, sub_pic_hrd_params_present_flag);
		}
	}
}

void hrd_parameters(nal_buffer_t* pnal_buffer, uint8 commonInfPresentFlag, uint32 maxNumSubLayersMinus1, hrd_parameters_data_t* hrd_parameters_data)
{
	if (commonInfPresentFlag)
	{
		hrd_parameters_data->nal_hrd_parameters_present_flag = (uint8)read_bit(pnal_buffer);
		hrd_parameters_data->vcl_hrd_parameters_present_flag = (uint8)read_bit(pnal_buffer);
		if (hrd_parameters_data->nal_hrd_parameters_present_flag || hrd_parameters_data->vcl_hrd_parameters_present_flag)
		{
			hrd_parameters_data->sub_pic_cpb_params_present_flag = (uint8)read_bit(pnal_buffer);
			if (hrd_parameters_data->sub_pic_cpb_params_present_flag)
			{
				hrd_parameters_data->tick_divisor_minus2 = (uint8)read_bits(pnal_buffer, 8);
				hrd_parameters_data->du_cpb_removal_delay_increment_length_minus1 = (uint8)read_bits(pnal_buffer, 5);
				hrd_parameters_data->sub_pic_cpb_params_in_pic_timing_sei_flag = (uint8)read_bit(pnal_buffer);
				hrd_parameters_data->dpb_output_delay_du_length_minus1 = (uint8)read_bits(pnal_buffer, 5);
			}
			hrd_parameters_data->bit_rate_scale = (uint8)read_bits(pnal_buffer, 4);
			hrd_parameters_data->cpb_size_scale = (uint8)read_bits(pnal_buffer, 4);
			if (hrd_parameters_data->sub_pic_cpb_params_present_flag)
			{
				hrd_parameters_data->cpb_size_du_scale = (uint8)read_bits(pnal_buffer, 4);
			}
			hrd_parameters_data->initial_cpb_removal_delay_length_minus1 = (uint8)read_bits(pnal_buffer, 5);
			hrd_parameters_data->au_cpb_removal_delay_length_minus1 = (uint8)read_bits(pnal_buffer, 5);
			hrd_parameters_data->dpb_output_delay_length_minus1 = (uint8)read_bits(pnal_buffer, 5);
		}
	}

	for (int i = 0; i <= maxNumSubLayersMinus1; i++)
	{
		hrd_parameters_data->fixed_pic_rate_general_flag[i] = read_bit(pnal_buffer);
		if (!hrd_parameters_data->fixed_pic_rate_general_flag[i])
		{
			hrd_parameters_data->fixed_pic_rate_within_cvs_flag[i] = read_bit(pnal_buffer);
		}
		hrd_parameters_data->fixed_pic_rate_within_cvs_flag[i] = read_bit(pnal_buffer);
		if (hrd_parameters_data->fixed_pic_rate_within_cvs_flag[i])
		{
			hrd_parameters_data->elemental_duration_in_tc_minus1[i] = read_uev(pnal_buffer);
		}
		else
		{
			hrd_parameters_data->low_delay_hrd_flag[i] = read_bit(pnal_buffer);
		}
		if (!hrd_parameters_data->low_delay_hrd_flag[i])
		{
			hrd_parameters_data->cpb_cnt_minus1[i] = read_uev(pnal_buffer);
		}
		if (hrd_parameters_data->nal_hrd_parameters_present_flag)
		{
			sub_layer_hrd_parameters(pnal_buffer, hrd_parameters_data->cpb_cnt_minus1[i], hrd_parameters_data->sub_pic_cpb_params_present_flag, &hrd_parameters_data->nal_sublayer_hrd_parameters_data[i]);
		}
		if (hrd_parameters_data->vcl_hrd_parameters_present_flag)
		{
			sub_layer_hrd_parameters(pnal_buffer, hrd_parameters_data->cpb_cnt_minus1[i], hrd_parameters_data->sub_pic_cpb_params_present_flag, &hrd_parameters_data->vcl_sublayer_hrd_parameters_data[i]);
		}
	}
}

void print_vui_parameters(nal_buffer_t* pnal_buffer, uint8 sps_max_sub_layers_minus1)
{
	fprintf(stdout, "\t--- vui_parameters(%d) ---\n", sps_max_sub_layers_minus1);
	uint8 aspect_ratio_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\taspect_ratio_info_present_flag=%d\n", aspect_ratio_info_present_flag);
	if (aspect_ratio_info_present_flag)
	{
		uint8 aspect_ratio_idc = read_bits(pnal_buffer, 8);
		fprintf(stdout, "\taspect_ratio_idc=%d\n", aspect_ratio_idc);
		if (aspect_ratio_idc == EXTENDED_SAR)
		{
			fprintf(stdout, "\tsar_width=%d\n", read_bits(pnal_buffer, 16));
			fprintf(stdout, "\tsar_height=%d\n", read_bits(pnal_buffer, 16));
		}
	}
	uint8 overscan_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\toverscan_info_present_flag=%d\n", overscan_info_present_flag);
	if (overscan_info_present_flag)
	{
		fprintf(stdout, "\toverscan_appropriate_flag=%d\n", read_bit(pnal_buffer));
	}
	uint8 video_signal_type_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tvideo_signal_type_present_flag=%d\n", video_signal_type_present_flag);
	if (video_signal_type_present_flag)
	{
		fprintf(stdout, "\tvideo_format=%d\n", read_bits(pnal_buffer, 3));
		fprintf(stdout, "\tvideo_full_range_flag=%d\n", read_bit(pnal_buffer));
		uint8 colour_description_present_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tcolour_description_present_flag=%d\n", colour_description_present_flag);
		if (colour_description_present_flag)
		{
			fprintf(stdout, "\tcolour_primaries=%d\n", read_bits(pnal_buffer, 8));
			fprintf(stdout, "\ttransfer_characteristics=%d\n", read_bits(pnal_buffer, 8));
			fprintf(stdout, "\tmatrix_coeffs=%d\n", read_bits(pnal_buffer, 8));
		}
	}
	uint8 chroma_loc_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tchroma_loc_info_present_flag=%d\n", chroma_loc_info_present_flag);
	if (chroma_loc_info_present_flag)
	{
		fprintf(stdout, "\tchroma_sample_loc_type_top_field=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tchroma_sample_loc_type_bottom_field=%d\n", read_uev(pnal_buffer));
	}
	fprintf(stdout, "\tneutral_chroma_indication_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tfield_seq_flag=%d\n", read_bit(pnal_buffer));
	fprintf(stdout, "\tframe_field_info_present_flag=%d\n", read_bit(pnal_buffer));
	uint8 default_display_window_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tdefault_display_window_flag=%d\n", default_display_window_flag);
	if (default_display_window_flag)
	{
		fprintf(stdout, "\tdef_disp_win_left_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tdef_disp_win_right_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tdef_disp_win_top_offset=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tdef_disp_win_bottom_offset=%d\n", read_uev(pnal_buffer));
	}
	uint8 vui_timing_info_present_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tvui_timing_info_present_flag=%d\n", vui_timing_info_present_flag);
	if (vui_timing_info_present_flag)
	{
		fprintf(stdout, "\tvui_num_units_in_tick=%d\n", read_bits(pnal_buffer, 32));
		fprintf(stdout, "\tvui_time_scale=%d\n", read_bits(pnal_buffer, 32));
		uint8 vui_poc_proportional_to_timing_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tvui_poc_proportional_to_timing_flag=%d\n", vui_poc_proportional_to_timing_flag);
		if (vui_poc_proportional_to_timing_flag)
		{
			fprintf(stdout, "\tvui_num_ticks_poc_diff_one_minus1=%d\n", read_uev(pnal_buffer));
		}
		uint8 vui_hrd_parameters_present_flag = read_bit(pnal_buffer);
		fprintf(stdout, "\tvui_hrd_parameters_present_flag=%d\n", vui_hrd_parameters_present_flag);
		if (vui_hrd_parameters_present_flag)
		{
			print_hrd_parameters(pnal_buffer, 1, sps_max_sub_layers_minus1);
		}
	}
	uint8 bitstream_restriction_flag = read_bit(pnal_buffer);
	fprintf(stdout, "\tbitstream_restriction_flag=%d\n", bitstream_restriction_flag);
	if (bitstream_restriction_flag)
	{
		fprintf(stdout, "\ttiles_fixed_structure_flag=%d\n", read_bit(pnal_buffer));
		fprintf(stdout, "\tmotion_vectors_over_pic_boundaries_flag=%d\n", read_bit(pnal_buffer));
		fprintf(stdout, "\trestricted_ref_pic_lists_flag=%d\n", read_bit(pnal_buffer));
		fprintf(stdout, "\tmin_spatial_segmentation_idc=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tmax_bytes_per_pic_denom=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tmax_bits_per_min_cu_denom=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tlog2_max_mv_length_horizontal=%d\n", read_uev(pnal_buffer));
		fprintf(stdout, "\tlog2_max_mv_length_vertical=%d\n", read_uev(pnal_buffer));
	}
}

void vui_parameters(nal_buffer_t* pnal_buffer, uint8 sps_max_sub_layers_minus1, vui_parameters_data_t* vui_parameters_data)
{
	vui_parameters_data->aspect_ratio_info_present_flag = (uint8)read_bit(pnal_buffer);
	if (vui_parameters_data->aspect_ratio_info_present_flag)
	{
		vui_parameters_data->aspect_ratio_idc = (uint8)read_bits(pnal_buffer, 8);
		if (vui_parameters_data->aspect_ratio_idc == EXTENDED_SAR)
		{
			vui_parameters_data->sar_width = read_bits(pnal_buffer, 16);
			vui_parameters_data->sar_height = read_bits(pnal_buffer, 16);
		}
	}

	vui_parameters_data->overscan_info_present_flag = (uint8)read_bit(pnal_buffer);
	if (vui_parameters_data->overscan_info_present_flag)
	{
		vui_parameters_data->overscan_appropriate_flag = (uint8)read_bit(pnal_buffer);
	}
	vui_parameters_data->video_signal_type_present_flag = (uint8)read_bit(pnal_buffer);
	if (vui_parameters_data->video_signal_type_present_flag)
	{
		vui_parameters_data->video_format = (uint8)read_bits(pnal_buffer, 3);
		vui_parameters_data->video_full_range_flag = (uint8)read_bit(pnal_buffer);
		vui_parameters_data->colour_description_present_flag = (uint8) read_bit(pnal_buffer);
		if (vui_parameters_data->colour_description_present_flag)
		{
			vui_parameters_data->colour_primaries = (uint8)read_bits(pnal_buffer, 8);
			vui_parameters_data->transfer_characteristics = (uint8)read_bits(pnal_buffer, 8);
			vui_parameters_data->matrix_coefficients = (uint8)read_bits(pnal_buffer, 8);
		}
	}
	vui_parameters_data->chroma_loc_info_present_flag = (uint8)read_bit(pnal_buffer);
	if (vui_parameters_data->chroma_loc_info_present_flag)
	{
		vui_parameters_data->chroma_sample_loc_type_top_field = (uint8)read_uev(pnal_buffer);
		vui_parameters_data->chroma_sample_loc_type_bottom_field = (uint8)read_uev(pnal_buffer);
	}
	vui_parameters_data->neutral_chroma_indication_flag = (uint8)read_bit(pnal_buffer);
	vui_parameters_data->field_seq_flag = (uint8)read_bit(pnal_buffer);
	vui_parameters_data->frame_field_info_present_flag = (uint8)read_bit(pnal_buffer);
	vui_parameters_data->default_display_window_flag = (uint8)read_bit(pnal_buffer);
	if (vui_parameters_data->default_display_window_flag)
	{
		vui_parameters_data->def_disp_win_left_offset = read_uev(pnal_buffer);
		vui_parameters_data->def_disp_win_right_offset = read_uev(pnal_buffer);
		vui_parameters_data->def_disp_win_top_offset = read_uev(pnal_buffer);
		vui_parameters_data->def_disp_win_bottom_offset = read_uev(pnal_buffer);
	}
	vui_parameters_data->vui_timing_info_present_flag = (uint8)read_bit(pnal_buffer);
	if (vui_parameters_data->vui_timing_info_present_flag)
	{
		vui_parameters_data->vui_num_units_in_tick = read_bits(pnal_buffer, 32);
		vui_parameters_data->vui_time_scale = read_bits(pnal_buffer, 32);
		vui_parameters_data->vui_poc_proportional_to_timing_flag = (uint8)read_bit(pnal_buffer);
		if (vui_parameters_data->vui_poc_proportional_to_timing_flag)
		{
			vui_parameters_data->vui_num_ticks_poc_diff_one_minus1 = read_uev(pnal_buffer);
		}
		vui_parameters_data->vui_hrd_parameters_present_flag = (uint8)read_bit(pnal_buffer);
		if (vui_parameters_data->vui_hrd_parameters_present_flag)
		{
			hrd_parameters(pnal_buffer, 1, sps_max_sub_layers_minus1, &vui_parameters_data->hrd_parameters);
		}
	}
	vui_parameters_data->bitstream_restriction_flag = (uint8)read_bit(pnal_buffer);
	if (vui_parameters_data->bitstream_restriction_flag)
	{
		vui_parameters_data->tiles_fixed_structure_flag = (uint8)read_bit(pnal_buffer);
		vui_parameters_data->motion_vectors_over_pic_boundaries_flag = (uint8)read_bit(pnal_buffer);
		vui_parameters_data->restricted_ref_pic_lists_flag = (uint8)read_bit(pnal_buffer);
		vui_parameters_data->min_spatial_segmentation_idc = (uint16)read_uev(pnal_buffer);
		vui_parameters_data->max_bytes_per_pic_denom = (uint8)read_uev(pnal_buffer);
		vui_parameters_data->max_bits_per_mincu_denom = (uint8)read_uev(pnal_buffer);
		vui_parameters_data->log2_max_mv_length_horizontal = (uint8)read_uev(pnal_buffer);
		vui_parameters_data->log2_max_mv_length_vertical = (uint8)read_uev(pnal_buffer);
	}
}

void print_scaling_list_data(nal_buffer_t* pnal_buffer)
{
	fprintf(stdout, "\tscaling_list_data() ---\n");
	for (int size_id = 0; size_id < 4; size_id++)
	{
		for (int matrix_id = 0; matrix_id < (size_id == 3) ? 2 : 6; matrix_id++)
		{
			uint8 scaling_list_pred_mode_flag = read_bit(pnal_buffer); // [sizeId][matrixId]
			fprintf(stdout, "\t\tscaling_list_pred_mode_flag[%d][%d] = %d\n", size_id, matrix_id, scaling_list_pred_mode_flag);

			uint32 scaling_list_pred_matrix_id_delta = 0;
			if (!scaling_list_pred_mode_flag)
			{ // [sizeId][matrixId] 
				scaling_list_pred_matrix_id_delta = read_uev(pnal_buffer); // [sizeId][matrixId] 
				fprintf(stdout, "\t\tscaling_list_pred_matrix_id_delta[%d][%d] = %d\n", size_id, matrix_id, scaling_list_pred_matrix_id_delta);
			}
			else
			{
				uint8 next_coef = 8;
				uint32 coef_num = MIN(64, (1 << (4 + (size_id << 1))));

				if (size_id > 1)
				{
					uint32 scaling_list_dc_coef_minus8 = read_uev(pnal_buffer); // [size_id − 2][matrix_id]
					fprintf(stdout, "\t\tscaling_list_dc_coef_minus8[%d][%d] = %d\n", size_id, matrix_id, scaling_list_dc_coef_minus8);
					next_coef = scaling_list_dc_coef_minus8; //  [sizeId − 2][matrixId] + 8
				}
				for (int i = 0; i < coef_num; i++)
				{
					sint32 scaling_list_delta_coef = read_sev(pnal_buffer); // se(v)
					fprintf(stdout, "\t\tscaling_list_delta_coef = %d\n", scaling_list_delta_coef);

					next_coef = (next_coef + scaling_list_delta_coef + 256) % 256;
					//scaling_list[size_id][matrix_id][i] = next_coef;
				}
			}
		}
	}
}

void scaling_list_data(nal_buffer_t* pnal_buffer, scaling_list_data_data_t* scaling_list_data)
{
	for (int size_id = 0; size_id < 4; size_id++)
	{
		for (int matrix_id = 0; matrix_id < (size_id == 3) ? 2 : 6; matrix_id++)
		{
			scaling_list_data->scaling_list_pred_mode_flag[size_id][matrix_id] = read_bit(pnal_buffer); // [sizeId][matrixId]
			if (!scaling_list_data->scaling_list_pred_mode_flag[size_id][matrix_id])
			{ // [sizeId][matrixId] 
				scaling_list_data->scaling_list_pred_matrix_id_delta[size_id][matrix_id] = read_uev(pnal_buffer); // [sizeId][matrixId] 
			}
			else
			{
				uint8 next_coef = 8;
				uint32 coef_num = MIN(64, (1 << (4 + (size_id << 1))));

				if (size_id > 1)
				{
					scaling_list_data->scaling_list_dc_coef_minus8[size_id - 2][matrix_id] = read_uev(pnal_buffer); // [size_id − 2][matrix_id]
					next_coef = scaling_list_data->scaling_list_dc_coef_minus8[size_id - 2][matrix_id]; //  [sizeId − 2][matrixId] + 8
				}
				for (int i = 0; i < coef_num; i++)
				{
					scaling_list_data->scaling_list_delta_coef[i] = read_sev(pnal_buffer); // se(v)
					next_coef = (next_coef + scaling_list_data->scaling_list_delta_coef[i] + 256) % 256;
					//					scaling_list_data->scaling_list[size_id][matrix_id][i] = next_coef;
				}
			}
		}
	}
}

void write_profile_tier_level(nal_buffer_t* pnal_buffer, int maxNumSubLayersMinus1, profile_tier_level_data_t* profile_tier_level_data)
{
	write_bits(pnal_buffer, &profile_tier_level_data->general_profile_space, 2);
	write_bit(pnal_buffer, profile_tier_level_data->general_tier_flag);
	write_bits(pnal_buffer, &profile_tier_level_data->general_profile_idc, 5);
	for (int j = 0; j < 32; j++)
	{
		write_bit(pnal_buffer, profile_tier_level_data->general_profile_compatibility_flag[j]);
	}
	write_bit(pnal_buffer, profile_tier_level_data->general_progressive_source_flag);
	write_bit(pnal_buffer, profile_tier_level_data->general_interlaced_source_flag);
	write_bit(pnal_buffer, profile_tier_level_data->general_non_packed_constraint_flag);
	write_bit(pnal_buffer, profile_tier_level_data->general_frame_only_constraint_flag);
	//profile_tier_level_data->general_reserved_zero_44bits = (uint64)read_bits64(pnal_buffer, 44);
	if (profile_tier_level_data->general_profile_idc == 4 || profile_tier_level_data->general_profile_compatibility_flag[4] ||
		profile_tier_level_data->general_profile_idc == 5 || profile_tier_level_data->general_profile_compatibility_flag[5] ||
		profile_tier_level_data->general_profile_idc == 6 || profile_tier_level_data->general_profile_compatibility_flag[6] ||
		profile_tier_level_data->general_profile_idc == 7 || profile_tier_level_data->general_profile_compatibility_flag[7])
	/* The number of bits in this syntax structure is not affected by this condition */
	{
		write_bit(pnal_buffer, profile_tier_level_data->general_max_12bit_constraint_flag);//	u(1)
		write_bit(pnal_buffer, profile_tier_level_data->general_max_10bit_constraint_flag);//	u(1)
		write_bit(pnal_buffer, profile_tier_level_data->general_max_8bit_constraint_flag);//	u(1)
		write_bit(pnal_buffer, profile_tier_level_data->general_max_422chroma_constraint_flag);//	u(1)
		write_bit(pnal_buffer, profile_tier_level_data->general_max_420chroma_constraint_flag);//	u(1)
		write_bit(pnal_buffer, profile_tier_level_data->general_max_monochrome_constraint_flag);//	u(1)
		write_bit(pnal_buffer, profile_tier_level_data->general_intra_constraint_flag);//	u(1)
		write_bit(pnal_buffer, profile_tier_level_data->general_one_picture_only_constraint_flag);//	u(1)
		write_bit(pnal_buffer, profile_tier_level_data->general_lower_bit_rate_constraint_flag);//	u(1)
		write_bits(pnal_buffer, &profile_tier_level_data->general_reserved_zero_34bits, 34);//	u(34)
	}
	else
	{
		write_bits(pnal_buffer, &profile_tier_level_data->general_reserved_zero_43bits, 43);//	u(43)
		if ((profile_tier_level_data->general_profile_idc >= 1 && profile_tier_level_data->general_profile_idc <= 5) ||
			profile_tier_level_data->general_profile_compatibility_flag[1] || profile_tier_level_data->general_profile_compatibility_flag[2] ||
			profile_tier_level_data->general_profile_compatibility_flag[3] || profile_tier_level_data->general_profile_compatibility_flag[4] ||
			profile_tier_level_data->general_profile_compatibility_flag[5])
		/* The number of bits in this syntax structure is not affected by this condition */
		{
			write_bit(pnal_buffer, profile_tier_level_data->general_inbld_flag); //u(1)
		}
		else
		{
			write_bit(pnal_buffer, profile_tier_level_data->general_reserved_zero_bit);//	u(1)
		}
	}
	write_bits(pnal_buffer, &profile_tier_level_data->general_level_idc, 8);
	for (int i = 0; i < maxNumSubLayersMinus1; i++)
	{
		write_bit(pnal_buffer, profile_tier_level_data->sub_layer_profile_present_flag[i]);
		write_bit(pnal_buffer, profile_tier_level_data->sub_layer_level_present_flag[i]);
	}

	if (maxNumSubLayersMinus1 > 0)
	{
		for (int i = maxNumSubLayersMinus1; i < 8; i++)
		{
			write_bits(pnal_buffer, &profile_tier_level_data->reserved_zero_2bits[i], 2);
		}
	}

	for (int i = 0; i < maxNumSubLayersMinus1; i++)
	{
		if (profile_tier_level_data->sub_layer_profile_present_flag[i])
		{
			write_bits(pnal_buffer, &profile_tier_level_data->sub_layer_profile_space[i], 2);
			write_bit(pnal_buffer, profile_tier_level_data->sub_layer_tier_flag[i]);
			write_bits(pnal_buffer, &profile_tier_level_data->sub_layer_profile_idc[i], 5);
			for (int j = 0; j < 32; j++)
			{
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_profile_compatibility_flag[i][j]);
			}
			write_bit(pnal_buffer, profile_tier_level_data->sub_layer_progressive_source_flag[i]);
			write_bit(pnal_buffer, profile_tier_level_data->sub_layer_interlaced_source_flag[i]);
			write_bit(pnal_buffer, profile_tier_level_data->sub_layer_non_packed_constraint_flag[i]);
			write_bit(pnal_buffer, profile_tier_level_data->sub_layer_frame_only_constraint_flag[i]);
			//profile_tier_level_data->sub_layer_reserved_zero_44bits[i] = (uint64)read_bits64(pnal_buffer, 44);

			if (profile_tier_level_data->sub_layer_profile_idc[i] == 4 ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[i][4] ||
				profile_tier_level_data->sub_layer_profile_idc[i] == 5 ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[i][5] ||
				profile_tier_level_data->sub_layer_profile_idc[i] == 6 ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[i][6] ||
				profile_tier_level_data->sub_layer_profile_idc[i] == 7 ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[i][7])
			{
				/* The number of bits in this syntax structure is not affected by this condition */
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_max_12bit_constraint_flag[i]);//	u(1)
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_max_10bit_constraint_flag[i]);//	u(1)
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_max_8bit_constraint_flag[i]);//	u(1)
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_max_422chroma_constraint_flag[i]);//	u(1)
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_max_420chroma_constraint_flag[i]);//	u(1)
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_max_monochrome_constraint_flag[i]);//	u(1)
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_intra_constraint_flag[i]);//	u(1)
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_one_picture_only_constraint_flag[i]);//	u(1)
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_lower_bit_rate_constraint_flag[i]);//	u(1)
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_reserved_zero_34bits[i]);//	u(34)
			}
			else
			{
				write_bits(pnal_buffer, &profile_tier_level_data->sub_layer_reserved_zero_43bits[i], 43);//	u(43)
			}
			if ((profile_tier_level_data->sub_layer_profile_idc[i] >= 1 && profile_tier_level_data->sub_layer_profile_idc[i] <= 5) ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[1] ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[2] ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[3] ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[4] ||
				profile_tier_level_data->sub_layer_profile_compatibility_flag[5])
			/* The number of bits in this syntax structure is not affected by this condition */
			{
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_inbld_flag[i]);//		u(1)
			}
			else
			{
				write_bit(pnal_buffer, profile_tier_level_data->sub_layer_reserved_zero_bit[i]);//		u(1)
			}
		}
		if (profile_tier_level_data->sub_layer_level_present_flag[i])
		{
			write_bits(pnal_buffer, &profile_tier_level_data->sub_layer_level_idc[i], 8);
		}
	}
}

void write_short_term_ref_pic_set(nal_buffer_t* pnal_buffer, int stRpsIdx, int num_short_term_ref_pic_sets, short_term_ref_pic_set_data_t* short_term_ref_pic_set_data)
{
	if (stRpsIdx != 0)
	{
		write_bit(pnal_buffer, short_term_ref_pic_set_data->inter_ref_pic_set_prediction_flag);
	}
	if (short_term_ref_pic_set_data->inter_ref_pic_set_prediction_flag)
	{
		if (stRpsIdx == num_short_term_ref_pic_sets)
		{
			write_uev(pnal_buffer, short_term_ref_pic_set_data->delta_idx_minus1);
		}
		write_bit(pnal_buffer, short_term_ref_pic_set_data->delta_rps_sign);
		write_uev(pnal_buffer, short_term_ref_pic_set_data->abs_delta_rps_minus1);
		int RefRpsIdx = stRpsIdx - (short_term_ref_pic_set_data->delta_idx_minus1 + 1);
		int NumDeltaPocs = short_term_ref_pic_set_data->num_negative_pics[RefRpsIdx] + short_term_ref_pic_set_data->num_positive_pics[RefRpsIdx];
		for (int j = 0; j <= NumDeltaPocs; j++)
		{
			write_bit(pnal_buffer, short_term_ref_pic_set_data->used_by_curr_pic_flag[j]);
			if (!short_term_ref_pic_set_data->used_by_curr_pic_flag[j])
				write_bit(pnal_buffer, short_term_ref_pic_set_data->use_delta_flag[j]);
		}
	}
	else
	{
		write_uev(pnal_buffer, short_term_ref_pic_set_data->num_negative_pics[stRpsIdx]);
		write_uev(pnal_buffer, short_term_ref_pic_set_data->num_positive_pics[stRpsIdx]);
		for (int i = 0; i < short_term_ref_pic_set_data->num_negative_pics[stRpsIdx]; i++)
		{
			write_uev(pnal_buffer, short_term_ref_pic_set_data->delta_poc_s0_minus1[i]);
			write_bit(pnal_buffer, short_term_ref_pic_set_data->used_by_curr_pic_s0_flag[i]);
		}
		for (int i = 0; i < short_term_ref_pic_set_data->num_positive_pics[stRpsIdx]; i++)
		{
			write_uev(pnal_buffer, short_term_ref_pic_set_data->delta_poc_s1_minus1[i]);
			write_bit(pnal_buffer, short_term_ref_pic_set_data->used_by_curr_pic_s1_flag[i]);
		}
	}
}

void write_sub_layer_hrd_parameters(nal_buffer_t* pnal_buffer, uint32 CpbCnt, uint8 sub_pic_hrd_params_present_flag, sub_layer_hrd_parameters_data_t* sub_layer_hrd_parameters_data)
{
	for (int i = 0; i <= CpbCnt; i++)
	{
		write_uev(pnal_buffer, sub_layer_hrd_parameters_data->bit_rate_value_minus1[i]);
		write_uev(pnal_buffer, sub_layer_hrd_parameters_data->cpb_size_value_minus1[i]);
		if (sub_pic_hrd_params_present_flag)
		{
			write_uev(pnal_buffer, sub_layer_hrd_parameters_data->cpb_size_du_value_minus1[i]);
			write_uev(pnal_buffer, sub_layer_hrd_parameters_data->bit_rate_du_value_minus1[i]);
		}
		write_bit(pnal_buffer, sub_layer_hrd_parameters_data->cbr_flag[i]);
	}
}

void write_hrd_parameters(nal_buffer_t* pnal_buffer, uint8 commonInfPresentFlag, uint32 maxNumSubLayersMinus1, hrd_parameters_data_t* hrd_parameters_data)
{
	if (commonInfPresentFlag)
	{
		write_bit(pnal_buffer, hrd_parameters_data->nal_hrd_parameters_present_flag);
		write_bit(pnal_buffer, hrd_parameters_data->vcl_hrd_parameters_present_flag);
		if (hrd_parameters_data->nal_hrd_parameters_present_flag || hrd_parameters_data->vcl_hrd_parameters_present_flag)
		{
			write_bit(pnal_buffer, hrd_parameters_data->sub_pic_cpb_params_present_flag);
			if (hrd_parameters_data->sub_pic_cpb_params_present_flag)
			{
				write_bits(pnal_buffer, &hrd_parameters_data->tick_divisor_minus2, 8);
				write_bits(pnal_buffer, &hrd_parameters_data->du_cpb_removal_delay_increment_length_minus1, 5);
				write_bit(pnal_buffer, hrd_parameters_data->sub_pic_cpb_params_in_pic_timing_sei_flag);
				write_bits(pnal_buffer, &hrd_parameters_data->dpb_output_delay_du_length_minus1, 5);
			}
			write_bits(pnal_buffer, &hrd_parameters_data->bit_rate_scale, 4);
			write_bits(pnal_buffer, &hrd_parameters_data->cpb_size_scale, 4);
			if (hrd_parameters_data->sub_pic_cpb_params_present_flag)
			{
				write_bits(pnal_buffer, &hrd_parameters_data->cpb_size_du_scale, 4);
			}
			write_bits(pnal_buffer, &hrd_parameters_data->initial_cpb_removal_delay_length_minus1, 5);
			write_bits(pnal_buffer, &hrd_parameters_data->au_cpb_removal_delay_length_minus1, 5);
			write_bits(pnal_buffer, &hrd_parameters_data->dpb_output_delay_length_minus1, 5);
		}
	}

	for (int i = 0; i <= maxNumSubLayersMinus1; i++)
	{
		write_bit(pnal_buffer, hrd_parameters_data->fixed_pic_rate_general_flag[i]);
		if (!hrd_parameters_data->fixed_pic_rate_general_flag[i])
		{
			write_bit(pnal_buffer, hrd_parameters_data->fixed_pic_rate_within_cvs_flag[i]);
		}
		write_bit(pnal_buffer, hrd_parameters_data->fixed_pic_rate_within_cvs_flag[i]);
		if (hrd_parameters_data->fixed_pic_rate_within_cvs_flag[i])
		{
			write_uev(pnal_buffer, hrd_parameters_data->elemental_duration_in_tc_minus1[i]);
		}
		else
		{
			write_bit(pnal_buffer, hrd_parameters_data->low_delay_hrd_flag[i]);
		}
		if (!hrd_parameters_data->low_delay_hrd_flag[i])
		{
			write_uev(pnal_buffer, hrd_parameters_data->cpb_cnt_minus1[i]);
		}
		if (hrd_parameters_data->nal_hrd_parameters_present_flag)
		{
			write_sub_layer_hrd_parameters(pnal_buffer, hrd_parameters_data->cpb_cnt_minus1[i], hrd_parameters_data->sub_pic_cpb_params_present_flag, &hrd_parameters_data->nal_sublayer_hrd_parameters_data[i]);
		}
		if (hrd_parameters_data->vcl_hrd_parameters_present_flag)
		{
			write_sub_layer_hrd_parameters(pnal_buffer, hrd_parameters_data->cpb_cnt_minus1[i], hrd_parameters_data->sub_pic_cpb_params_present_flag, &hrd_parameters_data->vcl_sublayer_hrd_parameters_data[i]);
		}
	}
}

void write_vui_parameters(nal_buffer_t* pnal_buffer, uint8 sps_max_sub_layers_minus1, vui_parameters_data_t* vui_parameters_data)
{
	write_bit(pnal_buffer, vui_parameters_data->aspect_ratio_info_present_flag);
	if (vui_parameters_data->aspect_ratio_info_present_flag)
	{
		write_bits(pnal_buffer, &vui_parameters_data->aspect_ratio_idc, 8);
		if (vui_parameters_data->aspect_ratio_idc == EXTENDED_SAR)
		{
			write_bits(pnal_buffer, &vui_parameters_data->sar_width, 16);
			write_bits(pnal_buffer, &vui_parameters_data->sar_height, 16);
		}
	}

	write_bit(pnal_buffer, vui_parameters_data->overscan_info_present_flag);
	if (vui_parameters_data->overscan_info_present_flag)
	{
		write_bit(pnal_buffer, vui_parameters_data->overscan_appropriate_flag);
	}
	write_bit(pnal_buffer, vui_parameters_data->video_signal_type_present_flag);
	if (vui_parameters_data->video_signal_type_present_flag)
	{
		write_bits(pnal_buffer, &vui_parameters_data->video_format, 3);
		write_bit(pnal_buffer, vui_parameters_data->video_full_range_flag);
		write_bit(pnal_buffer, vui_parameters_data->colour_description_present_flag);
		if (vui_parameters_data->colour_description_present_flag)
		{
			write_bits(pnal_buffer, &vui_parameters_data->colour_primaries, 8);
			write_bits(pnal_buffer, &vui_parameters_data->transfer_characteristics, 8);
			write_bits(pnal_buffer, &vui_parameters_data->matrix_coefficients, 8);
		}
	}
	write_bit(pnal_buffer, vui_parameters_data->chroma_loc_info_present_flag);
	if (vui_parameters_data->chroma_loc_info_present_flag)
	{
		write_uev(pnal_buffer, vui_parameters_data->chroma_sample_loc_type_top_field);
		write_uev(pnal_buffer, vui_parameters_data->chroma_sample_loc_type_bottom_field);
	}
	write_bit(pnal_buffer, vui_parameters_data->neutral_chroma_indication_flag);
	write_bit(pnal_buffer, vui_parameters_data->field_seq_flag);
	write_bit(pnal_buffer, vui_parameters_data->frame_field_info_present_flag);
	write_bit(pnal_buffer, vui_parameters_data->default_display_window_flag);
	if (vui_parameters_data->default_display_window_flag)
	{
		write_uev(pnal_buffer, vui_parameters_data->def_disp_win_left_offset);
		write_uev(pnal_buffer, vui_parameters_data->def_disp_win_right_offset);
		write_uev(pnal_buffer, vui_parameters_data->def_disp_win_top_offset);
		write_uev(pnal_buffer, vui_parameters_data->def_disp_win_bottom_offset);
	}
	write_bit(pnal_buffer, vui_parameters_data->vui_timing_info_present_flag);
	if (vui_parameters_data->vui_timing_info_present_flag)
	{
		write_bits(pnal_buffer, &vui_parameters_data->vui_num_units_in_tick, 32);
		write_bits(pnal_buffer, &vui_parameters_data->vui_time_scale, 32);
		write_bit(pnal_buffer, vui_parameters_data->vui_poc_proportional_to_timing_flag);
		if (vui_parameters_data->vui_poc_proportional_to_timing_flag)
		{
			write_uev(pnal_buffer, vui_parameters_data->vui_num_ticks_poc_diff_one_minus1);
		}
		write_bit(pnal_buffer, vui_parameters_data->vui_hrd_parameters_present_flag);
		if (vui_parameters_data->vui_hrd_parameters_present_flag)
		{
			write_hrd_parameters(pnal_buffer, 1, sps_max_sub_layers_minus1, &vui_parameters_data->hrd_parameters);
		}
	}
	write_bit(pnal_buffer, vui_parameters_data->bitstream_restriction_flag);
	if (vui_parameters_data->bitstream_restriction_flag)
	{
		write_bit(pnal_buffer, vui_parameters_data->tiles_fixed_structure_flag);
		write_bit(pnal_buffer, vui_parameters_data->motion_vectors_over_pic_boundaries_flag);
		write_bit(pnal_buffer, vui_parameters_data->restricted_ref_pic_lists_flag);
		write_uev(pnal_buffer, vui_parameters_data->min_spatial_segmentation_idc);
		write_uev(pnal_buffer, vui_parameters_data->max_bytes_per_pic_denom);
		write_uev(pnal_buffer, vui_parameters_data->max_bits_per_mincu_denom);
		write_uev(pnal_buffer, vui_parameters_data->log2_max_mv_length_horizontal);
		write_uev(pnal_buffer, vui_parameters_data->log2_max_mv_length_vertical);
	}
}

void write_scaling_list_data(nal_buffer_t* pnal_buffer, scaling_list_data_data_t* scaling_list_data)
{
	for (int size_id = 0; size_id < 4; size_id++)
	{
		for (int matrix_id = 0; matrix_id < (size_id == 3) ? 2 : 6; matrix_id++)
		{
			write_bit(pnal_buffer, scaling_list_data->scaling_list_pred_mode_flag[size_id][matrix_id]); // [sizeId][matrixId]
			if (!scaling_list_data->scaling_list_pred_mode_flag[size_id][matrix_id])
			{ // [sizeId][matrixId] 
				write_uev(pnal_buffer, scaling_list_data->scaling_list_pred_matrix_id_delta[size_id][matrix_id]); // [sizeId][matrixId] 
			}
			else
			{
				uint8 next_coef = 8;
				uint32 coef_num = MIN(64, (1 << (4 + (size_id << 1))));

				if (size_id > 1)
				{
					write_uev(pnal_buffer, scaling_list_data->scaling_list_dc_coef_minus8[size_id - 2][matrix_id]); // [size_id − 2][matrix_id]
					next_coef = scaling_list_data->scaling_list_dc_coef_minus8[size_id - 2][matrix_id]; //  [sizeId − 2][matrixId] + 8
				}
				for (int i = 0; i < coef_num; i++)
				{
					write_sev(pnal_buffer, scaling_list_data->scaling_list_delta_coef[i]); // se(v)
					next_coef = (next_coef + scaling_list_data->scaling_list_delta_coef[i] + 256) % 256;
					//					scaling_list_data->scaling_list[size_id][matrix_id][i] = next_coef;
				}
			}
		}
	}
}
