#ifndef REQUESTS_H
#define REQUESTS_H


#include <vector>
#include <string>


// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *old_compute_get_request(char *host, char *url, char *query_params,
							char **cookies, int cookies_count);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *old_compute_post_request(char *host, char *url, char* content_type, char **body_data,
							int body_data_fields_count, char** cookies, int cookies_count);


std::string compute_post_request(const std::string &host, const std::string &url,
                                 const std::string &content_type, std::string &body_data,
                                 std::string &cookies);


std::string compute_get_request(const std::string &host, const std::string &url,
                                std::string &query_params, std::string &cookies);


#endif /* REQUESTS_H */
