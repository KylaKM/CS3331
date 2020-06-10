// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 04/15/2020                                       
// PROGRAM ASSIGNMENT #5                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    The purpose of this program is to include the implementation
//    for my functions for the santa, reindeer, elves problem.      
// -----------------------------------------------------------
#include "thread.h" 
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// ----------------------------------------------------------- 
// FUNCTION  MyMonitor                          
//     the purpose of this function is to fill in the constructor
//     of the instance of MyMonitor                   
// PARAMETER USAGE :                                           
//    r -> total number of reindeer
//    t -> max number of deliveries               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
MyMonitor::MyMonitor(int r, int t):Monitor("Monitor", HOARE) {
	reindeer = r;
	elves = (int*)malloc(3 * sizeof(int));
	maxDeliveries = t;
	lastReindeer = 0;	
	delivery = 0;
	reindeerCount = 0;
	elfCount = 0;
	asleep = 0;
	waitingReindeer = 0;
	attachingReindeer = 0;
	group = 0;
	answered = 0;
	finishedDelivery = 0;
	sleigh = 0;
	wakeUp = new Condition("WakeUp");
	reindeerBack = new Condition("AllTheReindeerBack");
	reindeerWaiting = new Condition("WaitingToAttach");
	takeOff = new Condition("TakeOff");			
	releaseReindeer = new Condition("ReleaseReindeer");
	answers = new Condition("SantaAnswers");
	releaseElves = new Condition("ReleaseElves");
	rBusy = new Condition("GoToReindeer");
	eBusy = new Condition("GoToElves");
	ready = new Condition("ReadyForTakeOff");
		
}

// ----------------------------------------------------------- 
// FUNCTION  Sleep                          
//     the purpose of this function is to make santa sleep until
//     the reindeer are back or 3 elves need him                    
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
void MyMonitor::Sleep() {
	MonitorBegin();
	
	printf("Santa is asleep\n");
	
	if(reindeerCount == reindeer) { //if all reindeer are back 
		asleep = 0;
		rBusy->Signal();				
	}
	else if(elfCount == 3) { //if 3 elves waiting for help	
		asleep = 0;
		eBusy->Signal();		
	}
	else { //santa will stay asleep until either event occurs
		asleep = 1;
		wakeUp->Wait();
		asleep = 0;	
	}	
		
	MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  AskQuestion                          
//     the purpose of this function is for when an elf has a 
//     question it blocks the calling elf until there are three               
// PARAMETER USAGE :                                           
//    count -> number of elf that has the problem                
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
void MyMonitor::AskQuestion(int count) {
	MonitorBegin();
	
	if((group == 1) && (delivery < maxDeliveries)) {	//if there are groups of elves waiting before this elf
		releaseElves->Wait();		
	}
	elfCount++;
	if(delivery < maxDeliveries) { 
		printf("         Elf %d has a problem\n", count);
		if(elfCount == 3) { 	//if enough elves to wake up santa
			group = 1;
			elves[elfCount-1] = count;
			if(asleep == 1) { //if santa is asleep, wake him up
 				printf("         Elves %d, %d, %d wake up Santa\n", elves[0], elves[1], elves[2]);
				wakeUp->Signal();
			}
			else {	//if santa is with the reindeer, wait and then wake him up
				eBusy->Wait();	
				if(delivery < maxDeliveries) {
					printf("         Elves %d, %d, %d wake up Santa\n", elves[0], elves[1], elves[2]);
				}
			}
		}
		else {	//not enough elves to talk to santa
			answered = 0;	
			elves[elfCount-1] = count;
			releaseElves->Signal();
		}
		
		//wait for santa to answer the questions
		if((answered == 0) && (delivery != maxDeliveries)) {
			answers->Wait();
		}
		
		//once santa answers, the elves go back to work
		elfCount--;
		if(elfCount == 0) {
			printf("         Elves %d, %d, %d return to work\n", elves[0], elves[1], elves[2]);
			group = 0;
			releaseElves->Signal();
		}
	}
		
	MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  ReindeerBack                          
//     the purpose of this function is when a reindeer comes 
//     back from vacation one by one and the last one has to
//     wake up santa  
// PARAMETER USAGE :                                           
//    rein -> number of reindeer that comes back               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
int MyMonitor::ReindeerBack(int rein) {
	MonitorBegin();
	
	int last = 0;
	reindeerCount++;
	if(reindeerCount != reindeer) { //if not last reindeer
		printf("    Reindeer %d returns\n", rein);
	}
	else {	//if last reindeer, wake up santa or wait for him to be done with the elves
		last = 1;
	 	if(asleep == 1) { //if asleep
			printf("    The last reindeer %d wakes up Santa\n", rein);
			lastReindeer = 1;
			wakeUp->Signal();
		}
		else {	//if busy with elves
			rBusy->Wait();
			lastReindeer = 1;
			printf("    The last reindeer %d wakes up Santa\n", rein);
		}
		reindeerBack->Signal();		
	}	
		
	MonitorEnd();
	return last;
}

// ----------------------------------------------------------- 
// FUNCTION  WaitOthers                          
//     the purpose of this function is to wait for all of the
//     other reindeer to come back   
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
void MyMonitor::WaitOthers() {
	MonitorBegin();

	if(reindeerCount != reindeer) {	//wait for all reindeer to show up
		reindeerBack->Wait();
	}	
	reindeerBack->Signal();	

	MonitorEnd();
}  

// ----------------------------------------------------------- 
// FUNCTION  WaitSleigh                          
//     the purpose of this function is for all of the reindeer
//     to be attached to the sleigh                       
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a         
// ----------------------------------------------------------- 
void MyMonitor::WaitSleigh() {
	MonitorBegin();

	waitingReindeer++;
	if(waitingReindeer == reindeer) {	//signal when all reindeer ready to attach to sleigh
		reindeerWaiting->Signal();
	}		
	else {
		reindeerWaiting->Wait();	//keep all reindeer waiting until all are there
		reindeerWaiting->Signal();	
	}
	
	if(sleigh == 0) {	//wait until sleigh is complete
		ready->Wait();
		ready->Signal();			
	}	
	reindeerCount = 0;
	lastReindeer = 0;
	waitingReindeer--;

	MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  FlyOff                          
//     the purpose of this function is that all the reindeer
//     fly off to deliver toys and is released to go back on 
//     vacation                  
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    a list of functions that are called by this one          
// ----------------------------------------------------------- 
void MyMonitor::FlyOff() {
	MonitorBegin();
	
	attachingReindeer++;
	if(attachingReindeer == reindeer) {	//once all reindeer are attached to sleigh, they can deliver toys
		delivery++;
		printf("The team flies off (%d)!\n", delivery);
		takeOff->Signal();	
	}
	else {	//have to wait until all reindeer are attached
		takeOff->Wait();
		takeOff->Signal();
	}
	
	//once trip is done, reindeer can leave
	attachingReindeer--;
	if(attachingReindeer == 0) {
		sleigh = 0;
		finishedDelivery = 1;
		releaseReindeer->Signal();
	}
	if(finishedDelivery == 0) {
		releaseReindeer->Wait();
		releaseReindeer->Signal();	
	}	
	
	MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  Answer                         
//    the purpose of this function is to simulate Santa answering
//    the questions the 3 elves have                     
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
void MyMonitor::Answer() {
	MonitorBegin();
	
	printf("Santa answers the question posted by elves %d, %d, %d\n", elves[0], elves[1], elves[2]);	
	answered = 1;
	answers->Signal();
	answers->Signal();
	answers->Signal();

	MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  AttachesReindeer                          
//     the purpose of this function is to simulate Santa gathering
//     the reindeer and attaching them to the sleigh                  
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
void MyMonitor::AttachesReindeer() {
	MonitorBegin();
	
	printf("Santa is preparing the sleigh\n");
	finishedDelivery = 0;	
	sleigh = 1;
	ready->Signal();	

	MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  PutAway                          
//    the purpose of this function is to simulate Santa releasing
//    the reindeer and putting the sleigh away                   
// PARAMETER USAGE :                                           
//    N/a              
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
void MyMonitor::PutAway() {
	MonitorBegin();

	if(finishedDelivery == 0) {
		releaseReindeer->Wait();
		releaseReindeer->Signal();		
	}

	MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  AllReindeer                          
//     the purpose of this function is to check the lastReindeer
//     variable to see if all the reindeer are back                    
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a         
// ----------------------------------------------------------- 
int MyMonitor::AllReindeer() {
	int allReindeer = 0;
	MonitorBegin();
	allReindeer = lastReindeer;
	MonitorEnd();
	return allReindeer;
}

// ----------------------------------------------------------- 
// FUNCTION  ReindeerDone                          
//    the purpose of this function is so the reindeer can check
//    there are no more deliveries                     
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
int MyMonitor::ReindeerDone() {
	int done = 0;
	MonitorBegin();
	if(delivery > maxDeliveries || delivery == maxDeliveries) {
		done = 1;
	}
	MonitorEnd();
	return done;
}

// ----------------------------------------------------------- 
// FUNCTION  ElvesDone                          
//     the purpose of this function is for the elves to check
//     there are no more deliveries                       
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
int MyMonitor::ElvesDone() {
	int done = 0;
	MonitorBegin();
	if(delivery > maxDeliveries || delivery == maxDeliveries) {
		done = 1;
		releaseElves->Signal();
		answers->Signal();
		answers->Signal();
		answers->Signal();
		eBusy->Signal();
	}
	MonitorEnd();
	return done;
}

// ----------------------------------------------------------- 
// FUNCTION SantaThread                          
//     the purpose of this function is to be a constructor for
//     the SantaThread                   
// PARAMETER USAGE :                                           
//    monitor -> instance of monitor to access procedures
//    r -> number of total reindeer
//    t -> number of max deliveries               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
SantaThread::SantaThread(MyMonitor *monitor, int r, int t): monitor(monitor) {
	printf("Santa thread starts\n");
	deliveries = 0;
	reindeer = r;
	totalDeliveries = t;
}

// ----------------------------------------------------------- 
// FUNCTION  SantaThread                        
//    the purpose of this function is to simulate santa delivering
//    toys or answering a question from a group of 3 elves                 
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    MyMonitor::Sleep(), MyMonitor::AllReindeer(), Delay(),
//    MyMonitor::AttachesReindeer(), MyMonitor::PutAway(), 
//    MyMonitor::Answer()
// ----------------------------------------------------------- 
void SantaThread::ThreadFunc() {
	Thread::ThreadFunc();
	while(deliveries < totalDeliveries) {
		monitor->Sleep();	//take a nap
					//wakened up by elves or last reindeer
	        			//note that toy delivering is more important
	        int rein;
		if((rein = monitor->AllReindeer()) == 1) { //gather all reindeer
			monitor->AttachesReindeer();		//put on sleigh
							//and fly off
			deliveries++;
			Delay(300);			//santa delivers toys
			monitor->PutAway(); 		//release all reindeer for vacation
		}
		else {
			printf("Santa is helping elves\n");	//let elves in
			Delay(200);		//solve their problem
			monitor->Answer();	//solve the problem and release elves
		}
	}	
	monitor->ElvesDone();
	Exit();
}

// ----------------------------------------------------------- 
// FUNCTION  ElfThread                          
//     the purpose of this function is to is for the constructor
//     of the ElfThread                    
// PARAMETER USAGE :                                           
//    monitor -> instance of monitor to access procedures
//    count -> number of elf               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
ElfThread::ElfThread(MyMonitor *monitor, int count): monitor(monitor), count(count) {
	printf("         Elf %d starts\n", count);
}

// ----------------------------------------------------------- 
// FUNCTION  ElfThread                          
//     the purpose of this function is so that each elf calls
//     AskQuestion until all deliveries are made                       
// PARAMETER USAGE :                                           
//    N/a            
// FUNCTION CALLED :                                           
//    MyMonitor::AskQuestion, MyMonitor::ElvesDone, Delay()          
// ----------------------------------------------------------- 
void ElfThread::ThreadFunc() {
	Thread::ThreadFunc();

	while(1) {
		Delay(400);			//make toys
		monitor->AskQuestion(count);	//encounter problem
		Delay(200);			//problem solved, rest
		int i;
		if((i = monitor->ElvesDone()) == 1) {
			Exit();
		}
	}	
	Exit();
}

// ----------------------------------------------------------- 
// FUNCTION  ReindeerThread                         
//     the purpose of this function is to be the constructor
//     of the ReindeerThreads                      
// PARAMETER USAGE :                                           
//    monitor -> instance of monitor to access procedures
//    count -> number of reindeer               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
ReindeerThread::ReindeerThread(MyMonitor *monitor, int count): monitor(monitor), count(count) {
	printf("    Reindeer %d starts\n", count);
}

// ----------------------------------------------------------- 
// FUNCTION  ReindeerThread                          
//     the purpose of this function is to simulate the reindeer
//     coming back from vacation and delivering the toys a 
//     certain number of times                     
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    MyMonitor::ReindeerBack, MyMonitor::WaitOthers,
//    MyMonitor::WaitSleigh, MyMonitor::FlyOff, Delay(),
//    MyMonitor::ReindeerDone          
// ----------------------------------------------------------- 
void ReindeerThread::ThreadFunc() {
	Thread::ThreadFunc();
	int i;
	while(1) {
		Delay(600);				//tan on the beaches
		i = monitor->ReindeerBack(count);	//report to Santa
		if(i != 1) { 
			monitor->WaitOthers();		//wait for other reindeer
		}
						//last wakes up Santa
		monitor->WaitSleigh();		//wait for attaching Santa
		monitor->FlyOff();		//fly off to deliver toys
						//Santa will let go of all reindeer
		Delay(100);			//prepare for vacation
		int j;
		if((j = monitor->ReindeerDone()) == 1) {
			Exit();
		}
	}
	Exit();
}
