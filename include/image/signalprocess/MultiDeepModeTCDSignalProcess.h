/*
 * MultiDeepModeTCDSignalProcess.h
 *
 *  Created on: Apr 22, 2016
 *      Author: ljd
 */

#ifndef MULTIDEEPMODETCDSIGNALPROCESS_H_
#define MULTIDEEPMODETCDSIGNALPROCESS_H_
#include "image/GlobalDefine.h"
#include "image/utility/TimerTest.h"
#include "image/mode/MultiDeepModeTCDThread.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
//#include "arm_neon.h"
// ------------------------------------------------------------
// Description	:color数据自相关计算
// Parameter	:
//		float *dstCWF         ---自相关计算输入数据
//		INT8 *out_v         ---输出速度份量
//		uint8_t *out_c        ---输出方差份量
//		uint16_t *out_p       ---输出能量份量
//		int16_t linesNum      ---自相关计算输入数据行数
//		int16_t pointsNum     ---自相关计算输入数据点数
//		uint32_t c_PRE_lt     ---自相关低阀值
//		uint32_t c_PRE_ht     ---自相关高阀值
// Retrun Value	:
//		0-计算正确
//		-1-输入数据指针为空,有错误
// ------------------------------------------------------------
int CAutoCorrelation();

// ------------------------------------------------------------
// Description	:壁滤波
// Parameter	:
//		pCWFCoef-滤波系数，大小为[nEnsemble*nEnsemble]
//		pSrc-输入的数据指针
//		pDest-返回计算结果的指针
//		nEnsemble-滤波因子维数
//		nLineAmount-线数
//		nPointAmount-点数
// Retrun Value	:
//		0-计算正确
//		-1-输入数据指针为空,有错误
// ------------------------------------------------------------
int ColorWallFilter(const float *pCWFCoef, int *pSrc, float *pDest, unsigned int nEnsemble,
					unsigned int nPointAmount);
int ColorWallFilter_Neon(const float *pCWFCoef, int *pSrc, float *pDest, unsigned int nEnsemble,
		unsigned int nPointAmount);

// ------------------------------------------------------------
// Description	:速度阀值
// Parameter	:
//		pSource-输入矩阵
//		nLineAmount-线数
//		nPointAmount-点数
//		nVelocityThreshold-速度阀值
// Retrun Value	:void
// ------------------------------------------------------------
void VelocityThreshold ( INT8 * pSource , int nPointAmount , int nVelocityThreshold );

// ------------------------------------------------------------
// Description	:速度帪相关处理函数,处理完成后将相关结果拷贝到输入数据指针
// Parameter	:
//		pNewFrame-输入的新的数据帧
//		pCache-前一帧数据的缓冲
//		nLineAmount-线数
//		nPointAmount-点数
//		VFPCoef-帧相关系数
// Retrun Value	:void
// ------------------------------------------------------------
void VelocityCorrelationOneFrame(INT8 *pNewFrame, INT8 *pCache,
								 int16_t nPointAmount, uint32_t nVFPCoef);

// ------------------------------------------------------------
// Description	:速度平滑，采用开立的算法
// Parameter	:
//		pSource-输入矩阵
//		pTarget-输出矩阵
//		pCoef-平滑系数矩阵
//		nLineAmount-线数
//		nPointAmount-点数
//		nSmoothWeightCoeffRow-行平滑阶次
//		nSmoothWeightCoeffCol-列平滑阶次
//		nThreshold-速度阀值
// Retrun Value	:
//		0-计算正确
//		-1-输入数据指针为空,有错误
// ------------------------------------------------------------
int VelocitySmooth(INT8 *pSrc,  INT8 *pDst, const float *pCoef,
				   int nPointAmount, int nSmoothWeightCoeffRow,int nSmoothWeightCoeffCol,unsigned int nThreshold);
#endif /* MULTIDEEPMODETCDSIGNALPROCESS_H_ */
