#ifndef __UART1_H_
#define __UART1_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    // UART1 ��Ӧ����Ŷ���
    // UART1������ǹ̶��ģ�������ο�оƬԭ��ͼ



    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void UART1_CInit();
    void UART1_SendByte( u8 byte );
    void UART1_SendBytes( u8 *bytes, short length );
    
    void UART1_InterruptHandlerRX();
    void UART1_InterruptHandlerTX();
    
    void UART1_MainReceiveSendToLin();

    
        
#endif