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
*文件名     : device_usart3.c
*作用       : 串口设备,用于HMI屏
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2015/11/20       初始版本
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
void Usart3Systick1000Routine(void) 
{
    if (TxdQueue.Entries == 0) return;

    if ((USART3->SR&0x80)==0) return;

    

    USART3->DR = *TxdQueue.Out++;
    
    if (TxdQueue.Out > TxdQueue.End) 
        TxdQueue.Out = TxdQueue.Start;

    EnterCritical();
    TxdQueue.Entries--;
    ExitCritical();
}

/*******************************************************************************
* 描述	    : 串口中断处理函数
*******************************************************************************/
extern "C" void USART3_IRQHandler(void) 
{
    byte data;
    
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
    {
        data = (byte)USART_ReceiveData(USART3);
        
        System.PostMessage(MessageKey, data);
    } 
    
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
		USART_ReceiveData(USART3);
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
extern "C" void CSystem::Device::Usart3::WriteByte(byte data) 
{
    Assert(TxdQueue.Entries < TxdBufferSum);
    
    *TxdQueue.In++ = data;	
    
    if (TxdQueue.In > TxdQueue.End) 
        TxdQueue.In = TxdQueue.Start;

    EnterCritical();
    TxdQueue.Entries++;
    ExitCritical();
}

void CSystem::Device::Usart3::Write(byte * dataPointer, int sum)
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

CSystem::Device::Usart3::Usart3(void) 
{
// GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;   

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  

    
// USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
// USART3_RX   PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

// Usart3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); 

// USART 初始化设置
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);   //开启中断
    USART_Cmd(USART3, ENABLE);                       //使能串口
    
    InitQueue();
}

