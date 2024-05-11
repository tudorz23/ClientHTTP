#ifndef CLIENT_H
#define CLIENT_H


#include <vector>
#include <string>



class Client {
 private:
    int sockfd;
    std::string cookies;
    std::string jwt;

    std::string empty_string;


 public:
    Client();

    void run();


    void manage_register();


    void manage_login();
};


#endif /* CLIENT_H */
