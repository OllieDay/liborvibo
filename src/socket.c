#include "socket.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "device.h"
#include "list.h"
#include "message_sender.h"
#include "sockets.h"

#define ORVIBO_BROADCAST_ADDRESS "255.255.255.255"

ORVIBO_EVENT_HANDLER socket_event_handler = NULL;

static bool
is_orvibo_mac(const unsigned char mac[static ETHER_ADDR_LEN]);

static bool
change_state(struct orvibo_socket *socket, enum orvibo_state state);

static void
reverse_mac(unsigned char destination[static ETHER_ADDR_LEN],
            const unsigned char source[static ETHER_ADDR_LEN]);

struct orvibo_socket *
orvibo_socket_create(const unsigned char mac[static const ETHER_ADDR_LEN]) {
   if (!is_orvibo_mac(mac)) {
      return NULL;
   }
    struct orvibo_socket *const socket = malloc(sizeof(*socket));
    if (socket == NULL) {
        return NULL;
    }
    memcpy(socket->mac, mac, ETHER_ADDR_LEN);
    socket->ip = NULL;
    socket->subscribed = false;
    socket->state = ORVIBO_STATE_UNKNOWN;
    return socket;
}

void
orvibo_socket_destroy(struct orvibo_socket *const socket) {
    if (socket == NULL) {
        return;
    }
    remove_socket(socket);
    free(socket->ip);
    free(socket);
}

const unsigned char *
orvibo_socket_mac(const struct orvibo_socket *const socket) {
    return socket != NULL ? socket->mac : NULL;
}

const char *
orvibo_socket_ip(const struct orvibo_socket *const socket) {
    return socket != NULL ? socket->ip : NULL;
}

bool
orvibo_socket_subscribed(const struct orvibo_socket *const socket) {
    return socket != NULL ? socket->subscribed : false;
}

enum orvibo_state
orvibo_socket_state(const struct orvibo_socket *const socket) {
    return socket != NULL ? socket->state : ORVIBO_STATE_UNKNOWN;
}


bool
orvibo_socket_discover(struct orvibo_socket *const socket) {
    if (socket == NULL || socket->subscribed) {
        return false;
    }
    static unsigned char data[18];
    memcpy(data, ((char[]) {0x68, 0x64, 0x00, 0x12, 0x71, 0x67}), 6);
    memcpy(&data[6], socket->mac, 6);
    memcpy(&data[12], ((char[]) {0x20, 0x20, 0x20, 0x20, 0x20, 0x20}), 6);
    if (!send_message(ORVIBO_BROADCAST_ADDRESS, data, 18)) {
        return false;
    }
    return add_socket(socket);
}

bool
orvibo_socket_subscribe(struct orvibo_socket *const socket) {
    if (socket == NULL || socket->ip == NULL || socket->subscribed) {
        return false;
    }
    // Subscribe message contains the socket's MAC address in reverse.
    static unsigned char reversed_mac[ETHER_ADDR_LEN];
    reverse_mac(reversed_mac, socket->mac);
    static unsigned char data[30];
    memcpy(data, ((char[]) {0x68, 0x64, 0x00, 0x1E, 0x63, 0x6C}), 6);
    memcpy(&data[6], socket->mac, 6);
    memcpy(&data[12], ((char[]) {0x20, 0x20, 0x20, 0x20, 0x20, 0x20}), 6);
    memcpy(&data[18], reversed_mac, ETHER_ADDR_LEN);
    memcpy(&data[24], ((char[]) {0x20, 0x20, 0x20, 0x20, 0x20, 0x20}), 6);
    return send_message(socket->ip, data, 30);
}

bool
orvibo_socket_unsubscribe(struct orvibo_socket *const socket) {
    if (socket == NULL || !socket->subscribed) {
        return false;
    }
    // There's no unsubscribe message, but we still want to provide a method of unsubscribing from events.
    socket->subscribed = false;
    socket->state = ORVIBO_STATE_UNKNOWN;
    socket_event_handler(socket, ORVIBO_EVENT_UNSUBSCRIBE);
    return true;
}

bool
orvibo_socket_off(struct orvibo_socket *const socket) {
    return change_state(socket, ORVIBO_STATE_OFF);
}

bool
orvibo_socket_on(struct orvibo_socket *const socket) {
    return change_state(socket, ORVIBO_STATE_ON);
}

bool
is_orvibo_mac(const unsigned char mac[static const ETHER_ADDR_LEN]) {
   return memcmp(mac, (unsigned char[]) ORVIBO_SOCKET_IDENTIFIER, 2) == 0;
}

static bool
change_state(struct orvibo_socket *const socket, const enum orvibo_state state) {
    if (socket == NULL || socket->ip == NULL || !socket->subscribed || socket->state == state) {
        return false;
    }
    static unsigned char data[23];
    memcpy(data, ((char[]) {0x68, 0x64, 0x00, 0x17, 0x64, 0x63}), 6);
    memcpy(&data[6], socket->mac, 6);
    memcpy(&data[12], ((char[]) {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00}), 10);
    data[22] = state == ORVIBO_STATE_OFF ? 0x0 : 0x1;
    return send_message(socket->ip, data, 23);
}

static void
reverse_mac(unsigned char destination[static const ETHER_ADDR_LEN],
            const unsigned char source[static const ETHER_ADDR_LEN]) {
    for (int i = 0; i < ETHER_ADDR_LEN; i++) {
        destination[i] = source[ETHER_ADDR_LEN - i - 1];
    }
}
