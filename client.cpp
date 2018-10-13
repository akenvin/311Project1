/*
 *  client.cpp
 *  v0.0.1
 *  Updated on: May 15, 2017
 *  Updated by: Zheng Xing <zxing@robohome.org>
 */
#include <stdio.h>
#include <cstdlib>
#include <fstream>

#include <cstring>
#include <vector>
#include <memory.h>
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
//	char buf2[100];
	int data_fd;
	int rc;
//	int rc2;

	if ((data_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		cerr << "Error: socket() failed: " << strerror(errno) << endl;
		exit (EXIT_FAILURE);
	}

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;

	if (*socket_path == '\0') {
		*addr.sun_path = '\0';
		strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
	} else {
		strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
	}

	if (connect(data_fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		cerr << "Error: connect() failed: " << strerror(errno) << endl;
		exit (EXIT_FAILURE);
	}
	
	memset(buf, 0, strlen(buf));
	
	ifstream inFile;
//	string stringFind;
	if(argc!= 2){
                cout << "Usage ./a.out <input file> <string>" << endl;
                exit(1);
        }

        inFile.open(argv[1]);
      //  stringFind = argv[2];//string we need to find

        if(inFile.fail()){
                cout << "Could not open input file" << endl;
                exit(2);
        }
	string line("");
	
	

	while (!inFile.eof()) {
		
		getline(inFile, line);
		strcpy(buf, line.c_str());
		rc = sizeof(buf);
		if (write(data_fd, buf, rc) == -1) {
			cerr << "Error: write() failed: " << strerror(errno) << endl;
			exit (EXIT_FAILURE);
		} else {
			cout <<  " data: " << buf << endl;
		}
		
		memset(buf, 0, strlen(buf));
		
		if ((rc = read(data_fd, buf, sizeof(buf))) > 0) {
			cout << " read: " << buf << endl;
		} else if (rc == -1) {
			cerr << "Error: read() failed: " << strerror(errno) << endl;
			exit (EXIT_FAILURE);
		} else if (rc == 0) {
			cout << "read() returned 0" << endl;
			close(data_fd);
		}
		memset(buf, 0, strlen(buf));
	}
	inFile.close();
	return 0;
}
