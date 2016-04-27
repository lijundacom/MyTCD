/*
 * SynSemaphore.h
 *
 *  Created on: Apr 16, 2016
 *      Author: ljd
 */

#ifndef SYNSEMAPHORE_H_
#define SYNSEMAPHORE_H_
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>

namespace SynSem
{
//生产者消费者信号量
class PCSemphore
{
private:
	// 同步信号量，当满了时阻止生产者放产品
	sem_t m_semEmpty;
	// 同步信号量，当没产品时阻止消费者消费
	sem_t m_semFull;
public:
	PCSemphore();
	~PCSemphore();

public:
	//生产者对信号量的操作，等待缓冲区
	void ProducterWait();
	//生产者对信号量的操作，生产完成
	void ProducterDone();
	//消费者对信号量的操作，等待产品
	void ConsumerWait();
	//生产者对信号量的操作，消费完成
	void ConsumerDone();

};



//用于解析数据模块与信号处理模块的信号量
struct ParseAndSignalSem
{
	//B 解析线程与信号处理线程之间的信号量，这适合复合成像没有单独开启线程的模式
	//PCSemphore m_Sem_BMode;

	//B 信号解析与复合成像之间的信号量
	//PCSemphore m_Sem_BMode_ParseComp;

	//B 复合成像线程与信号处理之间的信号量
	//PCSemphore m_Sem_BMode_CompSignal;

	//MultiDeepModeTCD
	PCSemphore m_Sem_MultiDeepModeTCD;
	//C
	//PCSemphore m_Sem_CMode;

	//PWModeTCD
	//PCSemphore m_Sem_PWMode;


};


//用于图像处理模块与刷新显示模块的信号量
struct ImageAndDispSem
{
	//B
	//PCSemphore m_Sem_BMode;
	//PCSemphore m_Sem_MMode;

	//C
	//PCSemphore m_Sem_CMode;
	//MModeTCD
	PCSemphore m_Sem_MultiDeepModeTCD;
	//PW
	//PCSemphore m_Sem_PWMode;

};
//用于图像处理模块与刷新显示模块的信号量





// ------------------------------------------------------------
// Description	:获取解析模块与信号处理模块的信号量
// Parameter	:void
// Return Value	:解析模块与信号处理模块的信号量指针
// ------------------------------------------------------------
ParseAndSignalSem* GetPSSem(void);

// ------------------------------------------------------------
// Description	:获取图像处理模块与图像显示模块的信号量
// Parameter	:void
// Return Value	:图像处理模块与图像显示模块的信号量指针
// ------------------------------------------------------------
ImageAndDispSem * GetIDSem(void);

}//namespace End



#endif /* SYNSEMAPHORE_H_ */
