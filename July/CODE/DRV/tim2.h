#ifndef __TIM2_H_
#define __TIM2_H_



    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void TIM2_PWMO_Init( u32 freq );  // 初始化 TIM2 PWM 输出

    void TIM2_PWMO_Stop();                              // 终止 TIM2 通道的 PWM 输出
    bool TIM2_PWMO_Send( double duty, u32 sendLimit );  // 发送 TIM2 通道的 PWM 输出
    void TIM2_PWMO_SetDuty( double duty );              // 设置 TIM2 通道的 PWM 正占空比
    
    
    void TIM2_PWMI_Init();            // 初始化 TIM2 PWM 捕获
    void TIM2_PWMI_InitData();        // 
    
    void TIM2_PWMI_Stop();            // 终止 TIM2 PWM 捕获
    void TIM2_PWMI_Start();           // 开始 TIM2 PWM 捕获
    
    u16    TIM2_PWMI_GetFreq();       // 获取 TIM2 捕获的 PWM 频率
    double TIM2_PWMI_GetDuty();       // 获取 TIM2 捕获的 PWM 正占空比



    /* -------------------------------------------------- */
    // ● 函数声明（中断）
    /* -------------------------------------------------- */
    void TIM2_InterruptHandler();            // TIM2 中断
    void TIM2_InterruptHandlerUpdate();      // TIM2 中断：计数器值溢出更新
    void TIM2_InterruptHandlerCapturePwm();  // TIM2 中断：PWM 捕获
    
    
    
#endif