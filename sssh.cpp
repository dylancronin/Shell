#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <cstring>
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

using namespace std;


void changeDir(const vector<string> &);

int main()
{
	string input;
	int inputCheck, backgroundPID, status, x;
	vector<string> inputVector;
	vector<Background*> backgroundVect;
	char *str, *point;
	char * exec_args[1024];

	do
	{
		//Set variables to null
		inputVector.clear();
		input = "";
		inputCheck = 0;

		//get input
		//waitpid(backgroundPID, &status, WUNTRACED);
		cout << "sssh: ";
		getline(cin,input);
		
		cout << input << endl;

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
				isBackground = true;
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
				cout << "Background" << endl;
				fflush(stdout);
				pid = fork();

				if(pid == 0)
				{
					close(STDIN_FILENO);
					close(STDOUT_FILENO);
					close(STDERR_FILENO);
					x = open("/dev/null", O_RDWR);   // Redirect input, output and stderr to /dev/null
					dup(x);
					dup(x);
					//execvp(args[0], args);       // Execute background process - WILL GO INTO ZOMBIE/DEFUNCT STATE -- 
					//exit(1);
					backgroundPID = getpid();
					setpgid(0,0);
					execvp(exec_args[0], exec_args);
					perror("execvp");
					_exit(1);

				}
				else
				{
					Background *p = new Background(input);
					backgroundVect.push_back(p);

				}


			}
		}
				
	}while(inputVector[0] != "exit");//exit on input of "exit"


	for(int i = 0; i < backgroundVect.size(); i++)
	{	
		//cout << backgroundVect[i]->getPID() << endl;
		cout << "Command: " << backgroundVect[i]->getCMD() << endl;
	}

	printf("HERE");
	fflush(stdout);
	return 0;
}

void changeDir(const vector<string> &inputVector)
{

	chdir(inputVector[1].c_str());

}
