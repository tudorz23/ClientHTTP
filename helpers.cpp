#include <cstdlib>     /* exit, atoi, malloc, free */
#include <cstdio>
#include <unistd.h>     /* read, write, close */
#include <cstring>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <arpa/inet.h>


#include "helpers.h"
#include "buffer.h"
#include "utils.h"


using namespace std;


void error(const char *msg) {
    perror(msg);
    exit(0);
}


void compute_string_message(string &message, const string &line) {
    message.append(line);
    message.append("\r\n");
}


int open_connection(std::string &host_ip, int portno, int ip_type, int socket_type, int flag) {
    struct sockaddr_in serv_addr;
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip.c_str(), &serv_addr.sin_addr);

    /* Connect the socket */
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}


void close_connection(int sockfd) {
    close(sockfd);
}


void send_to_server(int sockfd, string &msg) {
    char *message = strdup(msg.c_str());
    DIE(!message, "strdup failed\n");

    int bytes, sent = 0;
    int total = strlen(message);

    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0) {
            error("ERROR writing message to socket");
        }

        if (bytes == 0) {
            break;
        }

        sent += bytes;
    } while (sent < total);

    free(message);
}


string receive_from_server(int sockfd) {
    char response[BUFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    do {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0){
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
        
        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0) {
            header_end += HEADER_TERMINATOR_SIZE;
            
            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
            
            if (content_length_start < 0) {
                continue;           
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (true);
    size_t total = content_length + (size_t) header_end;
    
    while (buffer.size < total) {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0) {
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
    }
    buffer_add(&buffer, "", 1);

    string final_response(buffer.data);

    free(buffer.data);
    return final_response;
}


bool contains_space(string &buff) {
    if (buff.empty()) {
        return true;
    }

    for (auto &ch : buff) {
        if (isspace(ch)) {
            return true;
        }
    }

    return false;
}


bool is_number(std::string &buff) {
    if (buff.empty()) {
        return false;
    }

    if (buff[0] == '0') {
        return false;
    }

    for (auto &ch : buff) {
        if (!isdigit(ch)) {
            return false;
        }
    }

    return true;
}
