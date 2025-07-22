#ifndef __74HC595_H_
#define __74HC595_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    /**
     * 74HC595 ��Ӧ����Ŷ���
     * ���� 74hc595.c �ļ��е� HC595_GetDataPin()        �����ڶ��� PIN
     * ���� 74hc595.c �ļ��е� HC595_GetDataPort()       �����ڶ��� PORT
     * ���� 74hc595.c �ļ��е� HC595_GetBitClockPin()    �����ڶ��� PIN
     * ���� 74hc595.c �ļ��е� HC595_GetBitClockPort()   �����ڶ��� PORT
     * ���� 74hc595.c �ļ��е� HC595_GetStoreClockPin()  �����ڶ��� PIN
     * ���� 74hc595.c �ļ��е� HC595_GetStoreClockPort() �����ڶ��� PORT
     */
    // GPIOX
    // GPIO_PIN_X



    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void HC595_Init();
    void HC595_SendByte( short number, u8 byte );
    void HC595_SendOutput( short number );
    
    
    
#endif