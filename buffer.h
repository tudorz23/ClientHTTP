#ifndef BUFFER_H
#define BUFFER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>


typedef struct {
    char *data;
    size_t size;
} buffer;

// initializes a buffer
buffer buffer_init();

// destroys a buffer
void buffer_destroy(buffer *buffer);

// adds data of size data_size to a buffer
void buffer_add(buffer *buffer, const char *data, size_t data_size);

// checks if a buffer is empty
int buffer_is_empty(buffer *buffer);

// finds data of size data_size in a buffer and returns its position
int buffer_find(buffer *buffer, const char *data, size_t data_size);

// finds data of size data_size in a buffer in a
// case-insensitive fashion and returns its position
int buffer_find_insensitive(buffer *buffer, const char *data, size_t data_size);


#endif /* BUFFER_H */
