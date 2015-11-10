#include "state.h"

const char *
orvibo_state_string(const enum orvibo_state state) {
	return state == ORVIBO_STATE_UNKNOWN ? "unknown" : state == ORVIBO_STATE_OFF ? "off" : "on";
}
