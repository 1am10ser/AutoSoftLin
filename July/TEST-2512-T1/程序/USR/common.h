#ifndef __COMMON_H_
#define __COMMON_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */
    #define ARR_LENGTH(arr) sizeof(arr)/sizeof(arr[0])



    /* -------------------------------------------------- */
    // �� ���ػ���ͷ�ļ�
    /* -------------------------------------------------- */
    #include "stm8s.h"
    #include "stdio.h"
    #include "stdbool.h"

    #include "74hc595.h"
    #include "74hc165.h"
    #include "74hc164.h"

    #include "adc2.h"
    #include "ioext.h"

    #include "lcd.h"
    #include "lcd_fonts.h"
    #include "lcd_msp3520.h"

    #include "tim1.h"
    #include "tim2.h"
    #include "tim3.h"

    #include "stm8s_flash.h"
    
    #include "motor.h"
    
    #include "key.h"
    
    #include "CONNECT.h"


    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void delay_ms( short millisec );
    void delay_us( short microsec );

    void TIME1_INTERRUPTHANDLER11(void);
    void ADC2_MotorValue(unsigned char J);
    void LCD_ValueDisplay(unsigned int Value,unsigned int Y);
    
    void Flash_360FlashValueDisplay(unsigned short s,unsigned short e,unsigned char g);
    
    void Motor_Function();
    
    void Value_Judge();
    
    void MOTOR_RESET();
    
    void Start_MeasurePWM();
    
    
    extern unsigned int GV_InitialX;
    extern unsigned int GV_InitialY;
    
    extern unsigned int GV_DisplayX;                                            //��ʼ��ʾ����
    extern unsigned int GV_DisplayY;                                            //��ʼ��ʾ����

    extern unsigned int GV_ADDX;                                                //��������
    extern unsigned int GV_ADDY;                                                //��������

    extern unsigned int GV_ADDXY;                                               //�����С�������Σ�
    
//    extern unsigned long GV_MotorOne;                                           //���תһȦ��ʱ�䣨ms�� ���鲻Ҫ����1600
//
//    extern unsigned long GV_Time1PrescalerValue;                                //Time1Ԥ��Ƶֵ ����16
//    extern unsigned long GV_Time2PrescalerValue;                                //Time2Ԥ��Ƶֵ ����16
//
//    extern unsigned char GV_MotorPrescalerValue;                                //���ϸ�� =1Ϊ1:16 =0Ϊ1:2 ����=1
//
//    extern unsigned long GV_MCUTime;                                            //оƬʱ��Ƶ�� stm8S007Ϊ16Mhz
//
//    extern unsigned long GV_Time1OverflowValue;                                 //Time1���ֵ
//    extern unsigned long GV_Time2OverflowValue;                                 //Time2���ֵ
    
//    extern unsigned char GV_Used1OR3;                                           //�ͺ�״ֵ̬
    
    extern void AD_360ADValueDisplay(unsigned short t);
    extern void AD_360ADTrueValueDisplay(unsigned short t);
    
//    extern unsigned char GV_PWMVALUESZ[1440]; 
    extern unsigned char GV_ADValueComplete;
    extern unsigned char GV_TIME1Start;
    
#endif