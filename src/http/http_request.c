#include "http_request.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int http_request_parse(http_request_t *req, const char *raw_data){
	if(!req || !raw_data) return -1;
	memset(req, 0, sizeof(http_request_t));

	char buffer[MAX_HEADER_SIZE];
	strncpy(buffer, raw_data, sizeof(buffer) - 1);
	buffer[sizeof(buffer) - 1] = '\0';

	char *line_saveptr = NULL;
	char *line = strtok_r(buffer, "\r\n", &line_saveptr);
	if(!line) return -1;

	if(sscanf(line, "%15s %511s %15s", req->method, req->path, req->version) != 3){
		return -1;
	}
	while((line = strtok_r(NULL, "\r\n", &line_saveptr)) != NULL){
		if(strlen(line) == 0) break;

		char *colon = strchr(line, ':');
		if(colon && req->header_count < MAX_HEADERS){
			*colon = '\0';
			char *val = colon + 1;
			while(*val == ' ') val++;

			strncpy(req->headers[req->header_count].key, line, sizeof(req->headers[0].key) - 1);
			strncpy(req->headers[req->header_count].value, val, sizeof(req->headers[0].value) - 1);
			req->header_count++;
		}
	}
	return 0;
}
const char *http_request_get_header(const http_request_t *req, const char *key){
	for(int i = 0; i < req->header_count; i++){
		if(strcasecmp(req->headers[i].key, key) == 0){
			return req->headers[i].value;
		}
	}
	return NULL;
}
