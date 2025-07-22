#ifndef __TIM3_H_
#define __TIM3_H_



    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void TIM3_PWMO_Init( u32 freq );  // 初始化 TIM3 PWM 输出

    void TIM3_PWMO_Stop();                              // 终止 TIM3 通道的 PWM 输出
    bool TIM3_PWMO_Send( double duty, u32 sendLimit );  // 发送 TIM3 通道的 PWM 输出
    void TIM3_PWMO_SetDuty( double duty );              // 设置 TIM3 通道的 PWM 正占空比
    
    
    void TIM3_PWMI_Init();            // 初始化 TIM3 PWM 捕获
    void TIM3_PWMI_InitData();        //
    
    void TIM3_PWMI_Stop();            // 终止 TIM3 PWM 捕获
    void TIM3_PWMI_Start();           // 开始 TIM3 PWM 捕获
    
    u16    TIM3_PWMI_GetFreq();       // 获取 TIM3 捕获的 PWM 频率
    double TIM3_PWMI_GetDuty();       // 获取 TIM3 捕获的 PWM 正占空比



    /* -------------------------------------------------- */
    // ● 函数声明（中断）
    /* -------------------------------------------------- */
    void TIM3_InterruptHandler();            // TIM3 中断
    void TIM3_InterruptHandlerUpdate();      // TIM3 中断：计数器值溢出更新
    void TIM3_InterruptHandlerCapturePwm();  // TIM3 中断：PWM 捕获
    
    
    
#endif