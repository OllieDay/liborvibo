#include "message_receiver.h"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "device.h"
#include "event_handler.h"
#include "message_parser.h"
#include "sockets.h"

bool running = false;

static void
receive_message(int fd);

static struct orvibo_socket *
find_socket(const char *ip, const struct message *message);

static void
handle_message(const char *ip, const struct message *message);

static void
handle_discover_message(struct orvibo_socket *socket, const struct message *message);

static void
handle_subscribe_message(struct orvibo_socket *socket, const struct message *message);

static void
handle_state_message(struct orvibo_socket *socket, const struct message *message);

static struct node *
create_address(struct in_addr address);

static struct node *
get_local_addresses(void);

static bool
is_address_equal(const char *x, const char *y);

static bool
is_local_address(const char *ip);

void
receive(void) {
    const int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == -1) {
        return;
    }
    static const struct sockaddr_in destination = {
        .sin_family = AF_INET,
        .sin_port = htons(ORVIBO_SOCKET_PORT),
        .sin_addr = {
            .s_addr = htonl(INADDR_ANY)
        },
    };
    // Short timeout to keep things running smoothly.
    static const struct timeval timeout = {
        .tv_sec = 0,
        .tv_usec = 100000
    };
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        close(fd);
        return;
    }
    if (bind(fd, (const struct sockaddr *) &destination, sizeof(destination)) == -1) {
        close(fd);
        return;
    }
    while (running) {
        receive_message(fd);
    }
}

static void
receive_message(const int fd) {
    static struct sockaddr_in source;
    static socklen_t size = sizeof(source);
    static unsigned char buffer[256];
    const ssize_t received = recvfrom(fd, buffer, 256, 0, (struct sockaddr *) &source, &size);
    if (received == -1) {
        return;
    }
    char *const ip = strdup(inet_ntoa(source.sin_addr));
    if (ip == NULL) {
        return;
    }
    // Ignore messages sent by us.
    if (is_local_address(ip)) {
        free(ip);
        return;
    }
    struct message *const message = parse_message(buffer, (size_t) received);
    if (message == NULL) {
        free(ip);
        return;
    }
    handle_message(ip, message);
    message_destroy(message);
    free(ip);
}

static struct orvibo_socket *
find_socket(const char *const ip, const struct message *const message) {
    // If it's a newly discovered socket we must set the IP.
    if (message->command == ORVIBO_COMMAND_DISCOVER) {
        unsigned char *const mac = parse_mac(message->data, message->length);
        if (mac == NULL) {
            return NULL;
        }
        struct orvibo_socket *const socket = find_socket_with_mac(mac);
        free(mac);
        // Sockets occasionally send duplicate messages; only assign the IP on the first discovery message.
        if (socket != NULL && socket->ip == NULL) {
            socket->ip = strdup(ip);
            if (socket->ip == NULL) {
                return NULL;
            }
        }
        return socket;
    }
    return find_socket_with_ip(ip);
}

static void
handle_message(const char *const ip, const struct message *const message) {
    struct orvibo_socket *const socket = find_socket(ip, message);
    if (socket == NULL) {
        return;
    }
    switch (message->command) {
        case ORVIBO_COMMAND_DISCOVER:
            handle_discover_message(socket, message);
            break;
        case ORVIBO_COMMAND_SUBSCRIBE:
            handle_subscribe_message(socket, message);
            break;
        case ORVIBO_COMMAND_STATE:
            handle_state_message(socket, message);
            break;
        case ORVIBO_COMMAND_UNKNOWN:
            // Other commands we don't handle.
            break;
    }
}

static void
handle_discover_message(struct orvibo_socket *const socket, const struct message *const message) {
    socket->state = message->state;
    handle_event(socket, ORVIBO_EVENT_DISCOVER);
}

static void
handle_subscribe_message(struct orvibo_socket *const socket, const struct message *const message) {
    if (!socket->subscribed) {
        socket->subscribed = true;
        socket->state = message->state;
        handle_event(socket, ORVIBO_EVENT_SUBSCRIBE);
    }
}

static void
handle_state_message(struct orvibo_socket *const socket, const struct message *const message) {
    if (socket->subscribed
        && socket->state != message->state
        && (message->state == ORVIBO_STATE_OFF || message->state == ORVIBO_STATE_ON)) {
        socket->state = message->state;
        const enum orvibo_event event = socket->state == ORVIBO_STATE_OFF ? ORVIBO_EVENT_OFF : ORVIBO_EVENT_ON;
        handle_event(socket, event);
    }
}

static struct node *
create_address(const struct in_addr address) {
    char *const ip = strdup(inet_ntoa(address));
    if (ip == NULL) {
        return NULL;
    }
    struct node *const node = node_create(ip);
    if (node == NULL) {
        free(ip);
    }
    return node;
}

static struct node *
get_local_addresses(void) {
    struct ifaddrs *ifaddrs = NULL;
    getifaddrs(&ifaddrs);
    if (ifaddrs == NULL) {
        return NULL;
    }
    const struct ifaddrs *temp = ifaddrs;
    struct node *addresses = NULL;
    struct node *current = NULL;
    while (temp != NULL) {
        if (temp->ifa_addr->sa_family == AF_INET) {
            struct node *const next = create_address(((struct sockaddr_in *) (void *) temp->ifa_addr)->sin_addr);
            if (next == NULL) {
                continue;
            }
            if (addresses == NULL) {
                current = addresses = next;
            } else {
                current = current->next = next;
            }
        }
        temp = temp->ifa_next;
    }
    freeifaddrs(ifaddrs);
    return addresses;
}

static bool
is_address_equal(const char *const x, const char *const y) {
    return x != NULL && strcmp(x, y) == 0;
}

static bool
is_local_address(const char *const ip) {
    struct node *const addresses = get_local_addresses();
    const bool is_local = node_find(addresses, ip, (NODE_DATA_MATCH) is_address_equal) != NULL;
    node_destroy_all(addresses, free);
    return is_local;
}
