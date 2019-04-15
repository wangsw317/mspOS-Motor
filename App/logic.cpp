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
*�ļ���     : logic.c
*����       : ҵ���߼������ļ�
*ԭ��       : ���������Ϣ���簴�����������ȣ���׼��һЩ�����Ĵ���ģʽ
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
********************************************************************************
*/


#include "system.h"
#include "app.h"



/*******************************************************************************
* ����	    : ����ҳ�潹���л���������л�����ҳ��
*******************************************************************************/
static void FormFocusProcess(void)
{    
    if (App.Menu.FocusFormPointer == &App.Menu.PortForm)
        App.Menu.FocusFormPointer = &App.Menu.WorkForm;
    else
        App.Menu.FocusFormPointer ++;
}

/*******************************************************************************
* ����	    : �洢�����ı��ؼ�������ֵ
*******************************************************************************/
void SaveProcess(void)
{
    if (App.Menu.FocusFormPointer->FocusTextBoxPointer == null) return;
   
    System.Device.Storage.WriteParameter((uint *)App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer);
}


/*******************************************************************************
* ����	    : �洢���в���
*******************************************************************************/
void LongSaveProcess(void)
{
    TextBox * textBoxPointer;
    textBoxPointer = App.Menu.FocusFormPointer->TextBoxPointer;
    while (textBoxPointer != null)
    {
        System.Device.Storage.WriteParameter(textBoxPointer->DataPointer);
        textBoxPointer = textBoxPointer->NextTextBoxPointer;
    }
}

/*******************************************************************************
* ����	    : Stopȱʡ������
*******************************************************************************/
static void StopDefaultProcess(void)
{
    App.Menu.FocusFormPointer->FocusTextBoxPointer = null;
    App.Menu.FocusFormPointer = &App.Menu.WorkForm;
}

static void TextBoxKeyProcess(KeyEnum key)
{
    void * dataPointer;
    // ���½�Ϊ����, �����ı��ؼ�ֵ�޸ĺ��ֱ�Ӳ������������ֱ�ӵ��ú���ʵ�ֶ���������ǳ���Ҫ
    dataPointer = App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer;
    if (dataPointer == &App.Data.Frequency)
    {
        ;
    }
    else if(dataPointer == &App.Data.PowerPercent)
    {
        ;
    }
}

static byte Counter = 0;
/*******************************************************************************
* ����	    : ������Ϣ������
* �������  : key�����ⰴ��ֵ
*******************************************************************************/
void KeyProcess(KeyEnum key)
{
    switch(key)
    {
        case KeyAdd:                // ���ְ�������
        case KeyLongAdd:
        case KeySub :
        case KeyLongSub:
            System.Gui.ModifyTextBoxData(key);
            TextBoxKeyProcess(key);
            break;
            
        case KeyAuxUp:              // д��־(����)
            System.Device.Storage.WriteLog("Log Test = %d\n", Counter++);
            break;
        case KeyLongAuxUp:          // ��PWMChannel3(����)
            //System.Device.DO.Config(DOPWM);
            //System.Device.DO.SetParameter(PwmChannel3, 18000, 1000, 500);

            //System.Device.DO.SetDutyRatio(PwmChannel4, 500);
            //System.Device.DO.Open(PwmChannel4);

            *App.Data.DO.pY0 = 0;
            *App.Data.DO.pY1 = 1;
            *App.Data.DO.pY2 = 0;
            *App.Data.DO.pY3 = 1;
            *App.Data.DO.pY4 = 0;
            *App.Data.DO.pY5 = 1;
            break;
        case KeyAuxDown:            // ����־(����)
            printf(System.Device.Storage.ReadLog(-1));
            break;
        case KeyLongAuxDown:        // �ر�PWMChannel0(����)
            System.Device.DO.Close(PwmChannel3);
            break;
            
        case KeySave:               // �������ݴ洢
            SaveProcess();     
            break;
        case KeyLongSave :          // ҳ���ݴ洢
            LongSaveProcess();
            break;
            
        case KeyTextBoxFocus:       // �ı��л�
            System.Gui.SwitchTextBoxFocus();
            break; 
        case KeyLongFormFocus:      // ҳ���л�
            FormFocusProcess();
            break;   
 
        case KeyStart:              // ��ʼ����
            App.Data.OnOff = on;
            break;
        case KeyLongStart:          // ��ʱ��(����)
            System.Device.Timer.Start(0, TimerMessage, 1000, FormFocusProcess);
            break;
            
        case KeyStop:               //ֹͣ����
            App.Data.OnOff = off;
        case KeyLongStop:
            //...
            StopDefaultProcess();
            break;         
    }
}

static void AppSystick100(void)
{
    //...
}

static void InitLogic(void)
{
    System.Device.Adc.Register(AdcChannel0, (ushort *)(&App.Data.Voltage));
    System.Device.Adc.Register(AdcChannel1, (ushort *)(&App.Data.Current));
    System.Device.Adc.Register(AdcTemperature, (ushort *)(&App.Data.Temperature));
    System.Device.Systick.Register(Systick100, AppSystick100);
}

void LogicTask(void)
{
    int i;
    uint message;
    uint data; 
    InitLogic();

    App.Menu.FocusFormPointer = &App.Menu.LogoForm;     //ҳ�潹��

    System.OS.DelayMs(2000);
    
    //��Ƶ��Ӧ�����豸ɨƵ�Լ죬��ȡг���
    App.Menu.FocusFormPointer = &App.Menu.CheckForm;   
    for(i = 0; i < 16; i++)
    {
        System.OS.DelayMs(100);
        App.Menu.CheckForm.ChartPointer->Column[i] = i % 5;
    }
    App.Menu.FocusFormPointer = &App.Menu.WorkForm;  

    System.Device.Key.Enable(true);
    //�߼�ҵ�������ȡ��Ϣ�����䴦��
    while(true)
    {     
        message = System.OS.PendMessageQueue();
        
        data = message & 0x00FFFFFF;
        switch(message >> 24)
        {
            case MessageKey:                    //������Ϣ
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
