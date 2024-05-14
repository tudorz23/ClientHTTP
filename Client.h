#ifndef CLIENT_H
#define CLIENT_H

#include <string>


class Client {
 private:
    int sockfd;
    std::string cookies;
    std::string jwt;

    /**
     * Parses the response from the server and extracts the
     * return code and the message associated with it.
     */
    std::string get_ret_code(const std::string &response);


    /**
     * Parses the response from the server and extracts the error
     * message that was received in JSON format.
     * Should only be called when the return code
     * (obtained with get_ret_code()) indicates failure.
     * @return Error message as std::string.
     */
    std::string get_error_message(const std::string &response);


    void manage_register();


    void manage_login();


    void manage_logout();


    void manage_enter_library();


    void manage_add_book();


    void manage_get_books();


    void manage_get_book();


    void manage_delete_book();


 public:

    /**
     * The main control function for the client. It opens a connection
     * with the server, then receives input from stdin with a command,
     * calling the driver method for the respective command, then closes
     * the connection. The loop ends when "exit" command is encountered.
     */
    void run();
};


#endif /* CLIENT_H */
