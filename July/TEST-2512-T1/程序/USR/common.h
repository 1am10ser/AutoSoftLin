#ifndef __COMMON_H_
#define __COMMON_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */
    #define ARR_LENGTH(arr) sizeof(arr)/sizeof(arr[0])



    /* -------------------------------------------------- */
    // ● 加载基础头文件
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
    // ● 函数声明
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
    
    extern unsigned int GV_DisplayX;                                            //初始显示坐标
    extern unsigned int GV_DisplayY;                                            //初始显示坐标

    extern unsigned int GV_ADDX;                                                //附加坐标
    extern unsigned int GV_ADDY;                                                //附加坐标

    extern unsigned int GV_ADDXY;                                               //长宽大小（正方形）
    
//    extern unsigned long GV_MotorOne;                                           //电机转一圈的时间（ms） 建议不要低于1600
//
//    extern unsigned long GV_Time1PrescalerValue;                                //Time1预分频值 建议16
//    extern unsigned long GV_Time2PrescalerValue;                                //Time2预分频值 建议16
//
//    extern unsigned char GV_MotorPrescalerValue;                                //电机细分 =1为1:16 =0为1:2 建议=1
//
//    extern unsigned long GV_MCUTime;                                            //芯片时钟频率 stm8S007为16Mhz
//
//    extern unsigned long GV_Time1OverflowValue;                                 //Time1溢出值
//    extern unsigned long GV_Time2OverflowValue;                                 //Time2溢出值
    
//    extern unsigned char GV_Used1OR3;                                           //型号状态值
    
    extern void AD_360ADValueDisplay(unsigned short t);
    extern void AD_360ADTrueValueDisplay(unsigned short t);
    
//    extern unsigned char GV_PWMVALUESZ[1440]; 
    extern unsigned char GV_ADValueComplete;
    extern unsigned char GV_TIME1Start;
    
#endif