#ifndef __BIO_H
#define __BIO_H

#include "cio.h"

void align_to_byte(nal_buffer_t * pnal_buffer);
uint8 read_bit(nal_buffer_t * pnal_buffer);
uint32 read_bits(nal_buffer_t * pnal_buffer, int nbits);
uint64 read_bits64(nal_buffer_t * pnal_buffer, int nbits);
uint32 read_uev(nal_buffer_t * pnal_buffer);
sint32 read_sev(nal_buffer_t * pnal_buffer);


int write_bit(nal_buffer_t * pnal_buffer, uint8 b);
int write_bits(nal_buffer_t * pnal_buffer, void * b, int nbits);
int write_uev(nal_buffer_t * pnal_buffer, uint32 b);
int write_sev(nal_buffer_t * pnal_buffer, sint32 b);

#endif