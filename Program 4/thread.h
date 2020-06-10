// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 04/01/2020                                       
// PROGRAM ASSIGNMENT #4                                        
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//   The purpose of this program is to contain all class 
// definitions for the student and landlord threads as well as
// method prototypes from thread-support.cpp.         
// -----------------------------------------------------------
#include "ThreadClass.h"

//prototypes
int CheckRoom(int count, int maxChecks, int maxStudents, int *numStudents, int *lastCheck, int *checking, int *breakingUp, Semaphore *enterBlock, Semaphore *exitBlock, Semaphore *lastStudent, Semaphore *studentsFinished, Mutex *lockEnter, Mutex *lockExit, Mutex *printLock);

int GoToParty(int count, int *numStudents, int *lastCheck, int *checking, int *breakingUp, Semaphore *entering, Semaphore *exiting, Semaphore *enterBlock, Semaphore *exitBlock, Semaphore *lastStudent, Semaphore *studentsFinished, Mutex *lockEnter, Mutex *lockExit, Mutex *printLock);


class LandlordThread: public Thread {
	public:
		//constructor
		LandlordThread(int max, int n, int *numStudents, int *lastCheck, int *checking, int *breakingUp, Semaphore *entering, Semaphore *exiting, Semaphore *enterBlock, Semaphore *exitBlock, Semaphore *lastStudent, Semaphore *studentsFinished, Mutex *lockEnter, Mutex *lockExit, Mutex *printLock);
	
	private:
		void ThreadFunc();	//thread body
		int max;		//max # of times to check the room
		int n;			//max # of students in room before breaking up party
		int *numStudents;	//current # of students in room
		int *lastCheck;		//0 if not last check, 1 if last check
		int *checking;		//0 if landlord isn't checking, 1 if he/she is
		int *breakingUp;	//1 if landlord needs to break up the party
		Semaphore *entering;	//queue to hold students wanting to enter the room
		Semaphore *exiting;	//queue to hold students wanting to exit the room
		Semaphore *enterBlock;	//binary semaphore used to block students from entering while landlord is checking
		Semaphore *exitBlock;	//binary semaphore used to block students from leaving while landlord is checking
		Semaphore *lastStudent;	//binary semaphore used to signal landlord when last student leaves room
		Semaphore *studentsFinished; //binary semaphore used to signal landlord when last student terminates
		Mutex *printLock;	//lock to secure printing for each thread
		Mutex *lockEnter;	//lock to make sure only one student at a time enters fully
		Mutex *lockExit;	//lock to make sure only one student at a time exits fully

};

class StudentThread: public Thread {
	public:
		//constructor
		StudentThread(int count, int *numStudents, int *lastCheck, int *checking, int *breakingUp, Semaphore *entering, Semaphore *exiting, Semaphore *enterBlock, Semaphore *exitBlock, Semaphore *lastStudent, Semaphore *studentsFinished, Mutex *lockEnter, Mutex *lockExit, Mutex *printLock);

	private:
		void ThreadFunc();	//thread body
		int count;		//# for student
		int *numStudents;	//current # of students
		int *lastCheck;    	//0 if not last check, 1 if last check
		int *checking;    	//0 if landlord isn't checking, 1 if he/she is
		int *breakingUp;	//1 if landlord needs to break up party
		Semaphore *entering;	//queue to hold students wanting to enter the room               	  
		Semaphore *exiting;	//queue to hold students wanting to leave the room
                Semaphore *enterBlock; 	//binary semaphore used to block students from entering while landlord is checking
		Semaphore *exitBlock;  	//binary semaphore used to block students from exiting wile landlord is checking
		Semaphore *lastStudent;	//binary semaphore used to signal landlord when last student leaves room   
		Semaphore *studentsFinished; //binary semaphore used to signal landlord when last student terminates
		Mutex *printLock;	//lock to secure printing for each thread  
		Mutex *lockEnter;	//lock to make sure only one student at a time enters fully
		Mutex *lockExit;	//lock to make sure only one student at a tiem exits fully

};
