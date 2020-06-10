/* ----------------------------------------------------------- */
/* NAME : Kyla Kane-Maystead                 User ID: kakanema */
/* DUE DATE : 02/24/2020                                       */
/* PROGRAM ASSIGNMENT 2                                        */
/* FILE NAME : main (main.c)                                   */
/* PROGRAM PURPOSE :                                           */
/*    The purpose of this program is to utilize shared memory  */
/*	between a parent and child processes. The shared memory*/
/*	will be used for a quicksort and binary merge of two   */
/*	arrays. 					       */
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
#include <errno.h>

int main(int argc, char**argv) {
	int k, m, n;
	printf("Quicksort and Binary Merge with Multiple Processes\n");
	/*Read number of k elements from input file*/
	scanf("%d", &k);	

	/*Create key for shared memory*/
	key_t qKey = ftok("./main", 'q');
	printf("*** MAIN: qsort shared memory key = %d\n", qKey);
	
	/*Allocate memory size of k ints*/
	int qID = shmget(qKey, k*sizeof(int), IPC_CREAT | 0666);
	printf("*** MAIN: qsort shared memory created\n");
	
	/*Attach memory to parent process*/
	int *qPTR = (int *) shmat(qID, NULL, 0);
	if(qPTR == (void *)-1) {
		printf("Error in attaching shared memory\n");
		exit(1);
	}
	printf("*** MAIN: qsort shared memory attached and is ready to use\n");

	/*Read array a elements from input file and put into quicksort shared memory*/
	int i, num, num1, num2;
	for(i = 0; i < k; i++) {
		scanf("%d", &num);
		qPTR[i] = num;
		printf("%d ", qPTR[i]);
	}
	
	/*Read number of elements and array elements from file and save to array*/
	scanf("%d", &m);
	int x[m];
	for(i = 0; i < m; i++) {
		scanf("%d", &num1);
		x[i] = num1;
		printf("%d ", x[i]);
	}

	/*Read number of elements and array elements from file and save to array*/
	scanf("%d", &n);
	int y[n];
	int j;
	for(j = 0; j < n; j++) {
		scanf("%d", &num2);
		y[j] = num2;
	}
	
	/*Create key for merge shared memory segment*/
	key_t mKey = ftok("./main", 'm');
	printf("*** MAIN: merge shared memory key = %d\n", mKey);
	
	/*Allocate memory size of (m+n) ints*/
	int mID = shmget(mKey, (m + n)*sizeof(int), IPC_CREAT | 0666);
	printf("*** MAIN: merge shared memorty created\n");
	
	/*Attach memory to parent process*/
	int *mPTR = (int *) shmat(mID, NULL, 0);
	printf("*** MAIN: merge shared memory attached and is ready to use\n");
	
	/*Create key for extra merge shared memory segment*/
	key_t mKey2 = ftok("./merge", 'm');
	printf("*** MAIN: temporary merge shared memory key = %d\n", mKey2);
	
	/*Allocate memory of size (m+n)*/
	int mID2 = shmget(mKey2, (m + n)*sizeof(int), IPC_CREAT | 0666);
	printf("*** MAIN: temporary merge shared memory created\n");

	/*Attach memory to parent process*/
	int *mPTR2 = (int *) shmat(mID2, NULL, 0);
	printf("*** MAIN: temporary merge shared memory attached and is ready to use\n");

	/*Print quicksort array elements*/
	printf("Input array for qsort has %d elements:\n", k);
        printf("    ");
	for(i = 0; i < k; i++) {
		printf("%d ", qPTR[i]);
	}
	printf("\n"); 
	
	/*Print array elements for x[]*/
	printf("Input array x[] for merge has %d elements:\n", m);                         printf("    ");
	for(i = 0; i < m; i++) {
		mPTR[i] = x[i];
		printf("%d ", mPTR[i]);
	}
	printf("\n");
	
	/*Print array elements for y[]*/
	printf("Input array y[] for merge has %d elements:\n", n);
	printf("    ");
	int index = 0;
	for(i = m; i < (m+n); i++) {
		mPTR[i] = y[(i - m)];
		printf("%d ", mPTR[i]);
	}
	printf("\n");

	int qpid, mpid, status, wpid;
	
	/*Convert int arguments to strings*/
	char buf[20];
	sprintf(buf, "%d", k-1);
	
	char qIDBuf[20];
	sprintf(qIDBuf, "%d", qID);
	
	/*Prepare command-line arguments for quicksort exec*/
	char *qargs[] = {"./qsort", qIDBuf, "0", buf, (char*)0};	
	if((qpid = fork()) == 0) {
		printf("*** MAIN: about to spawn the process for qsort\n");
		if(execvp(*qargs, qargs) < 0) {
			printf("Exec Failed\n");
			exit(1);
		}				
	}
	
	/*Convert int arguments to strings*/
	char mbuf[20];
	sprintf(mbuf, "%d", m);
	
	char nbuf[20];
	sprintf(nbuf, "%d", n);
	
	char mIDBuf[20];
	sprintf(mIDBuf, "%d", mID);

	char mID2Buf[20];
	sprintf(mID2Buf, "%d", mID2);
	
	/*Prepare command-line arguments for binary merge exec*/
	char *margs[] = {"./merge", mIDBuf, mID2Buf, mbuf, nbuf, (char*)0};
	if((mpid = fork()) == 0) {
		printf("*** MAIN: about to spawn the process for merge\n");
		if(execvp(*margs, margs) < 0) {
			printf("Exec Failed\n");
			exit(1);
		}	
	}
	
	/*Wait for children to finish*/
	while((wpid = wait(&status)) > 0);

	/*Print results for quicksort*/
	printf("*** MAIN: sorted array by qsort:\n");
	printf("     ");
	for(i = 0; i < k; i++) {
		printf("%d ", qPTR[i]);
	}		
	printf("\n");

	/*Print results for binary merge*/
	printf("*** MAIN: merged array:\n");
	printf("     ");
	for(i = 0; i < (m + n); i++) {
		printf("%d ", mPTR2[i]);
	}
	printf("\n");
	
	/*Detach quicksort shared memory*/
	shmdt((void *) qPTR);
	printf("*** MAIN: qsort shared memory successfully detached\n");
	
	/*Remove quicksort shared memory*/
	shmctl(qID, IPC_RMID, NULL);
	printf("*** MAIN: qsort shared memory successfully removed\n");
	
	/*Detach binary merge shared memory*/
	shmdt((void *) mPTR);
	printf("*** MAIN: merge shared memory successfully detached\n");
	
	/*Remove binary merge shared memory*/
	shmctl(mID, IPC_RMID, NULL);
	printf("*** MAIN: merge shared memory successfully removed\n");

	/*Detach extra binary merge shared memory*/
	shmdt((void *) mPTR2);
	printf("*** MAIN: temporary merge shared memory successfully detached\n");	

	/*Remove extra binary merge shared memory*/
	shmctl(mID2, IPC_RMID, NULL);
	printf("*** MAIN: temporary merge shared memory successfully removed\n");
	return 0;
}
