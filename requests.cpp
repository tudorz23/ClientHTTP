#include "requests.h"
#include "helpers.h"

using namespace std;


string compute_post_request(const string &host, const string &url,
                            const string &content_type, const string &body_data,
                            const string &cookies, const string &token) {
    string message, line;

    // Add method name, URL and protocol type.
    line.append("POST " + url + " HTTP/1.1");
    compute_string_message(message, line);
    line.clear();

    // Add host.
    line.append("Host: " + host);
    compute_string_message(message, line);
    line.clear();

    // Add Content-Type and Content-Length.
    line.append("Content-Type: " + content_type);
    compute_string_message(message, line);
    line.clear();

    line.append("Content-Length: " + to_string(body_data.length()));
    compute_string_message(message, line);
    line.clear();

    // Add cookies (if present).
    if (!cookies.empty()) {
        line.append("Cookie: " + cookies);
        compute_string_message(message, line);
        line.clear();
    }

    // Add token (if present).
    if (!token.empty()) {
        line.append("Authorization: Bearer " + token);
        compute_string_message(message, line);
        line.clear();
    }

    // Add new line at the end of the headers.
    compute_string_message(message, "");

    // Add the actual data.
    compute_string_message(message, body_data);

    return message;
}


string compute_get_delete_request(const string &name, const string &host,
                                  const string &url, const string &query_params,
                                  const string &cookies, const string &token) {
    string message, line;

    // Add method name, URL and protocol type.
    line.append(name + " " + url);
    if (!query_params.empty()) {
        line.append("?" + query_params);
    }
    line.append(" HTTP/1.1");
    compute_string_message(message, line);
    line.clear();

    // Add host.
    line.append("Host: " + host);
    compute_string_message(message, line);
    line.clear();

    // Add cookies (if present).
    if (!cookies.empty()) {
        line.append("Cookie: " + cookies);
        compute_string_message(message, line);
        line.clear();
    }

    // Add token (if present).
    if (!token.empty()) {
        line.append("Authorization: Bearer " + token);
        compute_string_message(message, line);
        line.clear();
    }

    // Add new line at the end of the headers.
    compute_string_message(message, "");

    return message;
}
