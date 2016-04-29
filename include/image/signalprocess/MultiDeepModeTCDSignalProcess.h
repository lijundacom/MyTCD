/*
 * MultiDeepModeTCDSignalProcess.h
 *
 *  Created on: Apr 22, 2016
 *      Author: ljd
 */

#ifndef MULTIDEEPMODETCDSIGNALPROCESS_H_
#define MULTIDEEPMODETCDSIGNALPROCESS_H_
#include "image/utility/ImageDef.h"
#include "image/GlobalDefine.h"
#include "image/utility/TimerTest.h"
#include "image/mode/MultiDeepModeTCDThread.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
extern int fprf;//采样频率
extern float cosTh;//cos(45),45为探头与血流角度
extern int transferFrequence;//探头频率
//#include "arm_neon.h"
// ------------------------------------------------------------
// Description	:color数据自相关计算
// Parameter	:
//		m_fAfterCWF:壁滤波之后的矩阵[nPoints*nEnsemble]
//		m_nVelocityAfterAC:自相关之后的速度矩阵[nPoints*1]
//		nEnsemble:自相关阶数
//		nPoints:每个深度的点数
// Retrun Value	:
//		0-计算正确
//		-1-输入数据指针为空,有错误
// ------------------------------------------------------------
int MultiDeepModeTCDAutoCorrelation(float* m_fAfterCWF,float* m_nVelocityAfterAC,int nEnsemble,int nPoints);
//int MultiDeepModeTCDAutoCorrelation(float* m_fAfterCWF,double* m_nVelocityAfterAC,int nEnsemble,int nPoints);
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
int MultiDeepModeTCDWallFilter(const float *pCWFCoef, signed short int *pSrc, float *pDest, int nEnsemble,
					int nPointAmount);
int MultiDeepModeTCDWallFilter_Neon(const float *pCWFCoef, int *pSrc, float *pDest, unsigned int nEnsemble,
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
