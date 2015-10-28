#ifndef ORVIBO_CLIENT_H
#define ORVIBO_CLIENT_H

#include "socket.h"

bool
orvibo_start(ORVIBO_EVENT_HANDLER event_handler);

bool
orvibo_stop(void);

#endif
