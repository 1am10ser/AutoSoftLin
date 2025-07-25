#ifndef __UART1_H_
#define __UART1_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    // UART1 ��Ӧ����Ŷ���
    // UART1������ǹ̶��ģ�������ο�оƬԭ��ͼ



    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    void UART1_CInit();                               // UART1 ��ʼ��
    void UART1_SendByte( u8 byte );                   // UART1 �����ֽ�
    void UART1_SendBytes( u8 *bytes, short length );  // UART1 �����ֽڼ�
    
    u8*  UART1_GetReceiveData();                      // ��ȡ���յ������� 
    u8   UART1_GetReceiveDataCount();                 // ��ȡ���յ������ݳ���
    u8   UART1_CopyReceiveData( u8 *data );           // ���ƽ��յ�������
    void UART1_ResetReceiveData();                    // ���ý��յ�������
    


    /* -------------------------------------------------- */
    // �� �����������жϣ�
    /* -------------------------------------------------- */
    void UART1_InterruptHandlerRX();                   // UART1 �����жϻص�����
    void UART1_InterruptHandlerTX();                   // UART1 �����жϻص�����
    


    /* -------------------------------------------------- */
    // �� �������������߼���
    /* -------------------------------------------------- */
    void UART1_MainReceiveAndSendToUsbByDirect();      // ���߼����������ݲ�ֱ��ת���͸� USB�������͸� UART1 ����
    void UART1_MainReceiveAndSendToUsbByHeadFoot();    // ���߼����������ݲ��ع����ݷ��͸� USB�������͸� UART1 ����
    //void UART1_MainReceiveAndSendToLin();            // ���߼����������ݲ����͸� LIN

    // ���߼���KedeAngleSenser.exe ��λ���е� ProductAutoTester ����̨ר��
    void UART1_MainProductAutoTester();
    short UART1_MainProductAutoTesterFuncExe( u8 funcCodeX1, u8 funcCodeAuxY1, u8 funcCodeAuxY2, u8 *sendBytes );
    

#endif