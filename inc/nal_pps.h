#ifndef __NAL_PPS_H
#define __NAL_PPS_H

void nal_pps_init();
void nal_pps_parse(nal_buffer_t * pnal_buffer);
void print_nal_pps(nal_buffer_t * pnal_buffer);
void print_nal_pps_parse(nal_buffer_t * pnal_buffer);

#endif