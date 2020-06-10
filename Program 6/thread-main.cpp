// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 04/27/2020                                       
// PROGRAM ASSIGNMENT #6                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//    The purpose of this program is to find all prime numbers
//    between 2 and n.        
// ----------------------------------------------------------- 
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[]) {
	int n;

	//default number for n
	if(argc < 2) {
		n = 30;
	}
	else if(argc == 2) {
		n = atoi(argv[1]);
	}
	else {
		printf("Too many arguments\n");
		return -1;
	}

	//create primes array
	int *primes = (int*)malloc(n*sizeof(int));
	
	//create master thread
	MasterThread *master = new MasterThread(primes, n);
	master->Begin();

	master->Join();
	
	//release memory
	free(primes);	
}
