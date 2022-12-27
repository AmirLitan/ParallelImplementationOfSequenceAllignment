/*
 ============================================================================
 Name        : Fp4.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello MPI World in C 
 ============================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"
#include "FileFunction.h"
#include "CalculatFunction.h"
#include "Fp4.h"

int main(int argc, char *argv[]) {
	int my_rank; /* rank of process */
	int p; /* number of processes */
	int source; /* rank of sender */
	int dest; /* rank of receiver */
	int tag = 0; /* tag for messages */
	MPI_Status status; /* return status for receive */
	char *buffer;
	double w1, w2, w3, w4;
	int numOfSeq2, i, *offset, *ms, sizeOfBuffer, sizeOfEndResult;
	char *seq1;
	char **seq2;
	char *endResult, *endTemp, *endTemp2;

	/* start up MPI */

	MPI_Init(&argc, &argv);

	/* find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if (my_rank != 0) {

		source = 0;
		dest = 0;

		tag = 1;
		MPI_Recv(&sizeOfBuffer, 1, MPI_INTEGER, source, tag,
		MPI_COMM_WORLD, &status);

		buffer = (char*) malloc(sizeof(char) * sizeOfBuffer+1);
		if (buffer == NULL){
			printf("Error 1 allocating memory at \"%s\" function\n", __func__);
			return EXIT_FAILURE;
		}

		tag = 2;
		MPI_Recv(buffer, sizeOfBuffer+1, MPI_CHAR, source, tag,
		MPI_COMM_WORLD, &status);


		////Spliting the string "buffer" to data
		seq2 = splitString(&w1, &w2, &w3, &w4, &numOfSeq2, &seq1, &buffer);
		if(seq2 == NULL){
			printf("Error spliting string at \"%s\" function\n", __func__);
			return EXIT_FAILURE;
		}

		if ((offset = (int*) malloc(sizeof(int) * numOfSeq2)) == NULL){
			printf("Error 2 allocating memory at \"%s\" function\n", __func__);
			return EXIT_FAILURE;
		}

		if ((ms = (int*) malloc(sizeof(int) * numOfSeq2)) == NULL){
			printf("Error 3 allocating memory at \"%s\" function\n", __func__);
			return EXIT_FAILURE;
		}

		//Comper seq1 to seq2 and return result
		for (i = numOfSeq2/p; i < numOfSeq2; i++) {
			if(comperSeq1ToSeq2(seq1, seq2[i], w1, w2, w3, w4, &offset[i],&ms[i], 					my_rank)){
				printf("Error 4 allocating memory at \"%s\" function\n", __func__);
				return EXIT_FAILURE;
			}	
		}

		//Convert results to String
		endResult = (char*) malloc(sizeof(char) * 100);
		endTemp = (char*) malloc(sizeof(char) * 100);
		for (i = numOfSeq2 / 2; i < numOfSeq2; i++) {
			if (i == numOfSeq2 / 2) {
				sprintf(endTemp, "seq2[%d] offset = |%d| MS(%d)\n", i,
						offset[i], ms[i]);
				strcpy(endResult, endTemp);
			} else {
				
				sprintf(endTemp, "seq2[%d] offset = |%d| MS(%d)\n", i,
						offset[i], ms[i]);
				strcat(endResult, endTemp);
			}
		}
		//sending result to parent
		sizeOfEndResult = strlen(endResult);
		tag = 3;
		MPI_Send(&sizeOfEndResult, 1, MPI_INT, dest, tag,
		MPI_COMM_WORLD);
		tag = 4;
		MPI_Send(endResult, sizeOfEndResult + 1, MPI_CHAR, dest, tag,
		MPI_COMM_WORLD);

		//free memory
		free(endResult);
		free(endTemp);

	} else {

		dest = 1;
		source = 1;

		//Reading from file
		if ((buffer = readFromFile()) == NULL) {
			printf("Error reading from file at \"%s\" function\n", __func__);
			return EXIT_FAILURE;
		}

		sizeOfBuffer = strlen(buffer);
		tag = 1;
		MPI_Send(&sizeOfBuffer,1, MPI_INTEGER, dest, tag,
		MPI_COMM_WORLD);
		tag = 2;
		MPI_Send(buffer, sizeOfBuffer + 1, MPI_CHAR, dest, tag,
		MPI_COMM_WORLD);

		//Spliting the string "buffer" to data
		seq2 = splitString(&w1, &w2, &w3, &w4, &numOfSeq2, &seq1, &buffer);
		if(seq2 == NULL){
			printf("Error spliting string at \"%s\" function\n", __func__);
			return EXIT_FAILURE;
		}

		if ((offset = (int*) malloc(sizeof(int) * numOfSeq2)) == NULL){
			printf("Error 5 allocating memory at \"%s\" function\n", __func__);
			return EXIT_FAILURE;
		}

		if ((ms = (int*) malloc(sizeof(int) * numOfSeq2)) == NULL){
			printf("Error 6 allocating memory at \"%s\" function\n", __func__);
			return EXIT_FAILURE;
		}

		//Comper seq1 to seq2 and return result
		for (i = 0; i < numOfSeq2 / p; i++) {

			if(comperSeq1ToSeq2(seq1, seq2[i], w1, w2, w3, w4, &offset[i],&ms[i], 					my_rank)){
				printf("Error 7 allocating memory at \"%s\" function\n", __func__);
				return EXIT_FAILURE;
			}
		}

		//Convert results to String
		endResult = (char*) malloc(sizeof(char) * 100*numOfSeq2);
		endTemp = (char*) malloc(sizeof(char) * 100);

		for (i = 0; i < numOfSeq2 / 2; i++) {
			if (i == 0) {
				sprintf(endTemp, "seq2[%d] offset = |%d| MS(%d)\n", i,
						offset[i], ms[i]);
				strcpy(endResult, endTemp);
			} else {
				//
				sprintf(endTemp, "seq2[%d] offset = |%d| MS(%d)\n", i,
						offset[i], ms[i]);
				strcat(endResult, endTemp);
			}
		}

		
		//receive finel result from child
		tag = 3;
		MPI_Recv(&sizeOfEndResult, 1, MPI_INTEGER, source, tag,
		MPI_COMM_WORLD, &status);

		tag = 4;
		endTemp2 = (char*) malloc(sizeof(char) * sizeOfEndResult);
		if (endTemp2 == NULL){
			printf("Error 8 allocating memory at \"%s\" function\n", __func__);
			return EXIT_FAILURE;
		}


		MPI_Recv(endTemp2, sizeOfEndResult+1, MPI_CHAR, source, tag,
		MPI_COMM_WORLD, &status);

		strcat(endResult, endTemp2);

		printf("end result :: \n%s", endResult);

		//weite to file end result
		if(writeToFile(endResult)){
			printf("Error writing to file at \"%s\" function\n", __func__);
			return EXIT_FAILURE;
		}
		
		//free memory
		free(endResult);
		free(endTemp);
		free(endTemp2);
	}
	//free all memory
	free(ms);
	free(offset);
	free(seq1);
	for (i = 0; i < numOfSeq2; i++) {
		free(seq2[i]);
	}
	free(seq2);
	free(buffer);
	/* shut down MPI */
	MPI_Finalize();
	printf("END END");
	return EXIT_SUCCESS;
}
