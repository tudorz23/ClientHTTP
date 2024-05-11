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
            manage_login();
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

        close_connection(sockfd);
    }
}


void Client::manage_register() {
    string username, password;

    cout << "username=";
    getline(cin, username, '\n');

    cout << "password=";
    getline(cin, password, '\n');

    if (contains_space(username) || contains_space(password)) {
        cout << "Username and password cannot contain spaces. Try again.\n";;
        return;
    }

    // Create JSON object and send the POST request.
    json credentials = {
            {"password", password},
            {"username", username},
    };

    string credentials_string = credentials.dump();
    string req = compute_post_request(SERVER_IP, REGISTER_PATH, JSON_TYPE,
                                      credentials_string, empty_string);
    send_to_server(sockfd, req);

    string response = receive_from_server(sockfd);

    size_t json_start_pos = response.find('{');
    if (json_start_pos == string::npos) {
        cout << "SUCCESS: User " << username << " successfully registered.\n";
        return;
    }

    string json_response = response.substr(json_start_pos);
    json error_mapping = json::parse(json_response);
    string error_msg = error_mapping.at("error");
    cout << "ERROR: " << error_msg << "\n";
}


void Client::manage_login() {
    if (!cookies.empty()) {
        cout << "ERROR: There is already a logged-in user.\n";
        return;
    }

    string username, password;

    cout << "username=";
    getline(cin, username, '\n');

    cout << "password=";
    getline(cin, password, '\n');

    if (contains_space(username) || contains_space(password)) {
        cout << "Username and password cannot contain spaces. Try again.\n";;
        return;
    }

    // Create JSON object and send the POST request.
    json credentials = {
            {"password", password},
            {"username", username},
    };

    string credentials_string = credentials.dump();
    string req = compute_post_request(SERVER_IP, LOGIN_PATH, JSON_TYPE,
                                      credentials_string, empty_string);
    send_to_server(sockfd, req);

    string response = receive_from_server(sockfd);

    // Check if there is an error.
    size_t json_start_pos = response.find('{');
    if (json_start_pos != string::npos) {
        string json_response = response.substr(json_start_pos);
        json error_mapping = json::parse(json_response);
        string error_msg = error_mapping.at("error");

        cout << "ERROR: " << error_msg << "\n";
        return;
    }

    // There is no error, so get the cookie from the server response.
    size_t cookie_start_pos = response.find(SET_COOKIE) + SET_COOKIE.length();

    string all_cookies = response.substr(cookie_start_pos);
    cookies = all_cookies.substr(0, all_cookies.find(';'));

    cout << "SUCCESS: User " << username << " successfully logged-in.\n";
}
