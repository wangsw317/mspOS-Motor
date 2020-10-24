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
*文件名     : device_key.c
*作用       : 按键设备
*原理       : 基于SystemTick 100/秒 10mS扫描获取按键值，按键值分为长按与短按两种
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/07/21       初始版本
********************************************************************************
*/



#include "drive.h"
#include "system.h"



#define ShortInterval       30		// 短按间隔
#define LongInterval        150		// 长按间隔

static byte Scan = invalid;
static int ScanCounter = 0;
static bool KeyEnable = false;

static byte RemapKey(byte scan) 
{
    switch(scan)
    {
        case 0xEF:  return(0);           
        case 0xDF:  return(1);         
        case 0xBF:  return(2);            
        case 0x7F:  return(3);       
        case 0xFE:  return(4);         
        case 0xFD:  return(5);       
        case 0xFB:  return(6);   
        case 0xF7:  return(7);
        default:    return(invalid);  
    }
}

static byte RemapLongKey(byte scan) 
{
    switch(scan)
    {
        case 0xEF:  return(0x30);
        case 0xDF:  return(0x31);
        case 0xBF:  return(0x32);
        case 0x7F:  return(0x33);
        case 0xFE:  return(0x34);
        case 0xFD:  return(0x35);
        case 0xFB:  return(0x36);
        case 0xF7:  return(0x37);
        default:    return(invalid);
    }	
}

void KeySystick1000Routine(void) 
{

}

CSystem::Device::Key::Key(void)
{

    
}

void CSystem::Device::Key::Enable(bool status)
{
    KeyEnable = status;
}
