/*****************************************************
File: background.h
Description: Create Background class to take care of all of the
background processes.
Author: Cronin, Slomski
Course: CS4080, Spring 2017
*****************************************************/



#ifndef BACKGROUND_H
#define BACKGROUND_H


#include <string>
#include <stdio.h>
#include <sstream>
#include <cstring>
#include <ctype.h>
#include <stdlib.h>
#include <cctype>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
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
		int pid;
		string fileName;
		int bgNum;


	public:
		Background(string cmd, int process, string fName, int num)
		{
			command = cmd;
			pid = process;
			fileName = fName;
			bgNum = num;
		}
		
		//char BUF[4096];
		string getCMD()
		{
			return this -> command;
		}

		int getPID()
		{
			return this -> pid;
		}

		string getFileName()
		{
			return this -> fileName;
		}

		int getbgnum()
		{
			return this -> bgNum;
		}

};



#endif