#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdlib>
#include <string>

#define SERVER_PORT 8080


/*
 * Error checking macro.
 */
#define DIE(assertion, call_description)                                \
    do {                                                                \
        if (assertion) {                                                \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);          \
            perror(call_description);                                   \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
    } while (0)


const std::string REGISTER_PATH = "/api/v1/tema/auth/register";
const std::string LOGIN_PATH = "/api/v1/tema/auth/login";
const std::string LOGOUT_PATH = "/api/v1/tema/auth/logout";
const std::string ACCESS_PATH = "/api/v1/tema/library/access";
const std::string BOOKS_PATH = "/api/v1/tema/library/books";

const std::string JSON_TYPE = "application/json";

const std::string SERVER_IP = "34.246.184.49";
const std::string SET_COOKIE = "Set-Cookie: ";


#endif /* UTILS_H */
