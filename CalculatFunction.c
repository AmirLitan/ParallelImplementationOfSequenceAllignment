/*
 * CalculatFunction.c
 *
 *  Created on: Aug 10, 2020
 *      Author: linuxu
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CalculatFunction.h"
#include "openMpfunction.h"
#include "Fp4.h"
#include <omp.h>

int comperSeq1ToSeq2(char *seq1, char *seq2, double w1, double w2, double w3,
		double w4, int *offset, int *ms, int my_rank) {

	char *msSeq2;
	int i, k, offsetLen, sizeSeq2, sizeMsSeq2;
	double score = 0, scoreArr;

	offsetLen = strlen(seq1) - strlen(seq2);
	sizeSeq2 = strlen(seq2);
	sizeMsSeq2 = sizeSeq2;
	
	for (i = 0; i < offsetLen; i++) {
		printf("%f%%-----%d\n", (float) (100 * i) / offsetLen, my_rank);
		for (k = 1; k <= sizeSeq2; k++) {
			msSeq2 = setMs(seq2, k);
			if (msSeq2 == NULL) {
				printf("Error at msSeq2");
				return EXIT_FAILURE;
			}

			scoreArr = ompComper(seq1, msSeq2,i, sizeMsSeq2, w1, w2,
					w3, w4);

			if (score < scoreArr) {
				score = scoreArr;
				*offset = i;
				*ms = k;
			}

		}
		free(msSeq2);
	}
	return EXIT_SUCCESS;
}

//bilding Ms from seq2
char* setMs(char *seq2, int k) {
	char *tempSet;
	char sing = '-';
	if ((tempSet = (char*) malloc(sizeof(char) * SIZEOFSEQ2)) == NULL) {
		printf("error malloc tempSet");
		return NULL;
	}
	memset(tempSet, 0, SIZEOFSEQ2);
	strncat(tempSet, &seq2[0], k);
	strncat(tempSet, &sing, 1);
	strncat(tempSet, &seq2[k], strlen(seq2) - k);

	return tempSet;
}
ncat(tempSet, &seq2[k], strlen(seq2) - k);

	return tempSet;
}
