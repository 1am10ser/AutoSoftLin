#ifndef __BUZZER_H_
#define __BUZZER_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    /**
     * 蜂鸣器对应的针脚定义
     * 请在 buzzer.c 文件中的 BUZZER_GetPin() 函数内定义 PIN
     * 请在 buzzer.c 文件中的 BUZZER_GetPort() 函数内定义 PORT
     */
    // GPIOX
    // GPIO_PIN_X



    /* -------------------------------------------------- */
    // ● 函数声明
    /* -------------------------------------------------- */
    void BUZZER_Init();

    void BUZZER_Open( short number );
    void BUZZER_OpenAll();

    void BUZZER_Close( short number );
    void BUZZER_CloseAll();

    void BUZZER_Reverse( short number );
    void BUZZER_ReverseAll();
    
    
    
#endif