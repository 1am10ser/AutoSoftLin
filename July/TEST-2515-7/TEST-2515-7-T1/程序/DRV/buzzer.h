#ifndef __BUZZER_H_
#define __BUZZER_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    /**
     * ��������Ӧ����Ŷ���
     * ���� buzzer.c �ļ��е� BUZZER_GetPin() �����ڶ��� PIN
     * ���� buzzer.c �ļ��е� BUZZER_GetPort() �����ڶ��� PORT
     */
    // GPIOX
    // GPIO_PIN_X



    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    void BUZZER_Init();                   // ��������ʼ��

    void BUZZER_Open( short number );     // ����������
    void BUZZER_OpenAll();                // ��������ȫ����

    void BUZZER_Close( short number );    // ����������
    void BUZZER_CloseAll();               // ������ȫ����

    void BUZZER_Reverse( short number );  // �������������߾����л�
    void BUZZER_ReverseAll();             // ��������������Ϩ��ȫ�л�
    
    
    
#endif