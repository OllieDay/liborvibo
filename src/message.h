#ifndef ORVIBO_MESSAGE_H
#define ORVIBO_MESSAGE_H

#include <sys/types.h>

#include "command.h"
#include "state.h"

struct message {
    unsigned char *data;
    size_t length;
    enum orvibo_command command;
    enum orvibo_state state;
};

struct message *
message_create(const unsigned char *data,
               size_t length,
               enum orvibo_command command,
               enum orvibo_state state);

void
message_destroy(struct message *message);

#endif
