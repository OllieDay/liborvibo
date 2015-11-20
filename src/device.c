#include "device.h"

#include <string.h>

// Orvibo sockets MAC addresses all begin 0xAC 0xCF
#define ORVIBO_SOCKET_IDENTIFIER 0xAC, 0xCF

bool
is_orvibo_mac(const unsigned char mac[static const ETHER_ADDR_LEN]) {
	return memcmp(mac, (unsigned char[]) {ORVIBO_SOCKET_IDENTIFIER}, 2) == 0;
}
