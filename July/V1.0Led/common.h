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
    #include "stm8s_gpio.h"

    #include "74hc164.h"

    #include "74hc595.h"



    #include <stdlib.h>

    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void delay_ms( short millisec );
    void delay_us( short microsec );

    
    
#endif