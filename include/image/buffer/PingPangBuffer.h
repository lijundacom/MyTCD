/*
 * PingPangBuffer.h
 *
 *  Created on: Apr 16, 2016
 *      Author: ljd
 */
// ------------------------------------------------------------
// Name        : PingPangBuffer.h
// Description : 双缓冲区模板,适用于一个生产者、一个消费者的情况
// History     :
// 1.0		2013-6-5		ljd
//                  Initial version
// ------------------------------------------------------------
#ifndef PINGPANGBUFFER_H_
#define PINGPANGBUFFER_H_
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <iostream>
//#include "image/utility/DebugPrint.h"

#define PING 0
#define PANG 1

#define EMPTY 0
#define FULL 1

template<class T>
class PingPangBuffer
{
	//构造函数
public:
	PingPangBuffer();
	PingPangBuffer(unsigned int size);
public:
	~PingPangBuffer();

	//互斥锁
private:

	//Ping缓冲区有数据标志:FULL-有数据；EMPTY-无数据
	int m_nPingFullFlag;
	//Pang缓冲区有数据标志:FULL-有数据；EMPTY-无数据
	int m_nPangFullFlag;

	//写入数据的缓冲区标志：PING-向ping缓冲区写数据；PANG-向pang缓冲区写数据
	int m_nWriteFlag;
	//读取数据的缓冲区标志：PING-从ping缓冲区读数据；PANG-从pang缓冲区读数据
	int m_nReadFlag;

	//Buffer的结构数据
public:
	//Ping 缓冲区的指针
	T * m_pPingBuffer;
	//Pang 缓冲区的指针
	T * m_pPangBuffer;

	//结构数据的大小，即缓冲区的大小，以字节为单位
	int m_nBufferSize;

public:
	//设置缓冲区的大小
	int SetBufferSize(unsigned int size);
	//获取缓冲区大小
	unsigned int GetBufferSize(void);

	//获取写入数据缓冲区
public:
	int GetWritePointer(T **);

	//获取读取数据缓冲区
public:
	int GetReadPointer(T **);

};

//构造函数
template<class T>
PingPangBuffer<T>::PingPangBuffer()
{
	//Ping缓冲区有数据标志:FULL-有数据；EMPTY-无数据
	m_nPingFullFlag = EMPTY;
	//Pang缓冲区有数据标志:FULL-有数据；EMPTY-无数据
	m_nPangFullFlag = EMPTY;

	//写入数据的缓冲区标志：PING-向ping缓冲区写数据；PANG-向pang缓冲区写数据
	m_nWriteFlag = PING;
	//读取数据的缓冲区标志：PING-从ping缓冲区读数据；PANG-从pang缓冲区读数据
	m_nReadFlag = PING;

	//初始化缓冲区指针和大小
	m_pPingBuffer = NULL;
	m_pPingBuffer = NULL;
	m_nBufferSize = 0;
}

// ------------------------------------------------------------
// Description	:构造函数
// Parameter	:
//		size-单个缓冲区大小，以字节为单位。PingPangBuffer类包含Ping缓冲区和Pang缓冲区
// Retrun Value	:void
// ------------------------------------------------------------
template<class T>
PingPangBuffer<T>::PingPangBuffer(unsigned int size)
{
	//Ping缓冲区有数据标志:FULL-有数据；EMPTY-无数据
	m_nPingFullFlag = EMPTY;
	//Pang缓冲区有数据标志:FULL-有数据；EMPTY-无数据
	m_nPangFullFlag = EMPTY;

	//写入数据的缓冲区标志：PING-向ping缓冲区写数据；PANG-向pang缓冲区写数据
	m_nWriteFlag = PING;
	//读取数据的缓冲区标志：PING-从ping缓冲区读数据；PANG-从pang缓冲区读数据
	m_nReadFlag = PING;

	//初始化缓冲区指针和大小
	if (!SetBufferSize(size))
	{
		printf("malloc memory failed at PingPangBuffer!\n");
	}

}

//析构函数
template<class T>
PingPangBuffer<T>::~PingPangBuffer()
{

	//释放之前申请的空间
	if (NULL != m_pPingBuffer)
	{
		free(m_pPingBuffer);
	}
	if (NULL != m_pPangBuffer)
	{
		free(m_pPangBuffer);
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
int PingPangBuffer<T>::GetWritePointer(T ** p)
{
	//写入Ping
	if ((PING == m_nWriteFlag) && (EMPTY == m_nPingFullFlag))
	{
		//std::cout<<"写入Ping"<<std::endl;
		//返回缓冲区标志
		*p = m_pPingBuffer;

		m_nWriteFlag = PANG;
		m_nPingFullFlag = FULL;

		return true;
	}

	//写入Pang
	if ((PANG == m_nWriteFlag) && (EMPTY == m_nPangFullFlag))
	{
		//返回缓冲区标志
		//std::cout<<"写入Pang"<<std::endl;
		*p = m_pPangBuffer;

		m_nWriteFlag = PING;
		m_nPangFullFlag = FULL;
		return true;
	}

	return false;
}

// ------------------------------------------------------------
// Description	:获取读取数据缓冲区指针
// Parameter	:
//		p-读数据缓冲区
// Retrun Value	:
//		false-获取读数据缓冲区指针失败
//		true-获取读数据缓冲区指针成功
// ------------------------------------------------------------
template<class T>
int PingPangBuffer<T>::GetReadPointer(T **p)
{
	//读Ping
	if ((PING == m_nReadFlag) && (FULL == m_nPingFullFlag))
	{
		//返回缓冲区标志
		*p = m_pPingBuffer;

		m_nReadFlag = PANG;
		m_nPingFullFlag = EMPTY;

		return true;
	}

	//读Pang
	if ((PANG == m_nReadFlag) && (FULL == m_nPangFullFlag))
	{
		//返回缓冲区标志
		*p = m_pPangBuffer;

		m_nReadFlag = PING;
		m_nPangFullFlag = EMPTY;

		return true;
	}
	//在存在错误的情况下，返回Ping指向的空间
	*p = m_pPingBuffer;

	return false;
}

// ------------------------------------------------------------
// Description	:设置缓冲区大小，动态申请空间
// Parameter	:
//		size-单个缓冲区大小，以字节为单位。PingPangBuffer类包含Ping缓冲区和Pang缓冲区
// Retrun Value	:
//		false-设置缓冲区大小失败
//		true-获设置缓冲区大小成功
// ------------------------------------------------------------
template<class T>
int PingPangBuffer<T>::SetBufferSize(unsigned int size)
{
	//释放之前申请的空间
	if (NULL != m_pPingBuffer)
	{
		free(m_pPingBuffer);
	}

	//保存缓冲区大小
	m_nBufferSize = size;

	//申请空间
	m_pPingBuffer = (T *) malloc(size);
	m_pPangBuffer = (T *) malloc(size);

	//Init the value to 0
	memset(m_pPingBuffer,0,size);
	memset(m_pPangBuffer,0,size);


	//检验申请空间是否成功
	if ((NULL == m_pPingBuffer) || (NULL == m_pPangBuffer))
	{
		return false;
	}

	return true;
}

// ------------------------------------------------------------
// Description	:获取缓冲区大小
// Parameter	:viod
// Retrun Value	:缓冲区的大小
// ------------------------------------------------------------
template<class T>
unsigned int PingPangBuffer<T>::GetBufferSize(void)
{
	return m_nBufferSize;
}



#endif /* PINGPANGBUFFER_H_ */
