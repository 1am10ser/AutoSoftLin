#ifndef __TIM1_H_
#define __TIM1_H_
    
    void TIME1_Init(void);


    void TIM1_PWMI_Stop();
    void TIM1_PWMI_Start();
    
    u16 TIM1_PWMI_GetFreq( short number );
    double TIM1_PWMI_GetDuty( short number );

    void TIM1_PWMI_Init();
    void TIM1_PWMI_Init56();
    void TIM1_PWMI_Init57();
    void TIM1_PWMI_InitData56();
    void TIM1_PWMI_InitData57();
    
    void TIM1_InterruptHandler();
    void TIM1_InterruptHandlerCapturePwm56();
    void TIM1_InterruptHandlerCapturePwm57();
    
#endif