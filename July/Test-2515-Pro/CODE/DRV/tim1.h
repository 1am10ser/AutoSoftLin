#ifndef __TIM1_H_
#define __TIM1_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    /**
     * Tim1 常量
     */
    #define TIM1_NUMBER_ALL   9999



    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void TIM1_PWMO_Init( u32 freq );           // 初始化 TIM1 PWM 输出

    void TIM1_PWMO_Stop( short number );                              // 终止 TIM1 通道的 PWM 输出
    bool TIM1_PWMO_Send( short number, double duty, u32 sendLimit );  // 发送 TIM1 通道的 PWM 输出
    void TIM1_PWMO_SetDuty( short number, double duty );              // 设置 TIM1 通道的 PWM 正占空比
    
    
    void TIM1_PWMI_Init();                     // 初始化 TIM1 PWM 捕获
    void TIM1_PWMI_Init56();                   //
    void TIM1_PWMI_Init57();                   //
    void TIM1_PWMI_InitData56();               //
    void TIM1_PWMI_InitData57();               //
    
    void TIM1_PWMI_Stop();                     // 终止 TIM1 PWM 捕获
    void TIM1_PWMI_Start();                    // 开始 TIM1 PWM 捕获
    
    u16    TIM1_PWMI_GetFreq( short number );  // 获取 TIM1 捕获的 PWM 频率
    double TIM1_PWMI_GetDuty( short number );  // 获取 TIM1 捕获的 PWM 正占空比



    /* -------------------------------------------------- */
    // ● 函数声明（中断）
    /* -------------------------------------------------- */
    void TIM1_InterruptHandler();              // TIM1 中断
    void TIM1_InterruptHandlerUpdate();        // TIM1 中断：计数器值溢出更新
    void TIM1_InterruptHandlerCapturePwm56();  // TIM1 中断：PWM 捕获（J56接口）
    void TIM1_InterruptHandlerCapturePwm57();  // TIM1 中断：PWM 捕获（J57接口）
    
    
    
#endif