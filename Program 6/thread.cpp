// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 04/27/2020                                       
// PROGRAM ASSIGNMENT #6                                        
// FILE NAME : thread.cpp           
// PROGRAM PURPOSE :                                           
//    The purpose of this program is to have the implementations
//    of the master channel thread and the prime threads that
//    remember the prime nubmers and input them into the array.     
// ----------------------------------------------------------- 
#include "thread.h"
#include <stdio.h>
#include <string.h>
using namespace std;

// ----------------------------------------------------------- 
// FUNCTION  MasterThread                          
//     the purpose of this function is to fill in the 
//     constructor                           
// PARAMETER USAGE :                                           
//    *primes -> pointer to the prime value array
//    max -> the max number for finding primes      
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
MasterThread::MasterThread(int *primes, int max): primes(primes), max(max)  {
	printf("Master starts\n");
	UserDefinedThreadID = 0;
}

// ----------------------------------------------------------- 
// FUNCTION  MasterThread                          
//     the purpose of this function is to pass numbers from 3
//     to the max to the second channel to find out if they are
//     prime numbers and then print out the prime numbers.                    
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
void MasterThread::ThreadFunc() {
	Thread::ThreadFunc();
	
	//create p2 so master can send numbers to it
	PrimeThread *p2 = new PrimeThread(1, 2, primes);
	p2->Begin();

	//send numbers from 3 to max
	int i;
	for(i = 3; i <= max; i++) {
		printf("Master sending %d to P2\n", i);
		p2->channel->Send(&i, sizeof(int));
	}

	//send end signal when done sending numbers
	printf("Master sends END\n");
	int end = -1;
	p2->channel->Send(&end, sizeof(int));

	p2->Join();

	//print out the array of prime numbers	
	printf("Master prints the complete result:\n");
	printf("  ");
	for(i = 1; i < max; i++) {
		if(primes[i] == 0) {
			break;
		}
		printf("%d ", primes[i]);
	}

	printf("\n");
	printf("Master terminates\n");

	Exit();
} 

// ----------------------------------------------------------- 
// FUNCTION  PrimeThread                        
//     the purpose of this function is to fill in the constructor                
// PARAMETER USAGE :                                           
//    i -> index of thread
//    n -> number to 'memorize'
//    max -> max number to look for primes
//    *primes -> pointer to prime number array	
// FUNCTION CALLED :                                           
//    a list of functions that are called by this one          
// ----------------------------------------------------------- 
PrimeThread::PrimeThread(int i, int n, int *primes): i(i), n(n), primes(primes) {
	
	UserDefinedThreadID = i;
	char name[100];
	sprintf(name, "Channel%d-%d", i, i+1);
	channel = new SynOneToOneChannel(name, i-1, i);		
}

// ----------------------------------------------------------- 
// FUNCTION  PrimeThread                          
//     the purpose of this function is to pass numbers until
//     max to other threads to memorize if they are a prime
//     number.                      
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
void PrimeThread::ThreadFunc() {
	Thread::ThreadFunc();
	
	//create number of spaces needed to be in front of output
	int p;
	char spaces[100];
        sprintf(spaces, "");
        for(p = 0; p < i; p++) {
                sprintf(spaces, "%s  ", spaces);
        }
	char first[100];
	sprintf(first, "%s", spaces);

	printf("%sP%d starts and memorizes %d\n", first, n, n);
	primes[i] = n;
	
	int j;
	int k;
	int end = -1;
	int last = 1;

	PrimeThread *next;

	while(1) {
		//receive number from previous thread
		channel->Receive(&k, sizeof(int));
		if(k != -1) {
			//if not the last thread
			printf("%sP%d receives %d\n", first, n, k);
			
			//if k is a multiple then it's not prime
			if(k % n == 0) {
				printf("%sP%d ignores %d\n", first, n, k);
			}
			else {
				//if last thread, create more when needed
				if(last) {
					printf("%sP%d creates P%d\n", first, n, k);
					next = new PrimeThread(i+1, k, primes);
					next->Begin();
					last = 0;
				}
				//send to next thread
				else {
					printf("%sP%d sends %d to P%d\n", first, n, k, next->n);
					next->channel->Send(&k, sizeof(int));
				}
			}
		}
		//end of numbers 
		else {
			printf("%sP%d receives END\n", first, n);
			
			//if not the last thread send signal to next thread
			if(!last) {
				printf("%sP%d sends END to P%d\n", first, n, next->n);
				next->channel->Send(&end, sizeof(int));
				next->Join();
			}
			break;
		}
	}	

	Exit();
}






