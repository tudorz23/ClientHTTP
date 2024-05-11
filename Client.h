#ifndef CLIENT_H
#define CLIENT_H


#include <vector>
#include <string>



class Client {
 private:
    int sockfd;
    std::vector<std::string> cookies;

    std::string empty_string;
    std::vector<std::string> empty_vector;


 public:
    Client();

    void run();


    void manage_register();
};


#endif /* CLIENT_H */
