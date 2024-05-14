#ifndef REQUESTS_H
#define REQUESTS_H

#include <string>

const std::string GET = "GET";
const std::string DELETE = "DELETE";


/**
 * Creates a valid POST request with the given parameters,
 * following the HTTP protocol format.
 */
std::string compute_post_request(const std::string &host, const std::string &url,
                                 const std::string &content_type, const std::string &body_data,
                                 const std::string &cookies, const std::string &token);


/**
 * Creates a valid GET or DELETE request with the given parameters,
 * following the HTTP protocol.
 * If name is "GET", a GET request is created.
 * If name is "DELETE", a DELETE request is created.
 */
std::string compute_get_delete_request(const std::string &name, const std::string &host,
                                       const std::string &url, const std::string &query_params,
                                       const std::string &cookies, const std::string &token);


#endif /* REQUESTS_H */
