## Basic Http Server
Please ensure that the desired port is available for use.

By default, the server is set to port 8888. If you wish to use a different port, you can modify the port number in the `static/server/config.txt` file and then proceed with the `cmake ..` command.

```bash
mkdir build && cd build
cmake ..
make
./http_srv
```

Additional information will be logged and stored in the `build/log.txt` file for reference.
## Usage
The server provides functionality for 2 basic URLs:

"http://0.0.0.0:8888/"

"http://0.0.0.0:8888/params/{name}"

In addition, I have included a webpage template from W3Schools, which consists of HTML, CSS, and JavaScript files. You can access this example webpage at:

"http://0.0.0.0:8888/resources/index.html"

## ToDo
- Other optimizations
- Refactor code
