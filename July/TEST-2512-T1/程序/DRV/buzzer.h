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
    // �� ��������
    /* -------------------------------------------------- */
    void BUZZER_Init();

    void BUZZER_Open( short number );
    void BUZZER_OpenAll();

    void BUZZER_Close( short number );
    void BUZZER_CloseAll();

    void BUZZER_Reverse( short number );
    void BUZZER_ReverseAll();
    
    
    
#endif