// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 04/15/2020                                       
// PROGRAM ASSIGNMENT #5                                        
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//    The purpose of this program is to have the headers for
//    my functions for the santa, reindeer, elves problem.      
// ----------------------------------------------------------- 
#include "ThreadClass.h"

class MyMonitor: public Monitor {
        public:
                MyMonitor(int r, int t);
                void AskQuestion(int count);
                int ReindeerBack(int rein);
                void WaitOthers();
                void WaitSleigh();
                void FlyOff();
                void Sleep();
                int AllReindeer();
                int ReindeerDone();
                int ElvesDone();
		void Answer();
                void AttachesReindeer();
                void PutAway();
	
	private:
                int *elves;
                int lastReindeer;
                int reindeer;
		int maxDeliveries;
                int delivery;
                int reindeerCount;
                int elfCount;
                int asleep;
                int waitingReindeer;
                int attachingReindeer;
                int group;
                int answered;
                int finishedDelivery;
                int sleigh;
                Condition *wakeUp;
                Condition *reindeerBack;
                Condition *reindeerWaiting;
                Condition *takeOff;
                Condition *releaseReindeer;
                Condition *answers;
                Condition *releaseElves;
                Condition *rBusy;
                Condition *eBusy;
                Condition *ready;
                Condition *finished;

};

class SantaThread: public Thread {
	public:
		//constructor
		SantaThread(MyMonitor *monitor, int r, int t);	

	private:
		void ThreadFunc();	//thread body
		MyMonitor *monitor;
		int reindeer;
		int totalDeliveries;
		int deliveries;
		
};

class ElfThread: public Thread {
	public:
		//constructor
		ElfThread(MyMonitor *monitor, int count);
	
	private:
		void ThreadFunc();	//thread body
		MyMonitor *monitor;
		int count;
};

class ReindeerThread: public Thread {
	public:
		//constructor
		ReindeerThread(MyMonitor *monitor, int count);

	private:
		void ThreadFunc();	//thread body
		MyMonitor *monitor;
		int count;
};


