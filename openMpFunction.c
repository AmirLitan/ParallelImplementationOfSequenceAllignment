/*
 * openMpFunction.c
 *
 *  Created on: Aug 31, 2020
 *      Author: linuxu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CalculatFunction.h"
#include "openMpfunction.h"
#include <omp.h>



double ompComper(char *seq1, char *seq2, int offset, int sizeSeq2,
		double w1, double w2, double w3, double w4) {

	double weight;
	char* sings;

	sings = biledSings(seq1,seq2,sizeSeq2,offset);


	weight = weightOfSigns(sings,sizeSeq2,w1,w2,w3,w4);

	free(sings);

	return weight;


}

char* biledSings(char *seq1, char *seq2, int size, int offset) {
	int i;
	char *sings;
	sings = (char*)malloc(sizeof(char)*(size+1));
	if(sings == NULL){
		printf("Error allocating memory at \"%s\" function\n", __func__);
		return NULL;
	}
omp_set_num_threads(4);
#pragma omp parallel for
	for (i = 0; i < size; i++) {
		if (seq1[offset + i] == seq2[i]) {
			sings[i] = '*';
		} else if (checkConservativeGroup(seq1[offset + i], seq2[i], 1)) {
			sings[i] = ':';
		} else if (checkConservativeGroup(seq1[offset + i], seq2[i], 2)) {
			sings[i] = '.';
		} else {
			sings[i] = ' ';
		}
	}
	sings[size] = '\0';
	return sings;
}

double weightOfSigns(char *signs,int sizeSigns, double w1, double w2, double w3, double w4) {
	double sum = 0;
	int i;
	for (i = 0; i < sizeSigns; i++) {
		if ((signs[i]) == '*') {
			sum += w1;
		} else if (signs[i] == ':') {
			sum -= w2;
		} else if (signs[i] == '.') {
			sum -= w3;
		} else if (signs[i] == ' ') {
			sum -= w4;
		}
	}
	return sum;
}

int checkConservativeGroup(char seq1, char seq2, int typeConservativeGroup) {
	
	char conservativeGroup[][7] = { "NDEQ", "NEQK", "STA", "MILV", "UHRK", "MHQK",
			"FYW", "HY", "MILF" };
	char semiConservativeGroup[][7] = { "SAG", "ATV", "CSA", "SGND", "STPA", "STNK",
			"NEQHRK", "NDEQHK", "SNDEQK", "HFY", "FVLIM" };	
	
	
	int i, j, isMatch = 0, seq1Match = 0, seq2Match = 0, strEnd = 0;
	int numOfGroups = sizeof(conservativeGroup) / sizeof(*conservativeGroup);
	int numOfSemiGroups = sizeof(semiConservativeGroup)
			/ sizeof(*semiConservativeGroup);

	if (typeConservativeGroup == 1) {
		for (i = 0; i < numOfGroups && isMatch == 0; i++) {
			strEnd = 0;
			for (j = 0; j < sizeof(conservativeGroup[i]) && strEnd == 0; j++) {
				if (conservativeGroup[i][j] != ' ') {
					if (seq1 == conservativeGroup[i][j]) {
						seq1Match = 1;
					}
					if (seq2 == conservativeGroup[i][j]) {
						seq2Match = 1;
					}
				} else {
					strEnd = 1;
				}
			}
			if (seq1Match == 1 && seq2Match == 1) {
				isMatch = 1;
			}
			seq1Match = 0;
			seq2Match = 0;
		}
	} else {
		for (i = 0; i < numOfSemiGroups && isMatch == 0; i++) {
			strEnd = 0;
			for (j = 0; j < sizeof(conservativeGroup[i]) && strEnd == 0; j++) {
				if (conservativeGroup[i][j] != '\0') {
					if (seq1 == semiConservativeGroup[i][j]) {
						seq1Match = 1;
					}
					if (seq2 == semiConservativeGroup[i][j]) {
						seq2Match = 1;
					}
				} else {
					strEnd = 1;
				}
			}
			if (seq1Match == 1 && seq2Match == 1) {
				isMatch = 1;
			}
			seq1Match = 0;
			seq2Match = 0;
		}
	}
	return isMatch;
}
