#ifndef ORVIBO_SOCKETS_H
#define ORVIBO_SOCKETS_H

#include "list.h"
#include "socket.h"

// List of registered sockets.
extern struct node *sockets;

struct orvibo_socket *
find_socket_with_ip(const char *ip);

struct orvibo_socket *
find_socket_with_mac(const unsigned char mac[static ETHER_ADDR_LEN]);

bool
add_socket(struct orvibo_socket *socket);

void
remove_socket(const struct orvibo_socket *socket);

#endif
