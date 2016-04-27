/*
 * MultiDeepModeTCDThread.h
 *
 *  Created on: Apr 21, 2016
 *      Author: ljd
 */

#ifndef MULTIDEEPMODETCDTHREAD_H_
#define MULTIDEEPMODETCDTHREAD_H_
#include "image/parameter/MultiDeepTCDParam.h"
#include "image/buffer/PingPangBuffer.h"
#include "image/GlobalDefine.h"
#include "image/utility/ReadFile.h"

//The MultiDeepModeTCD frame head,to declear the basic message of MultiDeepModeTCD frame

struct MultiDeepModeTCDFrameHead
{
	int nPUF;	//the parameter update flag,1-the parameter is changed;0-the parameter is stable
	//int nLineAmount;	//the line amount of one frame
	int nPointAmount;	//the point amount of one line
	int nEnsemble;	//the ensemble

};

// ------------------------------------------------------------
// Description	:获取解析之后的MultiDeepTCD缓冲区数据
// Parameter	:void
// Retrun Value	:ParsedCBuffer的引用
// ------------------------------------------------------------
PingPangBuffer<INT16> * GetParsedMulitDeepModeTCDBuffer(void);

// ------------------------------------------------------------
// Description	:The MulitDeepTCD Mode Of Simulate Data Thread
// Parameter	:void
// Return Value	:void
// ------------------------------------------------------------
void * MulitDeepModeTCDSimDataThread(void *);
// ------------------------------------------------------------
// Description	:MultiDeepModeTCD模式数据解析获取线程，从socket获取，或文件读取
// Parameter	:void
// Retrun Value	:void
// ------------------------------------------------------------
void * MultiDeepModeTCDGetDataThread(void *);

// ------------------------------------------------------------
// Description	:The MulitDeepTCD Mode Of Signal Process Thread
// Parameter	:void
// Return Value	:void
// ------------------------------------------------------------
void * MultiDeepModeTCDSignalProcessThread(void *);

// ------------------------------------------------------------
// Description	:The MulitDeepTCD Mode Of Image Process Thread
// Parameter	:void
// Return Value	:void
// ------------------------------------------------------------
void * MultiDeepModeTCDImageProcessThread(void *);

// --------------------------------------------------------------
// Description	:获取最新数据接口，供显示模块调用以获取Image处理后的数据
// Parameters	:
//		pCFrameDisp-C的速度数据的指针
// Return Value	:
//		false-获取数据失败
//		true-获取数据成功
// --------------------------------------------------------------
int GetDispMulitDeepModeTCDFrame(char** pCFrameDisp);

// ------------------------------------------------------------
// Description	:设置B模式的信号处理参数
// Parameter	:
//		param-B模式的参数类
// Return Value	:
//		false-设置B模式的信号处理参数失败
//		true-设置B模式的信号处理参数成功
// ------------------------------------------------------------
//int SetCModeParamOfSignal(ParamAdjust::CParam * param);



#endif /* MULTIDEEPMODETCDTHREAD_H_ */
