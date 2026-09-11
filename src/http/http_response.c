#include "http_response.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void http_response_init(http_response_t *res){
	res->status_code = 200;
	res->status_text = "OK";
	strncpy(res->content_type, "text/html", sizeof(res->content_type));
	res->content_length = 0;
	res->body = NULL;
}

int http_response_send(int client_fd, const http_response_t *res){
	char header_buf[1024];
	int header_len = snprintf(header_buf, sizeof(header_buf),
			"HTTP/1.1 %d %s\r\n"
			"Server: C-Layer7-From-Scratch\r\n"
			"Content-Type: %s\r\n"
			"Content-Length: %zu\r\n"
			"Connection: close\r\n"
			"\r\n",
			res->status_code, res->status_text,
			res->content_type, res->content_length);
	if(write(client_fd, header_buf, header_len) < 0) return -1;

	if(res->body && res->content_length > 0){
		if(write(client_fd, res->body, res->content_length) < 0) return -1;
	}
	return 0;
}
