#ifndef ORVIBO_STATE_H
#define ORVIBO_STATE_H

enum orvibo_state {
	ORVIBO_STATE_UNKNOWN,
	ORVIBO_STATE_OFF,
	ORVIBO_STATE_ON
};

const char *
orvibo_state_string(enum orvibo_state state);

#endif
