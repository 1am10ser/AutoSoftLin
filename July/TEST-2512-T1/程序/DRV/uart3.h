#ifndef __UART3_H_
#define __UART3_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    // UART3 ��Ӧ����Ŷ���
    // UART3������ǹ̶��ģ�������ο�оƬԭ��ͼ



    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void UART3_CInit();
    void UART3_CInitLinSlave();
    void UART3_CInitLinMaster();
    
    void UART3_SendByte( u8 byte );
    void UART3_SendBytes( u8 *bytes, short length );
        
    void UART3_InterruptHandlerRX();
    void UART3_InterruptHandlerTX();
    
    void UART3_InterruptHandlerRXTriggerLinSlave();
    void UART3_InterruptHandlerRXTriggerLinMaster();
    
    
        
#endif