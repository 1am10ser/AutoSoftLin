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
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void BUZZER_Init();                   // 蜂鸣器初始化

    void BUZZER_Open( short number );     // 蜂鸣器发声
    void BUZZER_OpenAll();                // 部蜂鸣器全发声

    void BUZZER_Close( short number );    // 蜂鸣器静音
    void BUZZER_CloseAll();               // 蜂鸣器全静音

    void BUZZER_Reverse( short number );  // 蜂鸣器发声或者静音切换
    void BUZZER_ReverseAll();             // 蜂鸣器点亮或者熄灭全切换
    
    
    
#endif