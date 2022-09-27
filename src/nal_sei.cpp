#include <stdio.h>

#include "types.h"
#include "bio.h"
#include "cio.h"

void sei_payload(nal_buffer_t * pnal_buffer, uint32 payload_type, uint32 payload_size) {
	fprintf(stdout, "\tsei message: %d, size: %d\n", payload_type, payload_size);
	if (payload_type == 5) {
		read_bits(pnal_buffer, 128);
		align_to_byte(pnal_buffer);
		fprintf(stdout, "\n");
		while (pnal_buffer->pos < pnal_buffer->posmax) {
			fprintf(stdout, "%c", pnal_buffer->data[pnal_buffer->pos++]);
		}
		fprintf(stdout, "\n");
	}
	else {
		pnal_buffer->pos += payload_size;
	}
}

// SEI_PREFIX
void nal_sei_prefix_init() {

}

void nal_sei_prefix_parse(nal_buffer_t * pnal_buffer) {
	dump_nal_buffer(pnal_buffer);
	pnal_buffer->pos += 2;
	do {
		// sei_message()
		uint8 bb = 0;

		uint32 payload_type = 0;
		do {
			bb = read_bits(pnal_buffer, 8);
			payload_type += bb;
		} while (bb == 0xFF);

		uint32 payload_size = 0;
		do {
			bb = read_bits(pnal_buffer, 8);
			payload_size += bb;
		} while (bb == 0xFF);

		sei_payload(pnal_buffer, payload_type, payload_size);
	} while (pnal_buffer->pos < pnal_buffer->posmax - 3);
}

// SEI_SUFFIX
void nal_sei_suffix_init() {

}

void nal_sei_suffix_parse(nal_buffer_t * pnal_buffer) {
	dump_nal_buffer(pnal_buffer);
}
