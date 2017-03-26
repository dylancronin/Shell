#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <iostream>
#include <vector>
using namespace std;



class Background
{
	private:
		string command;
		//int pid;




	public:
		Background(string cmd)
		{
			command = cmd;
			//pid = process;
		}
		// int getPID()
		// {
		// 	this -> pid;
		// }
		string getCMD()
		{
			return this -> command;
		}

};



#endif