# 471_Project3
Develop a Web Server capable of processing HTTP request messages

Notes for Code Release:

I have included the same html and jpeg files that are in included in: ~promig3/pub/sampleWebFiles

--------------------------------------------------------------------------------
Functional Requirements:

At a minimum, your submission must do the following to receive full credit.
Basic Requirements:
1. The executable should be called web_server.
2. Your program must be written in C/C++, and must compile and run on Isengard.mines.edu
3. You must provide a verbose mode that provides basic debugging information at each step your program takes. The –v flag should enable the verbose mode.
4. After selecting an available TCP port number to be used to listen for new connections be sure to print the port number selected so the grader knows what port to use for testing.
5. In addition to functionality you will be graded on the quality of the code, including readability, comments and the use of proper programing practices.

Accept and process HTTP 1.0 GET requests:
1. You are not required to process header lines, but they should not create errors (that is, you should gracefully ignore them).
2. Your program must serve only files located in the same directory as the server that match the pattern file[X].html or image[X].jpg. For example file1.html or image3.jpg would be valid. It is important that you do not serve any other files. This is to protect your data, as well as the grader’s. If you don’t filter the filenames a malicious user could find your server and request any file they want.

Respond to valid GET requests with a valid response. All responses should include at least:
1. The status line.
2. The Content-Length header line.
3. The Content-Type

Respond to invalid requests with the appropriate error.
1. If the request is a valid GET request but specifies a file other than those allowed, return a 404.
2. If the request is not a valid GET request, return a 400. Note that the request might be a valid HEAD or POST request, but you don’t have to implement those.

After responding to a request your code should return to the main loop and wait for the next request.
1. You can terminate your program with CTRL-C. It is best practice to catch the CTRL-C (i.e., SIGINT) and close your open socket before exiting. If you don’t close the socket the port number will remain open for a brief period, which makes developing the code a hassle.


Testing the code:
In a web browser:
    "http://isengard.mines.edu:XXXX/file1.html"
    "http://isengard.mines.edu:XXXX/file2.html"
    "http://isengard.mines.edu:XXXX/badname.html"
