// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 04/01/2020                                       
// PROGRAM ASSIGNMENT #4                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//     The purpose of this program is to contain all class
// implementations of the landlord/student problem       
// ----------------------------------------------------------- 
#include "thread.h"
#include <stdio.h>
using namespace std;

// ----------------------------------------------------------- 
// FUNCTION  LandlordThread : (Constructor)                          
//     The purpose of this function is to fill in the values 
//  of the constructor for each new thread and print that it
//  was created.                  
// PARAMETER USAGE :                                           
//    max -> max # of checks
//    n -> max # of students without breaking up party
//    numStudents -> pointer to current number of students in 
//    the room
//    lastCheck -> pointer to whether it is the last check
//    checking -> pointer to whether landlord is checking
//    breakingUp -> pointer to whether landlord is breaking
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
LandlordThread::LandlordThread(int max, int n, int *numStudents, int *lastCheck, int *checking, int *breakingUp, Semaphore *entering, Semaphore *exiting, Semaphore *enterBlock, Semaphore *exitBlock, Semaphore *lastStudent, Semaphore *studentsFinished, Mutex *lockEnter, Mutex *lockExit, Mutex *printLock): max(max), n(n), numStudents(numStudents), lastCheck(lastCheck), checking(checking), breakingUp(breakingUp), entering(entering), exiting(exiting), enterBlock(enterBlock), exitBlock(exitBlock), lastStudent(lastStudent), studentsFinished(studentsFinished), lockEnter(lockEnter), lockExit(lockExit), printLock(printLock) {
	printf("Landlord Starts\n");
}

// ----------------------------------------------------------- 
// FUNCTION  LandlordThread                          
//     The purpose of this function is to have landlord check
// room only a certain number of times.                 
// PARAMETER USAGE :                                          
//    N/a               
// FUNCTION CALLED :                                           
//    GoCheck()          
// ----------------------------------------------------------- 
void LandlordThread::ThreadFunc() {
	Thread::ThreadFunc();
	int i;
	//go for max number of checks
	for(i = 0; i < max; i++) {
		Delay();	
		CheckRoom((i+1), max, n, numStudents, lastCheck, checking, breakingUp, enterBlock, exitBlock, lastStudent, studentsFinished, lockEnter, lockExit, printLock);
		Delay();
	}
	Exit();	
}

// ----------------------------------------------------------- 
// FUNCTION  StudentThread : (Constructor)                          
//     The purpose of this function is to fill in the values 
//  of the constructor for each new thread and print that it
//  was created.                  
// PARAMETER USAGE :                                           
//    count -> # for student
//numStudents -> pointer to current number of students in
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
StudentThread::StudentThread(int count, int *numStudents, int *lastCheck, int *checking, int *breakingUp, Semaphore *entering, Semaphore *exiting, Semaphore *enterBlock, Semaphore *exitBlock, Semaphore *lastStudent, Semaphore *studentsFinished, Mutex *lockEnter, Mutex *lockExit, Mutex *printLock): count(count), numStudents(numStudents), lastCheck(lastCheck), checking(checking), breakingUp(breakingUp), entering(entering), exiting(exiting), enterBlock(enterBlock), exitBlock(exitBlock), lastStudent(lastStudent), studentsFinished(studentsFinished), lockEnter(lockEnter), lockExit(lockExit), printLock(printLock) {
	printf("     Student %d Starts\n", count);
}

// ----------------------------------------------------------- 
// FUNCTION  StudentThread                          
//     The purpose of this function is to have students go to
// the party room and leave for an unknown amount.                      
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    GoToParty()          
// ----------------------------------------------------------- 
void StudentThread::ThreadFunc() {
	Thread::ThreadFunc();
	int i;
	if(*lastCheck == 1) {
		Exit();
	}
	while(1) {
		Delay();
		i = GoToParty(count, numStudents, lastCheck, checking, breakingUp, entering, exiting, enterBlock, exitBlock, lastStudent, studentsFinished, lockEnter, lockExit, printLock);
		if(i == -1) {
			Exit();
		}
		Delay();
	}
}

