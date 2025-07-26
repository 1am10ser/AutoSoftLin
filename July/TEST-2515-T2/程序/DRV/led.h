#ifndef __LED_H_
#define __LED_H_



    /* -------------------------------------------------- */
    // �� LED ���� 74HC595 ����
    // �� ʹ��ǰ����ȶ� 74HC595 ���г�ʼ��
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void LED_Init( bool render );
    void LED_Render();

    void LED_Open( short number, bool render );
    void LED_OpenAll( bool render );
    
    void LED_Close( short number, bool render );
    void LED_CloseAll( bool render );
    
    void LED_Reverse( short number, bool render );
    void LED_ReverseAll( bool render );
         
    void LED_MainHorseLamp( short delay );
    

    
#endif