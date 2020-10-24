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
*�ļ���     : device_adc.c
*����       : ADC�豸
*ԭ��       : Adc�豸���ܿ飬����DMAʵ��ʵʱ���ݲ���
*           : ����ע�ắ���������������ݸ��ϲ�Ӧ�ñ���                  
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/01       ��ʼ�汾
********************************************************************************
*/

#include "drive.h"
#include "system.h"

#define Sum         4       // 4ͨ��
#define Depth       8       // ÿͨ����8��


static ushort DmaBuffer[Depth][Sum];


/*******************************************************************************
* ����	    : ϵͳʱ��10000/S����0.1mS�������һ�����У���ȡADCֵ��App�����
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
    ADC_TempSensorVrefintCmd(ENABLE);                                               // �¶�ʹ��

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


	
	 

