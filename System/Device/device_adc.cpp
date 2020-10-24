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
*文件名     : device_adc.c
*作用       : ADC设备
*原理       : Adc设备功能块，利用DMA实现实时数据采样
*           : 利用注册函数，回馈采样数据给上层应用变量                  
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/01       初始版本
********************************************************************************
*/

#include "drive.h"
#include "system.h"

#define Sum         4       // 4通道
#define Depth       8       // 每通道采8次


static ushort DmaBuffer[Depth][Sum];


/*******************************************************************************
* 描述	    : 系统时钟10000/S，即0.1mS间隔调用一次运行，获取ADC值给App层变量
*******************************************************************************/
void AdcSystick10000Routine(void)
{
    int i, j;
    uint acc;
    for (i = 0; i < Sum; i++)
    {
        acc = 0;
        for (j = 0; j < Depth; j++)
            acc = acc + DmaBuffer[j][i];
    }
}

CSystem::Device::Adc::Adc(void)
{

 	GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    // IO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ADC
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                       //12MHz
    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;            
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = Sum;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1 , ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2 , ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3 , ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4 , ADC_SampleTime_7Cycles5);
    ADC_TempSensorVrefintCmd(ENABLE);                                               // 温度使能

    ADC_DMACmd(ADC1, ENABLE);	                                            //ADC DMA Enable

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);

    while(ADC_GetResetCalibrationStatus(ADC1));

    ADC_StartCalibration(ADC1);

    while(ADC_GetCalibrationStatus(ADC1));

    // DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		 
	DMA_DeInit(DMA1_Channel1);		  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint)(&(ADC1->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint)DmaBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = Depth * Sum; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);              

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE);

}


	
	 

