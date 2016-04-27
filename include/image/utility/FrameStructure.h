/*
 * FrameStructure.h
 *
 *  Created on: Apr 22, 2016
 *      Author: ljd
 */

#ifndef FRAMESTRUCTURE_H_
#define FRAMESTRUCTURE_H_
#include <list>
#include <string>
#include <cstring>
#include <algorithm>

#include "ImageDef.h"
struct MultiDeepModeTCDFrameData
{

	 struct{
		 signed char v[MAX_BLINE_POINTS]; //速度数据
	 }data;
};
struct IQPoint{
	signed short int IData;
	signed short int Qdata;
};


#endif /* FRAMESTRUCTURE_H_ */
