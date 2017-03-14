//Implementation for Process class

#include "Process.h"
#include <iostream>
#include <unistd.h>

using namespace std;


bool InBackground(const vector<string> &vect);

void Process::forkshit()
{
	int pid = fork();
	if(pid == 0)
	{
		
		switch(this->cmd)
		{
			case CD:
				InBackground(this->vect);
				break;
			case LS:
				
				break;
			case WAIT:
				
				break;
	
			case RM:
			
				break;

			case X:
			
				break;

			case CP:

				break;

			case PWD:

				break;

			case OTHER:
	
				break;
		
		}
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
