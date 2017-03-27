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
void clearData(const vector<Background*> &backVect);
void outputBackground(const vector<Background*> &finishedVect, const vector<Background*> &runningVect);
//void backgroundCheck(const vector<Background*> &backVect, int &backgroundCount);




string to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

int main()
{
	// vector<Background*> backgroundVect;
	//pipeBUF[0] = 'a';
	//fstream infile("/dev/null");
	string input, line, fileName;
	int inputCheck, backgroundPID = 100, status, x, link[2], backgroundCount = 0;
	vector<string> inputVector;
	vector<Background*> backVect, finishedVect, runningVect;
	vector<Background*>::iterator it;
	pid_t result;

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
		if(inputVector[0] == "cd") //special case change directory
			changeDir(inputVector);
		else	//if not cd
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

				pid = fork();

				if(pid == 0)	//child
				{
					backgroundPID = getpid();
					// Pipe the pid to the parent
					close(link[0]);
					write(link[1], &backgroundPID, sizeof(backgroundPID));
					//close(link[0]);
					close(link[0]);
					fileName = to_string(backgroundPID) + input;
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
				else	//parent
				{
					//read the pid of the child from pipe
					close(link[1]);
					read(link[0], &backgroundPID, sizeof(backgroundPID));
					//close(link[1]);
					close(link[0]);
					//cout << "Child pid: " << backgroundPID << endl;
					fileName = to_string(backgroundPID) + input;

					Background *n = new Background(input, backgroundPID, fileName, backgroundCount);
					backVect.push_back(n);

				}//parent


			}//background
		}//not a cd command


		/*********************************************
		Handles the vector associated with background
		processes, and calling the function to output.
		*********************************************/
		if(backVect.size() > 0)
		{
			for(int i = 0; i < backVect.size(); i++)
			{
				result = waitpid(backVect[i]->getPID(), &status, WNOHANG);
				cout << "Result: " << result << "for " << backVect[i]->getCMD() << endl; 

				if(result == 0)
				{
					runningVect.push_back(backVect[i]);
					
				}
				else if (result == -1)
					cout << "PROBLEM" << endl;
				else
				{

				//cout << finishedVect.size();
					// for(int i = 0; i < backVect.size(); i++)
					// {
						// cout << "Finished:" << endl;
						// cout << "[" << backVect[i] -> getbgnum() << "] " << backVect[i]->getCMD() << endl; 
						if(ifstream(backVect[i]->getFileName().c_str()))
						{
							finishedVect.push_back(backVect[i]);
							backgroundCount--;

							for(it = backVect.begin(); it != backVect.end(); it++)
							{
								if((*it)->getFileName() == backVect[i]->getFileName())
								{	
									cout << "Iterator: " << (*it) -> getCMD() << endl;
									cout << "Vecotr: " << backVect[i] -> getCMD() << endl;
									break;
								}
							}

							cout << "Removing: " << (*it) -> getCMD() << endl;

							fileProcessing(backVect[i]->getFileName());
							backVect.erase(it);

						}
						else
							cout << "FILE NOT FOUND" << endl;
					//}

				}//if not running or error
			}//for loop

			outputBackground(finishedVect, runningVect);
			finishedVect.clear();
			runningVect.clear();
		}
						
	}while(inputVector[0] != "exit");//exit on input of "exit"

	clearData(backVect);

	//printf("HERE");
	//fflush(stdout);
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

void clearData(const vector<Background*> &backVect)
{
	if(backVect.size() > 0)
	{
		for(int i = 0; i < backVect.size(); i++)
		{
			remove(backVect[i] -> getFileName().c_str());
		}
	}
}

void outputBackground(const vector<Background*> &finishedVect, const vector<Background*> &runningVect)
{
	if(runningVect.size() > 0)
	{
		cout << "Running:" << endl;
		for(int i = 0; i < runningVect.size(); i++)
		{
			cout << "[" << runningVect[i] -> getbgnum() << "] " << runningVect[i]->getCMD() << endl; 
		}
	}

	if(finishedVect.size() > 0)
	{
		cout << "Finished:" << endl;
		for(int i = 0; i < finishedVect.size(); i++)
		{
			//cout << "Finished:" << endl;
			cout << "[" << finishedVect[i] -> getbgnum() << "] " << finishedVect[i]->getCMD() << endl; 
		}
	}
}

