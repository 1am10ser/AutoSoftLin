#include "common.h"



/* -------------------------------------------------- */
// �� �ⲿ����
/* -------------------------------------------------- */

/**
 * LIN �ⲿ����
 * ���±��������� lin.m.c �ļ���
 */
extern u8 GV_LinMasterMainARecResponseToUsbDnHead;
extern u8 GV_LinMasterMainARecResponseToUsbDnFoot;



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */
u8 GV_Uart1ReceiveData[20];         // ���ݽ�������
u8 GV_Uart1ReceiveDataCount  = 0;   // ���ݽ���ʵ�ʳ���
u8 GV_Uart1ReceiveDataLength = 20;  // ���ݽ�����������



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * UART1 ��ʼ��
 */
void UART1_CInit()
{
    // ���ô��ڲ���Ϊ��������115200��8λ����λ��1λֹͣλ����У�飬��ֹͬ�����䣬������շ���
    UART1_Init((u32)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    
    // ���� UART1 �ж�
    // UART1_IT_RXNE_OR ��ʾ �������ݼĴ����ǿջ����������ʱ �����ж�
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
    
    // ���� UART1
    UART1_Cmd(ENABLE);
}


/**
 * UART1 �����ֽ�
 *
 * @param byte  8λ�ֽ�
 */
void UART1_SendByte( u8 byte )
{
    // �����ֽ�
    UART1_SendData8(byte);
    
    // �ȴ��������
    // RESET = 0; SET = !RESET; 
    // UART1_FLAG_TXE ��ʾ �������ݼĴ����ձ�־������ʾUART1û�з��ͣ���
    // �������� 1����ô��ʾ URAR1 ������ɻ��������У��������� 0����ô��ʾ UART1 ���ڷ���
    while( UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET );
}


/**
 * UART1 �����ֽڼ�
 *
 * @param bytes   8λ�ֽڼ�
 * @param length  �ֽڼ�����
 */
void UART1_SendBytes( u8 *bytes, short length )
{
    for( short i = 0; i < length; i++ ) UART1_SendByte(bytes[i]);
}



/* -------------------------------------------------- */
// �� �жϻص�����
// �� ���������μ� stm8s.it.c
/* -------------------------------------------------- */

/**
 * UART1 �����жϻص�����
 */
void UART1_InterruptHandlerRX()
{
    // ��ֹ OR ���ش����²�ͣ�Ľ�����ж�
    // STM8 �ڲ����ƣ��ȶ�ȡ UART_SR���ٶ�ȡ UART_DR �ɰ� OR λ�ж�״̬����
    // ��ͨ��PC�˵Ĵ�������� STM8 һ���Է��Ͷ��ֽ�����ʱ�����磺01 02 03������ô STM8 �ͻ�һֱ��ͣ�Ľ�����жϣ�������ѭ��
    if( UART1_GetITStatus(UART1_IT_RXNE) == RESET ) return;
    
    // ��ȡ����
    u8 byte = UART1_ReceiveData8();
    
    // ���ݶ���
    if( GV_Uart1ReceiveDataCount > GV_Uart1ReceiveDataLength ) return;
    
    // �������ݲ��洢��ȫ�ֱ���
    GV_Uart1ReceiveData[GV_Uart1ReceiveDataCount++] = byte;
}


/**
 * UART1 �����жϻص�����
 */
void UART1_InterruptHandlerTX()
{
}



/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */

/**
 * UART1 ���߼����������ݲ�ת���� LIN
 */
void UART1_MainReceiveSendToLin()
{
    // δ��ȡ������
    if( GV_Uart1ReceiveDataCount == 0 ) return;

    // ��ʼ��
    u8 i, bytes[20];
    
    // ��ʱ�ȴ����պ�����ֽ�
    delay_ms(10);

    // ��ʼ��
    u8 recHead   = GV_Uart1ReceiveData[0];
    u8 recLength = GV_Uart1ReceiveData[1];

    // ���������Լ��
    // LIN USB ���������ݽṹ��AE [DatLen] [Dat1] [Dat2] [Dat3] ... AF
    // ��β�ֽ�ͨ���� AE �� AF���ɱ��� GV_LinMasterMainARecResponseToUsbDnHead �� GV_LinMasterMainARecResponseToUsbDnFoot ����
    if( recHead != GV_LinMasterMainARecResponseToUsbDnHead ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( recLength > (GV_Uart1ReceiveDataLength-3) ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( GV_Uart1ReceiveData[recLength+2] != GV_LinMasterMainARecResponseToUsbDnFoot ){ GV_Uart1ReceiveDataCount = 0; return; }

    // ��ȡ��Ч�����ݶ�
    // ȥ�������ֽ� AE��β�ֽ� AF���Լ���2�������ֽ�
    for( i = 0; i < recLength; i++ ) bytes[i] = GV_Uart1ReceiveData[2+i];
    
    // ���ý��ռ���
    GV_Uart1ReceiveDataCount = 0;
    
    // ���ò����� LIN ���ź�
    LIN_MASTER_SetDownASignalBytes(bytes, recLength);
    LIN_MASTER_SendDownASignal();
}



/* -------------------------------------------------- */
// �� ϵͳ���ú����ع�
/* -------------------------------------------------- */

/**
 * ������� printf ����
 * �� UART1 ���������ӡ����
 */

#ifdef __GNUC__
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
/*
// ����һ���ַ�Э��
PUTCHAR_PROTOTYPE 
{
    UART1_SendByte((unsigned char) ch);
    return (ch);
}
*/