// ----------------------------------------------------------- 
//  NAME : Kyla Kane-Maystead                User ID: kakanema 
//  DUE DATE : 04/01/2020                                       
//  PROGRAM ASSIGNMENT #4                                        
//  FILE NAME : thread-main.cpp            
//  PROGRAM PURPOSE :                                           
//     The purpose of this program is to contain the main 
//  function's implementation for the landlord/student problem.        
// ----------------------------------------------------------- 
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
using namespace std;

int main(int argc, char *argv[]) {
	//checks for correct number of arugments
	if(argc < 4) {
		printf("Not enough arguments");
		return -1;
	}
	if(argc > 4) {
		printf("Too many arguments");
		return -1;
	}
	
	//convert arguments to ints
	int m, n, k;
	m = atoi(argv[1]);
	n = atoi(argv[2]);
	k = atoi(argv[3]);

	//set to default values if zero
	if(m == 0) {
		m = 5;
	}
	if(n == 0) {
		n = 5;
	}
	if(k == 0) {
		k = 10;
	}

	//prepare all the variables that will be shared
	//between the threads
	int *numStudents = (int *)malloc(sizeof(int));
	*numStudents = 0;
	int *lastCheck = (int *)malloc(sizeof(int));
	*lastCheck = 0;
	int *checking = (int *)malloc(sizeof(int));
	*checking = 0;
	int *breakingUp = (int *)malloc(sizeof(int));
	*breakingUp = 0;
	Semaphore *entering = new Semaphore("entering", 1);
	Semaphore *exiting = new Semaphore("exiting", 1);
	Semaphore *enterBlock = new Semaphore("enterBlock", 1);
	Semaphore *exitBlock = new Semaphore("exitBlock", 1);
	Semaphore *lastStudent = new Semaphore("lastStudent", 0);
	Semaphore *studentsFinished = new Semaphore("studentsEnded", 0);
	Mutex *printLock = new Mutex("printLock");
	Mutex *lockEnter = new Mutex("lockEnter");
	Mutex *lockExit = new Mutex("lockExit");	

	//create landlord thread
	LandlordThread *landlord = new LandlordThread(m, n, numStudents, lastCheck, checking, breakingUp, entering, exiting, enterBlock, exitBlock, lastStudent, studentsFinished, lockEnter, lockExit, printLock);
	landlord->Begin();
	
	//create student threads
	StudentThread *students[k];
	int i;
	for(i = 0; i < k; i++) {
		students[i] = new StudentThread(i, numStudents, lastCheck, checking, breakingUp, entering, exiting, enterBlock, exitBlock, lastStudent, studentsFinished, lockEnter, lockExit, printLock);
		students[i]->Begin();
	}

	//join student threads
	for(i = 0; i < k; i++) {
		students[i]->Join();
	}
	
	//join landlord thread
	landlord->Join();
	
	printf("After checking the room %d times, the landlord retires and is on vacation!\n", m);
	
	//free all malloc'd pointers
	free(numStudents);
	free(lastCheck);
	free(checking);
	free(breakingUp);

	Exit();
	return 0;			
}
