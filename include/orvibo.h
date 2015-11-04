#ifndef ORVIBO_H
#define ORVIBO_H

#include <net/ethernet.h>
#include <stdbool.h>

enum orvibo_event {
    ORVIBO_EVENT_DISCOVER,
    ORVIBO_EVENT_SUBSCRIBE,
    ORVIBO_EVENT_UNSUBSCRIBE,
    ORVIBO_EVENT_OFF,
    ORVIBO_EVENT_ON
};

enum orvibo_state {
    ORVIBO_STATE_UNKNOWN,
    ORVIBO_STATE_OFF,
    ORVIBO_STATE_ON
};

struct orvibo_socket;

typedef void (*ORVIBO_EVENT_HANDLER) (struct orvibo_socket *, enum orvibo_event);

bool
orvibo_start(ORVIBO_EVENT_HANDLER handler);

bool
orvibo_stop(void);

struct orvibo_socket *
orvibo_socket_create(const unsigned char mac[static ETHER_ADDR_LEN]);

void
orvibo_socket_destroy(struct orvibo_socket *socket);

const unsigned char *
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
