#include "message_sender.h"

#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "device.h"

bool
send_message(const char *const ip, const unsigned char *const data, const size_t length) {
	const int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (fd == -1) {
		return false;
	}
	static struct sockaddr_in destination = {
		.sin_family = AF_INET,
		.sin_port = htons(ORVIBO_SOCKET_PORT),
		.sin_zero = {0}
	};
	if (inet_aton(ip, &destination.sin_addr) == 0) {
		close(fd);
		return false;
	}
	// Discovery messages are sent to broadcast address.
	if (destination.sin_addr.s_addr == INADDR_BROADCAST) {
		static const int broadcast = 1;
		if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) != 0) {
			close(fd);
			return false;
		}
	}
	const ssize_t sent = sendto(fd, data, length, 0, (struct sockaddr *) &destination, sizeof(destination));
	close(fd);
	return sent >= 0 && (size_t) sent == length;
}
