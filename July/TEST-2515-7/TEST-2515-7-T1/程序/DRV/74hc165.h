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
    // �� ����������API��
    /* -------------------------------------------------- */
    void HC165_Init();                                   // 74HC165 ��ʼ��
    void HC165_ReadDataValues( short number );           // ��ȡ 74HC165 ����
    
    GPIO_TypeDef* HC165_GetDataPort( short number );     // ��ȡ 74HC165 ��ӦоƬ�ϵĶ˿ںţ����ݽ� Q7��
    GPIO_Pin_TypeDef HC165_GetDataPin( short number );   // ��ȡ 74HC165 ��ӦоƬ�ϵ���źţ����ݽ� Q7��
        
    GPIO_TypeDef* HC165_GetLoadPort( short number );     // ��ȡ 74HC165 ��ӦоƬ�ϵĶ˿ںţ�����װ�ؽ� /PL��
    GPIO_Pin_TypeDef HC165_GetLoadPin( short number );   // ��ȡ 74HC165 ��ӦоƬ�ϵ���źţ�����װ�ؽ� /PL��
        
    GPIO_TypeDef* HC165_GetClockPort( short number );    // ��ȡ 74HC165 ��ӦоƬ�ϵĶ˿ںţ�ʱ�ӽ� CP��
    GPIO_Pin_TypeDef HC165_GetClockPin( short number );  // ��ȡ 74HC165 ��ӦоƬ�ϵ���źţ�ʱ�ӽ� CP��
    
    
    
#endif