#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
	ifstream inFile;
	
	if(argc!= 2){
		cout << "Usage ./a.out <input file> " << endl;
		exit(1);
	}
	
	inFile.open(argv[1]);
	
	if(inFile.fail()){
		cout << "Could not open input file" << endl;
		exit(2);
	}
	
	string line("");
	vector<string> wholeFile;
	while(!inFile.eof()){
		getline(inFile, line);
		wholeFile.push_back(line);
	}
	
	inFile.close();
	
	int size = wholeFile.size();
	for(int i = 0; i < size; i++){
		cout << wholeFile[i] << endl;
	}
	
	int fd[2]; //file descriptors for pipe
	pipe(fd); // create pipe
	
	if(fork() != 0){ //fork returns 0 if child process therefore this is parent process
		close(fd[0]);// close fd for reading. parent is writing to child
		write(fd[1], &wholeFile[0], wholeFile[0].length()); // write wholeFile line to fd[1] 
        close(fd[1]); // close the write descriptor
	}
	else
    {   
		char buf[30];
        close(fd[1]);// child: reading only, so close the write-descriptor
        read(fd[0], buf, wholeFile[0].length());// now read the data (will block)
        cout << "Child received value:" <<  buf << endl;
        close(fd[0]);// close the read-descriptor
    }
	
	inFile.close();
	return 0;
}


