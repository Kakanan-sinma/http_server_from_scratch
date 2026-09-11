#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "core/socket.h"
#include "http/http_request.h"
#include "http/http_response.h"
#include "router/router.h"

#define PORT 8080

int main(){
	server_socket_t server;
	if(server_socket_init(&server, PORT) < 0){
		fprintf(stderr, "Failed to start server\n");
		return EXIT_FAILURE;
	}

	printf("[L4 TCP] Server running on port %d...\n", PORT);

	while(1){
		struct sockaddr_in client_addr;
		int client_fd = server_socket_accept(&server, &client_addr);
		if(client_fd < 0) continue;

		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
		printf("[L4 TCP] Client connected: %s:%d\n", client_ip, ntohs(client_addr.sin_port));

		char buffer[8192] = {0};
		ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

		if(bytes_read > 0){
			http_request_t req;
			if(http_request_parse(&req, buffer) == 0){
				printf("[L7 HTTP] %s %s %s\n", req.method, req.path, req.version);
				router_handle_request(client_fd, &req);
			}
		}
		close(client_fd);
	}
	server_socket_close(&server);
	return EXIT_SUCCESS;
}
