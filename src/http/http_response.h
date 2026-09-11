#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <stddef.h>

typedef struct{
	int status_code;
	const char *status_text;
	char content_type[64];
	size_t content_length;
	const char *body;
}http_response_t;

void http_response_init(http_response_t *res);
int http_response_send(int client_fd, const http_response_t *res);

#endif
