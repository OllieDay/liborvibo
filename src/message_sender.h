#ifndef ORVIBO_MESSAGE_SENDER_H
#define ORVIBO_MESSAGE_SENDER_H

#include <stdbool.h>
#include <stddef.h>

bool
send_message(const char *ip, const unsigned char *data, size_t length);

#endif
