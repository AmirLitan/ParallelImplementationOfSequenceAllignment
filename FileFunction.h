/*
 * FileFunction.h
 *
 *  Created on: Aug 10, 2020
 *      Author: linuxu
 */

#ifndef FILEFUNCTION_H_
#define FILEFUNCTION_H_



char* readFromFile();
char** splitString(double *w1, double *w2, double *w3, double *w4,
		int *numOfSeq2, char **seq1, char **buffer);
int writeToFile(char *endResult);


#endif /* FILEFUNCTION_H_ */
