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
            manage_get_books();
        } else if (stdin_data == "get_book") {
            manage_get_book();
        } else if (stdin_data == "add_book") {
            manage_add_book();
        } else if (stdin_data == "delete_book") {
            manage_delete_book();
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


string Client::get_ret_code(const string &response) {
    // Parse the response.
    size_t ret_code_begin = response.find("HTTP/1.1 ") + strlen("HTTP/1.1 ");

    string all_from_code = response.substr(ret_code_begin);
    string code = all_from_code.substr(0, all_from_code.find("\r\n"));

    return code;
}


string Client::get_error_message(const string &response) {
    size_t json_start_pos = response.find('{');

    if (json_start_pos != string::npos) {
        string json_response = response.substr(json_start_pos);
        json error_mapping = json::parse(json_response);
        string error_msg = error_mapping.at("error");

        return error_msg;
    }

    // Should never be reached.
    return "";
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
    string code = get_ret_code(response);

    if (code != "201 Created") {
        // An error was received.
        string error_message = get_error_message(response);

        cout << "[" << code << "] ERROR: " << error_message << "\n";
        return;
    }

    cout << "[" << code << "] SUCCESS: User " << username << " successfully registered.\n";
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
    string code = get_ret_code(response);

    if (code != "200 OK") {
        // An error was received.
        string error_message = get_error_message(response);

        cout << "[" << code << "] ERROR: " << error_message << "\n";
        return;
    }

    // There is no error, so get the cookie from the server response.
    size_t cookie_start_pos = response.find(SET_COOKIE) + SET_COOKIE.length();

    string all_cookies = response.substr(cookie_start_pos);
    cookies = all_cookies.substr(0, all_cookies.find(';'));

    cout << "[" << code << "] SUCCESS: User " << username << " successfully logged-in.\n";
}


void Client::manage_logout() {
    string request = compute_get_delete_request(GET, SERVER_IP, LOGOUT_PATH,
                                         empty_string, cookies, empty_string);
    send_to_server(sockfd, request);

    string response = receive_from_server(sockfd);
    string code = get_ret_code(response);

    if (code != "200 OK") {
        // An error was received.
        string error_message = get_error_message(response);

        cout << "[" << code << "] ERROR: " << error_message << "\n";
        return;
    }

    // No error, so set the user as logged out.
    cookies.clear();
    jwt.clear();
    cout << "[" << code << "] SUCCESS: User successfully logged out.\n";
}


void Client::manage_enter_library() {
    if (!jwt.empty()) {
        cout << "ERROR: User already has access to the library.\n";
        return;
    }

    string request = compute_get_delete_request(GET, SERVER_IP, ACCESS_PATH,
                                         empty_string, cookies, empty_string);
    send_to_server(sockfd, request);

    string response = receive_from_server(sockfd);
    string code = get_ret_code(response);

    if (code != "200 OK") {
        // An error was received.
        string error_message = get_error_message(response);

        cout << "[" << code << "] ERROR: " << error_message << "\n";
        return;
    }

    // No error, parse the received JSON.
    size_t json_start_pos = response.find('{');
    if (json_start_pos == string::npos) {
        cout << "ERROR: No JSON object received.\n";
        return;
    }

    string json_str_response = response.substr(json_start_pos);
    json obj_json = json::parse(json_str_response);

    if (!obj_json.contains("token")) {
        cout << "ERROR: No token received.\n";
        return;
    }

    jwt = obj_json.at("token");
    cout << "[" << code << "] SUCCESS: User has access to the library.\n";
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
    string code = get_ret_code(response);

    // Check if there is an error.
    if (code != "200 OK") {
        // An error was received.
        string error_message = get_error_message(response);

        cout << "[" << code << "] ERROR: " << error_message << "\n";
        return;
    }

    // No error, print suggestive message.
    cout << "[" << code << "] SUCCESS: Book <" << title
         << "> successfully added to the library.\n";
}


void Client::manage_get_books() {
    if (cookies.empty()) {
        cout << "ERROR: No user logged in.\n";
        return;
    }

    if (jwt.empty()) {
        cout << "ERROR: User does not have access to the library.\n";
        return;
    }

    // Complete GET request.
    string request = compute_get_delete_request(GET, SERVER_IP, BOOKS_PATH,
                                                empty_string, cookies, jwt);
    send_to_server(sockfd, request);

    string response = receive_from_server(sockfd);

    // Parse the response.
    size_t ret_code_begin = response.find("HTTP/1.1 ") + strlen("HTTP/1.1 ");

    string from_code = response.substr(ret_code_begin);
    string code = from_code.substr(0, from_code.find("\r\n"));

    if (code != "200 OK") {
        // An error message was received.
        size_t json_start_pos = response.find('{');
        if (json_start_pos != string::npos) {
            string json_response = response.substr(json_start_pos);
            json error_mapping = json::parse(json_response);
            string error_msg = error_mapping.at("error");

            cout << "[" << code << "] ERROR: " << error_msg << "\n";
        }
        return;
    }

    // No error, get the books.
    size_t json_array_start = response.find('[');
    if (json_array_start == string::npos) {
        cout << "ERROR: No array of JSON objects received.\n";
        return;
    }

    string json_str_array = response.substr(json_array_start);
    json book_array = json::parse(json_str_array);

    cout << "[" << code << "] SUCCESS: Books are:\n" << book_array.dump(2) << "\n";
}


void Client::manage_get_book() {
    if (cookies.empty()) {
        cout << "ERROR: No user logged in.\n";
        return;
    }

    if (jwt.empty()) {
        cout << "ERROR: User does not have access to the library.\n";
        return;
    }

    string id;

    cout << "id=";
    getline(cin, id, '\n');
    if (!is_number(id) && (id != "0")) {
        cout << "ERROR: id must be a positive integer or zero.\n";
        return;
    }

    string specific_path = BOOKS_PATH + "/" + id;

    // Complete GET request.
    string request = compute_get_delete_request(GET, SERVER_IP, specific_path,
                                                empty_string, cookies, jwt);
    send_to_server(sockfd, request);

    string response = receive_from_server(sockfd);
    string code = get_ret_code(response);

    if (code != "200 OK") {
        // An error was received.
        string error_message = get_error_message(response);

        cout << "[" << code << "] ERROR: " << error_message << "\n";
        return;
    }

    // No error, get the book details.
    size_t json_start = response.find('{');
    if (json_start == string::npos) {
        cout << "ERROR: No JSON object received.\n";
        return;
    }

    string json_str = response.substr(json_start);
    json book_details = json::parse(json_str);

    cout << "[" << code << "] SUCCESS: Book details:\n" << book_details.dump(2) << "\n";
}


void Client::manage_delete_book() {
    if (cookies.empty()) {
        cout << "ERROR: No user logged in.\n";
        return;
    }

    if (jwt.empty()) {
        cout << "ERROR: User does not have access to the library.\n";
        return;
    }

    string id;

    cout << "id=";
    getline(cin, id, '\n');
    if (!is_number(id) && (id != "0")) {
        cout << "ERROR: id must be a positive integer or zero.\n";
        return;
    }

    string specific_path = BOOKS_PATH + "/" + id;

    // Complete DELETE request.
    string request = compute_get_delete_request(DELETE, SERVER_IP, specific_path,
                                                empty_string, cookies, jwt);
    send_to_server(sockfd, request);

    string response = receive_from_server(sockfd);
    string code = get_ret_code(response);

    if (code != "200 OK") {
        // An error was received.
        string error_message = get_error_message(response);

        cout << "[" << code << "] ERROR: " << error_message << "\n";
        return;
    }

    // No error, delete successful.
    cout << "[" << code << "] SUCCESS: Book <" << id << "> successfully deleted.\n";
}
