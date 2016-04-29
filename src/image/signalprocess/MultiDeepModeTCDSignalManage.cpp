/*
 * MultiDeepModeTCDSignalManage.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: ljd
 */
#include "image/signalprocess/MultiDeepModeTCDSignalManage.h"
//#include "image/utility/DebugPrint.h"
#include <math.h>
#include <stdlib.h>



namespace SignalProcess
{

//define the pi as 3.1415
//static const float PI = 3.1415;

// 虽然上传的数据I和Q分别都为16位有符号整型数,按理来讲,Power = I*I + Q*Q应为30位无符号整型,
// 但是考虑到上传的IQ数据值普遍都小,所以,为了充分利用系统处理数据的精度,
// 将Power范围限定在0-0xFFFF，大于0xFFFF的Power都置为0xFFFF，这样再将Power值转移到dB域上，
// 同样用16位无符号整型表示，所以对于限定范围后的Power值，取dB后再乘以POWERDBCOEF，则完成将Power转到
// 对数域上，同时保证了信号的动态范围。  注: Power = I*I + Q*Q 已经为幅值的平方，则在求取对数时为10*log
// 而不是20 * log。
//const float POWERDBCOEF = 0xFFFF / (10 * log10(float(0xFFFF)));

MultiDeepModeTCDSignalManage::MultiDeepModeTCDSignalManage()
{
	m_nMDEnsemble = MAX_MD_ENSEMBLE;		//回归滤波器维数/自相关线数

	m_nMDPointsNum = DEEP_POINTS;	//每条线深度方向的点数
	m_pfMDWFCoef = (float*) malloc(16 * 16 * sizeof(float));
	LoadWallCoef(m_pfMDWFCoef, m_nMDEnsemble,m_nMDWFIndex);
}

MultiDeepModeTCDSignalManage::~MultiDeepModeTCDSignalManage()
{
	if (NULL != m_pfMDWFCoef)
	{
		free(m_pfMDWFCoef);
	}
}

// ------------------------------------------------------------
// Description	:设置C模式的参数
// Parameter	:
//	param-C模式的参数
// Retrun Value	:void
// ------------------------------------------------------------
void MultiDeepModeTCDSignalManage::SetMultiDeepModeTCDParam()
{
	//壁滤波系数
	LoadWallCoef(m_pfMDWFCoef, m_nMDEnsemble,m_nMDWFIndex);
	//设置各种参数，实际过程中应从xml中读取
	//上层使用android写xml配置文件
	//下层使用c++直接读取
}

// ------------------------------------------------------------
// Description	:获取C模式的参数
// Parameter	:
//	param-C模式的参数
// Retrun Value	:void
// ------------------------------------------------------------
void MultiDeepModeTCDSignalManage::GetMultiDeepModeTCDParam()
{

}

// ------------------------------------------------------------
// Description	:加载壁滤波系数
// Parameter	:
//		fpCoef-壁滤波系数
//		nEnsemble-每一个点的数据采样个数，从4到16条
//		nCWFIndex-壁滤波滤波器截止频率索引(这里使用回归滤波器，该参数代表回归滤波器选取的多项式阶数D,取值范围[3 5])
// Retrun Value	:
//		0-加载壁滤波器系数成功
//		-1-加载壁滤波系数失败，找不到文件
// ------------------------------------------------------------
int MultiDeepModeTCDSignalManage::LoadWallCoef(float* fpCoef, int nEnsemble, int nMDWFIndex)
{
	//这里使用回归滤波器

	//check the nCWFIndex
	if((3>nMDWFIndex )||(6<nMDWFIndex ))
	{
		nMDWFIndex = 3;
	}

	//set the file path
	FILE* fp;
	char cPath[50];
	sprintf(cPath, "/home/ljd/data/WallFilter/CWF_D%dK%d", nMDWFIndex, nEnsemble);

	//open the file
	fp = fopen(cPath, "rb");
	if (fp == NULL)
	{
		//LOGI( "LoadWallCoef: open the file failed....%d\n", fp);
		printf( "LoadWallCoef: open the file failed....%d\n", fp);
		return -1;
	} else
	{
		//LOGI( "LoadWallCoef: open the file true...%d\n", fp);
	}

	//read the coef from the file
	char str[20];
	for (int i = 0; i < nEnsemble * nEnsemble; i++)
	{
		fgets(str, 20, fp);
		fpCoef[i] = atof(str);
		//LOGI( "open fpCoef [ %d ] is %f" , i, fpCoef [ i ]);
	}

	//close the file
	fclose(fp);

	return 0;
}

// ------------------------------------------------------------
// Description	:C模式的信号处理，属于CFM模式
// Parameter	:
//	pSrc-待处理数据
//	pDst_v-输出的速度矩阵指针
// Retrun Value	:void
// ------------------------------------------------------------
void MultiDeepModeTCDSignalManage::SignalProcess(INT16 *pSrc, float *pDst_v)
{
	int nEnsemble = m_nMDEnsemble;
	int nPoints = m_nMDPointsNum;

	//壁滤波（回归滤波器系数/原始数据nPoints*nEnsemble/滤波后数据nPoints*nEnsemble/滤波阶数/每个深度的点数）
	MultiDeepModeTCDWallFilter( m_pfMDWFCoef, pSrc ,m_fAfterCWF ,nEnsemble , nPoints);
	//ColorWallFilter_Neon(m_pfCWFCoef, pSrc, m_fAfterCWF, nEnsemble, nLines, nPoints);
	//printf("m_fAfterCWF\n");
	//PrintArray(nPoints,nEnsemble,m_fAfterCWF);
	//自相关（滤波后的数据/输出的速度数据/滤波阶数/每个深度的点数）
	MultiDeepModeTCDAutoCorrelation(m_fAfterCWF, m_nVelocityAfterAC, nEnsemble,nPoints);
	for(int i=0;i<nPoints;i++){
		pDst_v[i]=m_nVelocityAfterAC[i];
	}
	//printf("m_nVelocityAfterAC\n");
	//PrintArray(nPoints,m_nVelocityAfterAC);
	//CAutoCorrelation_Neon(m_fAfterCWF, m_nVelocityAfterAC, m_unPowerAfterAC, m_unVarianceAfterAC, nEnsemble, nLines, nPoints, m_nCovarianceCoef, m_fDigitalGain, m_nStartPointOfGain);

//	//速度阀值
//	VelocityThreshold(m_nVelocityOut, nPoints, m_nVelocityThreshold);
//
//	//速度平滑
//	VelocitySmooth(m_nVelocityOut, m_nVAfterSmooth, m_pSmoothWeightCoeff, nPoints, m_nSmoothWeightCoeffRow,
//			m_nSmoothWeightCoeffCol, m_nVelocityThreshold);
//	//速度相关
//	VelocityCorrelation(m_nVAfterSmooth, m_nVPersistCache, nPoints, m_nPersist);
//	//速度平滑
//	VelocitySmooth(m_nVAfterSmooth, pDst_v, m_pSmoothWeightCoeff, nPoints, m_nSmoothWeightCoeffRow,
//			m_nSmoothWeightCoeffCol, m_nVelocityThreshold);
//
//	if (0 == m_nMorphologyFlag)
//	{
//		VelocityThreshold(pDst_v, nPoints, m_nVelocityThreshold);
//	}


	//pDst_v与m_nVelocityOut转换的函数
}


}	//the end of namespace of SignalProcess


