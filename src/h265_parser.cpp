#include <stdio.h>
#include <string.h>

#include "types.h"
#include "cio.h"
#include "bio.h"

#include "nal_aud.h"
#include "nal_pps.h"
#include "nal_sei.h"
#include "nal_sps.h"
#include "nal_vps.h"
#include <cstdlib>
#include "debug.h"

// read chunk for stream
#define READ_BUF_LEN 16384 

// NAL unit parser states 00 -> 00 -> [00] -> 01 -> NUT
#define STATE_EXPECTING_ZERO_0 0
#define STATE_EXPECTING_ZERO_1 1
#define STATE_EXPECTING_ONE 2
#define STATE_EXPECTING_NAL_UNIT_TYPE 3
#define STATE_EXPECTING_THREE 4

// total number of NUT (6 bit)
#define NALUT_MAX 63

typedef enum _nal_unit_type {
	TRAIL_N, TRAIL_R, TSA_N, TSA_R, STSA_N, STSA_R, RADL_N, RADL_R,													// 0
	RASL_N, RASL_R, RSV_VCL_N10, RSV_VCL_R11, RSV_VCL_N12, RSV_VCL_R13, RSV_VCL_N14, RSV_VCL_R15,					// 8
	BLA_W_LP, BLA_W_RADL, BLA_N_LP, IDR_W_RADL, IDR_N_LP, CRA_NUT, RSV_IRAP_22, RSV_IRAP_23,						// 16
	RSV_VCL_24, RSV_VCL_25, RSV_VCL_26, RSV_VCL_27, RSV_VCL_28, RSV_VCL_29, RSV_VCL_30, RSV_VCL_31,					// 24
	VPS_NUT, SPS_NUT, PPS_NUT, AUD_NUT, EOS_NUT, EOB_NUT, FD_NUT, PREFIX_SEI_NUT,									// 32
	SUFFIX_SEI_NUT, RSV_VCL_N41, RSV_VCL_N42, RSV_VCL_N43, RSV_VCL_N44, RSV_VCL_N45, RSV_VCL_N46, RSV_VCL_N47,		// 40
	UNSPEC48, UNSPEC49, UNSPEC50, UNSPEC51, UNSPEC52, UNSPEC53, UNSPEC54, UNSPEC55,									// 48
	UNSPEC56, UNSPEC57, UNSPEC58, UNSPEC59, UNSPEC60, UNSPEC61, UNSPEC62, UNSPEC63									// 56
} nal_unit_type;

// just to print as string
const char * NAL_UNIT_TYPES[NALUT_MAX+1] = {
	"TRAIL_N",		"TRAIL_R",		"TSA_N",		"TSA_R",		"STSA_N",		"STSA_R",		"RADL_N",		"RADL_R",			// 0
	"RASL_N",		"RASL_R",		"RSV_VCL_N10",	"RSV_VCL_R11",	"RSV_VCL_N12",	"RSV_VCL_R13",	"RSV_VCL_N14",	"RSV_VCL_R15",		// 8
	"BLA_W_LP",		"BLA_W_RADL",	"BLA_N_LP",		"IDR_W_RADL",	"IDR_N_LP",		"CRA_NUT",		"RSV_IRAP_22",	"RSV_IRAP_23",		// 16
	"RSV_VCL_24",	"RSV_VCL_25",	"RSV_VCL_26",	"RSV_VCL_27",	"RSV_VCL_28",	"RSV_VCL_29",	"RSV_VCL_30",	"RSV_VCL_31",		// 24
	"VPS_NUT",		"SPS_NUT",		"PPS_NUT",		"AUD_NUT",		"EOS_NUT",		"EOB_NUT",		"FD_NUT",		"PREFIX_SEI_NUT",	// 32
	"SUFFIX_SEI_NUT","RSV_VCL_N41",	"RSV_VCL_N42",	"RSV_VCL_N43",	"RSV_VCL_N44",	"RSV_VCL_N45",	"RSV_VCL_N46",	"RSV_VCL_N47",		// 40
	"UNSPEC48",		"UNSPEC49",		"UNSPEC50",		"UNSPEC51",		"UNSPEC52",		"UNSPEC53",		"UNSPEC54",		"UNSPEC55",			// 48
	"UNSPEC56",		"UNSPEC57",		"UNSPEC58",		"UNSPEC59",		"UNSPEC60",		"UNSPEC61",		"UNSPEC62",		"UNSPEC63"			// 56
};

FILE * infile;
FILE * outfile;

typedef void(*nal_unit_initer)();
typedef void(*nal_unit_parser)(nal_buffer_t *);
typedef void(*nal_unit_writer)(nal_buffer_t *);

// stubs for ignored NAL units
void nal_noinit() { }

void nal_noparse(nal_buffer_t * pnal_buffer)
{
	copy_nal_to_file(pnal_buffer, outfile);
}

bool need_nal_copy[NALUT_MAX + 1] = {
	true, true, true, true, false, false, false, true,	// 0
	true, true, true, true, false, false, false, true,	// 8
	true, true, true, true, false, false, false, true,	// 16
	true, true, true, true, false, false, false, true, // 24
	true, true, true, true, false, false, false, true,		// 32
	true, true, true, true, false, false, false, true,	// 40
	true, true, true, true, false, false, false, true,	// 48
	true, true, true, true, false, false, false, true,	// 56
};

nal_unit_initer nal_initers[NALUT_MAX + 1] = {
	nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit,						//  0 
	nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit,						//  8 
	nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit,						// 16 
	nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit,						// 24 
	nal_vps_init, nal_sps_init, nal_pps_init, nal_aud_init, nal_noinit, nal_noinit, nal_noinit, nal_sei_prefix_init,	// 32 
	nal_sei_suffix_init, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit,			// 40 
	nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit,						// 48 
	nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit, nal_noinit						// 56 
};

nal_unit_parser nal_parsers[NALUT_MAX + 1] = {
	nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse,		//  0 
	nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse,		//  8 
	nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse,		// 16 
	nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse,		// 24 

	nal_noparse, nal_sps_parse, nal_pps_parse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, // 32 
	nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse,		// 40 

	nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse,		// 48 
	nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse, nal_noparse		// 56 
};



int main(int argc, char ** argv)
{
	// TODO: parse options here ..
	if (argc < 2) {
		fprintf(stderr, "usage: %s <stream.265>\n", argv[0]);
		return 1;
	}
	FILE * infile;
	if (!strcmp(argv[1], "-"))
	{
		infile = stdin;
	}
	else
	{
		infile = fopen(argv[1], "rb");
	}

	if (!infile) {
		fprintf(stderr, "cannot open file %s\n", argv[1]);
		return 2;
	}

	outfile = fopen(argv[2], "wb");
	if (!outfile) {
		fprintf(stderr, "cannot open file %s\n", argv[2]);
		return 2;
	}

	uint8 buffer[READ_BUF_LEN] = {0};
	long long byte_counter = 0;
	int state = STATE_EXPECTING_ZERO_0;
	bool extra_zero = false;
	nal_buffer_t nal_buffer = { 0 };
	uint8 nut = 0;

	while (!feof(infile)) {
		size_t bytes_read = fread(buffer, 1, READ_BUF_LEN, infile);

		for (int i = 0; i < bytes_read; i++)
		{
			switch (state)
			{
			case STATE_EXPECTING_ZERO_0:
				if (buffer[i] == 0)
				{
					state = STATE_EXPECTING_ZERO_1;
				}
				else if (need_nal_copy[nut] && nal_buffer.pos < NAL_BUFFER_MAX)
				{
					copy_to_nal_buf(&nal_buffer, buffer[i]);
				}
				break;

			case STATE_EXPECTING_ZERO_1:
				if (buffer[i] == 0)
				{
					state = STATE_EXPECTING_ONE;
				}
				else
				{
					// restore missed 0, copy current byte
					if (need_nal_copy[nut] && nal_buffer.pos < NAL_BUFFER_MAX)
					{
						copy_to_nal_buf(&nal_buffer, 0);
					}
					if (need_nal_copy[nut] && nal_buffer.pos < NAL_BUFFER_MAX)
					{
						copy_to_nal_buf(&nal_buffer, buffer[i]);
					}
					state = STATE_EXPECTING_ZERO_0;
					extra_zero = false;
				}
				break;

			case STATE_EXPECTING_ONE:
				if (buffer[i] == 1)
				{
					state = STATE_EXPECTING_NAL_UNIT_TYPE;
				}
				else if (buffer[i] == 0)
				{
					// allow more zeroes, it is ok
					extra_zero = true;
				}
				else
				{
					// restore two missed 0s, copy current byte
					if (need_nal_copy[nut] && nal_buffer.pos < NAL_BUFFER_MAX)
					{
						copy_to_nal_buf(&nal_buffer, 0);
					}
					if (need_nal_copy[nut] && nal_buffer.pos < NAL_BUFFER_MAX)
					{
						copy_to_nal_buf(&nal_buffer, 0);
					}
					if (extra_zero)
					{
						if (need_nal_copy[nut] && nal_buffer.pos < NAL_BUFFER_MAX)
						{
							copy_to_nal_buf(&nal_buffer, 0);
						}
					}
					if (need_nal_copy[nut] && nal_buffer.pos < NAL_BUFFER_MAX)
					{
						copy_to_nal_buf(&nal_buffer, buffer[i]);
					}
					state = STATE_EXPECTING_ZERO_0;
					extra_zero = false;
				}
				break;

			case STATE_EXPECTING_NAL_UNIT_TYPE:
				// parse previous NAL unit
				nal_buffer.posmax = nal_buffer.pos;
				nal_buffer.pos = 0;
				nal_buffer.bitpos = 8;
				nal_parsers[nut](&nal_buffer);
				nut = (buffer[i] & 0x7e) >> 1;
				debug("\n%08llx: nal unit: %02x -> %d, %s\n", byte_counter, buffer[i], nut, NAL_UNIT_TYPES[nut]);
				// reset buffer for the next NAL unit
				nal_buffer.pos = 0;
				nal_buffer.bitpos = 8;
				if (need_nal_copy[nut] && nal_buffer.pos < NAL_BUFFER_MAX)
				{
					nal_initers[nut]();
					copy_to_nal_buf(&nal_buffer, buffer[i]);
				}
				state = STATE_EXPECTING_ZERO_0;
				extra_zero = false;
				break;
			}
			byte_counter++;
		}
	}
	//need to write last NAL unit
	if (nal_buffer.pos > 0)
	{
		nal_buffer.posmax = nal_buffer.pos;
		nal_buffer.pos = 0;
		nal_buffer.bitpos = 8;
		nal_parsers[nut](&nal_buffer);
	}
	fclose(infile);
	fclose(outfile);
}