#include "socket.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "device.h"
#include "event_handler.h"
#include "list.h"
#include "message_sender.h"
#include "sockets.h"

#define ORVIBO_BROADCAST_ADDRESS "255.255.255.255"
#define ORVIBO_MESSAGE_PADDING 0x20, 0x20, 0x20, 0x20, 0x20, 0x20

static unsigned char *
parse_mac_string(const char *mac);

static bool
change_state(struct orvibo_socket *socket, enum orvibo_state state);

static void
reverse_mac(unsigned char destination[static ETHER_ADDR_LEN],
            const unsigned char source[static ETHER_ADDR_LEN]);

struct orvibo_socket *
orvibo_socket_create(const char *const mac) {
	char *const mac_string = strdup(mac);
	if (mac_string == NULL) {
		return NULL;
	}
	unsigned char *mac_bytes = parse_mac_string(mac_string);
	if (mac_bytes == NULL || !is_orvibo_mac(mac_bytes)) {
		goto cleanup;
	}
	struct orvibo_socket *const socket = malloc(sizeof(*socket));
	if (socket == NULL) {
		goto cleanup;
	}
	memcpy(socket->mac_bytes, mac_bytes, ETHER_ADDR_LEN);
	socket->mac_string = mac_string;
	socket->ip = NULL;
	socket->subscribed = false;
	socket->state = ORVIBO_STATE_UNKNOWN;
	return socket;
cleanup:
	free(mac_string);
	free(mac_bytes);
	return NULL;
}

void
orvibo_socket_destroy(struct orvibo_socket *const socket) {
    if (socket == NULL) {
        return;
    }
    remove_socket(socket);
	free(socket->mac_string);
    free(socket->ip);
    free(socket);
}

const char *
orvibo_socket_mac(const struct orvibo_socket *const socket) {
    return socket != NULL ? socket->mac_string : NULL;
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
    static unsigned char data[18] = {
        // Orvibo socket identifier.
        0x68, 0x64,
        // Message length.
        0x00, 0x12,
        // Command.
        0x71, 0x67,
        // Socket's MAC address.
        0, 0, 0, 0, 0, 0,
        // Padding.
        ORVIBO_MESSAGE_PADDING
    };
    memcpy(&data[6], socket->mac_bytes, 6);
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
    reverse_mac(reversed_mac, socket->mac_bytes);
    static unsigned char data[30] = {
        // Orvibo socket identifier.
        0x68, 0x64,
        // Message length.
        0x00, 0x1E,
        // Command.
        0x63, 0x6C,
        // Socket MAC address.
        0, 0, 0, 0, 0, 0,
        // Padding
        ORVIBO_MESSAGE_PADDING,
        // Socket's reversed MAC address.
        0, 0, 0, 0, 0, 0,
        // Padding.
        ORVIBO_MESSAGE_PADDING
    };
    memcpy(&data[6], socket->mac_bytes, 6);
    memcpy(&data[18], reversed_mac, ETHER_ADDR_LEN);
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
    handle_event(socket, ORVIBO_EVENT_UNSUBSCRIBE);
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

static unsigned char *
parse_mac_string(const char *const mac) {
	unsigned char *buf = malloc(sizeof(buf) * 6);
	if (sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]) == 6) {
		return buf;
	}
	free(buf);
	return NULL;
}

static bool
change_state(struct orvibo_socket *const socket, const enum orvibo_state state) {
    if (socket == NULL || socket->ip == NULL || !socket->subscribed || socket->state == state) {
        return false;
    }
    static unsigned char data[23] = {
        // Orvibo socket identifier.
        0x68, 0x64,
        // Message length.
        0x00, 0x17,
        // Command.
        0x64, 0x63,
        // Socket's MAC address.
        0, 0, 0, 0, 0, 0,
        // Padding.
        ORVIBO_MESSAGE_PADDING,
        // ???
        0x00, 0x00, 0x00, 0x00,
        // State.
        0
   };
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
