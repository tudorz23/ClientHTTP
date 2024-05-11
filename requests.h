#ifndef REQUESTS_H
#define REQUESTS_H

#include <vector>
#include <string>


std::string compute_post_request(const std::string &host, const std::string &url,
                                 const std::string &content_type, std::string &body_data,
                                 std::string &cookies, std::string &token);


std::string compute_get_request(const std::string &host, const std::string &url,
                                std::string &query_params, std::string &cookies,
                                std::string &token);


#endif /* REQUESTS_H */
