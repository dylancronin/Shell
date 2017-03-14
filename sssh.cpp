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
#include "Process.h"

using namespace std;

enum Commands
{
	CD = 1,
	LS,
	WAIT,
	RM,
	X,
	CP,
	PWD
	
};

int checkCommand(string input);
bool isInt(string);

int main()
{
	int inputCheck, numDir;
	struct direct **dirNamesLst;
	string input, temp;
	vector<string> inputVector;
	char *str, *point;
	Process *node = new Process;


	do
	{
		//Set variables to null
		inputVector.clear();
		input = "";
		inputCheck = 0;

		//get input
		cout << "sssh: ";
		getline(cin,input);
		

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
		inputCheck = checkCommand(inputVector[0]);
	
		delete[] str;		//delete dynamically allocated char pointer

		//Use switch to determine which command is being used
		switch(inputCheck)
		{
			case CD:
				chdir(inputVector[1].c_str());
				//cout << "change directory" << endl;
				break;


			case LS:
				cout << "list" << endl;
				if(inputVector[1] == "-l")
					cout << " -l" << endl;
				
				//RIGHT NOW USES DIRECTORY NAME AFTER LS
				//obtain the number of files within the directory
				//scandir is unix API command for file access			
				numDir = scandir(inputVector[1].c_str(), &dirNamesLst, NULL, alphasort);

				if(numDir < 0)			//if number of directories is less than 0
					perror("scandir");	//error
				else				//if no error
				{
					//loop and print file names in order
					for(int i = 0; i < numDir; i++)		
					{
						printf("%s\n", dirNamesLst[i]->d_name);
					
					}
					delete[] dirNamesLst;	
				}
				
				break;


			case WAIT:
				cout << "wait" << endl;
				if(isInt(inputVector[1]))
					cout << atoi(inputVector[1].c_str()) << endl;
				else
					cout << "ENTER NUMBER" << endl;
				break;


			case RM:
				cout << "remove" << endl;
				break;


			case X:
				cout << "xterm" << endl;
				break;


			case CP:
				cout << "copy" << endl;
				break;


			case PWD:
				char BUF[MAXPATHLEN];
				getcwd(BUF, MAXPATHLEN);
				cout << BUF << endl;
				break;
		
		}
				
	}while(inputVector[0] != "exit");//exit on input of "exit"


	return 0;
}

//Function to allocate an enum to each type of input
int checkCommand(string input)
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

//Function to check if input into wait command is an integer
bool isInt(string input2)
{
	for(int i = 0; i < input2.size(); i++)
	{
		if(isdigit(input2[i]) == false)
			return false;
		
	}	
	return true;
}
