/*
 * SynSemaphore.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: ljd
 */
// ------------------------------------------------------------
// Copyright (c) 2011, 深圳开立科技有限公司。All rights reserved.
// Name        : SynSemaphore.cpp
// Description : 全局信号量类的定义与实现
//	信号量有两个类：ParseAndSignalSem-解析模块与信号处理模块的信号量
//				ImageAndDispSem-图像处理模块与图像显示模块的信号量
//				对于ImageAndDispSem，在使用的时候只需要使用其生产者消费者信号量的两个方法：ProducterWait(),ConsumerDone().
// History     : GuiHongyun(ghy1874@yeah.net),2013-8-3
// ------------------------------------------------------------

#include "image/utility/SynSemaphore.h"
//#include "image/utility/DebugPrint.h"


namespace SynSem
{
//双缓冲，设置缓冲长度为2
#define BUFFERLENGTH 2

static ParseAndSignalSem g_objPSSem;
static ImageAndDispSem g_objIDSem;

// ------------------------------------------------------------
// Description	:获取解析模块与信号处理模块的信号量
// Parameter	:void
// Return Value	:解析模块与信号处理模块的信号量指针
// ------------------------------------------------------------
ParseAndSignalSem * GetPSSem(void)
{
	return &g_objPSSem;
}

// ------------------------------------------------------------
// Description	:获取图像处理模块与图像显示模块的信号量
// Parameter	:void
// Return Value	:图像处理模块与图像显示模块的信号量指针
// ------------------------------------------------------------
ImageAndDispSem * GetIDSem(void)
{
	return &g_objIDSem;
}

// ------------------------------------------------------------
// Description	:生产者消费者信号量的构造函数
// Parameter	:void
// Return Value	:void
// ------------------------------------------------------------
PCSemphore::PCSemphore()
{
	// 初始化同步信号量
	int nRntFull = sem_init(&m_semFull, 0, 0); //产品队列缓冲同步
	int nRntEmpty = sem_init(&m_semEmpty, 0, BUFFERLENGTH); //线程运行同步
	if (nRntFull || nRntEmpty)
	{
		printf("信号量初始化失败！\n");
	}
}

// ------------------------------------------------------------
// Description	:生产者消费者信号量的析够函数
// Parameter	:void
// Return Value	:void
// ------------------------------------------------------------
PCSemphore::~PCSemphore()
{
	sem_destroy(&m_semFull);
	sem_destroy(&m_semEmpty);
}

// ------------------------------------------------------------
// Description	:生产者对信号量的操作，等待缓冲区
// Parameter	:void
// Return Value	:void
// ------------------------------------------------------------
void PCSemphore::ProducterWait()
{
	sem_wait(&m_semEmpty);
}

// ------------------------------------------------------------
// Description	:生产者对信号量的操作，生产完成
// Parameter	:void
// Return Value	:void
// ------------------------------------------------------------
void PCSemphore::ProducterDone()
{
	sem_post(&m_semFull);
}

// ------------------------------------------------------------
// Description	:消费者对信号量的操作，等待产品
// Parameter	:void
// Return Value	:void
// ------------------------------------------------------------
void PCSemphore::ConsumerWait()
{
	sem_wait(&m_semFull);
}

// ------------------------------------------------------------
// Description	:生产者对信号量的操作，消费完成
// Parameter	:void
// Return Value	:void
// ------------------------------------------------------------
void PCSemphore::ConsumerDone()
{
	sem_post(&m_semEmpty);
}


}//namespace End




