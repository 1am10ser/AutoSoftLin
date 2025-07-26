#ifndef __ADC2_H_
#define __ADC2_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    /**
     * 常量
     */
    #define ADC2_INVALID_VOLT  -999



    /* -------------------------------------------------- */
    // ● 函数声明
    /* -------------------------------------------------- */
    void  ADC2_CInit();
    float ADC2_GetVoltValue( short number );
    
    void ADC2_MainVoltMapLed();
    void ADC2_MainShowAllAdcValue();



#endif