#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <iostream>
#include <vector>
using namespace std;



class Background
{
	private:
		string output;
		vector<Background> backgroundVector;
		vector<Background> finished;




	public:
		void sortVector();
		void removeJob();

};



#endif