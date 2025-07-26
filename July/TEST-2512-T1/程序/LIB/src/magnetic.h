#ifndef __MAGNETIC_H_
#define __MAGNETIC_H_



    /* -------------------------------------------------- */
    // �� MAGNETIC ���� ADC2 �� 3 �ű��
    // �� ʹ��ǰ����ȶ� ADC2 ���г�ʼ��
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    #define MAGNETIC_LED_PIN   GPIO_PIN_7
    #define MAGNETIC_LED_PORT  GPIOD

    #define MAGNETIC_LED_N     GPIO_WriteLow(MAGNETIC_LED_PORT, MAGNETIC_LED_PIN);
    #define MAGNETIC_LED_S     GPIO_WriteHigh(MAGNETIC_LED_PORT, MAGNETIC_LED_PIN);



    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void  MAGNETIC_Init();
    float MAGNETIC_Sampling();
    char  MAGNETIC_GetMagnetic( bool sampling );
    u16   MAGNETIC_GetMagneticGauss( bool sampling );

    void MAGNETIC_MainMagneticShowLed();
    
    
    
#endif