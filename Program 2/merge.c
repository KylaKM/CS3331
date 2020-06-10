/* ----------------------------------------------------------- */
/* NAME : Kyla Kane-Maystead                 User ID: kakanema */   
/* DUE DATE : 02/24/2020                                       */ 
/* PROGRAM ASSIGNMENT 2                                        */   
/* FILE NAME : merge (merge.c)                                 */   
/* PROGRAM PURPOSE :                                           */
/*    The purpose of this program is to use a modified binary  */
/*	to find the index where an element should be inserted  */
/*	into a final array using shared memory.                */    
/* ----------------------------------------------------------- */  
#define _SVID_SOURCE 1
#include <stdio.h>
#include <stdlib.h>             
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int binary(int *arr, int left, int r, int x);

int main(int argc, char **argv) {
	if(argc < 5) {
		printf("Not enough arguments\n");
		exit(1);
	}
	/*Convert command-line arguments*/
	int mID = atoi(argv[1]);
	int mID2 = atoi(argv[2]);
	int m = atoi(argv[3]);
	int n = atoi(argv[4]);
	
	/*Attach shared memory to process*/
	int *mPTR = (int *) shmat(mID, NULL, 0);
	int *mPTR2 = (int *) shmat(mID2, NULL, 0);
	
	int i, status, pid;

	/*Create (m+n) processes and give them value from x or y array*/
	for(i = 0; i < (m+n); i++) {
		if((pid = fork()) == 0) {
			if(i < m) {
				printf("      $$$ M-PROC(%d): handling x[%d] = %d\n", getpid(), i, mPTR[i]);
				if(mPTR[i] < mPTR[m]) {
					/*Value is less than all values in the other array*/
					printf("      $$$ M-PROC(%d): x[%d] = %d is smaller than y[0] = %d\n", getpid(), i, mPTR[i], mPTR[m]);
					printf("      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n", getpid(), i, mPTR[i], i);
					mPTR2[i] = mPTR[i];
					exit(0);
				}
				else if(mPTR[i] > mPTR[(m + (n-1))]) {
					/*Value is greater than all values in the other array*/
					printf("      $$$ M-PROC(%d): x[%d] = %d is found to be larger than y[%d] = %d\n", getpid(), i, mPTR[i], (m + (n-1)), mPTR[(m + (n-1))]);
					printf("      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n", getpid(), i, mPTR[i], i+n);
					mPTR2[i+n] = mPTR[i];
					exit(0);
				}
				else {
					/*Find out what indices the value is between*/
					int k = binary(mPTR, m, (m +(n-1)), mPTR[i]);
					printf("      $$$ M-PROC(%d): x[%d] = %d is found to be between y[%d] = %d and y[%d] = %d\n", getpid(), i, mPTR[i], k-m, mPTR[k], (k-m)+1, mPTR[k+1]);
					int index = k - (m-1);
					printf("      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n", getpid(), i, mPTR[i], i+index);
					mPTR2[i+index] = mPTR[i];
					exit(0);
				}
			}
			else {
				printf("      $$$ M-PROC(%d): handling y[%d] = %d\n", getpid(), (i-m), mPTR[i]);
				if(mPTR[i] < mPTR[0]) {
					/*Value of element is less than all values of the other array*/
					printf("      $$$ M-PROC(%d): y[%d] = %d is found to be smaller than x[0] = %d\n", getpid(), (i-m), mPTR[i], mPTR[0]);
					printf("      $$$ M-PROC(%d): about to write y[%d] = %d into position %d of the output array\n", getpid(), (i-m), mPTR[i], (i-m));
					mPTR2[(i-m)] = mPTR[i];
					exit(0);
				}
				else if(mPTR[i] > mPTR[m-1]) {
					/*Value of element is greater than the largest element in other array*/
					printf("      $$$ M-PROC(%d): y[%d] = %d is found to be larger than x[%d] = %d\n", getpid(), (i-m), mPTR[i], m-1, mPTR[m-1]);
					printf("      $$$ M-PROC(%d): about to write y[%d] = %d into position %d of the output array\n", getpid(), (i-m), mPTR[i], i);
					mPTR2[i] = mPTR[i];
					exit(0);
				}
				else {
					/*Find what indices the value is between*/
					int k = binary(mPTR, 0, m-1, mPTR[i]);
					printf("      $$$ M-PROC(%d): y[%d] = %d is found between x[%d] = %d and x[%d] = %d\n", getpid(), (i-m), mPTR[i], k, mPTR[k], k+1, mPTR[k+1]);
					printf("      $$$ M-PROC(%d): about to write y[%d] = %d into position %d of the output array\n", getpid(), (i-m), mPTR[i], (i-(m-1))+k);	
					mPTR2[((i-(m-1)) + k)] = mPTR[i];
					exit(0);
				}
			}
		}
		
		else if(pid < 0) {
			printf("Error in fork\n");
			exit(0);
		}
	}
	
	/*Wait for all children processes*/
	int j;
	for(j = 0; j < (m+n); j++) {
		wait(&status);
	}
	
	/*Detach and remove shared memory*/
	shmdt((void *) mPTR);
	shmctl(mID, IPC_RMID, NULL);
	shmdt((void *) mPTR2);
	shmctl(mID2, IPC_RMID, NULL);
	return 0;	
}

/* ----------------------------------------------------------- */
/* FUNCTION  Modified Binary Search : (binary)                 */
/*     to find index of where element should be inserted       */
/* PARAMETER USAGE :                                           */
/*    *arr -> int array shared memory segment                  */
/*    left -> index where to start search 	   	       */
/*    r ->  index where to end search   		       */
/*    x ->  value of element we wish to insert                 */
/* FUNCTION CALLED :                                           */
/*    N/a                                                      */
/* ----------------------------------------------------------- */
int binary(int *arr, int left, int r, int x) {
	while(left < r) {
		
		/*If element is between two indices return left-most index*/
		if(arr[left] <= x && arr[left+1] >= x) {
			return left;
		}
		
		int m = (left + r) / 2;
		
		if(arr[m] < x) { /*If array element is less than insert element make left equal middle index*/
			left = m;
		}
		
		else { /*If array element is greater than insert element make right equal middle index*/
			r = m;
		}
	}
	return -1;
}
