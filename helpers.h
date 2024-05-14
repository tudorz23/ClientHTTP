#ifndef HELPERS_H
#define HELPERS_H

#include <string>

#define BUFLEN 4096

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)


// Shows the current error.
void error(const char *msg);


// Updates the message with line, appending the necessary separators.
void compute_string_message(std::string &message, const std::string &line);


// Opens a connection with server host_ip on port portno, returns a socket.
int open_connection(const std::string &host_ip, int portno, int ip_type,
                    int socket_type, int flag);


// Closes a server connection on socket sockfd.
void close_connection(int sockfd);


// Send a message to a server.
void send_to_server(int sockfd, std::string &msg);


// Receives and returns the message from a server.
std::string receive_from_server(int sockfd);


// Checks if buff contains spaces or if it is empty.
bool contains_space(std::string &buff);


// Checks if buff is a valid number.
bool is_number(std::string &buff);


#endif /* HELPERS_H */
