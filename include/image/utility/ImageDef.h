/*
 * ImageDef.h
 *
 *  Created on: Apr 21, 2016
 *      Author: ljd
 *      Description 	: 定义数据、图像处理时所用到的数据信息
 */

#ifndef IMAGEDEF_H_
#define IMAGEDEF_H_

//--------常用量--------add by wxc 13/6/20-------//
#define SAMPLE_FREQ (40)
#define PI (3.14159265)
#define SOUND_SPEED (1540)

//--------图像信息-----------------//
#define MAX_BLINE_LINES	(512)
#define MAX_BLINE_POINTS (10)

#define TGC_NUM (8)
#define MAX_FOCUS_NUM (16)

//表示一个pos代表多少厘米的深度
//表示一个pos代表多少厘米的深度
#define FOCUSPOSUNIT_LINE   0.5
#define FOCUSPOSUNIT_CONVEX 1
#define TRIANGLEH_FOCUS 12

//00、01、10、11分别表示相控阵、凸阵、线阵、双平面、容积探头；
#define PHASE_PROBE 0 //相控阵探头，与凸阵只差半径，多用在心脏诊断中
#define CONVEX_PROBE 1 //凸阵探头
#define LINE_PROBE 2 //线阵探头
#define BIPLANE_PROBE 3 //双平面探头, 两个探头可以一起活
#define VOLUME_PROBE 4 //容积探头，实时三维成像用

//扫查步距-----add by wxc 13/7/16--------------------//
#define MAX_SCANSTEP_NUM 3 //double, full, half
#define SCANSTEP_LOW 0
#define SCANSTEP_MEDIAN 1
#define SCANSTEP_HIGH 2

//---------未分类-----------add by wxc 13/6/20-------//
#define MAX_PHASEPROBE_ANGLE  90
#define STOPSCAN 0
#define STARTSCAN 1
#define ALL_FRONT_LENGTH 32 //32个字节的头数据长度
#define MAX_USB_TRANS 22//按照0905 Download.cpp里的值赋值
#define PARAM_LOAD_TIME 15 //us
#define MAX_MLINE_WIDTH (800)
// #define DOUBLE_MAXBLINE_LEN 960 //彩色时方便计算
// #define MAXMULTI_FOCUSNUM 10	// unusd，与 MAX_FOCUS_NUM 重复
 #define MAXC_ENSEMBLE 8
// #define MAXC_SWEEPNUM 40 //最大的可重复因子,连续扫描C的线号
// #define MAXPW_WALLFILTERNUM 31
// #define MAXPW_HILBERTNUM 32

// #define MAX_FOCUS_NUM 8

// #define MAX_BEAMNUM 4 //多波束

#define MAX_BCOMPOUND_NUM (7)
#define MAXBLINE_LEN (480)
#define MAXC_ENSEMBLE 8
// #define MAX_BFREQ_NUM 5
#define BDYN_RANGE_LEN 32768
//-----------M-------------
#define MAX_M_SPEED_NUM (4)
#define MULTI_REALTIME_B 20 //ms，多实时时B占用的时间段
#define MULTI_REALTIME_PW 80 // 90ms, 多实时时PW占用的时间段
#define MAX_M_SPEED_DANG 4
#define PW_LINE_WIDTH (800)  //每帧800条线算扫描速度
#define MAXPW_WALLFILTERNUM 31

#define MAX_USB_TRANS_B 15
#define MAX_USB_TRANS_C 20
//------------MultiDeepModeTCD-----
//#define MDTCD_WF_NUM 8//做回归滤波器/自相关的点数

#endif /* IMAGEDEF_H_ */
