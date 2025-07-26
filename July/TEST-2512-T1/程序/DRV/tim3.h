#ifndef __TIM3_H_
#define __TIM3_H_



    /* -------------------------------------------------- */
    // ¡ñ º¯ÊýÉùÃ÷
    /* -------------------------------------------------- */
//    void TIM3_PWMO_Init( u32 freq );
//
//    void TIM3_PWMO_Stop();
//    bool TIM3_PWMO_Send( double duty, u32 sendLimit, short sendProtectPrev );
//    void TIM3_PWMO_SetDuty(double duty );
    
    void TIM3_Init();
    
    void TIM3_PWMI_InitData();
    
    void TIM3_PWMI_Init();
    void TIM3_PWMI_Stop();
    void TIM3_PWMI_Start();
    
    u16 TIM3_PWMI_GetFreq();
    double TIM3_PWMI_GetDuty();


    void TIM3_InterruptHandler();
    void TIM3_InterruptHandlerUpdate();
    void TIM3_InterruptHandlerCapturePwm();
    
    extern unsigned char GV_TIME3FLAG;
    
    extern short GV_TIME33Vlaue;
    extern short GV_TIME34Vlaue;
    
#endif