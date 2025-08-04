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
    // �� ����������API��
    /* -------------------------------------------------- */
    void  MAGNETIC_Init();                             // �ų���Ӧ��ʼ��
    float MAGNETIC_Sampling();                         // �����ų���ѹֵ������
    char  MAGNETIC_GetMagnetic( bool sampling );       // ��ȡ�ų��ż�
    u16   MAGNETIC_GetMagneticGauss( bool sampling );  // ��ȡ�ų���˹ֵ



    /* -------------------------------------------------- */
    // �� �������������߼���
    /* -------------------------------------------------- */
    void  MAGNETIC_MainMagneticShowLed();              // ���߼����ϱ�����ʾ��Ӧ�ĵ�
    
    
    
#endif