#ifndef ORVIBO_SOCKET_H
#define ORVIBO_SOCKET_H

#include <net/ethernet.h>
#include <stdbool.h>

#include "event.h"
#include "state.h"

struct orvibo_socket {
	char *mac_string;
	unsigned char mac_bytes[ETHER_ADDR_LEN];
	char *ip;
	bool subscribed;
	enum orvibo_state state;
};

struct orvibo_socket *
orvibo_socket_create(const char *mac);

void
orvibo_socket_destroy(struct orvibo_socket *socket);

const char *
orvibo_socket_mac(const struct orvibo_socket *socket);

const char *
orvibo_socket_ip(const struct orvibo_socket *socket);

bool
orvibo_socket_subscribed(const struct orvibo_socket *socket);

enum orvibo_state
orvibo_socket_state(const struct orvibo_socket *socket);

bool
orvibo_socket_discover(struct orvibo_socket *socket);

bool
orvibo_socket_subscribe(struct orvibo_socket *socket);

bool
orvibo_socket_unsubscribe(struct orvibo_socket *socket);

bool
orvibo_socket_off(struct orvibo_socket *socket);

bool
orvibo_socket_on(struct orvibo_socket *socket);

#endif
