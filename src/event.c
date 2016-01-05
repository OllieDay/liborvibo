#include "event.h"

const char *
orvibo_event_string(const enum orvibo_event event) {
	switch (event) {
		case ORVIBO_EVENT_DISCOVER:
			return "discover";
		case ORVIBO_EVENT_SUBSCRIBE:
			return "subscribe";
		case ORVIBO_EVENT_UNSUBSCRIBE:
			return "unsubscribe";
		case ORVIBO_EVENT_OFF:
			return "off";
		case ORVIBO_EVENT_ON:
			return "on";
	}
}
