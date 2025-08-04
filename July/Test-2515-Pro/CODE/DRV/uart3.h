#ifndef __UART3_H_
#define __UART3_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    // UART3 ��Ӧ����Ŷ���
    // UART3������ǹ̶��ģ�������ο�оƬԭ��ͼ



    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    void UART3_CInit();           // UART3 ��ʼ��
    void UART3_CInitUart();       // UART3 ���ڳ�ʼ��
    void UART3_CInitLinSlave();   // UART3 LIN ���óɴӽڵ�
    void UART3_CInitLinMaster();  // UART3 LIN ���ó����ڵ�
    
    void UART3_SendByte( u8 byte );                   // UART1 �����ֽ�
    void UART3_SendBytes( u8 *bytes, short length );  // UART1 �����ֽڼ�
    
    u8*  UART3_GetReceiveData();                      // ��ȡ���յ������� 
    u8   UART3_GetReceiveDataCount();                 // ��ȡ���յ������ݳ���
    u8   UART3_CopyReceiveData( u8 *data );           // ���ƽ��յ�������
    void UART3_ResetReceiveData();                    // ���ý��յ�������


    
    /* -------------------------------------------------- */
    // �� �����������жϣ�
    /* -------------------------------------------------- */
    void UART3_InterruptHandlerRX();  // UART3 �����жϻص�����
    void UART3_InterruptHandlerTX();  // UART3 �����жϻص�����
    
    void UART3_InterruptHandlerRXUart();              // UART3 ���жϻص����� - �������ݽ���
    void UART3_InterruptHandlerRXTriggerLinSlave();   // UART3 ���жϻص����� - LIN �ӻ����յ���������������
    void UART3_InterruptHandlerRXTriggerLinMaster();  // UART3 ���жϻص����� - LIN �������յ��ӻ�����Ӧ����
    
    
        
#endif