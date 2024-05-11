#include "Client.h"
#include "helpers.h"
#include "utils.h"
#include "requests.h"

#include <iostream>
#include <arpa/inet.h>

#include "json.hpp"
using json = nlohmann::json;


using namespace std;


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
            manage_enter_library();
        } else if (stdin_data == "get_books") {
            cout << "get books input" << "\n";
        } else if (stdin_data == "get_book") {
            cout << "get book input" << "\n";
        } else if (stdin_data == "add_book") {
            manage_add_book();
        } else if (stdin_data == "delete_book") {
            cout << "delete book input" << "\n";
        } else if (stdin_data == "logout") {
            manage_logout();
        } else if (stdin_data == "exit") {
            close_connection(sockfd);
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
                                      credentials_string, empty_string, empty_string);
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

    cout << "SUCCESS: User " << username << " successfully registered.\n";
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
            {"username", username}
    };

    string credentials_string = credentials.dump();
    string req = compute_post_request(SERVER_IP, LOGIN_PATH, JSON_TYPE,
                                      credentials_string, empty_string, empty_string);
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


void Client::manage_logout() {
    string request = compute_get_request(SERVER_IP, LOGOUT_PATH,
                                         empty_string, cookies, empty_string);
    send_to_server(sockfd, request);

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

    // No error, so set the user as logged out.
    cookies.clear();
    jwt.clear();
    cout << "SUCCESS: User successfully logged out.\n";
}


void Client::manage_enter_library() {
    if (!jwt.empty()) {
        cout << "ERROR: User already has access to the library.\n";
        return;
    }

    string request = compute_get_request(SERVER_IP, ACCESS_PATH,
                                         empty_string, cookies, empty_string);
    send_to_server(sockfd, request);

    string response = receive_from_server(sockfd);

    // Parse the received JSON.
    size_t json_start_pos = response.find('{');
    if (json_start_pos == string::npos) {
        cout << "ERROR: No JSON object received.\n";
        return;
    }

    string json_str_response = response.substr(json_start_pos);
    json obj_json = json::parse(json_str_response);

    if (obj_json.contains("error")) {
        string error_msg = obj_json.at("error");
        cout << "ERROR: " << error_msg << "\n";
        return;
    }

    if (!obj_json.contains("token")) {
        cout << "ERROR: No token received.\n";
        return;
    }

    jwt = obj_json.at("token");
    cout << "SUCCESS: User has access to the library.\n";
}


void Client::manage_add_book() {
    if (cookies.empty()) {
        cout << "ERROR: No user logged in.\n";
        return;
    }

    if (jwt.empty()) {
        cout << "ERROR: User does not have access to the library.\n";
        return;
    }

    string title, author, genre, publisher, page_count;

    cout << "title=";
    getline(cin, title, '\n');
    if (title.empty()) {
        cout << "ERROR: title cannot be empty.\n";
        return;
    }

    cout << "author=";
    getline(cin, author, '\n');

    cout << "genre=";
    getline(cin, genre, '\n');

    cout << "publisher=";
    getline(cin, publisher, '\n');

    cout << "page_count=";
    getline(cin, page_count, '\n');

    if (title.empty() || author.empty() || genre.empty() || publisher.empty()
        || page_count.empty()) {
        cout << "ERROR: Book details cannot be empty.\n";
        return;
    }

    if (!is_number(page_count)) {
        cout << "ERROR: page count must be a positive integer.\n";
        return;
    }

    // Create JSON object and send the POST request.
    json book = {
            {"title", title},
            {"author", author},
            {"genre", genre},
            {"publisher", publisher},
            {"page_count", page_count}
    };

    string book_string = book.dump();
    string request = compute_post_request(SERVER_IP, BOOKS_PATH, JSON_TYPE,
                                          book_string, cookies, jwt);
    send_to_server(sockfd, request);

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

    // No error, print suggestive message.
    cout << "SUCCESS: Book <" << title << "> successfully added to the library.\n";
}
