// Library for controlling Orvibo Smart Wi-Fi Plug.

#ifndef ORVIBO_H
#define ORVIBO_H

#include <net/ethernet.h>
#include <stdbool.h>

// Events raised when a socket change occurs.
// A socket change could be the result of calling orvibo_socket_* (discover, subscribe, unsubscribe, off, on) or
// otherwise something external, e.g. switching the socket off or on manually.
// Additional socket events exist that are not implemented by the library and are instead ignored.
enum orvibo_event {
    // Socket discovered.
    ORVIBO_EVENT_DISCOVER,
    // Socket subscribed.
    ORVIBO_EVENT_SUBSCRIBE,
    // Socket unsubscribed.
    ORVIBO_EVENT_UNSUBSCRIBE,
    // Socket switched off.
    ORVIBO_EVENT_OFF,
    // Socket switched on.
    ORVIBO_EVENT_ON
};

// Get event string: "discover", "subscribe", "unsubscribe", "off", "on".
const char *
orvibo_event_string(enum orvibo_event event);

// Physical socket state, i.e. unknown, off, on.
enum orvibo_state {
    // Socket state unknown, either because it is not discovered or received message data was corrupt.
    ORVIBO_STATE_UNKNOWN,
    // Socket switched off.
    ORVIBO_STATE_OFF,
    // Socket switched on.
    ORVIBO_STATE_ON
};

// Get state string: "unknown", "off", "on"
const char *
orvibo_state_string(enum orvibo_state state);

// Handle to an Orvibo socket created with orvibo_socket_create().
// Get socket info with orvibo_socket_* (mac, ip, subscribed, state).
struct orvibo_socket;

// Event handler for events raised when a socket change occurs.
typedef void (*ORVIBO_EVENT_HANDLER) (struct orvibo_socket *, enum orvibo_event);

// Start listening for messages from sockets on the network.
// Provided ORVIBO_EVENT_HANDLER is used to raise events when a socket change occurs.
// Must be called prior to orvibo_socket_* (discover, subscribe, unsubscribe, off, on).
// Stop listening for messages with orvibo_stop().
bool
orvibo_start(ORVIBO_EVENT_HANDLER handler);

// Stop listening for messages from sockets on the network.
bool
orvibo_stop(void);

// Create socket with specified MAC address.
// Call orvibo_socket_destroy() to free.
struct orvibo_socket *
orvibo_socket_create(const unsigned char mac[static ETHER_ADDR_LEN]);

// Free socket created with orvibo_socket_create().
void
orvibo_socket_destroy(struct orvibo_socket *socket);

// Get socket MAC address.
const unsigned char *
orvibo_socket_mac(const struct orvibo_socket *socket);

// Get socket IP address.
// Returns NULL if socket not discovered.
const char *
orvibo_socket_ip(const struct orvibo_socket *socket);

// Get socket subscribed status.
bool
orvibo_socket_subscribed(const struct orvibo_socket *socket);

// Get socket state.
enum orvibo_state
orvibo_socket_state(const struct orvibo_socket *socket);

// Discover socket.
// Must be called prior to orvibo_socket_* (subscribe, unsubscribe, off, on).
// Returns true if 'discover' message broadcast to network.
// ORVIBO_EVENT_DISCOVER is raised when socket acknowledges request and is discovered.
bool
orvibo_socket_discover(struct orvibo_socket *socket);

// Subscribe socket.
// Must be called prior to orvibo_socket_* (unsubscribe, off, on).
// Returns true if 'subscribe' message sent to socket.
// ORVIBO_EVENT_SUBSCRIBE is raised when socket acknowledges request and is subscribed.
bool
orvibo_socket_subscribe(struct orvibo_socket *socket);

// Unsubscribe socket.
// Must be subscribed.
// Returns true if successfully unsubscribed.
// ORVIBO_EVENT_UNSUBSCRIBE is raised when successfully unsubscribed.
bool
orvibo_socket_unsubscribe(struct orvibo_socket *socket);

// Switch socket off.
// Must be subscribed.
// Returns true if 'off' message sent to socket.
// ORVIBO_EVENT_OFF is raised when socket acknowledges request and is switched off.
bool
orvibo_socket_off(struct orvibo_socket *socket);

// Switch socket on.
// Must be subscribed.
// Returns true if 'on' message sent to socket.
// ORVIBO_EVENT_ON is raised when socket acknowledges request and is switched on.
bool
orvibo_socket_on(struct orvibo_socket *socket);

#endif
