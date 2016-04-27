/*
 * ThreadManage.h
 *
 *  Created on: Apr 27, 2016
 *      Author: ljd
 */

#ifndef THREADMANAGE_H_
#define THREADMANAGE_H_
#include <pthread.h>

#define THREAD_STATUS_RUNNING (true)
#define THREAD_STATUS_PAUSED (false)


namespace ThreadSpace
{
//线程类，包括线程属性以及线程的控制，主要是控制线程的挂起、运行。
class Thread
{
public:
	Thread();
	~Thread();

public:
	// ------------------------------------------------------------
	// Description	:挂起线程
	// Parameter	:void
	// Return Value	:
	//	false-线程本身处于挂起状态
	//	true-成功将线程从运行状态切换到挂起状态
	// ------------------------------------------------------------
	bool Pause(void);

	// ------------------------------------------------------------
	// Description	:恢复线程
	// Parameter	:void
	// Return Value	:
	//	false-线程本身处于运行状态
	//	true-成功将线程从挂起状态切换到运行状态
	// ------------------------------------------------------------
	bool Resume(void);

	// ------------------------------------------------------------
	// Description	:线程挂起检验
	//		此函数放在线程中，运行到此函数处时，如果该线程被设置为来挂起状态，那么线程在此函数处挂起，直到
	//		调用Resume()函数后程序继续运行；如果线程处于运行状态，则马上离开此函数继续运行
	// Parameter	:void
	// Return Value	:void
	// ------------------------------------------------------------
	void SuspendCheck(void);

	// ------------------------------------------------------------
	// Description	:获取线程的状态
	// Parameter	:void
	// Return Value	:
	// true: THREAD_STATUS_RUNNING-运行状态
	// false: THREAD_STATUS_PAUSED-挂起状态
	// ------------------------------------------------------------
	bool GetStatus(void);

	// ------------------------------------------------------------
	// Description	:设置线程的ID
	// Parameter	:void
	// Return Value	:thread_t-pthread_t类型的线程ID
	// ------------------------------------------------------------
	pthread_t GetThreadID() const;

	// ------------------------------------------------------------
	// Description	:设置线程的ID
	// Parameter	:thread_t-pthread_t类型的线程ID
	// Return Value	:void
	// ------------------------------------------------------------
	void SetThreadID(pthread_t thread_t);

private:
	pthread_mutex_t m_mutexPause;	//pthread_mutex_t 互斥锁
	pthread_cond_t m_condPause;		//条件变量
	bool m_bPauseStatus;			//线程状态: false-挂起状态； true-运行状态。
	pthread_t m_threadID;			//线程ID
};


// ------------------------------------------------------------
// Description	:所有线程初始化
// Parameter	:
//	ScanMode-扫描模式枚举量
// Return Value	:
//	true -模式切换成功
//	false-模式切换失败
// ------------------------------------------------------------
bool InitALLThreads(void);

// ------------------------------------------------------------
// Description	:挂起B模式的线程
// Parameter	:void
// Return Value	:
//	false-线程操作出错
//	true-线程操作成功
// ------------------------------------------------------------
bool PauseBModeThread();

// ------------------------------------------------------------
// Description	:恢复B模式的线程
// Parameter	:void
// Return Value	:
//	false-线程操作出错
//	true-线程操作成功
// ------------------------------------------------------------
bool ResumBModeThread();

// ------------------------------------------------------------
// Description	:模式切换
// Parameter	:
//	ScanMode-扫描模式枚举量
// Return Value	:
//	true -模式切换成功
//	false-模式切换失败
// ------------------------------------------------------------
//int ModeChange();

Thread * GetReadThread(void);
Thread * GetParseThread(void);

Thread * GetBSignalThread(void);

Thread * GetBImageThread(void);

Thread * GetMSignalThread(void);

Thread * GetMImageThread(void);

Thread * GetCSignalThread(void);

Thread * GetCImageThread(void);
Thread * GetMultiDeepModeTCDSignalThread(void);
Thread * GetMultiDeepModeTCDImageThread(void);

} /* namespace Thread */


//Add By Gui
//放在这里不太合适，暂且放这里吧
//为了获取系统状态，包括冻结状态、显示模式、扫描模式

// ------------------------------------------------------------
// Description	:获取电影回放的数据帧索引
// Parameter	:void
// Return Value	:电影回放的数据帧索引
// ------------------------------------------------------------
int GetMovieFrameIndex();

// ------------------------------------------------------------
// Description	:设置电影回放的数据帧索引
// Parameter	:nIndex-数据帧索引
// Return Value	:void
// ------------------------------------------------------------
void SetMovieFrameIndex(int nIndex);



#endif /* THREADMANAGE_H_ */
