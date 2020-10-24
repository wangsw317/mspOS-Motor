/*
********************************************************************************
*                                嵌入式微系统
*                                    msOS
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
**                        论坛:http://bbs.huayusoft.com
*                        淘宝店:http://52edk.taobao.com
*                博客:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*文件名     : device_do.c
*作用       : PLC开关量输出接口，PLC标识为X端
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2015/07/17       初始版本
********************************************************************************
*/

#include "drive.h"
#include "system.h"

#define CKTIM	((u32)72000000uL) 	/* Silicon running at 72MHz Resolution: 1Hz */
#define PWM_PRSC ((u8)0)                          
#define PWM_PERIOD ((u16) (CKTIM / (u32)(2 * PWM_FREQ *(PWM_PRSC+1)))) //72000000/(2*12500*(1))=2880  2880=PWM 100%
/****	Power devices switching frequency  ****/

#define PWM_FREQ ((u16)12500) // in Hz  (N.b.: pattern type is center aligned),???????12.5kHZ,???????
#define PWM_HalfPerMax   ((u16)1440)
#define REP_RATE (1)    // 1:?PWM??????,0:??????            
 // (N.b): Internal current loop is performed every (REP_RATE + 1)/(2*PWM_FREQ) seconds.               
 // REP_RATE has to be an odd number in case of three-shunt
 // current reading; this limitation doesn't apply to ICS
#define DEADTIME  (u16)((unsigned long long)CKTIM/2*(unsigned long long)DEADTIME_NS/1000000000uL)
#define DEADTIME_NS	((u16)3400)  // 1.5us  in nsec; range is [0...3500]  

#define DCBUS_VOLTAGE			(int32_t)_IQ(24)		//   Q10??,????24V  24576 

#define STATOR_VOLTAGE		 (int32_t)_IQ(13.867) 		// Udc/v3* Q15  13.867   14190  ??   

#define MAX_STATOR_VOLTAGE	(int32_t)_IQ(13.5)  	 // Udc/v3*0.97 13.467   13765 Q15??  



extern "C" void TIM1_BRK_IRQHandler(void)
{
   TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
}
bool b = false;
int i;
extern "C" void TIM1_UP_IRQHandler(void)
{
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    if (++i == 12500)
    {
        i = 0;
        b = !b;
    }
	System.Device.Misc.Led(b);
}
/*******************************************************************************
* 描述	    : 初始化PWM设备
*******************************************************************************/
CSystem::Device::DO::DO(void)
{    
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM1_TimeBaseStructure;
	TIM_OCInitTypeDef TIM1_OCInitStructure;
	TIM_BDTRInitTypeDef TIM1_BDTRInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

	// 8,9为TIM4,用于DA输出
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

	/* Enable TIM1 clock */

	//TIM1 PWM = 12.5KHz
	TIM1_TimeBaseStructure.TIM_Prescaler=0;
	TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
	TIM1_TimeBaseStructure.TIM_Period = PWM_PERIOD;
	TIM1_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM1_TimeBaseStructure.TIM_RepetitionCounter = REP_RATE;//1
	TIM_TimeBaseInit(TIM1, &TIM1_TimeBaseStructure);

	/*???PWM??????*/
	TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????? 1
	TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;   

	TIM1_OCInitStructure.TIM_Pulse =1000; //???
	TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //???????
	TIM1_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; 
	TIM1_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM1_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; 
	TIM_OC1Init(TIM1, &TIM1_OCInitStructure); 

	TIM1_OCInitStructure.TIM_Pulse = 1000; //???
	TIM_OC2Init(TIM1, &TIM1_OCInitStructure);

	TIM1_OCInitStructure.TIM_Pulse = 1000; //dummy value
	TIM_OC3Init(TIM1, &TIM1_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	

	TIM1_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM1_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM1_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1; 

	TIM1_BDTRInitStructure.TIM_DeadTime = DEADTIME;//
	TIM1_BDTRInitStructure.TIM_Break =TIM_Break_Enable;  //  TIM_Break_Enable  TIM_Break_Disable TIM_Break_Enable
	TIM1_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;//???????
	TIM1_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;  

	TIM_BDTRConfig(TIM1, &TIM1_BDTRInitStructure);

	TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
	TIM_ITConfig(TIM1,TIM_IT_Break ,ENABLE);

	TIM_ClearFlag(TIM1, TIM_FLAG_Update);  
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_ITConfig(TIM1,TIM_IT_Update ,ENABLE); 

	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);



	// Break
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//???????0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//???????0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// TIM1
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //   TIM1_UP_IRQn   TIM1_UP_IRQChannel
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//???????1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//???????0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

