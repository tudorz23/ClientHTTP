#include "Client.h"
#include "helpers.h"
#include "utils.h"
#include "requests.h"

#include <iostream>
#include <arpa/inet.h>

#include "json.hpp"
using json = nlohmann::json;


using namespace std;

Client::Client() {

}


void Client::run() {
    string stdin_data;
    string server_ip(SERVER_IP);

    while (true) {
        sockfd = open_connection(server_ip, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

        getline(cin, stdin_data, '\n');

        if (stdin_data == "register") {
            manage_register();
        } else if (stdin_data == "login") {
            cout << "Login input" << "\n";
        } else if (stdin_data == "enter_library") {
            cout << "Enter lib input" << "\n";
        } else if (stdin_data == "get_books") {
            cout << "get books input" << "\n";
        } else if (stdin_data == "get_book") {
            cout << "get book input" << "\n";
        } else if (stdin_data == "add_book") {
            cout << "add book input" << "\n";
        } else if (stdin_data == "delete_book") {
            cout << "delete book input" << "\n";
        } else if (stdin_data == "logout") {
            cout << "logout input" << "\n";
        } else if (stdin_data == "exit") {
            cout << "exit input" << "\n";
            break;
        } else {
            cout << "Invalid input. Please try again.\n";
        }
    }
}


void Client::manage_register() {
    string username;
    string password;

    cout << "username=";
    getline(cin, username, '\n');

    cout << "password=";
    getline(cin, password, '\n');

    if (contains_space(username) || contains_space(password)) {
        cout << "Username and password cannot contain spaces. Try again.\n";;
        return;
    }

    json credentials = {
            {"password", password},
            {"username", username},
    };

    string credentials_string = credentials.dump();

    string req = compute_post_request(SERVER_IP, REGISTER_PATH, JSON_TYPE,
                                      credentials_string, empty_vector);

    send_to_server(sockfd, req);

    string response = receive_from_server(sockfd);

    cout << req << "\n";
    cout << response << "\n";
}
