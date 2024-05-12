#ifndef REQUESTS_H
#define REQUESTS_H

#include <vector>
#include <string>

const std::string GET = "GET";
const std::string DELETE = "DELETE";


std::string compute_post_request(const std::string &host, const std::string &url,
                                 const std::string &content_type, std::string &body_data,
                                 std::string &cookies, std::string &token);


std::string compute_get_delete_request(const std::string &name, const std::string &host,
                                       const std::string &url, std::string &query_params,
                                       std::string &cookies, std::string &token);


#endif /* REQUESTS_H */
