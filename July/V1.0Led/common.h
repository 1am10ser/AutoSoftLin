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
    #include "stm8s_gpio.h"

    #include "74hc164.h"

    #include "74hc595.h"



    #include <stdlib.h>

    /* -------------------------------------------------- */
    // ● 函数声明
    /* -------------------------------------------------- */
    void delay_ms( short millisec );
    void delay_us( short microsec );

    
    
#endif