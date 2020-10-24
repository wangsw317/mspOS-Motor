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
*�ļ���   : system.c
*����     : ϵͳӦ���ļ�
*����ʱ�� : 2013.7.21
********************************************************************************
*/

#include "system.h"

typedef struct                              // �����������
{
    uint    *   Start;                      // ָ����п�ʼ
    uint    *   End;                        // ָ����н���
    uint    *   In;                         // ����һ����Ϣ
    uint    *   Out;                        // ȡ��һ����Ϣ
    ushort      Entries;                    // ��Ϣ����
} QueueStruct;

#define QueueBufferSum      40              // ��Ϣ�������
static QueueStruct MessageQueue;
static uint QueueBuffer[QueueBufferSum];                    // ҵ���߼���Ϣ����

uint Empty;
void Dummy(void) {};

ushort CriticalNesting = 0;

uint RomBase;
uint RamBase;
DataStruct * AppDataPointer;

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
* ������	: PostMessage
* ����	    : ��LogicTask������Ϣ
* �������  : messageType: ��Ϣ����
*             data: ��Ϣֵ
* ���ز���  : ��
*******************************************************************************/
bool CSystem::PostMessage(MessageEnum message, uint data)
{
    data |= (uint)message << 24;

    if (MessageQueue.Entries >= QueueBufferSum)
    {                                                              
        return (false);
    }

    EnterCritical();
    MessageQueue.Entries++;
    ExitCritical();
    
    *MessageQueue.In++ = data;
    if (MessageQueue.In > MessageQueue.End)
       	MessageQueue.In = MessageQueue.Start;    

    return true;
}

uint CSystem::PendMessageQueue(void)
{
    uint message;
		
    if (MessageQueue.Entries > 0)
    {                    
       	message = *MessageQueue.Out++;

        EnterCritical();
       	MessageQueue.Entries--;
        ExitCritical();
        
       	if (MessageQueue.Out > MessageQueue.End) 
            MessageQueue.Out = MessageQueue.Start;
        
       	return (message);
    }

    return 0;
}

CSystem::CSystem(void) 
{
    MessageQueue.Start   = QueueBuffer;
    MessageQueue.End     = QueueBuffer + QueueBufferSum - 1;
    MessageQueue.In      = MessageQueue.Start;
    MessageQueue.Out     = MessageQueue.Start;
    MessageQueue.Entries = 0;	
}

CSystem System;

