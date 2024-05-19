*Designed by Marius-Tudor Zaharia, 323CA, May 2024*

# ClientHTTP

---

## 1. What is ClientHTTP?
* ClientHTTP is a basic implementation of an HTTP client that interacts with a
REST API, exposed by a server created by the PCOM team.
* The client is an application that gets input from the STDIN and sends
requests to the server, the topic being a virtual library, that allows users
to log in and add and delete books.

---

## 2. File distribution
* The implementation is based on the skeleton provided in the 9th laboratory.
* Thus, the `buffer.cpp` and `buffer.h` are unmodified.
* Various helper functions are written in `helpers.cpp` and `helpers.h`. Some
of them are taken from the laboratory and modified to be better integrated in
the C++ project (i.e. they now return and accept as parameter `std::string`
instead of `char*`), while others are new (`contains_space()` and
`is_number()`).
* The functions from `requests.cpp` and `requests.h` follow the steps stated
in the lab skeleton, but are implemented "fully" in C++ (i.e. with `string`
operations instead of `mallocs` and `char**`).
* The client is described as a class, placed in `Client.h`, while the class
methods are implemented in `Client.cpp`.
* The driver code, containing the `main()` function, is written in
`client_main.cpp`.
* `DIE` macro and other constants are placed in `utils.h`.
* The `json.hpp` is a C++ JSON management library taken from `nlohmann`.

---

## 3. Running
* The client can be compiled by using the `make` command in the root directory.
* To run the client, the command `./client` should be used.

---

## 4. General details and design choices
* The app is written in C++ mainly for the ability to use `std::string`, as a
simpler alternative to C style `char*` and manual memory allocation.
* I chose to use a class for the client, thus being able to access the class
attributes from anywhere inside the class. Without the class, global variables
should have been used, or the variables should have been initialized in the
`main()` function and then repeatedly passed as parameters to all the
functions that needed them.
* Some sort of global variables are still used in the `utils.h` header.
However, they are constants for the paths to the server's file system. I tried
to use `#define` directives, but encountered problems with the functions from
`requests.h`, that treat their parameters as `std::string`, so I believe this
solution is the best (it also resembles Java static constants).
* The `EMPTY_STRING` constant is used because the functions from `requests.h`
accept references to strings, so `""` could not be passed as a parameter, as it
was not bound to a variable. I think it is better to use this constant than
not marking the parameters as references, because the cookies and the tokens
might be long strings, so it would be inefficient to always copy them as
parameters.
* I chose to use the `nlohmann` JSON-parsing library because it was the one
recommended in the assignment statement for C++. Also, its documentation is
really thorough and the methods are easy to use (and they "get the job done").

---

## 5. Program flow and logic
* The client receives commands from the user through STDIN and calls the
respective functions to manage the requests of the user.
* The user must firstly `register` in the server's database, by providing a
username and a password.
* Then, the user must `login` with the credentials used to register. The client
will send the request to the server and will receive a `session cookie`, used
for all the following commands while the user is still logged-in.
* The user should `enter_library` to gain access to his library. The server
will respond with a `jwt token`, that will also be used to prove the access to
the library for the following commands.
* The user can use the command `get_books` to see sneak-peeks of all his books,
or `get_book` with an id to see details about a certain book. He can also add
new books with `add_book`, or delete a certain book with `delete_book`.
* The user then uses `logout`, and the client must restrict the use of the
library commands until a user is again logged-in.
* When encountering the `exit` command, the client breaks the `while(true)`
loop and ends its execution.
* For every command, the validity of the user input is checked before sending
the requests to the server. The responses from the server are also checked.

---

## 6. Punctual implementation details
* As I understood from the forum, the server closes the TCP connection with the
client after a few requests, so the easiest way to handle this problem was for
the client to open the connection before every command and close it after the
command execution.
* After login, the server sends a response that contains 3 cookies, the first
one being the one personalized for the session. For some reason, only the first
cookie should be used in future requests to the server, else it does not work.
* To check whether a user is logged in or not, the `cookies` attribute is
checked. If it is empty, no user is currently logged in.
* Similarly, to control the access to the library, the `jwt` attribute is
checked. If it is empty, then the user does not have access to the library.
* At logout, the `cookies` and `jwt` attributes of the class are cleared, to
mark the logout, but also the exit from the library.
* To parse the errors more elegantly, the `get_ret_code()` method is used, that
extracts the return code and the message from a server response.
* In case the return code indicates a failure, the `get_error_message()`
method is used to extract the error message sent by the server in JSON format.
* The return codes and messages are always displayed, even at success, followed
by personalized messages for each command.

---

## 7. Final thoughts
* It is worth noting that all the tests from the checker work fine on my
machine (by that, I mean they display the right message, i.e. the commands that
are invalid and are supposed to fail do fail, and the valid ones do succeed).
Also, I manually tested all the situations I could think of using `valgrind`,
and there are no errors or memory leaks.
* I found this homework to be interesting and different from other assignments
from before, because here we are communicating with a real server that expects
a certain format for the messages.

---

## 8. Bibliography
* The 9th laboratory: https://pcom.pages.upb.ro/labs/lab9/http.html
* The JSON library: https://github.com/nlohmann/json
