#ifndef BACKGROUND_H
#define BACKGROUND_H


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
#include <fstream>

#include <iostream>
#include <vector>
using namespace std;



class Background
{
	private:
		string command;
		char pipBUF[4096];


	public:
		Background(string cmd)
		{
			command = cmd;
			//pid = process;
		}
		
		char BUF[4096];
		string getCMD()
		{
			return this -> command;
		}

		void piping()
		{
			int link[2];
			pipe(link);

			int pid = fork();
			
			if(pid == 0)
			{
					dup2(link[1], STDOUT_FILENO);
    				close(link[0]);
			}

			else
			{
					close(link[1]);
					read(link[0], pipeBUF, sizeof(pipeBUF));
			}
			
			dup2(link[1], STDOUT_FILENO);
			close(link[1]);
			read(link[0], this->pipBUF, sizeof(this->pipBUF));
			//printf("Hello");
			//printf("Pipe contents: %s\n", this->pipBUF);

		}

		void output()
		{
			printf("Pipe contents: %s\n", this->pipBUF);
		}

};



#endif