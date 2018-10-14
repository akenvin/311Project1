#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <cstdio>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

string LowerCase(string s){
	for(int i = 0; i < s.length(); i++){
		if(s[i] != ' '){
			s[i] = tolower(s[i]);
		}
	}
	return s;
	
}bool search(string s, string word){
	int j = 0;
	s = LowerCase(s);
	//cout << "search string: " << s << endl;
	for(int i = 0; i < s.length(); i++){
		//cout << s.length() << endl;
			if(s[i] == word[j]){
				if(j==(word.length()-1)){
					if(((i-(j+1) < 0) || (s[i-(j+1)] == ' ')) && (((i+1) == s.length()) || (s[i+1] == ' '))){
						return true;
					}
				}
				j++;
			}
			else{ 
				j =0;
			}
	}
	return false;
}


bool compareStr(string a, string b){
	a = LowerCase(a);
	b = LowerCase(b);
	return a<b;
}

int main(int argc, char **argv)
{
	//ifstream inFile;
	
	if(argc!= 2){
		cout << "Usage ./a.out <input file>" << endl;
		exit(1);
	}
	
	FILE *inFile = fopen(argv[1] , "r");
	
	if(inFile == NULL){
		cout << "Could not open input file" << endl;
		exit(2);
	}
	
	/*vector<string> wholeFile;
	while(!inFile.eof()){
		getline(inFile, line);
		wholeFile.push_back(line);
	}*/
	
	//inFile.close();
	
	//int size = wholeFile.size();
	//for(int i = 0; i < size; i++){
	//	cout << wholeFile[i] << endl;
	//}
	
	int fd[2]; //file descriptors for pipe
	pipe(fd); // create pipe
	pid_t pid = fork();
	char buf[100];
	vector<string> wordS;
		if(pid != 0){ //fork returns 0 if child process therefore this is parent process
			close(fd[0]);// close fd for reading. parent is writing to child
			while(!feof(inFile)){
				fgets(buf, sizeof(buf), inFile);
				write(fd[1], buf, sizeof(buf));
				//cout<< buf << "\n";
			}
			//close(fd[1]); // close the write descriptor
			//cout << "END OF IF!!!!!!!!" << line << "\n";
		}
		else{
			close(fd[1]);// child: reading only, so close the write-descriptor
			/*while(read(fd[0], &buf, 1) > 0){
				newLine += buf;
				if(buf == '\n'){
					cout << "Child received value: " << newLine << endl;
					newLine = "";
				}
			}*/
			while(read(fd[0], buf, 100) >0){
				string str(buf);
				if(search(str,"adam") == true){
					wordS.push_back(str);
				}
			}
			
			//close(fd[0]);// close the read-descriptor
		}
		sort(wordS.begin(), wordS.end(), compareStr);
		for(int i = 0; i <wordS.size();i++){
			cout<< wordS[i] << endl; 
		}
		
		fclose(inFile);
		
	return 0;
}


