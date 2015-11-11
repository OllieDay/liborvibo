#ifndef ORVIBO_CLIENT_H
#define ORVIBO_CLIENT_H

#include "event_handler.h"

bool
orvibo_start(ORVIBO_EVENT_HANDLER event_handler);

bool
orvibo_stop(void);

#endif
