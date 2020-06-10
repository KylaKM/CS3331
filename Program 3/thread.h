// ----------------------------------------------------------- 
// NAME : Kyla Kane-Maystead                 User ID: kakanema 
// DUE DATE : 03/18/2020                                       
// PROGRAM ASSIGNMENT #3                                        
// FILE NAME : thread.h          
// PROGRAM PURPOSE :                                           
//    The purpose of this program is to contain all class
// definitions of the even-odd sort..     
// ----------------------------------------------------------- 
#include "ThreadClass.h"

class EvenOddSortThread: public Thread {
	public:
		//constructor
		EvenOddSortThread(int count, int lower, int upper, int arr[], bool *swap);	

	private:
		void ThreadFunc();	//thread body
		int count;		//counter of threads
		int lower;		//left element to compare
		int upper;		//right element to compare
		int *a;			//pointer to arr[]
		bool *swap;		//tell whether values are swapped
};
