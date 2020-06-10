/* ----------------------------------------------------------- */
/* NAME : Kyla Kane-Maystead                 User ID: kakanema */          
/* DUE DATE : 02/24/2020                                       */ 
/* PROGRAM ASSIGNMENT 2                                        */   
/* FILE NAME : qsort (qsort.c)                                 */   
/* PROGRAM PURPOSE :                                           */  
/*    The purpose of this program is to sort an array of       */
/*	integers using quicksort. It also utilizes multiple    */
/*	child processes to sort each section.    	       */  
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

/*Prototypes*/
int partition(int *arr, int left, int right);
void quickSort(int *arr, int left, int right);

int main(int argc, char **argv) {	
	if(argc < 4) {
		printf("Not enough arguments\n");
		exit(1);
	}
	/*Convert command-line arguments*/
	int qID = atoi(argv[1]);
	int left = atoi(argv[2]);
	int right = atoi(argv[3]);
	
	/*Only continue if the left index is less than the right index*/
	if(left < right) {
		
		/*Attach the shared memory segment to process with id passed through command-line*/
		int *qPTR = shmat(qID, NULL, 0);
		printf("   ### Q-PROC(%d): entering with a[%d..%d]\n", getpid(), left, right);
		/*Print section*/
		int i;
		int count = 0;
		for(i = left; i <= right; i++) {
			if(count == 0) {
				printf("       ");
			}
			printf("%d ", qPTR[i]);
			count++;
		}
		printf("\n");
		printf("   ### Q-PROC(%d): pivot element is a[%d] = %d\n", getpid(), left, qPTR[left]);	
			
		/*Call method to partition elements*/
		int pivot = partition(qPTR, left, right);
		printf("   ### Q-PROC(%d): section a[%d..%d] sorted\n", getpid(), left, right);
		/*Print sorted elements from section*/
		count = 0;
		for(i = left; i <= right; i++) {
			if(count == 0) {
				printf("       ");
			}
			printf("%d ", qPTR[i]);
			count++;
		}
		printf("\n");
		int lchild, rchild, status;
		
		/*Convert all ints to string to pass through execvp()*/
		char lLeftLimit[20];
		sprintf(lLeftLimit, "%d", left);
		
		char lRightLimit[20];
		sprintf(lRightLimit, "%d", pivot-1);
		
		char memID[20];
		sprintf(memID, "%d", qID);
		
		char rLeftLimit[20];
		sprintf(rLeftLimit, "%d", pivot+1);
		
		char rRightLimit[20];
		sprintf(rRightLimit, "%d", right);
		
		/*Set-up exec arguments for left and right child*/
		char *largs[] = {"./qsort", memID, lLeftLimit, lRightLimit, (char*)0};
		char *rargs[] = {"./qsort", memID, rLeftLimit, rRightLimit, (char*)0};
		int wpid;
		if((lchild = fork()) == 0) {
			/*If error with exec*/
			if(execvp(*largs, largs) < 0) {
				printf("Left Child Error in exec\n");
				exit(1);
			}
		}
		if((rchild = fork()) == 0) {
			/*If error with exec*/
			if(execvp(*rargs, rargs) < 0) {
				printf("Right Child Error in exec\n");
				exit(1);
			}
		}
	
		/*Wait for children*/
		for(i = 0; i < 2; i++) {
			wait(&status);
		}		
		
		/*Detach and remove memory segment*/
		shmdt((void *) qPTR);
		shmctl(qID, IPC_RMID, NULL);	
		printf("   ### Q-PROC(%d): exits\n", getpid());
	}
	return 0;	
}

/* ----------------------------------------------------------- */
/* FUNCTION  partition : (partition)                           */
/*     to sort elements less than pivot and greater than pivot */
/* PARAMETER USAGE :                                           */
/*    *arr -> shared memory int array                          */  
/*    left -> left index to start sorting                      */
/*    right -> right index of sorting section                  */
/* FUNCTION CALLED :                                           */
/*    N/a                                                      */
/* ----------------------------------------------------------- */
int partition(int *arr, int left, int right) {
	/*Save off pivot value*/
	int val = arr[left];
	
	int index = left;
	int j = right;
	int temp;
	
	while(index < j) {
		while(arr[index] <= val) { /*Increment left index when array value is less than or equal to pivot value*/
			index++;
		}
		while(arr[j] > val) { /*Decrement right index when array value is greater than pivot value*/
			j--;	
		}
		if(index < j) { /*Swap when left and right index array values*/
			temp = arr[index];
			arr[index] = arr[j];
			arr[j] = temp;
		}
	}
	arr[left] = arr[j];
	arr[j] = val;
	return j;
}
