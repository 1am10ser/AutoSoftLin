#ifndef __MAGNETIC_H_
#define __MAGNETIC_H_



    /* -------------------------------------------------- */
    // ● MAGNETIC 基于 ADC2 的 3 号编号
    // ● 使用前务必先对 ADC2 进行初始化
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    #define MAGNETIC_LED_PIN   GPIO_PIN_7
    #define MAGNETIC_LED_PORT  GPIOD

    #define MAGNETIC_LED_N     GPIO_WriteLow(MAGNETIC_LED_PORT, MAGNETIC_LED_PIN);
    #define MAGNETIC_LED_S     GPIO_WriteHigh(MAGNETIC_LED_PORT, MAGNETIC_LED_PIN);



    /* -------------------------------------------------- */
    // ● 函数声明
    /* -------------------------------------------------- */
    void  MAGNETIC_Init();
    float MAGNETIC_Sampling();
    char  MAGNETIC_GetMagnetic( bool sampling );
    u16   MAGNETIC_GetMagneticGauss( bool sampling );

    void MAGNETIC_MainMagneticShowLed();
    
    
    
#endif