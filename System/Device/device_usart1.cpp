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
*                                标识:Wangsw
*
*                                QQ:26033613
*                               QQ群:291235815
*                        论坛:http://bbs.huayusoft.com
*                        淘宝店:http://52edk.taobao.com
*                博客:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*文件名     : device_usart1.c
*作用       : 串口设备
*原理       : 发送采用Buffer发送，接收采用App注册函数方式接收
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/07/21       初始版本
*V0.2    Wangsw        2013/09/13       第一次更改
********************************************************************************
*/

#include "drive.h"
#include "system.h"

typedef struct 
{
    byte    * Start;            //指向队列开始 
    byte    * End;              //指向队列结束 
    byte    * In;               //插入一个消息  
    byte    * Out;              //取出一个消息  
    volatile ushort    Entries;     //消息长度      
}QueueStruct;

#define TxdBufferSum 1000
static QueueStruct TxdQueue;
static byte TxdBuffer[TxdBufferSum];

static void InitQueue(void)
{
    TxdQueue.Start = TxdBuffer;
    TxdQueue.End = TxdBuffer + TxdBufferSum - 1;
    TxdQueue.In = TxdQueue.Start;
    TxdQueue.Out = TxdQueue.Start;
    TxdQueue.Entries = 0;
}

/*******************************************************************************
* 描述	    : 系统节拍1000/S,即1mS一次调用此服务程序发送缓冲区数据
*******************************************************************************/
void Usart1Systick1000Routine(void) 
{
    if (TxdQueue.Entries == 0) return;

    if ((USART1->SR&0x80)==0) return;

    

    USART1->DR = *TxdQueue.Out++;
    
    if (TxdQueue.Out > TxdQueue.End) 
        TxdQueue.Out = TxdQueue.Start;

    EnterCritical();
    TxdQueue.Entries--;
    ExitCritical();
}

/*******************************************************************************
* 描述	    : 串口中断处理函数
*******************************************************************************/
extern "C" void USART1_IRQHandler(void) 
{
    byte data;
    
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
    {
        data = (byte)USART_ReceiveData(USART1);
        
        PostMessage(MessageKey, data);
    } 
    
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
		USART_ReceiveData(USART1);
} 

void CSystem::Device::Usart1::Open(void)
{
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

void CSystem::Device::Usart1::Close(void)
{
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    USART_Cmd(USART1, DISABLE);
}

extern "C" void PrintfWriteByte(byte data) 
{
    Assert(TxdQueue.Entries < TxdBufferSum);
    
    *TxdQueue.In++ = data;	
    
    if (TxdQueue.In > TxdQueue.End) 
        TxdQueue.In = TxdQueue.Start;

    EnterCritical();
    TxdQueue.Entries++;
    ExitCritical();
}

/*******************************************************************************
* 描述	    : 串口1发送一个字节
* 输入参数  : data 字节类型，发送参数
* 返回参数  : bool 类型
*******************************************************************************/
extern "C" void CSystem::Device::Usart1::WriteByte(byte data) 
{
    Assert(TxdQueue.Entries < TxdBufferSum);
    
    *TxdQueue.In++ = data;	
    
    if (TxdQueue.In > TxdQueue.End) 
        TxdQueue.In = TxdQueue.Start;

    EnterCritical();
    TxdQueue.Entries++;
    ExitCritical();
}

/*******************************************************************************
* 描述	    : 串口1发送一串数据
* 输入参数  : dataPointer 数据指针，sum 数据总数
*******************************************************************************/
void CSystem::Device::Usart1::Write(byte * dataPointer, int sum)
{
    if (sum)
    {
        while (sum--) 
            WriteByte(*dataPointer++);
    }
    else
    {
        while (*dataPointer) 
            WriteByte(*dataPointer++);
    }
}

CSystem::Device::Usart1::Usart1(void) 
{
//GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
//USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

//Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	
  
//USART 初始化设置
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //开启中断
   
    USART_Cmd(USART1, ENABLE);                              //使能串口 

    InitQueue();
}


