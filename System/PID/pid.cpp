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
*�ļ���   : pid.c
*����ʱ�� : 2018.9.8
********************************************************************************
*/





#include "system.h"

#define Pid AppDataPointer->Pid

static void PidRoutine(void)
{
    static float I = 0;
    float error, errorLimit;

    // ���븳ֵ
    // Pid.Input = xxx;

    error = Pid.Target - Pid.Input;
    
    if (error > Pid.ErrorLimit) errorLimit = Pid.ErrorLimit;
    else if(error < -Pid.ErrorLimit) errorLimit = -Pid.ErrorLimit;
    else errorLimit = error;
    
    I += errorLimit;
    
    Pid.Output = Pid.P * error + Pid.I * I;

    // �����ֵ
    // xxx = Pid.Output;
}



void InitPid(void)
{    
    Pid.Acc = 0;
    TSystem.Device.Systick.Register(Systick100, PidRoutine); // ����10mSϵͳ���ģ�һ�㹻����
}

