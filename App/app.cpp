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
*                        �Ա���:http://52edk.taobao.com
*                      ��̳:http://gongkong.eefocus.com/bbs/
*                ����:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*�ļ���     : app.c
*����       : Ӧ�ó������
*ԭ��       : Ӧ�ò���ڣ�Ҳ��ϵͳ��ڣ���Ҫ��ʼ����������������
*           : ����LogicTask��MenuTask,����LogicTask�ڵ���Ϣ          
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
********************************************************************************
*/


#include "system.h"
#include "app.h"

CApp App;

/*******************************************************************************
* ����	    : ��ʼ�����ݿ�
*******************************************************************************/
CApp::CApp(void)
{
    AppDataPointer = &App.Data;
    App.Data.Voltage = 99;
    App.Data.Current = 101;

    App.Data.OnOff = on;

    App.Data.Rtc.Year = 0x7DD;
    App.Data.Rtc.Month = 4;
    App.Data.Rtc.Day = 0x1A;
    App.Data.Rtc.Hour = 0x11;
    App.Data.Rtc.Minute = 0x2E;
    App.Data.Rtc.Second = 0x28;

    App.Data.SerialNumber = 0;
    App.Data.Year = 17;
    App.Data.Month = 3;
    App.Data.Day = 20;

    App.Data.IdentifyNumber0 = pUint(IdentifyNumberAddress);
    App.Data.IdentifyNumber1 = pUint(IdentifyNumberAddress + 4);
    App.Data.IdentifyNumber2 = pUint(IdentifyNumberAddress + 8);

}

/*******************************************************************************
* ����	    : C�������Main����
*******************************************************************************/
int main(void) 
{      
	LogicTask();
}
