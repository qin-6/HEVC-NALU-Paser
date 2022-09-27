#include <stdio.h>

#include "types.h"
#include "cio.h"
#include "debug.h"
#include <cstdlib>

void align_to_byte(nal_buffer_t * pnal_buffer) {
	if (pnal_buffer->bitpos != 8) {
		pnal_buffer->pos++;
		pnal_buffer->bitpos = 8;
	}
}

uint8 read_bit(nal_buffer_t * pnal_buffer) {
	if (pnal_buffer->bitpos == 0) {
		pnal_buffer->pos++;
		pnal_buffer->bitpos = 8;
	}

	if (pnal_buffer->pos >= pnal_buffer->posmax) {
		fprintf(stderr, "!!! nal buffer overrun !!!\n");
	}

	uint8 ret = pnal_buffer->data[pnal_buffer->pos] & (1 << (pnal_buffer->bitpos - 1));
	pnal_buffer->bitpos--;
	debug("%d", ret > 0 ? 1 : 0);
	return ret > 0 ? 1 : 0;
}

uint32 read_bits(nal_buffer_t * pnal_buffer, int nbits) {
	uint32 ret = 0;
	for (int i = 0; i < nbits; i++) {
		ret = (ret << 1) | read_bit(pnal_buffer);
	}
	return ret;
}

uint64 read_bits64(nal_buffer_t * pnal_buffer, int nbits) {
	uint64 ret = 0;
	for (int i = 0; i < nbits; i++) {
		ret = (ret << 1) | read_bit(pnal_buffer);
	}
	return ret;
}

uint32 read_uev(nal_buffer_t * pnal_buffer) { // read exp-golomb code
	debug("[");
	int zero_leading_bits = -1;
	uint8 b = 0;
	for (b = 0; !b; zero_leading_bits++) {
		b = read_bit(pnal_buffer);
	}
	uint32 ret = (1 << zero_leading_bits) - 1 + read_bits(pnal_buffer, zero_leading_bits);
	debug("]");
	return ret;
}

sint32 read_sev(nal_buffer_t * pnal_buffer) { // read signed exp-golomb code
	uint32 val = read_uev(pnal_buffer);
	sint32 ret = 0;
	if (val > 0) {
		ret = ((val % 2) > 0 ? 1 : -1) * CEIL(val, 2);
	}
	return ret;

}

int write_bit(nal_buffer_t * pnal_buffer, uint8 b)
{
	if (pnal_buffer->bitpos == 0) {
		pnal_buffer->pos++;
		pnal_buffer->bitpos = 8;
	}
	uint8 ret = (b & 1);
	pnal_buffer->data[pnal_buffer->pos] |= (b & 1) << (pnal_buffer->bitpos -1);
	pnal_buffer->bitpos--;
	debug("%d", ret > 0 ? 1 : 0);
	return 1;
}

int write_bits(nal_buffer_t * pnal_buffer, void * v , int nbits)
{
	if (nbits > 0 && nbits <= 8)
	{
		uint8 b = 0;
		for (int i = nbits; i > 0; i--)
		{
			b = (*((uint8 *)v) >> (i - 1)) & 1;
			write_bit(pnal_buffer, b);
		}
		return nbits;
	}
	if (nbits <= 16)
	{
		uint16 b = 0;
		for (int i = nbits; i > 0; i--)
		{
			b = (*((uint16 *)v) >> (i - 1)) & 1;
			write_bit(pnal_buffer, b);
		}
		return nbits;
	}
	if (nbits <= 32)
	{
		uint32 b = 0;
		for (int i = nbits; i > 0; i--)
		{
			b = (*((uint32 *)v) >> (i - 1)) & 1;
			write_bit(pnal_buffer, b);
		}
		return nbits;
	}
	if (nbits <= 64)
	{
		uint64 b = 0;
		for (int i = nbits; i > 0; i--)
		{
			b = (*((uint64 *)v) >> (i - 1)) & 1;
			write_bit(pnal_buffer, b);
		}
		return nbits;
	}
	fprintf(stderr, "!!! can't write more then 64 bits !!!\n");
	return 0;
}

int write_uev(nal_buffer_t * pnal_buffer, uint32 num) //write exp-golomb code
{
	debug("[");
	if (num == 0xFFFFFFFF)
	{
		fprintf(stderr, "!!! can't write more then 32bit uev!!!\n");
		return 0;
	}

	int pos = 0;
	uint32 x = num + 1;
	for (int i = 32; i > 0; i--)
	{
		if ((x >> i - 1) & 1)
		{
			break;
		}
		pos++;
	}

	int leading_zeros = 32 - pos - 1;

	uint32 zero = 0;

	if (leading_zeros == 0)
	{
		write_bit(pnal_buffer, 1);

	} else if (leading_zeros == 1)
	{
		write_bit(pnal_buffer, 0);
		write_bits(pnal_buffer, &x, leading_zeros + 1);
	} else if (leading_zeros > 1)
	{
		write_bits(pnal_buffer, &zero, leading_zeros);
		write_bits(pnal_buffer, &x, leading_zeros + 1);
	}
	debug("]");
	return 32 - pos + 32 - pos -1;
}

int write_sev(nal_buffer_t * pnal_buffer, sint32 b)
{
	int ret = 0;
	if (b < 0)
	{
		ret = write_uev(pnal_buffer, -2*b);
	}
	else
	{
		ret = write_uev(pnal_buffer, 2*b - 1);
	}
	return ret;
}
