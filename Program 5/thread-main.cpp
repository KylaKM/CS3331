// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 04/15/2020                                       
// PROGRAM ASSIGNMENT #5                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//    The purpose of this program is to create the santa, 
//    reindeer, and elves threads for the problem.        
// ----------------------------------------------------------- 
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[]) {
	//checks for correct number of arguments
	if(argc < 4) {
		printf("Not enough arguments\n");
		return -1;
	}
	if(argc > 4) {
		printf("Too many arguments\n");
		return 1;
	}

	//convert arguments to ints
	int e, r, t;
	e = atoi(argv[1]);
	r = atoi(argv[2]);
	t = atoi(argv[3]);

	//set to default values if zero
	if(e == 0) {
		e = 7;
	}
	if(r == 0) {
		r = 9;
	}
	if(t == 0) {
		t = 5;
	}
	
	//create monitor
	MyMonitor *monitor = new MyMonitor(r, t);

	//create threads
	SantaThread *santa = new SantaThread(monitor, r, t);
	ElfThread *elves[e];
	ReindeerThread *reindeer[r];
	
	//start threads
	santa->Begin();

	int i, j;
	for(i = 0; i < r; i++) {
		reindeer[i] = new ReindeerThread(monitor, i);
		reindeer[i]->Begin();		
	}	
	
	for(j = 0; j < e; j++) {
		elves[j] = new ElfThread(monitor, j);
		elves[j]->Begin();
	}

	//join threads
	for(i = 0; i < r; i++) {
		reindeer[i]->Join();
	}
	for(j = 0; j < e; j++) {
		elves[j]->Join();
	}
	santa->Join();

	printf("After (%d) deliveries, Santa retires and is on vacation!\n", t);
	return 0;
}	
