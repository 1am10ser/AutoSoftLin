#ifndef __LIN_SLAVE_H_
#define __LIN_SLAVE_H_

    
    
    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    void LIN_SLAVE_InitSignalResponse();          // ��ʼ����Ӧ���ź�
    void LIN_SLAVE_SendSignalResponse();          // ������Ӧ���ź�
    void LIN_SLAVE_SetSignalResponse( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue );  // ������Ӧ���ź�  

    void LIN_SLAVE_ReceiveStart();                // ����֡��������
    void LIN_SLAVE_ReceiveHandle( u8 dataByte );  // ����֡���մ���
    
    
      
#endif