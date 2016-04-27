/*
 * ThreadManager.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: ljd
 */
#include "image/utility/ThreadManage.h"
//#include "image/utility/DebugPrint.h"
//#include "image/usb/ParseInterface.h"
#include "image/signalprocess/MultiDeepModeTCDSignalManage.h"
//#include "image/signalprocess/PWModeSignalManage.h"
//#include "image/buffer/BCineBuffer.h"
//#include "image/buffer/CCineBuffer.h"
//#include "image/buffer/MCineBuffer.h"
//#include "image/buffer/PWCineBuffer.h"
#include "image/buffer/CycleBuffer.h"
#include "image/utility/SynSemaphore.h"
//#include "image/utility/ImageDef.h"
#include "image/mode/MultiDeepModeTCDThread.h"

//#include "image/mode/BModeThread.h"
//#include "image/mode/CModeThread.h"
//#include "image/mode/PWModeThread.h"
namespace ThreadSpace
{

Thread::Thread() :
		m_bPauseStatus(THREAD_STATUS_RUNNING)
{
	pthread_mutex_init(&m_mutexPause, NULL);
	pthread_cond_init(&m_condPause, NULL);

}

Thread::~Thread()
{
	pthread_mutex_destroy(&m_mutexPause);
	pthread_cond_destroy(&m_condPause);
}

// ------------------------------------------------------------
// Description	:获取线程的状态
// Parameter	:void
// Return Value	:
//	true: THREAD_STATUS_RUNNING-运行状态
//	false: THREAD_STATUS_PAUSED-挂起状态
// ------------------------------------------------------------
bool Thread::GetStatus(void)
{
	return m_bPauseStatus;
}

// ------------------------------------------------------------
// Description	:设置线程的ID
// Parameter	:void
// Return Value	:thread_t-pthread_t类型的线程ID
// ------------------------------------------------------------
pthread_t Thread::GetThreadID() const
{
	return m_threadID;
}

// ------------------------------------------------------------
// Description	:设置线程的ID
// Parameter	:thread_t-pthread_t类型的线程ID
// Return Value	:void
// ------------------------------------------------------------
void Thread::SetThreadID(pthread_t thread_t)
{
	m_threadID = thread_t;
}

// ------------------------------------------------------------
// Description	:挂起线程
// Parameter	:void
// Return Value	:
// false-线程本身处于挂起状态
// true-成功将线程从运行状态切换到挂起状态
// ------------------------------------------------------------
bool Thread::Pause(void)
{
	bool nRet = true;
	if (THREAD_STATUS_RUNNING == m_bPauseStatus)
	{
		pthread_mutex_lock(&m_mutexPause);
		m_bPauseStatus = THREAD_STATUS_PAUSED;
		//LOGI( "thread stop!");
		pthread_mutex_unlock(&m_mutexPause);

	} else
	{
		printf( "pthread pause already");
		nRet = false;
	}

	return nRet;
}

// ------------------------------------------------------------
// Description	:恢复线程
// Parameter	:void
// Return Value	:
//	false-线程本身处于运行状态
//	true-成功将线程从挂起状态切换到运行状态
// ------------------------------------------------------------
bool Thread::Resume(void)
{
	bool nRet = true;
	if (THREAD_STATUS_PAUSED == m_bPauseStatus)
	{
		pthread_mutex_lock(&m_mutexPause);
		m_bPauseStatus = THREAD_STATUS_RUNNING;
		pthread_cond_signal(&m_condPause);
		//LOGI( "pthread run!");
		pthread_mutex_unlock(&m_mutexPause);
	} else
	{
		printf( "pthread run already");
		nRet = false;
	}

	return nRet;
}

// ------------------------------------------------------------
// Description	:线程挂起检验
//		此函数放在线程中，运行到此函数处时，如果该线程被设置为来挂起状态，那么线程在此函数处挂起，直到
//		调用Resume()函数后程序继续运行；如果线程处于运行状态，则马上离开此函数继续运行
// Parameter	:void
// Return Value	:void
// ------------------------------------------------------------
void Thread::SuspendCheck(void)
{
	pthread_mutex_lock(&m_mutexPause);
	while (!m_bPauseStatus)
	{
		pthread_cond_wait(&m_condPause, &m_mutexPause);
	}
	pthread_mutex_unlock(&m_mutexPause);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
static Thread g_ReadThread;	//读取数据线程
static Thread g_ParseThread;	//数据解析线程

//static Thread g_BSignalThread;	//B 模式信号处理线程
//static Thread g_BImageThread; 	//B 图像处理线程
static Thread g_MSignalThread;	//M 模式信号处理线程
static Thread g_MImageThread; 	//M 图像处理线程

static Thread g_PWSignalThread;	//PW 模式信号处理线程
static Thread g_PWImageThread;	//PW 模式图像处理线程

Thread * GetParseThread(void)
{
	return &g_ParseThread;
}

Thread * GetReadThread(void)
{
	return &g_ReadThread;
}


Thread * GetMultiDeepModeTCDSignalThread(void)
{
	return &g_MSignalThread;
}

Thread * GetMultiDeepModeTCDImageThread(void)
{
	return &g_MImageThread;
}

//Thread * GetPWSignalThread(void)
//{
//	return &g_PWSignalThread;
//}
//
//Thread * GetPWImageThread(void)
//{
//	return &g_PWImageThread;
//}

// ------------------------------------------------------------
// Description	:初始化M模式的线程
// Parameter	:void
// Return Value	:
//	false- 初始化出错
//	true - 正常初始化
// ------------------------------------------------------------
bool InitMultiDeepModeTCDThread(void)
{
	//Init The M CineBuffer,处理模块和显示模块的buffer
//	MCineBuffer::InitMCineBuffer();
//	MCineBuffer::SetAllMaxSize(800);
//	MCineBuffer::SetActiveIndex(0);

	pthread_t thread_id;
	int nRet = 0;

	//	//Sim M data
	//	nRet = pthread_create(&thread_id, NULL, MSimDataThread, NULL);
	//	if (nRet)
	//	{
	//		LOGE("ERROR: Failed create MSimDataThread");
	//		return false;
	//	}

	//M Signal Thread信号处理线程，包括滤波和自相关
	nRet = pthread_create(&thread_id, NULL, MultiDeepModeTCDSignalProcessThread, NULL);
	ThreadSpace::GetMultiDeepModeTCDSignalThread()->SetThreadID(thread_id);
	if (nRet)
	{
		printf( "ERROR: Failed create MSignalProcessThread\n");
		return false;
	}
	//ThreadSpace::GetMSignalThread()->Resume();

	//M Image Thread//用于显示线程，从电影缓冲区中取数据显示，可以一次取1条，或者1次取多条，冻结时不取
//	nRet = pthread_create(&thread_id, NULL, MultiDeepModeTCDImageProcessThread, NULL);
//	ThreadSpace::GetMultiDeepModeTCDImageThread()->SetThreadID(thread_id);
//	if (nRet)
//	{
//		printf( "ERROR: Failed create MImageProcessThread");
//		return false;
//	}
	usleep(10000);
	//ThreadSpace::GetMImageThread()->Resume();
}



// ------------------------------------------------------------
// Description	:初始化解析线程
// Parameter	:void
// Return Value	:
//	false- 初始化出错
//	true - 正常初始化
// ------------------------------------------------------------
bool InitParseThread(void)
{
	//LOGI( "Gui Usb ParseThread is start! ");
	printf( "Gui Usb ParseThread is start!\n ");

	pthread_t thread_id;
	int nRet = 0;

	//初始化USB接口
	int ret;
	unsigned char tmpbuf[2048];
	//ret = Start_USB_Device();
	//LOGI( "Start The usb devices,the result is %d", ret);
	//printf( "Start The usb devices,the result is %d", ret);
	//DownFreezeCommand();
	//ClearResidualEchoData(tmpbuf, 2048);

//设置线程优先级
	pthread_attr_t attr;	//线程属性
	struct sched_param param;
	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_RR);
	param.sched_priority = 10;
	pthread_attr_setschedparam(&attr, &param);

	//usb读取解析线程
//	nRet = pthread_create(&thread_id, &attr, usb_thread_read, NULL);
//	nRet = pthread_create(&thread_id, 0, usb_thread_read, NULL);
	//从socket读取数据并解析数据线程
//	nRet = pthread_create(&thread_id, 0, socket_thread_read, NULL);
	nRet = pthread_create(&thread_id, 0, MultiDeepModeTCDGetDataThread, NULL);
	ThreadSpace::GetReadThread()->SetThreadID(thread_id);
	if (nRet)
	{
		//LOGE( "ERROR: Failed create usb_thread_read");
		printf( "ERROR: Failed create usb_thread_read\n");
		return false;
	}

	//usb解析线程
//	nRet = pthread_create(&thread_id, &attr, usb_thread_parse, NULL);
//	nRet = pthread_create(&thread_id, 0, usb_thread_parse, NULL);
	//socket解析线程
//	nRet = pthread_create(&thread_id, 0, socket_thread_parse, NULL);
	//file解析线程
//	nRet = pthread_create(&thread_id, 0, file_thread_parse, NULL);
//	ThreadSpace::GetParseThread()->SetThreadID(thread_id);
//	if (nRet)
//	{
//		//LOGE( "ERROR: Failed create usb_thread_parse");
//		printf( "ERROR: Failed create usb_thread_parse");
//		return false;
//	}

	pthread_attr_destroy(&attr);

	//Stop the parse thread, at here the thread is stoped by defaulted, so don't need to call this function manual
	//StopParseThread();

	return true;
}


// ------------------------------------------------------------
// Description	:初始化B模式的线程
// Parameter	:void
// Return Value	:
//	false- 初始化出错
//	true - 正常初始化
// ------------------------------------------------------------


//	//开启信号处理线程
//	ThreadSpace::GetBSignalThread()->Resume();
//
//	//B 模式图像线程打开
//	ThreadSpace::GetBImageThread()->Resume();



// ------------------------------------------------------------
// Description	:初始化C模式的线程
// Parameter	:void
// Return Value	:
//	false- 初始化出错
//	true - 正常初始化
// ------------------------------------------------------------
/*
bool InitPWModeThread(void)
{
	//Init The C CineBuffer
	PWCineBuffer::InitPWCineBuffer();
	PWCineBuffer::SetAllMaxSize(80);
	PWCineBuffer::SetActiveIndex(0);

	pthread_t thread_id;
	int nRet = 0;

//	//Sim C data
//	nRet = pthread_create(&thread_id, NULL, CSimDataThread, NULL);
//	if (nRet)
//	{
//		LOGE("ERROR: Failed Create CSimDataThread");
//		return false;
//	}

	//C Signal Thread
	nRet = pthread_create(&thread_id, NULL, PWSignalProcessThread, NULL);
	ThreadSpace::GetPWSignalThread()->SetThreadID(thread_id);
	if (nRet)
	{
		//LOGE( "ERROR: Failed Create CSignalProcessThread");
		printf( "ERROR: Failed Create PWSignalProcessThread");
		return false;
	}
	//	ThreadSpace::GetCSignalThread()->Resume();

	//C Image Thread
	nRet = pthread_create(&thread_id, NULL, PWImageProcessThread, NULL);
	ThreadSpace::GetPWImageThread()->SetThreadID(thread_id);
	if (nRet)
	{
		//LOGE( "ERROR: Failed Create CImageProcessThread");
		printf( "ERROR: Failed Create CImageProcessThread");
		return false;
	}
	usleep(10000);
	//	ThreadSpace::GetCImageThread()->Resume();

}
*/
// ------------------------------------------------------------
// Description	:初始化
// Parameter	:void
// Return Value	:
//	false-初始化出错
//	true-正常初始化
// ------------------------------------------------------------
bool InitALLThreads()
{
	//LOG( "GUI", "Init All Threads");
	printf( "GUI", "Init All Threads\n");

	//前端解析数据流线程
	InitParseThread();

	//B 数据流处理线程
	//InitBModeThread();

	//M 数据流处理线程
	InitMultiDeepModeTCDThread();

	//PW 数据流处理线程
	//InitPWModeThread();

	return true;

}

// ------------------------------------------------------------
// Description	:挂起B模式的线程
// Parameter	:void
// Return Value	:
//	false-线程操作出错
//	true-线程操作成功
// ------------------------------------------------------------
//bool PauseBModeThread()
//{
	//GetBSignalThread()->Pause();
	//GetBImageThread()->Pause();

//	return true;
//}

// ------------------------------------------------------------
// Description	:恢复B模式的线程
// Parameter	:void
// Return Value	:
//	false-线程操作出错
//	true-线程操作成功
// ------------------------------------------------------------
//bool ResumBModeThread()
//{
//	GetBSignalThread()->Resume();
//	GetBImageThread()->Resume();

//	return true;
//}

} /* namespace Thread */


