/*
 * FileFunction.c
 *
 *  Created on: Aug 10, 2020
 *      Author: linuxu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FileFunction.h"
#include "Fp4.h"

char* readFromFile() {
	FILE *fp;
	char *buffer;
	long numbytes;

	fp = fopen(FILEPATH, "r");
	if (fp == NULL)
		return NULL;

	fseek(fp, 0L, SEEK_END);
	numbytes = ftell(fp);

	fseek(fp, 0L, SEEK_SET);

	buffer = (char*) calloc(numbytes, sizeof(char));
	if (buffer == NULL)
		return NULL;

	fread(buffer, sizeof(char), numbytes, fp);
	fclose(fp);
	return buffer;
}

char** splitString(double *w1, double *w2, double *w3, double *w4,
		int *numOfSeq2, char **seq1, char **buffer) {
	char **seq2;
	int i;
	char delim1[] = " ";
	char delim2[] = "\n";
	char *temp;
	char *ptr;
	
	temp = (char*) malloc(sizeof(char) * SIZEOFSEQ1);
	if(temp == NULL){
		printf("error malloc temp");
		return NULL;
	}

	temp = strtok(*buffer, delim1);
	*w1 = strtod(temp, &ptr);

	for (i = 1; i < 4; i++) {
		switch (i) {

		case 1:
			temp = strtok(NULL, delim1);
			*w2 = strtod(temp, &ptr);
			break;
		case 2:
			temp = strtok(NULL, delim1);
			*w3 = strtod(temp, &ptr);
			break;
		case 3:
			temp = strtok(NULL, delim2);
			*w4 = strtod(temp, &ptr);
			break;
		}
	}
	for (i = 0; i < 2; i++) {
		switch (i) {
		case 0:
			temp = strtok(NULL, delim2);
			*seq1 = (char*) malloc(SIZEOFSEQ1 * sizeof(char));
			if (*seq1 == NULL) {
				printf("error malloc seq1");
				return NULL;
			}

			strncat(*seq1, temp, strlen(temp) - 1);
			break;
		case 1:
			temp = strtok(NULL, delim2);
			*numOfSeq2 = atoi(temp);
			break;
		}
	}
	seq2 = (char**) malloc((SIZEOFSEQ2) * sizeof(char) * (*numOfSeq2));
	if (seq2 == NULL) {
		printf("error malloc **seq2");
		return NULL;
	}
	for (int i = 0; i < *numOfSeq2; i++) {
		seq2[i] = (char*) malloc(SIZEOFSEQ2 * sizeof(char));
		if (seq2 == NULL){
			printf("error malloc *seq2");
			return NULL;
		}
	}
	for (i = 0; i < *numOfSeq2; i++) {
		temp = strtok(NULL, delim2);
		strncat(seq2[i], temp, strlen(temp) - 1);

	}
	return seq2;
}

int writeToFile(char *endResult){
	FILE *fp;

	fp = fopen(OUTFILEPATH, "w");
    if (fp == NULL) {
        printf("Error! fp open");
	return EXIT_FAILURE;
    }


	fprintf(fp,"%s",endResult);
	fclose(fp);
	return EXIT_SUCCESS;
}
