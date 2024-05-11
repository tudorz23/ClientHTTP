#include <cstdlib>     /* exit, atoi, malloc, free */
#include <cstdio>
#include <unistd.h>     /* read, write, close */
#include <cstring>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>

#include "helpers.h"
#include "requests.h"
#include "utils.h"

using namespace std;

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count) {
    char *message = (char*) calloc(BUFLEN, sizeof(char));
    char *line = (char*) calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    memset(line, 0, LINELEN);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (cookies != NULL) {
        sprintf(line, "Cookie: ");
        for (int i = 0; i < cookies_count - 1; i++) {
            sprintf(line + strlen(line), "%s; ", cookies[i]);
        }

        sprintf(line + strlen(line), "%s", cookies[cookies_count - 1]);

        compute_message(message, line);
    }

    // Step 4: add final new line
    compute_message(message, "");

    free(line);
    return message;
}


char *old_compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, int cookies_count) {
    char *message = (char*) calloc(BUFLEN, sizeof(char));
    char *line = (char*) calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char *) calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    memset(line, 0, LINELEN);
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    int total_len = 0;

    for (int i = 0; i < body_data_fields_count; i++) {
        total_len += strlen(body_data[i]);
    }

    memset(line, 0, LINELEN);
    sprintf(line, "Content-Length: %d", total_len + 1);
    compute_message(message, line);

    // Step 4 (optional): add cookies
    if (cookies != NULL) {
       sprintf(line, "Cookie: ");
        for (int i = 0; i < cookies_count - 1; i++) {
            sprintf(line, "%s; ", cookies[i]);
        }

        sprintf(line, "%s", cookies[cookies_count - 1]);

        compute_message(message, line);
    }

    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    memset(line, 0, LINELEN);

    for (int i = 0; i < body_data_fields_count - 1; i++) {
        strcat(body_data_buffer, body_data[i]);
        strcat(body_data_buffer, "&");
    }
    strcat(body_data_buffer, body_data[body_data_fields_count - 1]);

    strcat(message, body_data_buffer);
    compute_message(message, "");

    free(line);
    free(body_data_buffer);
    return message;
}


string compute_post_request(const string &host, const string &url,
                            const string &content_type, string &body_data,
                            string &cookies) {
    string message;
    string line;

    line.append("POST " + url + " HTTP/1.1");
    compute_string_message(message, line);
    line.clear();

    line.append("Host: " + host);
    compute_string_message(message, line);
    line.clear();

    line.append("Content-Length: " + to_string(body_data.length()));
    compute_string_message(message, line);
    line.clear();

    line.append("Content-Type: " + content_type);
    compute_string_message(message, line);
    line.clear();

    compute_string_message(message, "");

    compute_string_message(message, body_data);

    return message;
}
