#ifndef ROUTER_H
#define ROUTER_H

#include "http/http_request.h"

void router_handle_request(int client_fd, const http_request_t *req);

#endif
