/*
******************************************************************************
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
******************************************************************************
*�ļ���   : drive.h
*����     : ������ͷ�ļ�
*����ʱ�� : 2013.7.21
******************************************************************************
*/

#include "drive.h"

#pragma import(__use_no_semihosting)             //��׼����Ҫ��֧�ֺ���    

typedef struct __FILE { 	int handle;  }FILE; 
FILE __stdout;

void _sys_exit(int x) {x = x;}

extern void PrintfWriteByte(unsigned char data);

int fputc(int ch, FILE *f)
{
    if (ch == '\n')                 //���з�
    PrintfWriteByte('\r');                //���ӻس���

    PrintfWriteByte((unsigned char)ch);

    return ch;
}

