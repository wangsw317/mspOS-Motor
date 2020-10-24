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
*                                ��ʶ:Wangsw
*
*                                QQ:26033613
*                               QQȺ:291235815
*                        ��̳:http://bbs.huayusoft.com
*                        �Ա���:http://52edk.taobao.com
*                ����:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*�ļ���     : logic.c
*����       : ҵ���߼������ļ�
*ԭ��       : ���������Ϣ���簴�����������ȣ���׼��һЩ�����Ĵ���ģʽ
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
********************************************************************************
*/


#include "system.h"
#include "app.h"



void KeyProcess(KeyEnum key)
{
	printf("Key = %d\n", key);
	System.Device.Misc.Led(true);
}

static void AppSystick100(void)
{
    //...
}

static void InitLogic(void)
{
    System.Device.Systick.Register(Systick100, AppSystick100);
}

void LogicTask(void)
{

    uint message;
    uint data; 
    InitLogic();


    while(true)
    {     
        message = System.PendMessageQueue();
        
        data = message & 0x00FFFFFF;
        switch(message >> 24)
        {
            case MessageKey:                    //������Ϣ
                KeyProcess((KeyEnum)data);
                break;
            case MessageTimer:
                Function(data + RomBase);
                break;
            default:
                break;
        }
    }
}


