/*
 * CalculatFunction.h
 *
 *  Created on: Aug 10, 2020
 *      Author: linuxu
 */

#ifndef CALCULATFUNCTION_H_
#define CALCULATFUNCTION_H_

int comperSeq1ToSeq2(char *seq1, char *seq2, double w1, double w2,
		double w3, double w4, int *offset, int *ms,int my_rank);
char* setMs(char *seq2, int k);



#endif /* CALCULATFUNCTION_H_ */
