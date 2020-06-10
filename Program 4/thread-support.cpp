// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 04/01/2020                                       
// PROGRAM ASSIGNMENT #4                                        
// FILE NAME : thread-support.cpp            
// PROGRAM PURPOSE :                                           
//    The purpose of this program is to contain the GoCheck()
// and the GoToParty() methods for the landlord and student 
// threads.        
// ----------------------------------------------------------- 
#include "thread.h"
#include <stdio.h>
using namespace std;

// ----------------------------------------------------------- 
// FUNCTION : CheckRoom                          
//     the purpose of this function is to check the room                          
// PARAMETER USAGE :                                           
//    count -> # of check
//    maxChecks -> max times to check the room
//    maxStudents -> max number of students in the room before
//    breaking up the party
//    numStudents -> pointer to current number of students in
//    the room                                                                     //    lastCheck -> pointer to whether it is the last check                         //    checking -> pointer to whether landlord is checking                          //    breakingUp -> pointer to whether landlord is breaking
//    up the party
//    entering -> queue for students entering
//    exiting -> queue for students exiting
//    enterBlock -> block students from entering
//    exitBlock -> block students from exiting
//    lastStudent -> signal that last student is exiting
//    studentsFinished -> signal that students are terminated
//    printLock -> print each thread one at a time
//    enterLock -> lock so students enter one at a time
//    exitLock -> lock so students exit one at a time                
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
int CheckRoom(int count, int maxChecks, int maxStudents, int *numStudents, int *lastCheck, int *checking, int *breakingUp, Semaphore *enterBlock, Semaphore *exitBlock, Semaphore *lastStudent, Semaphore *studentsFinished, Mutex *lockEnter, Mutex *lockExit, Mutex *printLock) {
	//increment checking to symbolize landlord checking room
	(*checking)++;
	
	//lock enter and exit so student completely enters and exits one at a time before checking
	lockEnter->Lock();
	lockExit->Lock();

	//print current check
	printLock->Lock();
	printf("The landlord checks the room the %d-th time\n", count);
	printLock->Unlock();

	//if last check
	if(count == maxChecks) {
		*lastCheck = 1;
	
		//if the room is empty
		if(*numStudents == 0) {
			*checking == 0;

			printLock->Lock();
			printf("The landlord finds the room has no students and leaves\n");
			printLock->Unlock();
			
			//unlock everything
			enterBlock->Signal();
			lockEnter->Unlock();
			return 1;
		}
		
		//print number of students
		printLock->Lock();
		printf("The landlord finds there are %d students in the room and forces everyone to leave\n", *numStudents);
		printLock->Unlock();
		
		//unlock all exit locks
		lockExit->Unlock();
		exitBlock->Signal();

		//wait for all students to terminated
		studentsFinished->Wait();
		
		//print leaving room
		printLock->Lock();
		printf("The landlord leaves the room and the room is empty\n");
		printLock->Unlock();

		//unlock enter locks
		*checking = 0;
		enterBlock->Signal();
		lockEnter->Unlock();
		return 1;	
	}

	//case with no students
	else if(*numStudents == 0) {
		printLock->Lock();
		printf("The landlord finds the room has no students and leaves\n");
		printLock->Unlock();
		
		//unlock all locks so students can resume
		lockEnter->Unlock();
		lockExit->Unlock();
		enterBlock->Signal();
		exitBlock->Signal();
		*checking = 0;
	}

	//case for acceptable number of students in room
	else if(*numStudents <= maxStudents) {
		//print number of students
		printLock->Lock();
		printf("The landlord finds there are %d students in the room (condition being good) and leaves\n", *numStudents);
		printLock->Unlock();

		//unlock all locks so students can resume
		lockEnter->Unlock();
		lockExit->Unlock();
		enterBlock->Signal();
		exitBlock->Signal();
		*checking = 0;
	}

	//case for too many students in the room
	else {
		//print number of students
		printLock->Lock();
		printf("The landlord finds %d students in the room and breaks up the party\n", *numStudents);
		printLock->Unlock();

		//print force leave
		printLock->Lock();
		printf("The landlord finishes checking and forces everyone to leave\n");
		printLock->Unlock();
		
		//change to 1 so students know to leave and not enter room
		*breakingUp = 1;
		
		//unlock exit locks
		lockExit->Unlock();
		exitBlock->Signal();

		//wait for signal from last student
		lastStudent->Wait();

		//print leaving room
		printLock->Lock();
		printf("The landlord leaves the room and the room is empty\n");
		printLock->Unlock();
		
		//unlocks all locks
		*breakingUp = 0;
		lockEnter->Unlock();
		enterBlock->Signal();
		*checking = 0;
	}
	return 1;		
}

// ----------------------------------------------------------- 
// FUNCTION  GoToParty                          
//     the purpose of this function     
// PARAMETER USAGE :                                           
//    count -> number for student
//    numStudents -> pointer to current number of students in
//    the room                                                                     //    lastCheck -> pointer to whether it is the last check                         //    checking -> pointer to whether landlord is checking                          //    breakingUp -> pointer to whether landlord is breaking
//    up the party
//    entering -> queue for students entering
//    exiting -> queue for students exiting
//    enterBlock -> block students from entering
//    exitBlock -> block students from exiting
//    lastStudent -> signal that last student is exiting
//    studentsFinished -> signal that students are terminated
//    printLock -> print each thread one at a time
//    enterLock -> lock so students enter one at a time
//    exitLock -> lock so students exit one at a time                
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
int GoToParty(int count, int *numStudents, int *lastCheck, int *checking, int *breakingUp, Semaphore *entering, Semaphore *exiting, Semaphore *enterBlock, Semaphore *exitBlock, Semaphore *lastStudent, Semaphore *studentsFinished, Mutex *lockEnter, Mutex *lockExit, Mutex *printLock) {
	//if not last check, print wait
	if(*lastCheck == 0) {
		printf("     Student %d waits to enter the room\n", count);
	}
	
	//enter queue
	entering->Wait();
	
	//if landlord is checking, then block
	if(*checking == 1) {
		enterBlock->Wait();
	}

	//lock so that students enter fully before landlord checks
	lockEnter->Lock();

	//if last check, then signal landlord that students are finished
	if(*lastCheck == 1) {	
		if(*numStudents == 0) {
			studentsFinished->Signal();
		}
		entering->Signal();
		lockEnter->Unlock();
		return -1;
	}
	
	//print entering room
	printLock->Lock();
	printf("     Student %d enters the room (%d students in the room)\n", count, ++(*numStudents));
	printLock->Unlock();
	
	//unlock locks for entering
	lockEnter->Unlock();
	entering->Signal();

	//printing for waiting to leave
	printLock->Lock();
	printf("     Student %d waits to leave the room\n", count);
	printLock->Unlock();

	//exit queue
	exiting->Wait();
	
	//if landlord checking, then block
	if(*checking == 1) {
		exitBlock->Wait();
	}

	//lock exit so student completely exits before landlord checks
	lockExit->Lock();

	//print student leaving room
	printLock->Lock();
	printf("     Student %d leaves the room (%d students in the room)\n", count, --(*numStudents));
	printLock->Unlock();

	//if breaking up party and greater then 0 students, unlock exit block	
	if((*numStudents != 0 && *breakingUp == 1) || (*numStudents != 0 && *lastCheck == 1)) {
		exitBlock->Signal();
	}

	//if last check and 0 students, then signal students finished to landlord 
	if(*lastCheck == 1 && *numStudents == 0) {
		studentsFinished->Signal();
		return -1;
	}

	//if 0 zero students while breaking up party, then signal last student
	//exiting to landlord
	if(*numStudents == 0 && *breakingUp == 1) {
		lastStudent->Signal();
	}
	
	//unlock all exit locks
	lockExit->Unlock();
	exiting->Signal();
	return 1;				
}
