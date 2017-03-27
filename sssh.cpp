#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
//#include <cstring>
#include <ctype.h>
#include <stdlib.h>
#include <cctype>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/dir.h>
#include "background.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <iomanip>

using namespace std;



void fileProcessing(string name);
char pipeBUF[4096];
vector<string> ProcVect;
void changeDir(const vector<string> &);


string to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

int main()
{
	// vector<Background*> backgroundVect;
	pipeBUF[0] = 'a';
	//fstream infile("/dev/null");
	string input, line, fileName;
	int inputCheck, backgroundPID = 100, status, x, link[2], backgroundCount = 0;
	vector<string> inputVector, finished;
	vector<Background*> backVect;

	char *str, *point;
	char * exec_args[1024];
	//char *pipeBUF;

	do
	{
		//Set variables to null
		fileName = "";
		inputVector.clear();
		input = "";
		inputCheck = 0;

		cout << "sssh: ";
		getline(cin,input);
		cout << "Vect size: " << ProcVect.size() << endl;

		


		if(backVect.size() > 0)
		{
			for(int i = 0; i < backVect.size(); i++)
			{
				pid_t result = waitpid(backVect[0]->getPID(), &status, WNOHANG);
				if(result == 0)
				{
					cout << "ALIVE" << endl;
				}
				else if (result == -1)
					cout << "PROBLEM" << endl;
				else
					}cout << "DEAD" << endl;
			}
		}


		if(ProcVect.size() > 0)
		{
			for(int i = 0; i < ProcVect.size(); i++)
			{
				if(ifstream(ProcVect[i].c_str()))
				{
					fileProcessing(ProcVect[i]);
					ProcVect.erase(ProcVect.begin() + i);
				}
				else
				{
					cout << "Running:" << endl;
					cout << ProcVect[i] << endl;
				}
			}
		}
		
		//cout << input << endl;

		//copy input to char pointer		
		str = new char [input.size()+1];
		strcpy(str, input.c_str());


		//split input string up into tokens
		point = strtok(str, " ");
		while(point != NULL)
		{
			//cout << point << endl;
			inputVector.push_back(point);	//put each part of the command into a vector
			point = strtok(NULL, " ");
			
		}
		//Check the input string
	
		delete[] str;		//delete dynamically allocated char pointer

		//Use switch to determine which command is being used
		int pid;
		//string bin = "/bin/";
		int arg_count = 0;
		bool isBackground = false;
		for (x = 0; x < inputVector.size(); x++) 
		{
			if(inputVector[x] != "&")
				exec_args[arg_count++] = strdup(inputVector[x].c_str());
			else
			{
				backgroundCount++;
				isBackground = true;
			}
		}
		exec_args[arg_count++] = 0; 
		



		// if (inputVector[0] == "exit")
		// {
		// 	cout << "EXIT" << endl;
		// 	exit(-1);
		// }
		if(inputVector[0] == "cd")
			changeDir(inputVector);
		else
		{	
			if(!isBackground)	//not background
			{
				pid = fork();
				if(pid == 0)		//child process
				{	
					execvp(exec_args[0], exec_args);
					// execv(bin.c_str(), exec_args);
					perror("execvp");
					//out << "Invalid command." << endl;
					_exit(1);
				}
				else	//parent process
				{
					waitpid(pid, NULL, 0);
				}
			
			}
			else	//background
			{

				pipe(link);

				//cout << "Background" << endl;
				fflush(stdout);

				pid = fork();

				if(pid == 0)
				{
					backgroundPID = getpid();
					// close(link[1]);
					close(link[0]);
					write(link[1], &backgroundPID, sizeof(backgroundPID));
					//close(link[0]);
					close(link[0]);
					freopen(fileName.c_str(), "a+", stdout);
					//x = open("/dev/null", O_RDWR);   // Redirect input, output and stderr to /dev/null
					//dup(STDOUT_FILENO);
					//dup(x);
					execvp(exec_args[0], exec_args); 
					perror("execvp");      // Execute background process - WILL GO INTO ZOMBIE/DEFUNCT STATE -- 
					_exit(1);
					// dup2(link[1], STDOUT_FILENO);
    	// 			close(link[0]);
					//p->piping();



				}
				else
				{
					//close(link[0]);
					close(link[1]);
					read(link[0], &backgroundPID, sizeof(backgroundPID));
					//close(link[1]);
					close(link[0]);
					//cout << "Child pid: " << backgroundPID << endl;
					
					fileName = to_string(backgroundPID) + input;

					Background *n = new Background(input, backgroundPID, fileName);
					backVect.push_back(n);

					//cout << back


					ProcVect.push_back(fileName);

					close(link[0]);

					//printf("Pipe contents: %s\n", pipeBUF);

					//waitpid();
					//cout << "parent: " << backgroundVect.size() << endl;
					// close(link[1]);
					// read(link[0], pipeBUF, sizeof(pipeBUF));
					// Background *p = new Background(input);
					// backgroundVect.push_back(p);

				}


			}
		}

						
	}while(inputVector[0] != "exit");//exit on input of "exit"


	// for(int i = 0; i < backgroundVect.size(); i++)
	// {	
	// 	//cout << backgroundVect[i]->getPID() << endl;
	// 	cout << "Command: " << backgroundVect[i]->getCMD() << endl;
	// }


	//printf("HERE");
	fflush(stdout);
	return 0;
}

void changeDir(const vector<string> &inputVector)
{

	chdir(inputVector[1].c_str());

}


void fileProcessing(string name)
{	
	int count = 0;
	string line;
	ifstream myFile;
	myFile.open(name.c_str());
	if(myFile.is_open())
	{
		while(getline(myFile, line))
		{
			if(line != name)	
				cout << line << endl;
		}
	}

	myFile.close();

	remove(name.c_str());

}
