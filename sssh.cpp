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


int checkCommand(string input);
bool isInt(string);

int main()
{
	string input;
	int inputCheck;
	vector<string> inputVector;
	char *str, *point;

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
	
		delete[] str;		//delete dynamically allocated char pointer

		//Use switch to determine which command is being used
		Process *p = new Process(inputVector);
		p->forkshit();
				
	}while(inputVector[0] != "exit");//exit on input of "exit"


	return 0;
}
