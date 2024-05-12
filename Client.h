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


    std::string get_ret_code(const std::string &response);


    std::string get_error_message(const std::string &response);

 public:
    void run();


    void manage_register();


    void manage_login();


    void manage_logout();


    void manage_enter_library();


    void manage_add_book();


    void manage_get_books();


    void manage_get_book();
};


#endif /* CLIENT_H */
