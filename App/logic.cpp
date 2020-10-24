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
*文件名     : logic.c
*作用       : 业务逻辑处理文件
*原理       : 处理各类消息，如按键、传感器等，标准化一些按键的处理模式
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/07/21       初始版本
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
            case MessageKey:                    //按键消息
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


