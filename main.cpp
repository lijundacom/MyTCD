/*
 * main.cpp
 *
 *  Created on: Apr 25, 2016
 *      Author: ljd
 */
#include "image/utility/ThreadManage.h"
#include "stdio.h"
int main(){
	ThreadSpace::InitALLThreads();
	getchar();
	return 0;
}

