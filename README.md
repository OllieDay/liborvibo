# liborvibo
Library for controlling Orvibo Smart Wi-Fi Plug

## Installation
```
$ git clone https://github.com/OllieDay/liborvibo.git
$ cd liborvibo
$ mkdir build && cd build
$ cmake ..
$ make install clean
```

## Usage
Include the header file:
```c
#include <orvibo.h>
```

Define an event handler to receive notifications:
```c
void event_handler(struct orvibo_socket *socket, enum orvibo_event event);
```

Create a socket from its MAC address:
```c
struct orvibo_socket *socket = orvibo_socket_create("ac:cf:00:00:00:00");
```

Start the client:
```c
orvibo_start(event_handler);
```

Discover the socket:
```c
orvibo_socket_discover(socket);
```

Get info on the socket:
```c
const char *mac = orvibo_socket_mac(socket);
const char *ip = orvibo_socket_ip(socket);
bool subscribed = orvibo_socket_subscribed(socket);
enum orvibo_state state = orvibo_socket_state(socket);
```

Subscribe the socket:
```c
orvibo_socket_subscribe(socket);
```

Switch the socket off and on:
```c
orvibo_socket_off(socket);
orvibo_socket_on(socket);
```

Unsubscribe the socket:
```c
orvibo_socket_unsubscribe(socket);
```

Stop the client:
```c
orvibo_stop();
```

Destroy the socket:
```c
orvibo_socket_destroy(socket);
```
