#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct {
	int server_fd;
	int port;
	struct sockaddr_in address;
} server_socket_t;

int server_socket_init(server_socket_t *server, int port);
int server_socket_accept(const server_socket_t *server, struct sockaddr_in *client_addr);
void server_socket_close(server_socket_t *server);

#endif
