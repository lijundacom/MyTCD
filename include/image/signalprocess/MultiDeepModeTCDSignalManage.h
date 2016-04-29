/*
 * MultiDeepModeTCDSignalManage.h
 *
 *  Created on: Apr 21, 2016
 *      Author: ljd
 */

#ifndef MULTIDEEPMODETCDSIGNALMANAGE_H_
#define MULTIDEEPMODETCDSIGNALMANAGE_H_
#include "image/signalprocess/MultiDeepModeTCDSignalProcess.h"
#include "image/utility/ImageDef.h"
//#include "image/parameter/IProcessParam.h"

namespace SignalProcess
{


class MultiDeepModeTCDSignalManage
{
private:
	// The Data Before Color Wall Filter
	float m_fBeforCWF[MAX_MD_ENSEMBLE * DEEP_POINTS * 2];
	// The Data After Color Wall Filter
	float m_fAfterCWF[MAX_MD_ENSEMBLE * DEEP_POINTS * 2];

	//Velocity After AutoCorrelation
	float m_nVelocityAfterAC[DEEP_POINTS];


	//Velocity Out Data
	INT8 m_nVelocityOut[DEEP_POINTS ];


	//Velocity TMP Buffer
	INT8 m_nVAfterSmooth[DEEP_POINTS ];

	//壁滤波系数
	float* m_pfMDWFCoef;

	//每一个点的数据采样个数，从4到16条
	int m_nMDEnsemble;


	// point amount of one line
	int m_nMDPointsNum;

	// Velocity threshold
	int m_nVelocityThreshold;
	//the color wall filter index
	int m_nMDWFIndex;


	//Color的抽取倍率，暂时认为这个参数代表Color线的需要放大的比例倍数
	float m_fCExtraMultiple;


	//速度帧相关的缓存
	INT8 m_nVPersistCache[1];

	// 速度模式处理环节的专用buffer
	INT8 m_VMedian[3][255 * MAXBLINE_LEN];



	//用于帧相关判断是否有缓存数据
	bool m_bIsFirstFrame;
	//方差系数
	//double m_dCovarianceCoef;



private:
	// ------------------------------------------------------------
	// Description	:速度帪相关处理函数,处理完成后将相关结果拷贝到输入数据指针,当不存在缓存时直接拷贝
	// Parameter	:
	//		pNewFrame-输入的新的数据帧
	//		pCache-前一帧数据的缓冲
	//		nLineAmount-线数
	//		nPointAmount-点数
	//		VFPCoef-帧相关系数
	// Retrun Value	:void
	// ------------------------------------------------------------
	void VelocityCorrelation(INT8 *pNewFrame, INT8 *pCache, int nPointAmount, int nVFPCoef);

	// ------------------------------------------------------------
	// Description	:能量帧相关处理函数,处理完成后缓存帧和输出帧结果一样
	// Parameter	:
	//		pNewFrame-输入的新的数据帧
	//		pCache-前一帧数据的缓冲
	//		nLineAmount-线数
	//		nPointAmount-点数
	//		nPersist-帧相关系数，取值[0~7]，数值越大相关性越强
	// Retrun Value	:void
	// ------------------------------------------------------------
	//void PoweCorrelation(UINT16 *pNewFrame, UINT16 *pCache, int nLineAmount, int nPointAmount, int nPersist);

	// ------------------------------------------------------------
	// Description	:加载壁滤波系数
	// Parameter	:
	//		fpCoef-壁滤波系数
	//		nEnsemble-每一个点的数据采样个数，从4到16条
	//		nCWFIndex-壁滤波滤波器截止频率索引(这里使用回归滤波器，该参数代表回归滤波器选取的多项式阶数D)
	// Retrun Value	:
	//		0-加载壁滤波器系数成功
	//		-1-加载壁滤波系数失败，找不到文件
	// ------------------------------------------------------------
	int LoadWallCoef(float* fpCoef, int nEnsemble, int nCWFIndex);

	// ------------------------------------------------------------
	// Description	:计算能量动态范围变换映射表m_DRCurve，看算法属于线性压缩
	// Parameter	:
	//	ndB-代表了压缩的中值点附近的范围，值越大范围越宽
	//	nControlPoint-中值点的意思
	// Retrun Value	:
	//	true-计算正确
	//	false-计算出现错误
	// ------------------------------------------------------------
	//bool CalcDRCurve(int ndB, int nControlPoint);

	// ------------------------------------------------------------
	// Description	:计算数值增益，用于计算能量补偿，具体算法不清楚，但是用到的变量却相当的多
	// Parameter	:	void
	// Retrun Value	:void
	// ------------------------------------------------------------
	//void DigitalGain();

	void Threshold(INT8* aVelocity, UINT16* aPower, UINT8* aVariance, int PointAmount);

	bool MedianFilter(INT8* pTarget, int nPointAmount);

	bool RestoreMask(INT8* pTarget, INT8* pMask, int nPointAmount);

	bool ProduceMask(INT8* pSource, INT8* pMask, int nPointAmount, int nThreshold);

	bool MorphologyEx(INT8* pTarget, int nPointAmount, int MorphologyType);

public:
	MultiDeepModeTCDSignalManage();
	~MultiDeepModeTCDSignalManage();

public:

	// ------------------------------------------------------------
	// Description	:C模式的信号处理，属于CFM模式
	// Parameter	:
	//	pDst_v-输出的速度矩阵指针
	// Retrun Value	:void
	// ------------------------------------------------------------
	//void SignalProcessCFM(INT32 *pSrc , INT8 *pDst_v);
	// ------------------------------------------------------------
	// Description	:MultiDeepModeTCD模式的信号处理
	// Parameter	:
	//	pDst_v-输出的速度矩阵指针
	// Retrun Value	:void
	// ------------------------------------------------------------
	void SignalProcess(INT16 *pSrc , float *pDst_v);
	void SignalProcess(INT16 *pSrc , double *pDst_v);

	// ------------------------------------------------------------
	// Description	:C模式的信号处理，属于DPI模式
	// Parameter	:
	//	pDst_p-输出的能量矩阵指针
	// Retrun Value	:void
	// ------------------------------------------------------------
	//void SignalProcessDPI(INT32 *pSrc ,UINT8 *pDst_p);

	// ------------------------------------------------------------
	// Description	:C模式的信号处理，属于TDI模式
	// Parameter	:
	//	pDst_v-输出的速度矩阵指针
	//	pDst_p-输出的能量矩阵指针
	//	pDst_c-输出的方差矩阵指针
	// Retrun Value	:void
	// ------------------------------------------------------------
	//void SignalProcessTDI ( INT32 *pSrc , INT8 *pDst_v ,  UINT8 *pDst_p,  UINT8 *pDst_c);

	// ------------------------------------------------------------
	// Description	:设置C模式的参数
	// Parameter	:
	//	param-C模式的参数
	// Retrun Value	:void
	// ------------------------------------------------------------
	void SetMultiDeepModeTCDParam();

	// ------------------------------------------------------------
	// Description	:获取C模式的参数
	// Parameter	:
	//	param-C模式的参数
	// Retrun Value	:void
	// ------------------------------------------------------------
	void GetMultiDeepModeTCDParam();

};

}	//namespace SignalProcess



#endif /* MULTIDEEPMODETCDSIGNALMANAGE_H_ */
