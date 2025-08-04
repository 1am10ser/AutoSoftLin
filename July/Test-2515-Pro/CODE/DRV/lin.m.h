#ifndef __LIN_MASTER_H_
#define __LIN_MASTER_H_


    
    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    void  LIN_MASTER_InitAuxSignal();                            //�����źš���ʼ���·��ĸ��ź�
    void  LIN_MASTER_SendAuxSignal();                            //�����źš������·��ĸ��ź�

    void  LIN_MASTER_SetAuxSignalBytes( u8 *bytes, u8 length );  //�����źš������·��ĸ��ź�
    void  LIN_MASTER_SetAuxSignalBLedOn();                       //�����źš������·��ĸ��ź� - �������
    void  LIN_MASTER_SetAuxSignalLLedOn();                       //�����źš������·��ĸ��ź� - ��ʹ�ܵ���
    void  LIN_MASTER_SetAuxSignalRLedOn();                       //�����źš������·��ĸ��ź� - ��ʹ�ܵ���

    void  LIN_MASTER_SendPrimarySignal();                        //�����źš��������źŸ��ӻ�
    void  LIN_MASTER_SendPrimaryReceiveToUsb();                  //�����źš����ͽ��յ��źŵ� USB
    void  LIN_MASTER_PrimaryReceiveReset();                      //�����źš����ý��շ����ź�
    short LIN_MASTER_PrimaryReceiveContain( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue );  //�����źš������źŰ������

    void  LIN_MASTER_FindReceiveReset();                         //��LIN ID ���ҡ����ý��շ����ź�
    void  LIN_MASTER_SendFindSignal( u8 id );                    //��LIN ID ���ҡ��������źŸ��ӻ�
    void  LIN_MASTER_SendFindReceiveToUsb();                     //��LIN ID ���ҡ����ͽ��յ��źŵ� USB

    
    
    /* -------------------------------------------------- */
    // �� �����������жϣ�
    /* -------------------------------------------------- */
    void LIN_MASTER_InterruptHandlerRxPrimaryReceive( u8 byte );  //�����źš����շ������ź�
    void LIN_MASTER_InterruptHandlerRxFindReceive( u8 byte );     //��LIN ID ���ҡ����շ������ź�



    /* -------------------------------------------------- */
    // �� �������������߼���
    /* -------------------------------------------------- */
    void LIN_MASTER_MainFindSlaveId( bool process );  // ���߼������Ҵӻ��� ID ��ʶ
    void LIN_MASTER_MainPrimaryReceiveToUsb();        // ���߼����������źŲ����մӻ��ķ����źţ�Ȼ������� USB
    void LIN_MASTER_MainPrimaryReceiveCheck();        // ���߼����������źŲ����մӻ��ķ����źţ�Ȼ����м��

    

#endif