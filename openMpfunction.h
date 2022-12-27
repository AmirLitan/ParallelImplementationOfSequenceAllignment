/*
 * openMpfunction.h
 *
 *  Created on: Aug 31, 2020
 *      Author: linuxu
 */

#ifndef SRC_OPENMPFUNCTION_H_
#define SRC_OPENMPFUNCTION_H_

double ompComper(char *seq1, char *seq2, int offset, int sizeSeq2,
		double w1, double w2, double w3, double w4);
char* biledSings(char *seq1, char *seq2, int size, int offset);
double weightOfSigns(char *signs,int sizeSigns, double w1, double w2, double w3, double w4);
int checkConservativeGroup(char seq1, char seq2, int typeConservativeGroup);

#endif /* SRC_OPENMPFUNCTION_H_ */
