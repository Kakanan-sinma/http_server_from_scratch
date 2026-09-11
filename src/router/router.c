#include "router.h"
#include "http/http_response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

static void serve_file(int client_fd, const char *filepath, const char *mime_type);

void router_handle_request(int client_fd, const http_request_t *req){
        char relative_path[PATH_MAX];

        if(strcmp(req->path, "/") == 0){
                snprintf(relative_path, sizeof(relative_path), "public/index.html");
        }else{
                snprintf(relative_path, sizeof(relative_path), "public%s", req->path);
        }

        char base_path[PATH_MAX];
        if(realpath("public", base_path) == NULL){
                http_response_t res;
                http_response_init(&res);
                res.status_code = 500;
                res.status_text = "Internal Server Error";
                http_response_send(client_fd, &res);
                return;
        }

        char resolved_path[PATH_MAX];
        if(realpath(relative_path, resolved_path) == NULL){
                serve_file(client_fd, "public/404.htm", "text/html");
                return;
        }

        size_t base_len = strlen(base_path);
        if(strncmp(resolved_path, base_path, base_len) != 0 ||
                        (resolved_path[base_len] != '/' && resolved_path[base_len] != '\0')){
                http_response_t res;
                http_response_init(&res);
                res.status_code = 403;
                res.status_text = "Forbidden";
                const char *forbidden_body = "<h1>403 Forbidden: Access Denied</h1>";
                res.body = forbidden_body;
                res.content_length = strlen(forbidden_body);
                http_response_send(client_fd, &res);
                return;
        }
        const char *mime = "text/html";
        if(strstr(resolved_path, ".css")) mime = "text/css";
        else if(strstr(resolved_path, ".js")) mime = "application/javascript";
        else if(strstr(resolved_path, ".png")) mime = "image/png";

        serve_file(client_fd, resolved_path, mime);
}

static void serve_file(int client_fd, const char *filepath, const char *mime_type){
        FILE *file = fopen(filepath, "rb");
        http_response_t res;
        http_response_init(&res);
        if(!file){
                res.status_code = 404;
                res.status_text = "Not Found";
                const char *not_found_body = "<h1>404 Not Found</h1>";
                res.body = not_found_body;
                res.content_length = strlen(not_found_body);
                http_response_send(client_fd, &res);
                return;
        }
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *buffer = malloc(size);
        if(buffer){
                fread(buffer, 1, size, file);
                res.status_code = 200;
                res.status_text = "OK";
                strncpy(res.content_type, mime_type, sizeof(res.content_type));
                res.body = buffer;
                res.content_length = size;
                http_response_send(client_fd, &res);
                free(buffer);
        }
        fclose(file);
}
