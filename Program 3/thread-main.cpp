// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 03/18/2020                                       
// PROGRAM ASSIGNMENT #3                                        
// FILE NAME : thread-main.cpp         
// PROGRAM PURPOSE :                                           
//    The purpose of this program is to contain the main 
// function's implementation for the even-odd sort..       
// -----------------------------------------------------------  
#include <iostream>
#include "thread.h"
using namespace std;

int main(int argc, char *argv[]) {
	cout << "Concurrent Even-Odd Sort" << endl; 
	cout << endl;
	
	//read in length of array from standard input
	int length;
	cin >> length;
	
	cout << "Number of input data = " << length << endl;
	
	//read in elements of array and print them out
	int *arr = new int[length];
	cout << "Input array: " << endl;
	for(int i = 0; i < length; i++) {
		cin >> arr[i];
		if((i % 20 == 0) && (i != 0)) { //if 20 integers on line, go to next line
			cout << endl;	
		}
		cout << "  " << arr[i];
	}
	cout << endl << endl;
	
	bool *swap = new bool;
	*swap = true;
	int count = 1;
	while(*swap == true) {
		cout << "Iteration " << count << ":" << endl;
		
		//initialize even and odd thread arrays;
		int even, odd;
		if(length % 2 == 0) {
			even = length/2;
			odd = (length/2)-1;
		}
		else {
			even = length/2;
			odd = length/2;
		}
		EvenOddSortThread *Eventhreads[even];
		EvenOddSortThread *Oddthreads[odd];
		
		int i;
		cout << "    Even Pass:" << endl;
		*swap = false;

		//create and initialize even threads
		for(i = 0; i < even; i++) {
			Eventhreads[i] = new EvenOddSortThread(i, 2*i, (2*i)+1, arr, swap);
			Eventhreads[i]->Begin();
		}

		//wait for all of the even threads to join 
		for(i = 0; i < even; i++) {
                        Eventhreads[i]->Join();                                                       
		}
	
		cout << "    Odd Pass:" << endl;
		
		//create and initialize odd threads
		for(i = 0; i < odd; i++) {
			Oddthreads[i] = new EvenOddSortThread(i, (2*i)+1, (2*i)+2, arr, swap);
			Oddthreads[i]->Begin();
		}

		//wait for all of the odd threads to join
		for(i = 0; i < odd; i++) {
			Oddthreads[i]->Join();
		}
		
		//print out results from iteration
		cout << "Result after iteration " << count << ":" << endl;	
		for(i = 0; i < length; i++) {
			if((i % 20 == 0) && (i != 0)) {
				cout << endl;
			}
			cout << "   " << arr[i];
		}
		cout << endl;
		if(*swap == true) { //if elements swapped then increment count for next iteration
			count++;
		}			
	}
	delete swap;
	cout << endl;
	
	//print final results
	cout << "Final result after iteration " << count << ":" << endl;
	int i;
	for(i = 0; i < length; i++) {
		if((i % 20 == 0) && (i != 0)) { //if 20 numbers at line, go to next line
			cout << endl;                                                                                                                                 
		}
		cout << "   " << arr[i];
	}
	cout << endl;	
	Exit();
	return 0;	
}
