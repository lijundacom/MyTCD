#ifndef READFILE_H
#define READFILE_H
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
	signed short int* ReadFile(char* fileName,int deepPoint,int ensemble,long IQOffset);
	void PrintArray(int deepPoint,int  ensemble,signed short int* IQArray);
	void PrintArray(int deepPoint,signed short int* Array);
#endif
