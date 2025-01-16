# WebServer

## Overview
This project is a lightweight HTTP web server built entirely in C++98, implementing core functionalities to handle web requests. It supports serving both static files and dynamic content, making it a versatile foundation for various web-based applications.

## Features
- **HTTP Methods**: Supports `GET`, `POST`, and `DELETE` requests.
- **Static and Dynamic Content**: Serves static files and processes dynamic content through CGI (Common Gateway Interface).
- **Concurrency**: Handles multiple simultaneous client connections efficiently using `select()`.
- **Standards Compliance**: Fully compatible with C++98.
- **Optimized Performance**: Designed for minimal overhead and efficient resource usage.

## Technologies Used
- **Programming Language**: C++98
- **Networking**: Socket programming
- **Concurrency Handling**: `select()` system call for managing multiple connections.

## Installation
1. **Clone the Repository**:
   ```bash
   git clone <repository_url>
   cd <repository_name>
   ```

2. **Compile the Source Code**:
   Use the provided `Makefile` to compile the project.
   ```bash
   make
   ```

3. **Run the Server**:
   ```bash
   ./webserver
   ```

## Configuration
The server reads its configuration from a file named `server.conf` located in the root directory. The configuration includes settings such as:
- Port number
- Document root
- CGI script directory

### Example `server.conf`:
```plaintext
port=8080
document_root=/var/www/html
cgi_dir=/var/www/cgi-bin
```

## Usage
1. **Start the Server**:
   After running the server, it will listen for incoming HTTP requests on the specified port.

2. **Access via Browser**:
   Navigate to `http://localhost:<port>` in a web browser to view served content.

3. **Test HTTP Methods**:
   Use tools like `curl` to test supported HTTP methods:
   - GET: `curl http://localhost:<port>/file.html`
   - POST: `curl -X POST -d "key=value" http://localhost:<port>/script.cgi`
   - DELETE: `curl -X DELETE http://localhost:<port>/file.html`

## Directory Structure
```
├── src/               # Source code files
├── include/           # Header files
├── cgi-bin/           # CGI scripts
├── www/               # Document root (static files)
├── server.conf        # Configuration file
├── Makefile           # Build script
└── README.md          # Project documentation
```

## Contributing
Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Submit a pull request with a detailed description of your changes.

## License
This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Acknowledgments
Special thanks to all open-source contributors and communities that provided guidance and inspiration for this project.

