#ifndef __IOEXT_H_
#define __IOEXT_H_



    /* -------------------------------------------------- */
    // �� LED ���� 74HC595 ����
    // �� ʹ��ǰ����ȶ� 74HC595 ���г�ʼ��
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    void IOEXT_Init( bool send );                          // IOEXT ��ʼ��

    void IOEXT_VarPowerOpen( short number, bool send );    // �򿪿ɿص�Դ
    void IOEXT_VarPowerClose( short number, bool send );   // �رտɿص�Դ
    
    void IOEXT_LcdMsp3520Reset();                          // LCD ����
    void IOEXT_LcdMsp3520Enable( bool send );              // LCD ����
    void IOEXT_LcdMsp3520SpiOpen( u8 device, bool send );  // LCD �� SPI ͨѶ����
    void IOEXT_LcdMsp3520SpiClose( bool send );            // LCD �� SPI ͨѶ�ر�
    void IOEXT_LcdMsp3520SendModeSetCmd( bool send );      // LCD ���÷�������
    void IOEXT_LcdMsp3520SendModeSetData( bool send );     // LCD ���÷�������
    
    
    
#endif