#ifndef __74HC165_H_
#define __74HC165_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    /**
     * 74HC165 ��Ӧ����Ŷ���
     * ���� 74hc165.c �ļ��е� HC165_GetDataPin()   �����ڶ��� PIN
     * ���� 74hc165.c �ļ��е� HC165_GetDataPort()  �����ڶ��� PORT
     * ���� 74hc165.c �ļ��е� HC165_GetClockPin()  �����ڶ��� PIN
     * ���� 74hc165.c �ļ��е� HC165_GetClockPort() �����ڶ��� PORT
     */
    // GPIOX
    // GPIO_PIN_X

         

    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void HC165_Init();
    void HC165_ReadDataValues( short number );
    
    GPIO_TypeDef* HC165_GetDataPort( short number );
    GPIO_Pin_TypeDef HC165_GetDataPin( short number );
        
    GPIO_TypeDef* HC165_GetLoadPort( short number );
    GPIO_Pin_TypeDef HC165_GetLoadPin( short number );
        
    GPIO_TypeDef* HC165_GetClockPort( short number );
    GPIO_Pin_TypeDef HC165_GetClockPin( short number );
    
    
    
#endif