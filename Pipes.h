#ifndef PIPES_H
#define PIPES_H

#include <string>
#include <vector>
#include <stdio.h>
#include <cstring>
#include <ctype.h>
#include <stdlib.h>
#include <cctype>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>

#include <iostream>
using namespace std;


/*****************************************************
Create a class to hold the arguments to be used in  
the execvp function.
*****************************************************/
class Pipes
{
	private:
		int count;
	public:

		char * exec_args[1024];

		Pipes(char * args[], int count)
		{
			for(int i = 0; i < count; i++)
			{
				exec_args[i] = args[i];
			}
		}


		char* getElement(int element)
		{
			return this -> exec_args[element];
		}

};



#endif