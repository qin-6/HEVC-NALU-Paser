#ifndef __NAL_SEI_H
#define __NAL_SEI_H

#include "cio.h"

void nal_sei_prefix_init();
void nal_sei_prefix_parse(nal_buffer_t * pnal_buffer);
void nal_sei_suffix_init();
void nal_sei_suffix_parse(nal_buffer_t * pnal_buffer);

#endif