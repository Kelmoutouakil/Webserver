## WebServer42
is a custom HTTP server developed as part of the 42 coding school's curriculum. 
This project provides hands-on experience in network programming, HTTP protocol handling, and server management.

## Features
HTTP/1.1 Compliance: Supports essential HTTP methods including GET, POST, PUT, DELETE, and HEAD.
Static and Dynamic Content Serving: Serves static files and executes CGI scripts for dynamic content generation.
Non-Blocking I/O: Utilizes efficient I/O multiplexing to handle multiple client connections simultaneously without blocking.
Configurable Settings: Allows customization through a configuration file, enabling users to define server behavior, root directories, and more.
Error Handling: Returns appropriate error codes and customizable error pages for various client requests.
Directory Listing: Automatically generates a directory listing when a directory is requested, if enabled.
File Uploads: Supports file uploads using the POST method.
Redirections: Can redirect clients to different URLs based on the request.
