#include "message.h"

#include <stdlib.h>
#include <string.h>

struct message *
message_create(const unsigned char *const data,
			   const size_t length,
			   const enum orvibo_command command,
			   const enum orvibo_state state) {
	struct message *const message = malloc(sizeof(*message));
	if (message == NULL) {
		return NULL;
	}
	message->data = malloc(sizeof(*message->data) * length);
	if (message->data == NULL) {
		free(message);
		return NULL;
	}
	memcpy(message->data, data, length);
	message->length = length;
	message->command = command;
	message->state = state;
	return message;
}

void
message_destroy(struct message *const message) {
	free(message->data);
	free(message);
}
