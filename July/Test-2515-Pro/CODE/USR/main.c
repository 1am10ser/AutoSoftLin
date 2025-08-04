//�����޸�
/* -------------------------------------------------- */
// ��Ŀ˵����
// ����.............
// ����..........................
//
//
// ʹ��ע�⣨TesterProductB.v2_20220702����
// ����1. KEY ģ��� PB0/PE7�˿� �� ADC2 ģ��� J68/J69 ���ã�ʹ��ʱע�⣻ͬһʱ��Ҫô���� KEY ģ�飬Ҫô���� ADC2-J68/J69��ע�� J68/J69 ��ص���ҲҪȥ������������ KEYģ�飩
// ����2. �� TIM2 ���� PWM ����ʱ��ͬʱ���� �ⲿ�жϽţ�TIM2_CH2/PD3�� �ⲿ�жϣ������ͻ�����Է���ʹ��
//
//
/* -------------------------------------------------- */

#include "common.h"


/* -------------------------------------------------- */
// �� ������
/* -------------------------------------------------- */

int main()
{
    // �����ڲ�ʱ�ӣ�1��Ƶ
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);


    // ��ʼ��
    // �������ȳ�ʼ��������δ��ʼ��ǰ�����õ��µ�Ƭ������
    UART1_CInit();  //������ģ�飺USB/WIFI/BLE��MOTOR-TMC-UART��
    //UART3_CInit();  //������ģ�飺LIN��MOTOR-ZDT-UART��
 

    // ��ʼ��
    //LIN_Init();


    // ������
    HC164_Init();  //������ģ�飺CONNECT��
    HC165_Init();  //������ģ�飺CONNECT��KEY��
    //HC595_Init();  //������ģ�飺LCD��IOEXT��VPOWER��
    ADC2_CInit();  //������ģ�飺ADC2��
    

    /* �ⲿ�ж� */
    //EXTI_Init();  //������ģ�飺MOTOR��  


    /* IOEXT��IO��չ�� */
    //IOEXT_Init(true);  //������ģ�飺LCD��VPOWER��
    //IOEXT_VarPowerClose(1, true);
    //IOEXT_VarPowerClose(2, true);

    
    /* PWM ��� */
    //TIM1_PWMO_Init(100);
    //TIM1_PWMO_Send(56, 56, 0);
    //TIM1_PWMO_Send(57, 57, 0);
    
    //TIM2_PWMO_Init(200);
    //TIM2_PWMO_Send(55, 0);

    //TIM3_PWMO_Init(300);
    //TIM3_PWMO_Send(54, 0);


    /* PWM ���� */   
    //TIM1_PWMI_Init();
    //TIM1_PWMI_Start();

    //TIM2_PWMI_Init();
    //TIM2_PWMI_Start();
    
    //TIM3_PWMI_Init();
    //TIM3_PWMI_Start();

    
    /* ������ */
    BUZZER_Init();
    //BUZZER_Open(1);
    //BUZZER_Close(1);
    

    /* LCD��Һ������ */
    //LCD_MainInit();
    //LCD_MainLedSceneInit();
    //LCD_MSP3520_SpiOpenScreen();

    
    /* �������ж� */
    //enableInterrupts();  //������ģ�飺LIN��TIM��MOTOR��MOTOR-TMC-UART��MOTOR-ZDT-UART��EXTI��UART1��UART3��


    /* MOTOR������� */
    //MOTOR_Init();  // ��ʼ������ϸ�֡�˳ʱ�롢������á�TIM2��ʼ��
    //MOTOR_SetDir(MOTOR_DIR_CW);  // ���� MOTOR ����
    //MOTOR_SetDiv(MOTOR_DIV_HIGH);  // ���� MOTOR ϸ��
    //MOTOR_InitSpeed(30);  // ��ʼ������ٶ� 
    //MOTOR_RunKeep();  // ��������
    

    /* MOTOR-TMC-UART�����ڵ���� */
    /* ע�����´�����Ϊǣ�浽 UART1 �����жϽ��գ���Ҫ�� enableInterrupts() ֮��ִ�� */
    //MOTOR_TMCUART_Init();                // ��ʼ����������˫�����������
    //MOTOR_TMCUART_SetIRunIHold(80, 10);  // ��������ʱ��ߵ����ٷֱȣ���ֹʱ�����ٷֱ�

    //MOTOR_TMCUART_SetCoolStep(220);      // ���� CoolStep ���ܣ����ܽ���������˿�˵����2ϸ��1000�ٶ�=200��4ϸ��1000�ٶ�=100������˿�˵����2ϸ��1000�ٶ�=130��8ϸ��3000�ٶ�=70��
    //MOTOR_TMCUART_CloseCoolStep();       // �ر� CoolStep ���ܣ����ܽ�����

    //MOTOR_TMCUART_SetMircoStep(1);               // ����ϸ��
    //MOTOR_TMCUART_SetMircoStep(MOTOR_GetDiv());  // ����ϸ�֣����� MOTOR��
    
    //MOTOR_TMCUART_MainRunKeep(2, 1000);  // �������
    //MOTOR_TMCUART_MainRunKeep(0, 0);     // ���ֹͣ


    /* MOTOR-ZDT-UART */
    /* ע�����´�����Ϊǣ�浽 UART3 �����жϽ��գ���Ҫ�� enableInterrupts() ֮��ִ�� */
    //delay_ms(500);  // �ϵ���ʱ
    //MOTOR_ZDTUART_Init();  // ��ʼ����������á�����ϸ��/����/�ٶ�
    //MOTOR_ZDTUART_SetZeroPoint(true);  // �������
    //MOTOR_ZDTUART_SetZeroConfig(true);  // ��������


    /* MOTOR-ZDT-UART + STEP/DIR */
    //MOTOR_SetDiv(MOTOR_DIV_HIGH);
    //MOTOR_SetDir(MOTOR_ZDTUART_DIR_CW);  // ���� MOTOR ����
    //if( MOTOR_ZDTUART_SetDiv(MOTOR_DIV_HIGH,true) ){ MOTOR_ZDTUART_SetDisable(true); return 0; }  // ����ϸ��ֵ���Ӷ���֤ UART �� STEP/DIR ����ģʽ������һ��
    
    
    // ��ѭ������
    while(1){       
        /* PWM ��� */
        //TIM1_PWMO_Send(56, 19, 0);
        //TIM1_PWMO_Send(57, 68, 0);
        
        //if( KEY_IsPress(51,true) == KEY_STATE_YES ) TIM1_PWMO_Send(56, 51, 0);
        //if( KEY_IsPress(50,true) == KEY_STATE_YES ) TIM1_PWMO_Stop(56);
        //if( KEY_IsPress(48,true) == KEY_STATE_YES ) TIM1_PWMO_Send(57, 48, 0);
        //if( KEY_IsPress(47,true) == KEY_STATE_YES ) TIM1_PWMO_Stop(57);

        //if( KEY_IsPress(51,true) == KEY_STATE_YES ) TIM2_PWMO_Send(51, 0);
        //if( KEY_IsPress(50,true) == KEY_STATE_YES ) TIM2_PWMO_Send(100, 0);
        //if( KEY_IsPress(48,true) == KEY_STATE_YES ) TIM2_PWMO_Send(48, 0);
        //if( KEY_IsPress(47,true) == KEY_STATE_YES ) TIM2_PWMO_Stop();
              
        //if( KEY_IsPress(51,true) == KEY_STATE_YES ) TIM3_PWMO_Send(51, 0);
        //if( KEY_IsPress(50,true) == KEY_STATE_YES ) TIM3_PWMO_Stop();
        //if( KEY_IsPress(48,true) == KEY_STATE_YES ) TIM3_PWMO_Send(48, 0);
        //if( KEY_IsPress(47,true) == KEY_STATE_YES ) TIM3_PWMO_Stop();
        
        
        /* PWM ���� */
        //printf("============================\r\n");
        //printf("J54: %dHz, Duty = %f%\r\n", TIM3_PWMI_GetFreq(54), TIM3_PWMI_GetDuty(54));
        //printf("J55: %dHz, Duty = %f%\r\n", TIM2_PWMI_GetFreq(55), TIM2_PWMI_GetDuty(55));
        //printf("J56: %dHz, Duty = %f%\r\n", TIM1_PWMI_GetFreq(56), TIM1_PWMI_GetDuty(56));
        //printf("J57: %dHz, Duty = %f%\r\n", TIM1_PWMI_GetFreq(57), TIM1_PWMI_GetDuty(57));
        
        
        /* VPOWER���ɿص�Դ�� */
        //IOEXT_VarPowerOpen(1, true); delay_ms(2000); IOEXT_VarPowerClose(1, true); delay_ms(2000);
        //IOEXT_VarPowerOpen(2, true); delay_ms(2000); IOEXT_VarPowerClose(2, true); delay_ms(2000);
        //IOEXT_VarPowerOpen(3, true); delay_ms(2000); IOEXT_VarPowerClose(3, true); delay_ms(2000);
        
        
        /* LCD��Һ������ */
        //LCD_MainAnimateIcons();  // ��ʾ��̬ͼ��
        //LCD_MainMotorMonitor();
        //LCD_MSP3520_ShowInteger(0, 0, 5, 12, LCD_COLOR_RED, LCD_COLOR_WHITE, 1, 0);

        
        /* CONNECT��ͨ��·�� */
        //CONNECT_MainConnectMapLed();             // ��ͨ�����ƣ������� LCD_MainLedSceneInit()
        CONNECT_MainConnectAnyMapBuzzer();       // ��һ·��ͨ��ô����������
        //CONNECT_MainConnectMapLedFilterShort();  // ��ͨ�����Ʋ������·�������� LCD_MainLedSceneInit()
  
        
        /* ADC2����ѹ������ */ 
        //ADC2_MainPrintAllAdcValue();  // ��ʾ����ADC�����
        
        
        /* MOTOR������� */
        //MOTOR_MainTwoPointRunByLength(10);  // ��������������У������Ȳ�����
        //MOTOR_MainTwoPointRunByDegree(180);  // ��������������У����ǶȲ�����


        /* MOTOR-TMC-UART�����ڵ���� */
        //MOTOR_TMCUART_MainRoundTrip();            // �������2���������ú����ڲ����Զ�������������У�
        //MOTOR_TMCUART_MainMircoStepLoop();        // ѭ��ִ��ȫ��ϸ��ֵ���ú����ڲ����Զ�������������У�
        //MOTOR_TMCUART_MainStallReverse(220*0.7);  // �����ת��ת���ú����ڲ������Զ�������������У�
        //MOTOR_TMCUART_MainMonitorSGResult();      // ���� SG_RESULT ֵ���ú����ڲ������Զ�������������У�
        
        
        /* MOTOR-ZDT-UART�����ڵ���� */
        //MOTOR_ZDTUART_MainRunAndZero();  // �������ָ���ǶȲ�����
        //MOTOR_ZDTUART_MainRunAndZeroWithPulse();  // �������ָ���ǶȲ����㣨UART+STEP/DIR ģʽ��
        //MOTOR_ZDTUART_MainTwoPointRunByDegree(90);  // ��������������У����ǶȲ�����

        
        /* UART1 */
        UART1_SendByte(0x0F);
        //UART1_MainProductAutoTester();
        //UART1_MainReceiveAndSendToLin();
        //UART1_MainReceiveAndSendToUsbByDirect();    // �������ݲ�ֱ��ת���͸� USB�������͸� UART1 ����
        //UART1_MainReceiveAndSendToUsbByHeadFoot();  // �������ݲ����͸� USB�������͸� UART1 ����
        

        /* LIN */
        //LIN_MASTER_MainFindSlaveId(true);      // ���Ҵӻ��� ID ��ʶ
        //LIN_MASTER_MainPrimaryReceiveToUsb();  // �������źŲ����մӻ��ķ����źţ�Ȼ������� USB
        
        //LIN_MASTER_InitAuxSignal();       // ��ʼ���·��ĸ��ź�
        //LIN_MASTER_SetAuxSignalLLedOn();  // ���źţ���ʹ�ܵ���
        //LIN_MASTER_SetAuxSignalRLedOn();  // ���źţ���ʹ�ܵ���
        //LIN_MASTER_SetAuxSignalBLedOn();  // ���źţ��������
        //LIN_MASTER_SendAuxSignal();       // �����·��ĸ��ź�

        
        /* KEY�������� */
        //KEY_MainKeyMapLed();     // ���µ����ƣ������� LCD_MainLedSceneInit()
        //KEY_MainKeyMapBuzzer();  // ���·�������
    }
}


/**
 * ���Ժ���
 * �ڱ�̵Ĺ�����Ϊ�����ṩ�������
 * STM8 �̶��ĺ���������ṩ�����������ܳ�������ο� stm8s_conf.h
 */
#ifdef USE_FULL_ASSERT
    void assert_failed(u8* file, u32 line)
    {
        while(1){
            // TODO
            // ������ ��ƫ����ѭ�� ʱ�����ڴ˴���ӡ�������ڲ���
        }
    }
#endif