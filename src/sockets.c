#include "sockets.h"

#include <string.h>

struct node *sockets = NULL;

static struct orvibo_socket *
find_socket(const unsigned char *data, NODE_DATA_MATCH match);

static bool
is_address_equal(const struct orvibo_socket *socket, const char *ip);

static bool
is_mac_equal(const struct orvibo_socket *socket, const unsigned char mac[static ETHER_ADDR_LEN]);

struct orvibo_socket *
find_socket_with_ip(const char *const ip) {
    return find_socket((const unsigned char *) ip, (NODE_DATA_MATCH) is_address_equal);
}

struct orvibo_socket *
find_socket_with_mac(const unsigned char mac[static const ETHER_ADDR_LEN]) {
    return find_socket(mac, (NODE_DATA_MATCH) is_mac_equal);
}

bool
add_socket(struct orvibo_socket *const socket) {
    struct node *const node = node_create(socket);
    if (node == NULL) {
        return false;
    }
    node_prepend(node, sockets);
    sockets = node;
    return true;
}

void
remove_socket(const struct orvibo_socket *const socket) {
    node_remove(sockets, socket, (NODE_DATA_MATCH) is_address_equal);
}

static struct orvibo_socket *
find_socket(const unsigned char *const data, const NODE_DATA_MATCH match) {
    const struct node *const node = node_find(sockets, data, match);
    return node != NULL ? node->data : NULL;
}

static bool
is_address_equal(const struct orvibo_socket *const socket, const char *const ip) {
    return socket != NULL && socket->ip != NULL && strcmp(socket->ip, ip) == 0;
}

static bool
is_mac_equal(const struct orvibo_socket *const socket, const unsigned char mac[static const ETHER_ADDR_LEN]) {
    return socket != NULL && memcmp(socket->mac_bytes, mac, ETHER_ADDR_LEN) == 0;
}
