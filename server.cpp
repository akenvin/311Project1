/*
 *  server.cpp
 *  v0.0.1
 *  Updated on: May 15, 2017
 *  Updated by: Zheng Xing <zxing@robohome.org>
 */
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <errno.h>

//const char* socket_path = "./socket";
const char* socket_path = "\0hidden";

int main(int argc, char** argv) {
	using namespace std;
	
	char buf[100];
	int connection_fd, data_fd;
	int rc;
	
	string stringFind;
	if(argc!= 2){
		cout << "Usage ./a.out <string>" << endl;
		exit(1);
	}
	stringFind = argv[1];//string we need to find
	
	if ((connection_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		cerr << "Error: socket() failed: " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	
	if (*socket_path == '\0') {
		*addr.sun_path = '\0';
		strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
	} else {
		strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
		unlink(socket_path);
	}
	
	if (bind(connection_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		cerr << "Error: bind() failed: " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	
	// set backlog size to 5
	if (listen(connection_fd, 5) == -1) {
		cerr << "Error: listen() failed: " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
		
//	read(data_fd,buf,sizeof(buf));
//	cout << stringFind << endl;	
	string compStr;
	while (1) {
		if ((data_fd = accept(connection_fd, NULL, NULL)) == -1) {
			cerr << "Error: accept() failed: " << strerror(errno) << endl;
			continue;
		} else {
			cout << "accept() returned " << data_fd << endl;
		}
		
		memset(buf, 0, strlen(buf));

		while ((rc = read(data_fd, buf, sizeof(buf))) > 0) {
			compStr = buf;
			cout << "read " << buf << endl;
			if (rc  == -1) {
				cerr << "Error: write() failed: " << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			} else if(compStr.find(stringFind)  != std::string::npos){
				strcpy(buf, compStr.c_str());
				write(data_fd, buf, rc);
				cout << "wrote " << rc << " Bytes" << " data: " << buf << endl;
				}
			else{
				char buf2[0];
				write(data_fd, buf2, rc);
				cout << "word not found" << endl;
			}	
	}	
			memset(buf, 0, strlen(buf));
		
		if (rc == -1) {
			cerr << "Error: read() failed: " << strerror(errno) << endl;
			exit(EXIT_FAILURE);
		} else if (rc == 0) {
			cout << "read() returned 0" << endl;
			close(data_fd);
		}
	}
	
	close(connection_fd);
	
	unlink(socket_path);
	
	return 0;
}
