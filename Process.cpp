//Implementation for Process class

#include "Process.h"
#include <iostream>
#include <unistd.h>

using namespace std;


bool InBackground(const vector<string> &vect);

void Process::forkshit()
{
	int inputCheck = this->checkCommand(this->inputVector[0]);
	int pid;
	switch(inputCheck)
	{
		case CD:

			changeDir();
			break;


		case LS:
			if(!this->isBackground())	//not in Background
			{
				pid = fork();
				if(pid == 0)
				{			
						listDir();

				}
				else					
					waitpid(pid, NULL, 0);
			}
			else	//in background
				cout << "BITCH";
			break;


		case WAIT:
			if(!this-> isBackground())
			{
				pid = fork();
				if(pid == 0)
				{	
					wait();
				}
				else
					waitpid(pid, NULL, 0);
			}
			break;


		case RM:
			if(!this->isBackground())
			{
				pid = fork();
				if(pid == 0)
				{
					remove();
				}
				else
					waitpid(pid, NULL, 0);

			}
			break;


		case X:
			if(!this->isBackground())
			{
				pid = fork();
				if(pid == 0)
				{
					xterm();
				}
				else
					waitpid(pid, NULL, 0);
			}

			break;


		case CP:
			if(!this->isBackground())
			{
				cout << "copy" << endl;
			}
			break;


		case PWD:
			if(!this->isBackground())
			{		
				presentDir();
			}

			// char BUF[MAXPATHLEN];
			// getcwd(BUF, MAXPATHLEN);
			// cout << BUF << endl;
			break;
	
	}
	cout <<" done" << endl;
}

bool InBackground(const vector<string> &vect)
{
	for(int i = 0; i < vect.size(); i++)
	{
		cout << vect[i];
	}
}


int Process::checkCommand(string input)
{
	if(input == "cd")
		return CD;
	else if(input == "ls")
		return LS;
	else if(input == "wait")
		return WAIT;
	else if (input == "rm")
		return RM;
	else if (input == "xterm")
		return X;
	else if (input == "cp")
		return CP;
	else if (input == "pwd")
		return PWD;
}

void Process::changeDir()
{
	// arg_count = 0;
	// for (int x = 0; x < this->inputVector.size(); x++)
 //    {
 //        this->exec_args[arg_count++] = strdup(this->inputVector[x].c_str());
 //    }
 //    this->exec_args[arg_count++] = 0; // tell it when to stop!

 //    execv("/bin/cd", this->exec_args);
	// cout << "Hello" << endl;	
	chdir(this->inputVector[1].c_str());

}


void Process::listDir()
{
	arg_count = 0;
	for (int x = 0; x < this->inputVector.size(); x++) 
	{
	   this->exec_args[arg_count++] = strdup(this->inputVector[x].c_str());
	}
	this->exec_args[arg_count++] = 0; // tell it when to stop!

	execv("/bin/ls", exec_args);
	cout << "Hello" << endl;
}

void Process::wait()
{

	cout << "wait" << endl;
	if(isInt(inputVector[1]))
		cout << atoi(this->inputVector[1].c_str()) << endl;
	else
		cout << "ENTER NUMBER" << endl;

}

void Process::remove()
{
	arg_count = 0;
    for (int x = 0; x < this->inputVector.size(); x++)
    {
       this->exec_args[arg_count++] = strdup(this->inputVector[x].c_str());
    }
    this->exec_args[arg_count++] = 0; // tell it when to stop!

    execv("/bin/rm", exec_args);

}

bool Process::isInt(string input2)
{
	for(int i = 0; i < input2.size(); i++)
	{
		if(isdigit(input2[i]) == false)
			return false;
		
	}	
	return true;
}

void Process::presentDir()
{
	arg_count = 0;
    for (int x = 0; x < this->inputVector.size(); x++)
    {
       this->exec_args[arg_count++] = strdup(inputVector[x].c_str());
    }
    this->exec_args[arg_count++] = 0; // tell it when to stop!

    execv("/bin/pwd", exec_args);
}

bool Process::isBackground()
{
	int last = this->inputVector.size() - 1;
	if(this->inputVector[last] == "&")
		return true;
	else
		return false;	

}

void Process::xterm()
{
	arg_count = 0;
    for (int x = 0; x < this->inputVector.size(); x++)
    {
       this->exec_args[arg_count++] = strdup(this->inputVector[x].c_str());
    }
    this->exec_args[arg_count++] = 0; // tell it when to stop!

    execv("/usr/bin/xterm", exec_args);
}