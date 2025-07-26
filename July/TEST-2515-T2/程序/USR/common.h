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

    #include "lin.h"
    #include "lin.m.h"
    #include "lin.s.h"

    #include "led.h"
    #include "key.h"
    #include "adc2.h"
    #include "connect.h"

    #include "lcd.h"
    #include "lcd_st7735s.h"

    #include "tim1.h"
    #include "tim2.h"
    #include "tim3.h"

    #include "uart1.h"
    #include "uart3.h"

    #include "a4988.h"

    #include "74hc164.h"
    #include "74hc165.h"
    #include "74hc595.h"



    /* -------------------------------------------------- */
    // ● 函数声明
    /* -------------------------------------------------- */
    void delay_ms( short millisec );
    void delay_us( short microsec );

    
    
#endif