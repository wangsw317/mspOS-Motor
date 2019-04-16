/*
********************************************************************************
*                                嵌入式微系统
*                                   msOS
*
*                            硬件平台:msPLC DEMO
*                          主芯片:STM32F103R8T6/RBT6
*                           深圳市雨滴科技有限公司
*
*                                作者:王绍伟
*                                网名:凤舞天
*                                标识:wangsw
*
*                                QQ:26033613
*                               QQ群:291235815
*                        论坛:http://bbs.huayusoft.com
*                        淘宝店:http://52edk.taobao.com
*                博客:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*文件名   : system.c
*作用     : 系统应用文件
*创建时间 : 2013.7.21
********************************************************************************
*/

#include "system.h"

uint Empty;
void Dummy(void) {};

ushort CriticalNesting = 0;

uint RomBase;
uint RamBase;
DataStruct * AppDataPointer;
MenuStruct * AppMenuPointer;

static const byte AsciiArray[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

#define DelayUsTick MainClock / 9000000

#define DelayMsTick MainClock / 9000

void Delay(int times)
{
    volatile int counter;
    counter = times;
	while(counter--);
}

void DelayUs(int times)
{
	while(times--)
		Delay(DelayUsTick);
}

void DelayMs(int times)
{
	while(times--)
		Delay(DelayMsTick);
}


byte Ascii(byte hex)
{
	Assert(hex < 16);

    return AsciiArray[hex];

}

/*******************************************************************************
* 函数名	: PostMessage
* 描述	    : 向LogicTask发送消息
* 输入参数  : messageType: 消息类型
*             data: 消息值
* 返回参数  : 无
*******************************************************************************/
void PostMessage(MessageEnum message, uint data)
{
    data |= (uint)message << 24;
    System.OS.PostMessageQueue(data);	
}



CSystem System;


