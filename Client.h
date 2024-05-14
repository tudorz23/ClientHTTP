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


    /**
     * Gets stdin input corresponding to a new user and sends
     * a POST request to the server, asking to register him.
     */
    void manage_register();


    /**
     * Gets stdin input with user credentials and sends a POST request
     * to the server, asking to log the user in. Updates the session
     * cookies in case of success.
     */
    void manage_login();


    /**
     * Sends a GET request to the server, asking to log out the current user
     * Clears the cookies and the jwt in case of success.
     */
    void manage_logout();


    /**
     * Sends a GET request to the server, asking for access to the library
     * for the currently logged in user. Updates the jwt in case of success.
     */
    void manage_enter_library();


    /**
     * Gets stdin input containing details of a new book. Sends POST request
     * to the server, asking to add the new book to the library.
     */
    void manage_add_book();


    /**
     * Sends a GET request to the server, asking for a JSON object containing
     * all books that the currently logged in user has in the library.
     */
    void manage_get_books();


    /**
     * Gets stdin input containing the id of a book and sends GET request
     * to the server, asking for the details associated with the book.
     */
    void manage_get_book();


    /**
     * Gets stdin input containing the id of a book and sends DELETE request
     * to the server, asking for removal of the book from the library of the
     * currently logged in user.
     */
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
