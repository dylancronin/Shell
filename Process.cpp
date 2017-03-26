//Implementation for Process class

#include "Process.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


using namespace std;


bool InBackground(const vector<string> &vect);

void Process::forkshit()
{
	int pid;
	string bin = "/bin/";
	arg_count = 0;
	bool isBackground;
	for (int x = 0; x < this->inputVector.size(); x++) 
	{
		if(this->inputVector[x] != "&")
			this->exec_args[arg_count++] = strdup(this->inputVector[x].c_str());
		else
			isBackground = true;
	}
	this->exec_args[arg_count++] = 0; 
	
	// if(this->inputVector[0] == "xterm")
	// {
	// 	bin = "/usr/bin/";
	// 	//cout << bin << endl;
	// }
	if (this->inputVector[0] == "exit")
	{
		cout << "EXIT" << endl;
		exit(-1);
	}
	bin += this->inputVector[0];

	// execvp(this->commandString.c_str(), exec_args);

	if(this->inputVector[0] == "cd")
		changeDir();
	else
	{	
		if(!isBackground)	//not background
		{
			pid = fork();
			if(pid == 0)		//child process
			{	
				execvp(exec_args[0], exec_args);
				// execv(bin.c_str(), exec_args);

				cout << "Invalid command." << endl;
				exit(-1);
			}
			else	//parent process
			{
				waitpid(pid, NULL, 0);
				// kill(pid, SIGKILL);

			}
		
		}
		else	//background
		{
			cout << Background << endl;
		}
	}







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
	else
		return OTHER;
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