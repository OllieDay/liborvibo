#ifndef ORVIBO_EVENT_HANDLER_H
#define ORVIBO_EVENT_HANDLER_H

#include "socket.h"

typedef void (*ORVIBO_EVENT_HANDLER) (struct orvibo_socket *, enum orvibo_event);

void
set_event_handler(ORVIBO_EVENT_HANDLER handler);

void
clear_event_handler(void);

void
handle_event(struct orvibo_socket *socket, enum orvibo_event event);

#endif
