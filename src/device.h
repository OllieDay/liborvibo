#ifndef ORVIBO_DEVICE_H
#define ORVIBO_DEVICE_H

#include <net/ethernet.h>
#include <stdbool.h>

#define ORVIBO_SOCKET_PORT 10000

bool
is_orvibo_mac(const unsigned char mac[static ETHER_ADDR_LEN]);

#endif
