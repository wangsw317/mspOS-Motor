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
*                        淘宝店:http://52edk.taobao.com
*                      论坛:http://gongkong.eefocus.com/bbs/
*                博客:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*文件名     : app.c
*作用       : 应用程序入口
*原理       : 应用层入口，也是系统入口，主要初始化各类驱动、数据
*           : 建立LogicTask和MenuTask,处理LogicTask内的消息          
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/07/21       初始版本
********************************************************************************
*/


#include "system.h"
#include "app.h"

CApp App;

/*******************************************************************************
* 描述	    : 初始化数据库
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
* 描述	    : C语言入口Main函数
*******************************************************************************/
int main(void) 
{      
	LogicTask();
}
