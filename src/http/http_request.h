#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#define MAX_HEADER_SIZE 8192
#define MAX_HEADERS 32

typedef struct{
	char key[128];
	char value[256];
}http_header_t;

typedef struct{
	char method[16];
	char path[512];
	char version[16];
	http_header_t headers[MAX_HEADERS];
	int header_count;
	char *body;
}http_request_t;

int http_request_parse(http_request_t *req, const char *raw_data);
const char *http_request_get_header(const http_request_t *req, const char *key);

#endif
