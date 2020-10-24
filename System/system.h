/*
********************************************************************************
*                                Ƕ��ʽ΢ϵͳ
*                                   msOS
*
*                            Ӳ��ƽ̨:msPLC DEMO
*                          ��оƬ:STM32F103R8T6/RBT6
*                           ��������οƼ����޹�˾
*
*                                ����:����ΰ
*                                ����:������
*                                ��ʶ:wangsw
*
*                                QQ:26033613
*                               QQȺ:291235815
*                        ��̳:http://bbs.huayusoft.com
*                        �Ա���:http://52edk.taobao.com
*                ����:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*�ļ���   : system.h
*����     : ϵͳ��ȫ��ͷ�ļ�
*����ʱ�� : 2013.7.21
********************************************************************************
*/


#ifndef __SYSTEM_H
#define __SYSTEM_H


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>



/*
1�����Ͷ������C#����������C#��׼�����õ�Ϊint
2��char��C#��Ϊ16bit��unicode��keil C��Ϊ8bit
*/
//8bit
//typedef char                sbyte;	    /*char Keil4Ĭ�϶���Ϊbyte����Ҫ�����ţ���Ҫ����keil����*/
typedef unsigned char       byte;       
//16bit
//typedef short             short
typedef unsigned short      ushort;
//32bit
//typedef int               int;
typedef unsigned int        uint;
//�ַ���
typedef char *              string;
#ifndef __cplusplus
    typedef unsigned char       bool;
#endif 

typedef union
{
    byte    Byte[2];
    ushort  Ushort;
}Union16;

typedef union
{
    byte    Byte[4];
    ushort  Ushort[2];
    uint    Uint;
}Union32;

typedef void (*function)(void);
#define Function(address)   ((function)(address))()

typedef enum
{
    MessageKey      = 0x01,     // ��Ϣ���Ͳ���Ϊ0x00
    MessageTimer    = 0x02
    /*���������*/
}MessageEnum;

#include "os.h"
#include "gui.h"
#include "device.h"  
#include "modbus.h"
#include "pid.h"
#include "data.h"





#define LCD12864
//#define msMenu

#define true                        1
#define false                       0

#define on                          1
#define off                         0

#define ok                          1
#define yes                         1
#define no                          0
#ifdef __cplusplus
#define null                        0                               // ��ָ��
#else
#define null                        ((void *) 0)                    // ��ָ��
#endif
#define invalid                     0xFF                            // ��Чֵ
#define MainClock                   72000000                        // ϵͳʱ��Ƶ��
#define IdentifyNumberAddress       0x1FFFF7E8                      // оƬID��

#define SetBit(data, offset)        ((data) |= 1U << (offset))      // ��λ
#define ResetBit(data, offset)      ((data) &= ~(1U << (offset)))   // ��λ
#define GetBit(data, offset)        (((data) >> (offset)) & 0x01)   // ��ȡλ

#define Byte0(data)                 ((byte *)(&(data)))[0]          // ��Ҫ���ڶ�̬����
#define Byte1(data)                 ((byte *)(&(data)))[1]          // ��Ҫ���ڶ�̬����
#define Byte2(data)                 ((byte *)(&(data)))[2]          // ��Ҫ���ڶ�̬����
#define Byte3(data)                 ((byte *)(&(data)))[3]          // ��Ҫ���ڶ�̬����

#define Ushort0(data)               ((ushort *)(&(data)))[0]        // ��Ҫ���ڶ�̬����
#define Ushort1(data)               ((ushort *)(&(data)))[1]        // ��Ҫ���ڶ�̬����

#define Byte(data)                  *((byte *)(&data)) 
#define Ushort(data)                *((ushort *)(&data)) 
#define Uint(data)                  *((uint *)(&data)) 
#define Float(data)                 *(float *)(&data)

#define pByte(address)              *((byte *)(address)) 
#define pUshort(address)            *((ushort *)(address)) 
#define pUint(address)              *((uint *)(address)) 
#define pFloat(address)             *(float *)(address)

#define Assert(express)  if (!(express)){printf("\nASSERT: " #express "\n");}   //(void)0
 				  
extern uint Empty;
extern void Dummy(void);
extern ushort CriticalNesting;
#define	EnterCritical()		__disable_irq(); CriticalNesting++;
#define ExitCritical()		if(--CriticalNesting == 0){__enable_irq();}

#define EnableIrq()         __enable_irq();
#define DisableIrq()        __disable_irq();

extern uint RomBase;
extern uint RamBase;
extern DataStruct * AppDataPointer;


extern void Delay(int times);

extern void DelayUs(int times);

extern void DelayMs(int times);

extern byte Ascii(byte hex);

extern void PostMessage(MessageEnum message, uint data);

/*******************************************************************************
* �ṹ����	: SystemStruct
* ����	    : msOS������Ҫ��һ���ṹ�壬��װ��System������нӿ�
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/



class CSystem
{
public:
    CSystem(void);
    bool PostMessage(MessageEnum message, uint data);
    uint PendMessageQueue(void);
    
    class Device
    {     
    public:
        class Init
        {
        public:
            Init(void);
        }Init;
#if 0
        class Usart1
        {
        public:
            Usart1(void);
            void Open(void);
            void Close(void);
            void WriteByte(byte data);
            void Write(byte * dataPointer, int sum);
        }Usart1;
        
        class Usart2
        {
        public:
            Usart2(void);
            void Register(uint rxdFucntion);
            void Write(byte * dataPointer, int sum);
        }Usart2;
#endif
        class Usart3
        {
        public:
            Usart3(void);
            void WriteByte(byte data);
            void Write(byte * dataPointer, int sum);
        }Usart3;
        
        class Adc
        {
        public:
            Adc(void);
        }Adc;
        
        class DI
        {
        public:
            DI(void);
        }DI;

        class DO
        {
        public:
            DO(void);
        }DO;
#if 0
        class Key
        {
        public:
            Key(void);
            void Enable(bool status);
        }Key;

        class Lcd
        {
        public:
            Lcd(void);
            void DisplayString(byte y, string string);
        }Lcd;
#endif
        class Misc
        {
        public:
            Misc(void);
            void Led(bool enable);
            void SetDA1(int dutyRatio);
            void SetDA2(int dutyRatio);
        }Misc;
        
        class Storage
        {
        public:
            Storage(void);
            bool WriteParameter(void * dataPointer);  
            void EraseLog(void);
            char * ReadLog(int sequence);
            bool WriteLog(char *fmt, ...);
        }Storage;

        class Timer
        {
        public:
            void Start(int id, TimerModeEnum mode, int times, function functionPointer);
            void Stop(int id); 
        }Timer;

        class Systick
        {
        public:
            Systick(void);
            bool Register(SystickEnum type, function functionPointer);
        }Systick;
    }Device;
};

extern CSystem System;
#endif 
