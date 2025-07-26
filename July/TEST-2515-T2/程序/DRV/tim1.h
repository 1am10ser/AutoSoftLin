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
    // ● 函数声明
    /* -------------------------------------------------- */
    void TIM1_PWMO_Init( u32 freq );

    void TIM1_PWMO_Stop( short number );
    bool TIM1_PWMO_Send( short number, double duty, u32 sendLimit, short sendProtectPrev );
    void TIM1_PWMO_SetDuty( short number, double duty );

    
    void TIM1_PWMI_Init();
    void TIM1_PWMI_Init56();
    void TIM1_PWMI_Init57();
    
    void TIM1_PWMI_Stop();
    void TIM1_PWMI_Start();
    
    u16 TIM1_PWMI_GetFreq( short number );
    double TIM1_PWMI_GetDuty( short number );

        
    void TIM1_InterruptHandler();
    void TIM1_InterruptHandlerUpdate();
    void TIM1_InterruptHandlerCapturePwm56();
    void TIM1_InterruptHandlerCapturePwm57();
    
    
    
#endif