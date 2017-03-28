#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <cctype>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include "background.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include "Pipes.h"

using namespace std;



void fileProcessing(string name);
void changeDir(const vector<string> &);
void clearData(const vector<Background*> &backVect);
void outputBackground(const vector<Background*> &finishedVect, const vector<Background*> &runningVect);
void piping(const vector<string> &inputVector);
int executePipeCmds(int read, int write, Pipes* p);




string to_string(int i)
{
    stringstream STREAM;
    STREAM << i;
    return STREAM.str();
}

int main()
{
	string input, line, fileName;
	int inputCheck, backgroundPID = 100, status, x, link[2], backgroundCount = 0;
	vector<string> inputVector;
	vector<Background*> backVect, finishedVect, runningVect;
	vector<Background*>::iterator it;
	pid_t result;
	bool isPiping;

	char *str, *point;
	char * exec_args[1024];

	do
	{

		//Set variables to null
		fileName = "";
		inputVector.clear();
		input = "";
		inputCheck = 0;
		isPiping = false;

		cout << "sssh: ";
		while(getline(cin,input))
		{
			if(input == "")
				cout << "sssh: ";
			else
				break;
		}

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
				if(inputVector[x] != "&" && inputVector[x] != "|")
					exec_args[arg_count++] = strdup(inputVector[x].c_str());
				else if(inputVector[x] == "|")
				{
					isPiping = true;
					pid = fork();
					if(pid == 0)
					{
						piping(inputVector);
						_exit(1);
					}
					else
					{
						waitpid(pid, NULL, 0);
					}
					
				}
				else
				{
					backgroundCount++;
					isBackground = true;
				}
			}
			exec_args[arg_count++] = 0; 
		
		

		if(inputVector[0] == "cd") //special case change directory
			changeDir(inputVector);
		else if(inputVector[0] == "exit")	//special case exit
			return 0;
		else if(isPiping)					//special case piping
			;
		else if(inputVector[0] == "wait")
		{
			pid_t result;
			//char c = inputVector[1];
			vector<Background*>::iterator it;
			//int temp = to_string(inputVector[1]);
			//cout << "Char: " << c << endl << "Input: " << inputVector[1] << endl;
			for(it = backVect.begin(); it != backVect.end(); it++)
			{
				if(to_string((*it)->getbgnum()) == inputVector[1])
				{	
					do{
						result = waitpid((*it)->getPID(), &status, WNOHANG);
					}while(result == 0);
					//break;

					fileProcessing((*it)->getFileName());
					backVect.erase(it);
					backgroundCount--;
					break;
					//cout << "HERE" << endl;

				}
			}

		}
		else	//if not cd, exit, nor piping
		{	
			if(!isBackground)	//not background
			{
				pid = fork();
				if(pid == 0)		//child process
				{	
					execvp(exec_args[0], exec_args);
					perror("execvp");
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
					close(link[0]);

					//create file with the pid+command as filename
					fileName = to_string(backgroundPID) + input;
					freopen(fileName.c_str(), "a+", stdout);
					execvp(exec_args[0], exec_args); 
					perror("execvp");
					//cout << endl;     
					_exit(1);

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
				
				//Obtain the status of the running processes
				result = waitpid(backVect[i]->getPID(), &status, WNOHANG);
				//cout << "Result: " << result << "for " << backVect[i]->getCMD() << endl; 


				if(result == 0)	//if still running
				{
					//put onto running list
					runningVect.push_back(backVect[i]);
					
				}
				else if (result == -1)	//if process is in error
				{
					perror("waitpid");
					_exit(1);
					//cout << "PROBLEM" << endl;
				}
				else	//if finished
				{
						if(ifstream(backVect[i]->getFileName().c_str()))	//check if process exists
						{
							finishedVect.push_back(backVect[i]);	//add to finished vector
							backgroundCount--;						//decrement background num

							//iterate through vector and find what to delete
							for(it = backVect.begin(); it != backVect.end(); it++)
							{
								if((*it)->getFileName() == backVect[i]->getFileName())
								{	
									// cout << "Iterator: " << (*it) -> getCMD() << endl;
									// cout << "Vecotr: " << backVect[i] -> getCMD() << endl;
									break;
								}
							}

							//cout << "Removing: " << (*it) -> getCMD() << endl;

							//erase the finished process from vector
							fileProcessing(backVect[i]->getFileName());
							backVect.erase(it);

						}
						else
						{
							cout << "Command not found." << endl;
							_exit(1);
						}

				}//if not running or error
			}//for loop

			//call output function to print out finished and running processes
			outputBackground(finishedVect, runningVect);
			finishedVect.clear();
			runningVect.clear();
		}
		cout << endl;
						
	}while(true);//exit on input of "exit"

	clearData(backVect);

	//printf("HERE");
	//fflush(stdout);
	return 0;
}

void changeDir(const vector<string> &inputVector)
{

	chdir(inputVector[1].c_str());

}

/******************************************
Purpose: To output what is in each
background process's file.
******************************************/
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


/**************************************************
Purpose: Delete any files that were created during
execution.from background processes.
**************************************************/
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


/**************************************************8
Purpose: To create a function that outputs the
finished and running background processes.
***************************************************/
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


/****************************************************
Purpose: Use this function when the user input 
indicates use of | character to pipe from one 
command to another.
****************************************************/
void piping(const vector<string> &inputVector)
{
	char * exec_args[1024];
	int arg_count = 0;
	int exec_count = 0;
	vector<vector<string> > commandsVect;
	vector<string> singleArg;
	vector<Pipes*> pVect;
	string temp;
	int index = 0;
	int link[2];
	int read = 0;

	// Output the vector that contains all the strings
	// cout << "Input vector: " << endl;
	// for(int i = 0; i < inputVector.size(); i++)
	// {
	// 	cout << inputVector[i] << endl;
	// }


	//Within the input, check for | characters
	for(int i = 0; i < inputVector.size(); i++)
	{
		if(inputVector[i] != "|") //if there is not a bar add to vector
		{
			singleArg.push_back(inputVector[i]);
			//commandsVect.push_back(singleArg);
			if(i == inputVector.size() - 1) //resolve for not having a bar at the end of input
			{
				commandsVect.push_back(singleArg);
				singleArg.clear();
			}
		}
		else	//if there is a bar, skip the bar and push to commandsVect	
		{
			commandsVect.push_back(singleArg);
			singleArg.clear(); //clear the singleArg vector to start obtaining next command
		}
	}

	//Place the input vector's strings into char* arrays
	for(int i = 0; i < commandsVect.size(); i++)
	{			
			arg_count = 0;
			for (int x = 0; x < commandsVect[i].size(); x++) 
			{
				//place commands into vector
				exec_args[arg_count++] = strdup(commandsVect[i][x].c_str());

			}
			exec_args[arg_count++] = 0; 

			//Create a new pipe object 
			Pipes* n = new Pipes(exec_args, arg_count);
			//put the object onto a stack
			pVect.push_back(n);

	}


	int i = 0;
	for(i = 0; i < pVect.size() - 1; i++)
	{
		//create a pipe 
		pipe(link);
		//call command to execute the pipes
		executePipeCmds(read, link[1], pVect[i]);
		//close pipes
		close(link[1]);
		//update write
		read = link[0];
	}

	//take output of last execution for input of last command
	if(read != 0)
		dup2(read,0);


	//cout << pVect[i] -> getElement(0) << endl;
	//execute the last command
	execvp(pVect[i]->getElement(0), pVect[i] -> exec_args);

}


/*******************************************************
Purpose: Fork and execute each command.
*******************************************************/
int executePipeCmds(int read, int write, Pipes* p)
{
	int pid;
	pid = fork();

	if(pid == 0)
	{
		//check read pipe
		if(read != 0)
		{
			dup2(read, 0);
			close(read);
		}
		//check write pipe
		if(write != 1)
		{
			dup2(write,1);
			close(write);
		}

		//execute command
		execvp(p->getElement(0), p->exec_args);
	}

	return pid;
}