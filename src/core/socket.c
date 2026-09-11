#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int server_socket_init(server_socket_t *server, int port){
	server->port = port;

	server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server->server_fd < 0){
		perror("Socket creation failed");
		return -1;
	}
	int opt = 1;
	if(setsockopt(server->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
		perror("setsockopt failed");
		close(server->server_fd);
		return -1;
	}

	memset(&server->address, 0, sizeof(server->address));
	server->address.sin_family = AF_INET;
	server->address.sin_addr.s_addr = INADDR_ANY;
	server->address.sin_port = htons(port);

	if(bind(server->server_fd, (struct sockaddr *)&server->address, sizeof(server->address)) < 0){
		perror("Bind failed");
		close(server->server_fd);
		return -1;
	}
	if(listen(server->server_fd, 10) < 0){
		perror("Listen failed");
		close(server->server_fd);
		return -1;
	}
	return 0;
}
int server_socket_accept(const server_socket_t *server, struct sockaddr_in *client_addr){
	socklen_t addrlen = sizeof(struct sockaddr_in);
	int client_fd = accept(server->server_fd, (struct sockaddr *)client_addr, &addrlen);
	if(client_fd < 0){
		perror("Accept failed");
	}
	return client_fd;
}
void server_socket_close(server_socket_t *server){
	if(server->server_fd >= 0){
		close(server->server_fd);
		server->server_fd = -1;
	}
}
