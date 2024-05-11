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
    void run();


    void manage_register();


    void manage_login();


    void manage_logout();


    void manage_enter_library();
};


#endif /* CLIENT_H */
