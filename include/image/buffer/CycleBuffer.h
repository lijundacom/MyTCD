/*
 * CycleBuffer.h
 *
 *  Created on: Apr 16, 2016
 *      Author: ljd
 */
// ------------------------------------------------------------
// Name        : CycleBuffer.h
// Description : 循环缓冲区，缓冲区能够存放3个模板数据，每一个数据的大小个动态设置。
// History     : 李俊达，2016-4-16
// ------------------------------------------------------------

#ifndef CYCLEBUFFER_H_
#define CYCLEBUFFER_H_
#include <pthread.h>
#include <vector>
#include <stdio.h>

namespace Buffer
{

//循环缓冲区的长度
const int CYCLELENGTH (20);

template<class T>
class CycleBuffer
{
	//构造函数
public:
	CycleBuffer();
	CycleBuffer(unsigned int size);
public:
	~CycleBuffer();

private:
	//互斥锁，用于保证有效个数的修改
	pthread_mutex_t m_lock;

	//缓冲区的有效数据个数
	int m_nEffitiveNum;

	//写入数据的缓冲区标志：PING-向ping缓冲区写数据；PANG-向pang缓冲区写数据
	int m_nWriteFlag;
	//读取数据的缓冲区标志：PING-从ping缓冲区读数据；PANG-从pang缓冲区读数据
	int m_nReadFlag;

	//缓冲区的指针
	T * m_pCycleBuffer[CYCLELENGTH];
	//申请的空间的指针,pCycleBuffer[0]指向的位置即为pMemory。
	char * m_pMemory;


	//结构数据的大小，即缓冲区的大小，以字节为单位
	int m_nBufferSize;

public:
	//设置缓冲区的大小
	int SetBufferSize(unsigned int size);
	//获取缓冲区大小
	unsigned int GetBufferSize(void);

	//获取写入数据缓冲区
	//当消费者快，当缓冲区没有数据，那么会返回上一次的数据。
	int GetWritePointer(T **);

	//获取读取数据缓冲区
	int GetReadPointer(T **);


};

//构造函数
template<class T>
CycleBuffer<T>::CycleBuffer()
{
	pthread_mutex_init(&m_lock, NULL);

	m_nEffitiveNum = 0;

	//写入数据的缓冲区标志：(0~CYCLELENGTH-1)-(对应位置的缓冲区)
	m_nWriteFlag = 0;
	//读取数据的缓冲区标志：(0~CYCLELENGTH-1)-(对应位置的缓冲区)
	m_nReadFlag = 0;

	//默认设置大小为0
	m_nBufferSize = 0;

	//初始化缓冲区指针
	for(int i=0;i<CYCLELENGTH;++i)
	{
		m_pCycleBuffer[i] = NULL;
	}
	m_pMemory = NULL;
}

// ------------------------------------------------------------
// Description	:构造函数，设置缓冲区的大小
// Parameter	:
//		size-数据缓冲区的大小
// Retrun Value	:
// ------------------------------------------------------------
template<class T>
CycleBuffer<T>::CycleBuffer(unsigned int size)
{
	pthread_mutex_init(&m_lock, NULL);

	m_nEffitiveNum = 0;

	//写入数据的缓冲区标志：(0~CYCLELENGTH-1)-(对应位置的缓冲区)
	m_nWriteFlag = 0;
	//读取数据的缓冲区标志：(0~CYCLELENGTH-1)-(对应位置的缓冲区)
	m_nReadFlag = 0;

	//默认设置大小为0
	m_nBufferSize = size;

	//初始化缓冲区指针
	for(int i=0;i<CYCLELENGTH;++i)
	{
		m_pCycleBuffer[i] = NULL;
	}
	m_pMemory = NULL;

	//
	SetBufferSize(m_nBufferSize);
}

template<class T>
CycleBuffer<T>::~CycleBuffer()
{
	//释放之前申请的空间
	if (NULL != m_pMemory)
	{
		free(m_pMemory);
	}
}

// ------------------------------------------------------------
// Description	:获取写入数据缓冲区指针
// Parameter	:
//		p-写入数据缓冲区
// Retrun Value	:
//		false-获取写入数据缓冲区指针失败
//		true-获取写入数据缓冲区指针成功
// ------------------------------------------------------------
template<class T>
int CycleBuffer<T>::GetWritePointer(T ** p)
{
	//缓冲区满
	if(m_nEffitiveNum >= CYCLELENGTH)
	{
		//printf("The CycleBuffer is Full!\n");
		return false;
	}

	//正常情况

	//返回缓冲区标志
	*p = m_pCycleBuffer[m_nWriteFlag];

	//缓冲区有效数据个数加1
	pthread_mutex_lock(&m_lock);
	m_nEffitiveNum ++;
	pthread_mutex_unlock(&m_lock);

	//计算下一次写入数据的位置
	m_nWriteFlag++;
	m_nWriteFlag = m_nWriteFlag % CYCLELENGTH ;

	return true;
}


// ------------------------------------------------------------
// Description	:获取读取数据缓冲区指针
// Parameter	:
//		p-读数据缓冲区
// Retrun Value	:
//		false-缓冲区中没有有效数据，返回上一次的缓冲区指针
//		true-获取读数据缓冲区指针成功
// ------------------------------------------------------------
template<class T>
int CycleBuffer<T>::GetReadPointer(T **p)
{
	//缓冲区无数据
	if(0 >= m_nEffitiveNum)
	{
		//printf("The CycleBuffer is Empty!\n");
		*p = m_pCycleBuffer[(m_nReadFlag+CYCLELENGTH-1)%CYCLELENGTH];
		return false;
	}

	//正常情况

	//返回缓冲区标志
	*p = m_pCycleBuffer[m_nReadFlag];

	//缓冲区有效数据个数减1
	pthread_mutex_lock(&m_lock);
	m_nEffitiveNum--;
	pthread_mutex_unlock(&m_lock);

	//计算下一次读取数据的位置
	m_nReadFlag++;
	m_nReadFlag = m_nReadFlag % CYCLELENGTH ;

	return true;
}

// ------------------------------------------------------------
// Description	:设置缓冲区大小，动态申请空间
// Parameter	:
//		size-单个缓冲区大小，以字节为单位。
// Retrun Value	:
//		false-设置缓冲区大小失败
//		true-获设置缓冲区大小成功
// ------------------------------------------------------------
template<class T>
int CycleBuffer<T>::SetBufferSize(unsigned int size)
{
	//释放之前申请的空间
	if (NULL != m_pMemory)
	{
		free(m_pMemory);
	}
	//保存缓冲区大小
	m_nBufferSize = size;

	//申请空间
	m_pMemory =  (char *)malloc(CYCLELENGTH * size);

	//检验申请空间是否成功
	if (NULL == m_pMemory)
	{
		return false;
	}

	//给每一个缓冲区指定指针
	for(int i=0;i<CYCLELENGTH;++i)
	{
		m_pCycleBuffer[i] = (T *)(m_pMemory + size*i);
	}

	return true;
}

// ------------------------------------------------------------
// Description	:获取缓冲区大小
// Parameter	:viod
// Retrun Value	:缓冲区的大小
// ------------------------------------------------------------
template<class T>
unsigned int CycleBuffer<T>::GetBufferSize(void)
{
	return m_nBufferSize;
}


}//namespace End



#endif /* CYCLEBUFFER_H_ */
