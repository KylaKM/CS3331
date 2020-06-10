// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema
// DUE DATE : 03/18/2020                                      
// PROGRAM ASSIGNMENT #3                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    The purpose of this program is to contain all class
// implementations of the even-odd sort.          
// ----------------------------------------------------------- 
#include <iostream>
#include <stdio.h>
#include "thread.h"
using namespace std;

// ----------------------------------------------------------- 
// FUNCTION  EvenOddSortThread : (Constructor)                          
//     The purpose of this function is to fill in the values
//  of the constructor for each new thread and print that it
//  was created.                          
// PARAMETER USAGE :                                           
//    count -> number of thread in thread array
//    lower -> left index to compare
//    upper -> right index to compare
//    arr[] -> array of integers
//    *swap -> pointer to swap variable so that it can 
//    	       modify the shared swap variable               
// FUNCTION CALLED :                                           
//    N/a          
// -----------------------------------------------------------
EvenOddSortThread::EvenOddSortThread(int count, int lower, int upper, int arr[], bool *swap): count(count), lower(lower), upper(upper), a(arr), swap(swap) {
	printf("        Thread %d Created\n", count);
}

// ----------------------------------------------------------- 
// FUNCTION  EvenOddSortThread                          
//     The purpose of this function is to determine whether
//  two values in the array need to be swapped.                            
// PARAMETER USAGE :                                           
//    N/a               
// FUNCTION CALLED :                                           
//    N/a          
// ----------------------------------------------------------- 
void EvenOddSortThread::ThreadFunc() {
	Thread::ThreadFunc();
	
	//save off left and right values	
	int left = a[lower];
	int right = a[upper];
	
	printf("        Thread %d compares x[%d] and x[%d]\n", count, lower, upper);
	if(right < left) { //if out of order, then swap
		a[lower] = right;
		a[upper] = left;
		printf("        Thread %d swaps x[%d] and x[%d]\n", count, lower, upper);
		*swap = true;
	}
	printf("        Thread %d exits\n", count);
	Exit();	
}
