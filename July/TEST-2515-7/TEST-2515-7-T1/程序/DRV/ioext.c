#include "common.h"



/* -------------------------------------------------- */
// �� IOEXT ���� 74HC595 ����
// �� ʹ��ǰ����ȶ� 74HC595 ���г�ʼ��
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * IOExt ��������
 */

//��ϵͳʹ�á�IOEXT �� 74HC595 ���ݣ���λ���ȷ��ͣ����� GV_IOExtData ���ݵ� ��λ->��λ ӳ�䵽 U2:Q0->U2:Q7
u8 GV_IOExtData = 0x00;

//��ϵͳʹ�á�IOEXT ʹ�õ� 74HC595 ��� 
u8 GV_IOExtHC595Num = 2;



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * IOEXT ��ʼ��
 * �ú���ʹ��ǰ����ȵ��� HC595_Init() �� 74HC595 ���г�ʼ��
 *
 * IOEXT ��ʼ����Ĭ�Ϸ��͵�����ȫ�� 0��IOEXT 8���Ŷ�Ӧ���豸��Ҫ���е��ö�Ӧ�ĺ������й��ܼ���
 * ���磺U2Q1 ���� 0 ��LCD ���ڸ�λ״̬����ô����Ҫ���� IOEXT_LcdMsp3520Enable(true) ����ʹ LCD �������
 */
void IOEXT_Init( bool send )
{
    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * �򿪿ɿص�Դ
 *
 * @param short number  PCB�ϵĿɿص�Դ���
 * @param bool  send    �Ƿ���
 */
void IOEXT_VarPowerOpen( short number, bool send )
{
    if( number == 1 ) GV_IOExtData |= 0x04;  // U2Q5
    if( number == 2 ) GV_IOExtData |= 0x02;  // U2Q6 
    if( number == 3 ) GV_IOExtData |= 0x01;  // U2Q7

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * �رտɿص�Դ
 *
 * @param short number  PCB�ϵĿɿص�Դ���
 * @param bool  send    �Ƿ���
 */
void IOEXT_VarPowerClose( short number, bool send )
{
    if( number == 1 ) GV_IOExtData &= 0xFB;  // U2Q5
    if( number == 2 ) GV_IOExtData &= 0xFD;  // U2Q6 
    if( number == 3 ) GV_IOExtData &= 0xFE;  // U2Q7

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD ����
 * ���� LCD RESET ��Ϊ�ߵ�ƽ���͵�ƽʱ LCD ��λ��
 *
 * @param bool send  �Ƿ���
 */
void IOEXT_LcdMsp3520Enable( bool send )
{
    GV_IOExtData |= 0x40;  // U2Q1

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD ����
 * ���� LCD RESET ��Ϊ�͵�ƽ -> �ߵ�ƽ��ʵ�� ��λ-> ���� �Ķ���
 */
void IOEXT_LcdMsp3520Reset()
{
    GV_IOExtData &= 0xBF;  // U2Q1;

    // ���͵͵�ƽ��������
    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    HC595_SendByte(GV_IOExtHC595Num);
    
    // �ӳ�
    delay_ms(120);
    
    // LCD ����
    IOEXT_LcdMsp3520Enable(true);
}


/**
 * LCD ����Ļ SPI ͨѶ����
 * �� LCD ����Ļ SPI Ƭѡ��������
 *
 * @param bool send  �Ƿ���
 */
void IOEXT_LcdMsp3520SpiScreenEnable( bool send )
{
    GV_IOExtData &= 0xEF;  // U2Q3
    
    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD ����Ļ SPI ͨѶ�ر�
 * �� LCD ����Ļ SPI Ƭѡ��������
 *
 * @param bool send  �Ƿ���
 */
void IOEXT_LcdMsp3520SpiScreenDisable( bool send )
{
    GV_IOExtData |= 0x10;  // U2Q3

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD �Ĵ洢�� SPI ͨѶ����
 * �� LCD �Ĵ洢�� SPI Ƭѡ��������
 *
 * @param bool send  �Ƿ���
 */
void IOEXT_LcdMsp3520SpiStorageEnable( bool send )
{
    GV_IOExtData &= 0xDF;  // U2Q2
    
    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD �Ĵ洢�� SPI ͨѶ�ر�
 * �� LCD �Ĵ洢�� SPI Ƭѡ��������
 *
 * @param bool send  �Ƿ���
 */
void IOEXT_LcdMsp3520SpiStorageDisable( bool send )
{
    GV_IOExtData |= 0x20;  // U2Q2

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD �Ĵ��� SPI ͨѶ����
 * �� LCD �Ĵ��� SPI Ƭѡ��������
 *
 * @param bool send  �Ƿ���
 */
void IOEXT_LcdMsp3520SpiTouchEnable( bool send )
{
    GV_IOExtData &= 0xF7;  // U2Q4
    
    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD �Ĵ��� SPI ͨѶ�ر�
 * �� LCD �Ĵ��� SPI Ƭѡ��������
 *
 * @param bool send  �Ƿ���
 */
void IOEXT_LcdMsp3520SpiTouchDisable( bool send )
{
    GV_IOExtData |= 0x08;  // U2Q4

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD �� SPI ͨѶ����
 * �� LCD �� SPI Ƭѡ��������
 *
 * @param u8   device  LCD ����豸
 * @param bool send    �Ƿ���
 */
void IOEXT_LcdMsp3520SpiOpen( u8 device, bool send )
{
    IOEXT_LcdMsp3520SpiClose(false);

    if( device == LCD_MSP3520_SPI_DEVICE_TOUCH ) IOEXT_LcdMsp3520SpiTouchDisable(true);
    if( device == LCD_MSP3520_SPI_DEVICE_SCREEN ) IOEXT_LcdMsp3520SpiScreenEnable(true);
    if( device == LCD_MSP3520_SPI_DEVICE_STORAGE ) IOEXT_LcdMsp3520SpiStorageDisable(true);
}


/**
 * LCD �� SPI ͨѶ�ر�
 * �� LCD �� SPI Ƭѡ��������
 *
 * @param bool send  �Ƿ���
 */
void IOEXT_LcdMsp3520SpiClose( bool send )
{
    IOEXT_LcdMsp3520SpiTouchDisable(send);
    IOEXT_LcdMsp3520SpiScreenDisable(send);
    IOEXT_LcdMsp3520SpiStorageDisable(send);
}


/**
 * LCD ���÷�������
 */
void IOEXT_LcdMsp3520SendModeSetCmd( bool send )
{
    GV_IOExtData &= 0x7F;  // U2Q0

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD ���÷�������
 */
void IOEXT_LcdMsp3520SendModeSetData( bool send )
{
    GV_IOExtData |= 0x80;  // U2Q0

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}

