// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 04/27/2020                                       
// PROGRAM ASSIGNMENT #6                                        
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//    The purpose of this program is to have the headers.  
// ----------------------------------------------------------- 
#include "ThreadClass.h"
using namespace std;

class MasterThread : public Thread {
	public:
		//constructor
		MasterThread(int *primes, int max);

	private:
		void ThreadFunc();	//thread body
		int *primes;
		int max;

};

class PrimeThread : public Thread {
	public:
		//constructor
		PrimeThread(int i, int n, int *primes);
		SynOneToOneChannel *channel;

	private:
		void ThreadFunc();	//thread body
		int i;
		int n;
		int *primes;
		
};
