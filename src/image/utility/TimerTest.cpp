/*
 * TimerTest.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: ljd
 */
// ------------------------------------------------------------
// Copyright (c) 2011, 深圳开立科技有限公司。All rights reserved.
// Name        : TimerTest.cpp
// Description :
// History     : GuiHongyun(ghy1874@yeah.net),2013-9-5
//		ver1.1:
//			Modefied By GuiHongyun;
//			Change the frequency method, the pre ver take the count as basic, and here set the time as basic
// ------------------------------------------------------------

#include "image/utility/TimerTest.h"
//#include "image/utility/DebugPrint.h"

TimerTest::TimerTest() :
		m_nTotalCnt(100), m_nCnt(0), m_fATime(0), m_fUsedTime(0), m_nFCnt(0), m_fFreq(0), m_Tag("Timer")
{
	// TODO Auto-generated constructor stub
	m_startTime.tv_sec = 0;
	m_startTime.tv_usec = 0;
	m_endTime.tv_sec = 0;
	m_endTime.tv_usec = 0;
	m_freqTime.tv_sec = 0;
	m_freqTime.tv_usec = 0;

}

//设置统计次数
TimerTest::TimerTest(unsigned int nCnt) :
		m_nCnt(0), m_fATime(0), m_fUsedTime(0), m_nFCnt(0), m_fFreq(0), m_Tag("Timer_")
{
	if (0 == nCnt)
	{
		m_nTotalCnt = 100;
	} else
	{
		m_nTotalCnt = nCnt;
	}
	m_startTime.tv_sec = 0;
	m_startTime.tv_usec = 0;
	m_endTime.tv_sec = 0;
	m_endTime.tv_usec = 0;

	m_freqTime.tv_sec = 0;
	m_freqTime.tv_usec = 0;

	// TODO Auto-generated constructor stub

}

//设置统计次数和计数器标签
TimerTest::TimerTest(unsigned int nCnt, std::string tag) :
		m_nCnt(0), m_fATime(0), m_fUsedTime(0), m_nFCnt(0), m_fFreq(0), m_Tag(tag)
{
	if (0 == nCnt)
	{
		m_nTotalCnt = 100;
	} else
	{
		m_nTotalCnt = nCnt;
	}
	m_startTime.tv_sec = 0;
	m_startTime.tv_usec = 0;
	m_endTime.tv_sec = 0;
	m_endTime.tv_usec = 0;

	m_freqTime.tv_sec = 0;
	m_freqTime.tv_usec = 0;

	// TODO Auto-generated constructor stub

}

TimerTest::~TimerTest()
{
	// TODO Auto-generated destructor stub
}

void TimerTest::Start(void)
{
	gettimeofday(&m_startTime, NULL);
	m_nCnt++;
}

float TimerTest::End(void)
{
	gettimeofday(&m_endTime, NULL);
	m_fUsedTime += (float) (m_endTime.tv_sec - m_startTime.tv_sec) * 1000
			+ (float) (m_endTime.tv_usec - m_startTime.tv_usec) / 1000.0;
	if (m_nTotalCnt <= m_nCnt)
	{
		m_fATime = m_fUsedTime / m_nTotalCnt;
		m_nCnt = 0;
		m_fUsedTime = 0;
	}
	return m_fATime;
}
void TimerTest::SetCnt(unsigned int nTotalCnt)
{
	m_nTotalCnt = nTotalCnt;
}

void TimerTest::PrintAverageTime()
{
	if (0 == m_nCnt)
	{
		//LOGI("Tag: %s, Cal %d Count ,The Avirage Time is %f ms", m_Tag.data(), m_nTotalCnt, m_fATime);
		printf("Tag: %s, Cal %d Count ,The Avirage Time is %f ms", m_Tag.data(), m_nTotalCnt, m_fATime);
	}
}

// ------------------------------------------------------------
// Description	:Check the Frequence, the statistic time is 1s
// Parameter	:void
// Retrun Value	:The Frequence
// ------------------------------------------------------------
float TimerTest::CheckFreq(void)
{

//	if (m_nFCnt >= m_nTotalCnt)
//	{
//		m_nFCnt = 0;
//		timeval curTime;
//		gettimeofday(&curTime, NULL);
//		float fUsedTime = (float) (curTime.tv_sec - m_freqTime.tv_sec) * 1000 + (float) (curTime.tv_usec - m_freqTime.tv_usec) / 1000.0;
//		m_fFreq = m_nTotalCnt * 1000 / fUsedTime;
////		LOGI("Tag: %s, Cal %d Count ,The frequence is %f", m_Tag.data(),m_nTotalCnt, m_fFreq);
//	}
//
//	if (0 == m_nFCnt)
//	{
//		m_nFCnt++;
//		gettimeofday(&m_freqTime, NULL);
//	}else
//	{
//		m_nFCnt++;
//	}

	//get the start time
	if (0 == m_nFCnt)
	{
		gettimeofday(&m_freqTime, NULL);
	}

	//the count number click
	m_nFCnt++;

	//get the cur time
	timeval curTime;
	gettimeofday(&curTime, NULL);
	float fUsedTime = (float) (curTime.tv_sec - m_freqTime.tv_sec) * 1000
			+ (float) (curTime.tv_usec - m_freqTime.tv_usec) / 1000.0;

	//the statistic time is 1s(1000ms)
	float fStatisticTime = 1000;
	if (1000 <= fUsedTime)
	{
		m_fFreq = m_nFCnt * 1000 / fUsedTime;
		//LOGI("Tag: %s, Cal %d Count ,The frequence is %f", m_Tag.data(),m_nFCnt, m_fFreq);
		m_nFCnt = 0;
	}

	return m_fFreq;

}




