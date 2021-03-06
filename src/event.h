#ifndef ORVIBO_EVENT_H
#define ORVIBO_EVENT_H

enum orvibo_event {
	ORVIBO_EVENT_DISCOVER,
	ORVIBO_EVENT_SUBSCRIBE,
	ORVIBO_EVENT_UNSUBSCRIBE,
	ORVIBO_EVENT_OFF,
	ORVIBO_EVENT_ON
};

const char *
orvibo_event_string(enum orvibo_event event);

#endif
