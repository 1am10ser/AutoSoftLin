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
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void  MAGNETIC_Init();                             // 磁场感应初始化
    float MAGNETIC_Sampling();                         // 采样磁场电压值并返回
    char  MAGNETIC_GetMagnetic( bool sampling );       // 获取磁场磁极
    u16   MAGNETIC_GetMagneticGauss( bool sampling );  // 获取磁场高斯值



    /* -------------------------------------------------- */
    // ● 函数声明（主逻辑）
    /* -------------------------------------------------- */
    void  MAGNETIC_MainMagneticShowLed();              // 主逻辑：南北极显示对应的灯
    
    
    
#endif