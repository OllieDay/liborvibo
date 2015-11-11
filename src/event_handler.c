#include "event_handler.h"

#include <stddef.h>

static ORVIBO_EVENT_HANDLER event_handler = NULL;

void
set_event_handler(const ORVIBO_EVENT_HANDLER handler) {
	event_handler = handler;
}

void
clear_event_handler(void) {
	event_handler = NULL;
}

void
handle_event(struct orvibo_socket *socket, enum orvibo_event event) {
	if (event_handler != NULL) {
		event_handler(socket, event);
	}
}
