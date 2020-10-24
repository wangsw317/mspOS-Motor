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
*�ļ���     : device_misc.c
*����       : ��������豸
*ԭ��       : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/08/13       ��ʼ�汾
********************************************************************************
*/

#include "drive.h"
#include "system.h"



#define PinLed	PbOut->Bit2


void CSystem::Device::Misc::Led(bool enable)
{
	PinLed = !enable;
}

void CSystem::Device::Misc::SetDA1(int dutyRatio)
{
	TIM_SetCompare3(TIM4, dutyRatio);
}
void CSystem::Device::Misc::SetDA2(int dutyRatio)
{
	TIM_SetCompare4(TIM4, dutyRatio);
}

/******************************************************************************
* ����	    : ����PWM������Ƶ�� = 72000000 / Prescaler / Period
* �������  : channel: ��һ·Pwm; Prescaler:72MHzʱ��Ԥ��Ƶ; Period:��������ʱ����
*           : PwmChannel0~PwmChannel1:Prescaler < 65536, Period < 65536��  16bit������
*           : PwmChannel3~PwmChannel5:Prescaler < 65536, Period < 65536��  16bit������
*******************************************************************************/
static void InitPwm4(int prescaler, int period, int dutyRatio)
{
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;  
    
    // Ƶ��=72000000 / Prescaler / Period
    TIM_TimeBaseStructure.TIM_Prescaler     = prescaler - 1;
    TIM_TimeBaseStructure.TIM_Period        = period - 1; 
    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse = dutyRatio;

 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);                      
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
    TIM_ARRPreloadConfig(TIM4, ENABLE);
    TIM_Cmd(TIM4, ENABLE);

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);                      
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); 
    TIM_ARRPreloadConfig(TIM4, ENABLE);
    TIM_Cmd(TIM4, ENABLE);       
}

CSystem::Device::Misc::Misc(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	PinLed = 1;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	InitPwm4(1, 1000, 500);
}

