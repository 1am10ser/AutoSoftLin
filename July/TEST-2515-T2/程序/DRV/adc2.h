#ifndef __ADC2_H_
#define __ADC2_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    /**
     * ����
     */
    #define ADC2_INVALID_VOLT  -999



    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void  ADC2_CInit();
    float ADC2_GetVoltValue( short number );
    
    void ADC2_MainVoltMapLed();
    void ADC2_MainShowAllAdcValue();



#endif