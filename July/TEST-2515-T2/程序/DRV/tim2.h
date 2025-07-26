#ifndef __TIM2_H_
#define __TIM2_H_



    /* -------------------------------------------------- */
    // ¡ñ º¯ÊýÉùÃ÷
    /* -------------------------------------------------- */
    void TIM2_PWMO_Init( u32 freq );

    void TIM2_PWMO_Stop();
    bool TIM2_PWMO_Send( double duty, u32 sendLimit, short sendProtectPrev );
    void TIM2_PWMO_SetDuty( double duty );
    
    
    void TIM2_PWMI_Init();
    void TIM2_PWMI_Stop();
    void TIM2_PWMI_Start();
    
    u16 TIM2_PWMI_GetFreq();
    double TIM2_PWMI_GetDuty();


    void TIM2_InterruptHandler();
    void TIM2_InterruptHandlerUpdate();
    void TIM2_InterruptHandlerCapturePwm();
    
    
    
#endif