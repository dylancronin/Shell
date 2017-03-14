#ifndef PROCESS_H
#define PROCESS_H


#include <vector>
#include <iostream>


using namespace std;

class Process
{

	private:
		int cmd;
		vector<string> vect; 
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

		

	public:
		Process()
		{
			cmd = OTHER;
		};
		Process(int command, vector<string> argVect)
		{
			cmd = command;
			vect = argVect;
		};
		void forkshit();








};



#endif
