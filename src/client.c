#include "client.h"

#include <pthread.h>

#include "message_receiver.h"
#include "sockets.h"

static pthread_t receive_thread;

bool
orvibo_start(const ORVIBO_EVENT_HANDLER event_handler) {
    if (running) {
        return true;
    }
    if (event_handler == NULL) {
        return false;
    }
    socket_event_handler = event_handler;
    if (pthread_create(&receive_thread, NULL, (void *(*) (void *)) receive, NULL) != 0) {
        socket_event_handler = NULL;
        return false;
    }
    return running = true;
}

bool
orvibo_stop(void) {
    if (!running) {
        return true;
    }
    running = false;
    if (pthread_join(receive_thread, NULL) != 0) {
        return false;
    }
    socket_event_handler = NULL;
    node_destroy_all(sockets, NULL);
    return true;
}
