//#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <string>

using namespace std;

int main()
{
	int num = 24; // Num motores
	do
	{
		ofstream archivo[2];
		archivo[0].open("../temperatura", ios::trunc);
		archivo[1].open("../corriente", ios::trunc);
		for(int i = 0; i < num ; i++ ){
			archivo[0] << rand() % 90 + 0 << "\t";
			archivo[1] << rand() % 10 + 0 << "\t";
		}
		archivo[0].close();
		archivo[1].close();
		sleep(1);
	} 
	while (1);
	
	return 0;
}
