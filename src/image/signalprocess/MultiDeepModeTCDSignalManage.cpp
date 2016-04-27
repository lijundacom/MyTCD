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
//	static float fCWFCoef [ 16 * 16 ] = { 0.106060606060658 , - 0.242424242424264 , 0.0606060606060222 , 0.121212121212100 , 0.0454545454545552 , - 0.0606060606060281 ,
//			- 0.0909090909090633 , 0.0606060606060321 , - 0.242424242424264 , 0.625541125541133 , - 0.329004329004313 , - 0.181818181818171 , - 0.00865800865800970 ,
//			0.114718614718604 , 0.112554112554102 , - 0.0909090909090846 , 0.0606060606060221 , - 0.329004329004311 , 0.582251082251116 , - 0.311688311688286 ,
//			- 0.116883116883110 , 0.0606060606060535 , 0.114718614718615 , - 0.0606060606060179 , 0.121212121212100 , - 0.181818181818170 , - 0.311688311688291 ,
//			0.686147186147201 , - 0.233766233766232 , - 0.116883116883125 , - 0.00865800865801525 , 0.0454545454545618 , 0.0454545454545559 , - 0.00865800865800592 ,
//			- 0.116883116883116 , - 0.233766233766231 , 0.686147186147193 , - 0.311688311688302 , - 0.181818181818174 , 0.121212121212123 , - 0.0606060606060300 ,
//			0.114718614718607 , 0.0606060606060482 , - 0.116883116883113 , - 0.311688311688280 , 0.582251082251141 , - 0.329004329004253 , 0.0606060606061298 ,
//			- 0.0909090909090590 , 0.112554112554110 , 0.114718614718604 , - 0.00865800865800992 , - 0.181818181818166 , - 0.329004329004298 , 0.625541125541161 ,
//			- 0.242424242424221 , 0.0606060606060272 , - 0.0909090909090899 , - 0.0606060606060539 , 0.0454545454545396 , 0.121212121212095 , 0.0606060606060161 ,
//			- 0.242424242424292 , 0.106060606060574 };

	static float fCoef[9] = { 1 / 16.0, 2 / 16.0, 1 / 16.0, 2 / 16.0, 4 / 16.0, 2 / 16.0, 1 / 16.0, 2 / 16.0, 1 / 16.0 };
	//平滑系数，这里暂时使用固定的系数。开立给定的平滑系数计算方式以后再考虑。
	m_pSmoothWeightCoeff = fCoef;
	//the color wall filter index
	m_nCWFIndex = 3;



	m_nCEnsemble = MAXC_ENSEMBLE;		//线阵、凸阵-12 //相控阵-8
	m_nCLD = 1;

	//彩色框垂直方向的起始点
//	m_nColorFrameStartPoint = 30;		//凸阵

	// point amount of one line
	m_nCPointsNum = 212 / 2;
	// line amount of one frame
//	m_nCLineNum = 47;

	//Color的抽取倍率
	m_fCExtraMultiple = 2;
	//Persist value
	m_nPersist = 1;
	//Smooth parameter
	m_nSmoothWeightCoeffRow = 3;
	m_nSmoothWeightCoeffCol = 3;

	//判断帧相关是否有缓存数据
	m_bIsFirstFrame = true;

//	SmoothWeightCoeff rowIndex = 4
//	SmoothWeightCoeff colIndex = 2
	m_nNoiseOffset = 50;
	m_nCSmo = 43;
	m_nCVarianceThreshold = 129;
//	m_dCovarianceCoef = 3.501409;
	m_nAGain = 50;
	m_nNoiseSlope = 50;
	m_nContrast = 41;
	m_nDYN = 50;
	m_nPrioSlope = 158;

	m_nVelocityThreshold = 30;
//	m_nCTissueThreshold = 52;
	m_nCTissueThreshold = 80;
	m_nNoiseThreshold = 60000;	//凸阵15000，
	m_nCPMidleThreshold = 16327;
	m_nVarLowThreshold = 64059;

	m_nMorphologyFlag = 1;
	//用于能量计算的增益补偿，未测试过
//	m_nStartPointOfGain = 10;
//	for (int i = 0; i < 480; i++)
//	{
//		m_fDigitalGain[i] = 1;
//	}
	//数据增益加上之后跟其他参数没有配合好，导致彩色图像效果很差，所以暂时不用
//	DigitalGain();

	//壁滤波系数
	//m_pfCWFCoef = (float*) malloc(m_nCEnsemble * m_nCEnsemble * sizeof(float));
	//LoadWallCoef(m_pfCWFCoef, m_nCEnsemble,m_nCWFIndex);

//	//计算能量的动态范围变换映射表参数
//	m_nCPowerDynamicRange = 80;
//	m_nPowerControlPoint = MAX_POWER_VALUE * 40 / 100;
//	CalcDRCurve(m_nCPowerDynamicRange, m_nPowerControlPoint);




}

MultiDeepModeTCDSignalManage::~MultiDeepModeTCDSignalManage()
{
	if (NULL != m_pfCWFCoef)
	{
		free(m_pfCWFCoef);
	}
}

// ------------------------------------------------------------
// Description	:速度帪相关处理函数,处理完成后将相关结果拷贝到输入数据指针,当不存在缓存时直接拷贝
//				根据新值和旧值各种条件依次处理
//				1.数据反号; 2.新值小于旧值; 3.正常加权
// Parameter	:
//		pNewFrame-输入的新的数据帧
//		pCache-前一帧数据的缓冲
//		nLineAmount-线数
//		nPointAmount-点数
//		VFPCoef-帧相关系数
// Retrun Value	:void
// ------------------------------------------------------------
//void MultiDeepModeTCDSignalManage::VelocityCorrelation(INT8 *pNewFrame, INT8 *pCache, int nPointAmount,
//		int nVFPCoef)
//{
//	if (true == m_bIsFirstFrame)
//	{
//		//当前数据为第一帧，没有缓存，直接拷贝数据
//		memcpy(pCache, pNewFrame, sizeof(char)  * nPointAmount);
//		m_bIsFirstFrame = false;
//	} else
//	{
//		VelocityCorrelationOneFrame(pNewFrame, pCache, nPointAmount, nVFPCoef);
//	}
//}



// ------------------------------------------------------------
// Description	:设置C模式的参数
// Parameter	:
//	param-C模式的参数
// Retrun Value	:void
// ------------------------------------------------------------
void MultiDeepModeTCDSignalManage::SetMultiDeepModeTCDParam()
{
	//判断帧相关是否有缓存数据
	m_bIsFirstFrame = true;
	//壁滤波系数
	LoadWallCoef(m_pfCWFCoef, m_nCEnsemble,m_nCWFIndex);
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
int MultiDeepModeTCDSignalManage::LoadWallCoef(float* fpCoef, int nEnsemble, int nCWFIndex)
{
	//这里使用回归滤波器

	//check the nCWFIndex
	if((3>nCWFIndex )||(6<nCWFIndex ))
	{
		nCWFIndex = 3;
	}

	//set the file path
	FILE* fp;
	char cPath[50];
	sprintf(cPath, "/data/downdata/Color_WFilter/CWF_D%dK%d", nCWFIndex, nEnsemble);
//	sprintf(cPath, "/data/downdata/Color_WFilter/WFcoef%d.txt", nEnsemble);

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
void MultiDeepModeTCDSignalManage::SignalProcess(INT16 *pSrc, INT16 *pDst_v)
{
	int nEnsemble = m_nCEnsemble;
	int nPoints = m_nCPointsNum;
	for(int i=0;i<MAX_BLINE_POINTS;i++){
		pDst_v[i]=pSrc[i*2];
	}
	//壁滤波
	//ColorWallFilter( m_pfCWFCoef , pSrc , m_fAfterCWF , nEnsemble , nPoints);
	//ColorWallFilter_Neon(m_pfCWFCoef, pSrc, m_fAfterCWF, nEnsemble, nLines, nPoints);

	//自相关
	//CAutoCorrelation();
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

}


}	//the end of namespace of SignalProcess


