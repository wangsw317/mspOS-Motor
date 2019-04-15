/*
******************************************************************************
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
******************************************************************************
*文件名   : drive.h
*作用     : 驱动库头文件
*创建时间 : 2013.7.21
******************************************************************************
*/

#include "drive.h"
#if 1
#pragma import(__use_no_semihosting)             //标准库需要的支持函数    

typedef struct __FILE { 	int handle;  }FILE; 
FILE __stdout;

void _sys_exit(int x) {x = x;}

int fputc(int ch, FILE *f)
{
    //if (ch == '\n')                         //换行符
        //System.Device.Usart1.WriteByte('\r');            //增加回车符

    //System.Device.Usart1.WriteByte((byte)ch);

    return ch;
}
#endif
