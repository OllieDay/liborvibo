#ifndef ORVIBO_MESSAGE_PARSER_H
#define ORVIBO_MESSAGE_PARSER_H

#include <net/ethernet.h>

#include "message.h"

struct message *
parse_message(const unsigned char *data, size_t length);

unsigned char *
parse_mac(const unsigned char data[static ETHER_ADDR_LEN], size_t length);

#endif
