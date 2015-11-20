#include "message_parser.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "device.h"

static bool
validate(const unsigned char *data, size_t length);

static enum orvibo_command
parse_command(unsigned char data);

static enum orvibo_state
parse_state(unsigned char data);

struct message *
parse_message(const unsigned char *const data, const size_t length) {
	if (!validate(data, length)) {
		return NULL;
	}
	const enum orvibo_command command = parse_command(data[4]);
	// Last byte of the message contains the state.
	const enum orvibo_state state = parse_state(data[length - 1]);
	return message_create(data, length, command, state);
}

unsigned char *
parse_mac(const unsigned char data[static const ETHER_ADDR_LEN], const size_t length) {
	for (size_t i = 0; i <= (length - ETHER_ADDR_LEN); i++) {
		if (is_orvibo_mac(&data[i])) {
			unsigned char *const mac = malloc(sizeof(*mac) * ETHER_ADDR_LEN);
			if (mac != NULL) {
				memcpy(mac, &data[i], ETHER_ADDR_LEN);
			}
			return mac;
		}
	}
	return NULL;
}

static bool
validate(const unsigned char data[static const ETHER_ADDR_LEN], const size_t length) {
	// Orvibo messages begin with 0x68 0x64.
	if (data[0] != 0x68 || data[1] != 0x64) {
		return false;
	}
	// Message length is included in the message. Verify it is correct.
	const uint16_t actual_len = (uint16_t) (data[3] | (data[2] << 8));
	return length == actual_len;
}

static enum orvibo_command
parse_command(const unsigned char data) {
	switch (data) {
		case 0x71:
			return ORVIBO_COMMAND_DISCOVER;
		case 0x63:
			return ORVIBO_COMMAND_SUBSCRIBE;
		case 0x73:
			return ORVIBO_COMMAND_STATE;
	}
	// Other commands we don't handle.
	return ORVIBO_COMMAND_UNKNOWN;
}

static enum orvibo_state
parse_state(const unsigned char data) {
	return data == 0x0 ? ORVIBO_STATE_OFF : data == 0x1 ? ORVIBO_STATE_ON : ORVIBO_STATE_UNKNOWN;
}
