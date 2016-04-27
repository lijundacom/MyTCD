/*
 * TimerTest.h
 *
 *  Created on: Apr 21, 2016
 *      Author: ljd
 */

#ifndef TIMERTEST_H_
#define TIMERTEST_H_

#include <sys/time.h>
#include <string>
#include <stdio.h>

//
//
class TimerTest
{
public:
	TimerTest();
	TimerTest(unsigned int);
	TimerTest(unsigned int, std::string);
	~TimerTest();
public:
	void Start(void);//开始计时
	float End(void);//停止，返回平均时间
	void SetCnt(unsigned int nTotalCnt);
	void PrintAverageTime(void); //打印设定统计次数的平均时间，只有在统计到m_nTotalCnt的是否打印

	// ------------------------------------------------------------
	// Description	:Check the Frequence, The statistic count is m_nTotalCnt
	// Parameter	:void
	// Retrun Value	:The Frequence
	// ------------------------------------------------------------
	float CheckFreq(void);

private:
	unsigned int m_nCnt;	//运行次数
	unsigned int m_nTotalCnt; //默认统计次数
	timeval m_startTime;
	timeval m_endTime;
	float m_fUsedTime;
	float m_fATime;//平均时间

	timeval m_freqTime;	//the time for test frequence
	unsigned int m_nFCnt;	//the count for test frequence
	float m_fFreq;	//the frequence for test frequece
	std::string m_Tag;	//tag

};


#endif /* TIMERTEST_H_ */
