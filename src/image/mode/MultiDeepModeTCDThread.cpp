/*
 * MultiDeepModeTCDThread.CPP
 *
 *  Created on: Apr 21, 2016
 *      Author: ljd
 */

#include "image/mode/MultiDeepModeTCDThread.h"
#include "image/utility/ThreadManage.h"
//#include "image/buffer/CCineBuffer.h"
//#include "image/utility/DebugPrint.h"
#include "image/buffer/CycleBuffer.h"
#include "image/utility/SynSemaphore.h"
#include "image/signalprocess/MultiDeepModeTCDSignalManage.h"
//#include "image/usb/ParseInterface.h"
//#include "image/utility/ReadContentFromFile.h"
#include "image/utility/TimerTest.h"
//#include "app/modeswitch/Display.h"
#include "image/utility/ImageDef.h"
#include "image/utility/FrameStructure.h"
#include "image/utility/ReadFile.h"
#include <sys/prctl.h>
#include <list>

//MultiDeepModeTCD Mode Signal Process class obj
static SignalProcess::MultiDeepModeTCDSignalManage objMultiDeepModeTCDSignalManage;

//MultiDeepModeTCD模式的数据帧，解析模块和信号处理模块之间的缓冲，这里仅仅用与采用CSimDataThread仿真Color数据时使用
//MAX_BLINE_POINTS应该是动态变换的，从数据头文件解析得到
static PingPangBuffer<INT16> ParsedMultiDeepModeTCDBuffer(sizeof(INT16) * MAXC_ENSEMBLE * MAX_BLINE_POINTS*2);

//MultiDeepModeTCD模式的数据帧，图像处理模块和显示模块之间的缓冲
static Buffer::CycleBuffer<INT16> MultiDeepModeTCDFrameBuffer(sizeof(INT16) * MAXC_ENSEMBLE * MAX_BLINE_POINTS*2);//*2是为了后面char转int不溢出

// ------------------------------------------------------------
// Description	:获取解析之后的MultiDeep缓冲区数据
// Parameter	:void
// Retrun Value	:ParsedCBuffer的引用
// ------------------------------------------------------------
PingPangBuffer<INT16> * GetParsedMultiDeepModeTCDBuffer(void)
{
	return &ParsedMultiDeepModeTCDBuffer;
}
// ------------------------------------------------------------
// Description	:MultiDeepModeTCD模式数据解析获取线程，从socket获取，或文件读取
// Parameter	:void
// Retrun Value	:void
// ------------------------------------------------------------
void * MultiDeepModeTCDGetDataThread(void *)
{
	//LOGI( "C Mode Sim Data thread Begin!");
	printf( "MultiDeepModeTCD Get Data thread Begin!\n");
	//定义了两个数组，其实就是地址偏移量
	signed short int * pParsedMultiDeepModeTCDFrame = NULL;
	signed short int * IQData = NULL;
	char* IQFileName = "/home/ljd/data/IQdata4.dat";
	int nCnt = 0;
	int k=0;
	while (1)
	{
	//for(int k =0;k<10;k++){
		SynSem::GetPSSem()->m_Sem_MultiDeepModeTCD.ProducterWait();
		//一个pingpangbuffer,获得写指针，接下来向里面写
		ParsedMultiDeepModeTCDBuffer.GetWritePointer(&pParsedMultiDeepModeTCDFrame);
		if (NULL == pParsedMultiDeepModeTCDFrame)
		{
			//LOGE("Get write pointer failed in the c mode sim data thread");
			printf("Get write pointer failed in the c mode sim data thread\n");
		}


		IQData = ReadFile(IQFileName,MAX_BLINE_POINTS,MAXC_ENSEMBLE,k);
		for (int i=0;i<MAXC_ENSEMBLE;i++){
			for (int j =0;j<MAX_BLINE_POINTS;j++){
				pParsedMultiDeepModeTCDFrame[i*MAX_BLINE_POINTS*2+j*2]=IQData[i*MAX_BLINE_POINTS*2+j*2];
				pParsedMultiDeepModeTCDFrame[i*MAX_BLINE_POINTS*2+j*2+1]=IQData[i*MAX_BLINE_POINTS*2+j*2+1];
			}

		}
		std::cout<<"生产"<<std::endl;
		PrintArray(MAX_BLINE_POINTS,MAXC_ENSEMBLE,pParsedMultiDeepModeTCDFrame);
		usleep(1000 * 500);
		SynSem::GetPSSem()->m_Sem_MultiDeepModeTCD.ProducterDone();
		k++;
		//LOGI( "C Mode Sim Data One Frame Done!");
	}
	//}
}

// ------------------------------------------------------------
// Description	:C数据信号处理线程
// Parameter	:void
// Retrun Value	:void
// ------------------------------------------------------------
void * MultiDeepModeTCDSignalProcessThread(void*)
{
	//LOGI( "C mode signal thread is start here");
	printf( "MultiDeepModeTCD mode signal thread is start here\n");

	prctl(PR_SET_NAME, (unsigned long) "Signal_MultiDeepModeTCD_Mode");

	// MultiDeepModeTCD 从乒乓缓存中读取IQ数据的指针
	signed short int * pParsedMultiDeepModeTCDData = NULL;
	//IQ数据处理后生成的速度矩阵
	signed short int * pParsedMultiDeepModeTCDVelocity= NULL;
	//用于显示的RGB数据
	signed short int * pParsedMultiDeepModeTCDFrame= NULL;

	//MultiDeepModeTCDSignalManage
	while (1)
	{
		ThreadSpace::GetMultiDeepModeTCDSignalThread()->SuspendCheck();
		SynSem::GetPSSem()->m_Sem_MultiDeepModeTCD.ConsumerWait();
		//获取乒乓缓存中的数据指针
		if (false == ParsedMultiDeepModeTCDBuffer.GetReadPointer(&pParsedMultiDeepModeTCDData))
		{
			//获取数据出错
			//LOGE( "GetParsedCFrame Failed, in the c mode signal thread\n");
			printf( "GetParsedCFrame Failed, in the c mode signal thread\n");
		}
		printf("消费\n");
		PrintArray(MAX_BLINE_POINTS,MAXC_ENSEMBLE,pParsedMultiDeepModeTCDData);

		static TimerTest objCTime(100, "MultiDeepModeTCDSignal");
		//objCTime.CheckFreq();
		objCTime.Start();
		//数据处理(传入待处理数据，传出处理后数据)
		objMultiDeepModeTCDSignalManage.SignalProcess(pParsedMultiDeepModeTCDData, pParsedMultiDeepModeTCDVelocity);
		objCTime.End();

		MultiDeepModeTCDFrameBuffer.GetWritePointer(&pParsedMultiDeepModeTCDFrame);
		if (NULL == pParsedMultiDeepModeTCDFrame)
		{
			//LOGE("Get write pointer failed in the c mode sim data thread");
			printf("Get write pointer failed in the pParsedMultiDeepModeTCDVelocityn");
		}
		//把数据处理记过保存到显示缓存区中
		//velocity 到RGB的映射函数
		//
			//？？？？？？？？？？？？
			//pParsedMultiDeepModeTCDFrame[x]=pParsedMultiDeepModeTCDVelocity[y];

		//
		SynSem::GetPSSem()->m_Sem_MultiDeepModeTCD.ConsumerDone();
		//LOGI( "C Mode SignalProcess  One Frame Done!");

	} //while(1)

}
// ------------------------------------------------------------
// Description	:图像处理C模式线程
// Parameter	:void
// Retrun Value	:void
// ------------------------------------------------------------
//void * MultiDeepModeTCDImageProcessThread(void*)
//{
//	//LOGI( "C mode image thread is start here");
//	printf( "C mode image thread is start here\n");
//	int nCnt = 0;
//
//	//Color Frame
//	char *pMultiDeepModeTCDFrameDisp = NULL;
//	char *pMultiDeepModeTCDFrameDispData = NULL;
//	//MultiDeepModeTCDFrameHead* pMultiDeepModeTCDFrameHead = NULL;	//C mode frame of head
//
//	while (1)
//	{
//		//suspend check
//		ThreadSpace::GetMultiDeepModeTCDImageThread()->SuspendCheck();
//
//		// point amount of one line
//		int nCPointsNum = 212 / 2;
//		// line amount of one frame
//		int nCLineNum = 46;
//
//		//wait m_Sem_MultiDeepModeTCD
//		SynSem::GetIDSem()->m_Sem_MultiDeepModeTCD.ProducterWait();
//		//get the cycle buffer
//		int nRet = DispBufferMultiDeepModeTCDFrame.GetWritePointer(&pMultiDeepModeTCDFrameDisp);
//		if (0 == nRet)
//		{
//			//LOGE("GetWritePointer failed in the c mode image thread!");
//			printf("GetWritePointer failed in the c mode image thread!");
//		}
//
//		//从CineBuffer获取数据
//		//根据冻结状态与非冻结状态，确定从CineBuffer中取出数据的位置
//		MultiDeepModeTCDFrameData::PointType pMultiDeepModeTCDFrameData = NULL;
//		if (false == Display::Instance()->IsFreeze())
//		{
//			//非冻结状态，取最新的数据
//			pMultiDeepModeTCDFrameData = CCineBuffer::GetLatestImge(NULL);
//			//pCFrameData = CCineBuffer::GetCFrameImage(0);
//			//LOGI("Gui The system is running");
//		} else
//		{
//			//冻结状态，根据电影回放控件给定的参数取数据，这里暂时取最新数据
//			pMultiDeepModeTCDFrameData = CCineBuffer::GetLatestImge(NULL);
//			//pCFrameData = CCineBuffer::GetCFrameImage(0);
//			//LOGI("Gui The system is Freezed");
//		}
//		//set the head
//		//pMultiDeepModeTCDFrameHead = (MultiDeepModeTCDFrameHead *) pMultiDeepModeTCDFrameDisp;
//
//		//set the M multi line head
//		//pMultiDeepModeTCDFrameHead->nPUF = 0;
//		//Update parameter
////		if (1 == nPUFOfCImageModule)
////		{
////			//update parameter,(at here is nothing to do)
////
////			//send the PUF to display module
////			pMultiDeepModeTCDFrameHead->nPUF = 1;
////			//reset the flag
////			nPUFOfMultiDeepModeTCDImageModule = 0;
////		}
//
//		//get data pointer
//		//pMultiDeepModeTCDFrameDispData = (char *) ((char *) pMultiDeepModeTCDFrameDisp + sizeof(MultiDeepModeTCDFrameHead));
//		pMultiDeepModeTCDFrameDispData = (char *) ((char *) pMultiDeepModeTCDFrameDisp);
//
//		//set the data
////		if (NULL == pMultiDeepModeTCDFrameData)
////		{
////			//LOGI( "Cine Buffer have No Date\n");
////			pMultiDeepModeTCDFrameHead->nLineAmount = 20;
////			pMultiDeepModeTCDFrameHead->nPointAmount = 20;
////			memset(pMultiDeepModeTCDFrameDispData, 0, sizeof(CMultiDeepModeTCDFrameData));
////		} else
////		{
////			//this two parameter should read from the pCFrameData
//////			pCFrameHead->nLineAmount = nCLineNum;
//////			pCFrameHead->nPointAmount = nCPointsNum;
////			pMultiDeepModeTCDFrameHead->nLineAmount = pMultiDeepModeTCDFrameData->nLineCount;
////			pMultiDeepModeTCDFrameHead->nPointAmount = pMultiDeepModeTCDFrameData->nPointCount;
//////			//Direct copy ,No Image Process
//////			memcpy(pCFrameDispData, (char *) pCFrameData.get(), sizeof(CFrameData));
//			memcpy(pMultiDeepModeTCDFrameDispData, (char *) (&(pMultiDeepModeTCDFrameData->data)), sizeof(MultiDeepModeTCDFrameData));
////		}
//
//		SynSem::GetIDSem()->m_Sem_MultiDeepModeTCD.ProducterDone();
//		//LOGI( "The C mode of imageProcess one frame data done");
//		nCnt++;
//	}
//
//}