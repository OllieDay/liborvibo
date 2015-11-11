#include "client.h"

#include <pthread.h>

#include "message_receiver.h"
#include "sockets.h"

static pthread_t receive_thread;

bool
orvibo_start(const ORVIBO_EVENT_HANDLER handler) {
    if (running) {
        return true;
    }
    set_event_handler(handler);
    if (pthread_create(&receive_thread, NULL, (void *(*) (void *)) receive, NULL) != 0) {
        clear_event_handler();
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
    clear_event_handler();
    node_destroy_all(sockets, NULL);
    sockets = NULL;
    return true;
}
