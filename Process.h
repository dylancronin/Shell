#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <string>
//#include <vector>
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
#include <vector>
#include <iostream>
#include <sys/wait.h>
using namespace std;

enum Commands
{
			 CD = 1,
			 LS,
			 WAIT,
			 RM,
			 X,
			 CP,
			 PWD,
	 	 OTHER
};


class Process
{

	private:
		vector<string>::iterator it;
		char * exec_args[1024];
		int arg_count;
		int cmd;
		vector<string> inputVector; 

		void listDir();
		void changeDir();
		void wait();
		void remove();
		void presentDir();
		void xterm();
	public:
		Process()
		{
			cmd = OTHER;
		};
		Process(vector<string> argVect)
		{
			inputVector = argVect;
		};
		//int checkCommand();
		void forkshit();
		int checkCommand(string);
		bool isInt(string);
		bool isBackground();









};



#endif
