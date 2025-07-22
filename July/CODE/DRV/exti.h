#ifndef __EXTI_H_
#define __EXTI_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    /**
     * 常量
     */
    #define EXTI_INT_JITTER  0  // 中断消抖时间，单位毫秒，0表示不消抖动



    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void EXTI_Init();                    // 外部中断初始化
    char EXTI_GetValue( short number );  // 获取外部中断时的电压状态


       
    /* -------------------------------------------------- */
    // ● 函数声明（中断）
    /* -------------------------------------------------- */
    void EXTI_InterruptHandler();                    // EXTI 中断
    void EXTI_InterruptHandlerFall( short number );  // EXTI 中断（下降沿）
    void EXTI_InterruptHandlerRise( short number );  // EXTI 中断（上升沿）
    
    
    
#endif