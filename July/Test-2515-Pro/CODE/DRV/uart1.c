#include "common.h"



/* -------------------------------------------------- */
// �� �ⲿ����
/* -------------------------------------------------- */

/**
 * LIN �ⲿ����
 * ���±��������� lin.m.c �ļ���
 */
//extern u8 GV_LinMasterMainARecResponseToUsbDnHead;
//extern u8 GV_LinMasterMainARecResponseToUsbDnFoot;



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */
u8 GV_Uart1ReceiveData[40];         // ���ݽ�������
u8 GV_Uart1ReceiveDataCount  = 0;   // ���ݽ���ʵ�ʳ���
u8 GV_Uart1ReceiveDataLength = 40;  // ���ݽ�����������



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


/**
 * ��ȡ���յ�������
 *
 * return u8*  ����ָ��
 */
u8* UART1_GetReceiveData()
{
    return GV_Uart1ReceiveData;
}


/**
 * ��ȡ���յ������ݳ���
 *
 * return u8
 */
u8 UART1_GetReceiveDataCount()
{
    return GV_Uart1ReceiveDataCount;
}


/**
 * ���ƽ��յ�������
 *
 * @param u8 *data  ���飬�������ֽ��������� >= GV_Uart1ReceiveDataLength
 * return u8 ���������ݸ���
 */
u8 UART1_CopyReceiveData( u8 *data )
{
    // ��������
    for( u8 i = 0; i < GV_Uart1ReceiveDataCount; i++ ) data[i] = GV_Uart1ReceiveData[i];

    // ����
    return GV_Uart1ReceiveDataCount;
}


/**
 * ���ý��յ�������
 */
void UART1_ResetReceiveData()
{
    // ���ñ���
    GV_Uart1ReceiveDataCount = 0;
    for( u8 i = 0; i < GV_Uart1ReceiveDataLength; i++ ) GV_Uart1ReceiveData[i] = 0;
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
    // ��ֹ OR ���ش����²�ͣ�Ľ�����жϣ�OR Դ�� UART1_IT_RXNE_OR��
    // STM8 �ڲ����ƣ��ȶ�ȡ UART_SR���ٶ�ȡ UART_DR �ɰ� OR λ�ж�״̬����
    // ��ͨ��PC�˵Ĵ�������� STM8 һ���Է��Ͷ��ֽ�����ʱ�����磺01 02 03������ô STM8 �ͻ�һֱ��ͣ�Ľ�����жϣ�������ѭ��
    if( UART1_GetITStatus(UART1_IT_RXNE) == RESET ) return;
    
    // ��ȡ����
    u8 byte = UART1_ReceiveData8();
    
    // �������ݣ��������������
    if( GV_Uart1ReceiveDataCount < GV_Uart1ReceiveDataLength ) GV_Uart1ReceiveData[GV_Uart1ReceiveDataCount++] = byte;
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
 * ���߼����������ݲ����͸� LIN
 * ���������������ݺ����ʵ����Ҫ�����ڸú��������ݽ��вɼ�/�ع���Ȼ���ٷ��͸� LIN
 *
 * ע�⣺���ڸú��������� UART1 �Ľ��ռ���������ע������� UART1_MainXXX ��ͻ
 */
/*
void UART1_MainReceiveAndSendToLin()
{
    // δ��ȡ������
    if( GV_Uart1ReceiveDataCount == 0 ) return;

    // ��ʼ��
    u8 i, bytes[20];
    
    // ��ʱ�ȴ����պ�����ֽ�
    delay_ms(20);

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
*/


/**
 * ���߼����������ݲ�ֱ��ת���͸� USB�������͸� UART1 ����
 * ע�⣺���ڸú��������� UART1 �Ľ��ռ���������ע������� UART1_MainXXX ��ͻ
 */
void UART1_MainReceiveAndSendToUsbByDirect()
{
    // δ��ȡ������
    if( GV_Uart1ReceiveDataCount == 0 ) return;

    // ��ʼ��
    u8 i, sendLength, sendBytes[40];  // ���͵����ݳ��Ⱥ��������飨�����ֽڳ��Ⱥ� GV_Uart1ReceiveDataLength һ�£� 
    
    // ��ʱ�ȴ����պ�����ֽ�
    delay_ms(20);


    // �������ص����ݣ����ƽ��յ�������
    sendLength = GV_Uart1ReceiveDataCount;
    for( i = 0; i < sendLength; i++ ) sendBytes[i] = GV_Uart1ReceiveData[i];

    
    // ���ý��ռ���
    GV_Uart1ReceiveDataCount = 0;

    // UART1 ����
    UART1_SendBytes(sendBytes, sendLength);
}


/**
 * ���߼����������ݲ��ع����ݷ��͸� USB�������͸� UART1 ����
 * ���������������ݺ����ʵ����Ҫ�����ڸú��������ݽ��вɼ�/�ع���Ȼ���ٷ��͸� USB
 *
 * ע�⣺���ڸú��������� UART1 �Ľ��ռ���������ע������� UART1_MainXXX ��ͻ
 * 
 * ע�⣺UART1 ���յ����ݸ�ʽҪ�󣨼���λ���������԰�����ݸ�ʽҪ�󣩣�AE [DatLen] [Dat1] [Dat2] [Dat3] ... AF
 * ������UART1 ���ص����ݸ�ʽ��׼�������԰巢����λ�������ݸ�ʽ��׼����AE [DatLen] [Dat1] [Dat2] [Dat3] ... AF
 */
void UART1_MainReceiveAndSendToUsbByHeadFoot()
{
    // δ��ȡ������
    if( GV_Uart1ReceiveDataCount == 0 ) return;

    // ��ʼ��
    u8 i, sendLength, sendBytes[40];  // ���͵����ݳ��Ⱥ��������飨�����ֽڳ��Ⱥ� GV_Uart1ReceiveDataLength һ�£� 
    
    // ��ʱ�ȴ����պ�����ֽ�
    delay_ms(20);

    // ��ʼ��
    u8 recHead = GV_Uart1ReceiveData[0];
    u8 recDatLength = GV_Uart1ReceiveData[1];

    // ���������Լ��
    if( recHead != 0xAE ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( recDatLength > (GV_Uart1ReceiveDataLength-3) ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( GV_Uart1ReceiveData[recDatLength+2] != 0xAF ){ GV_Uart1ReceiveDataCount = 0; return; }

    // �������ص����ݣ����ƽ��յ�������
    for( i = 0; i < recDatLength; i++ ) sendBytes[2+i] = GV_Uart1ReceiveData[2+i];

    // ��ȫ���ص����ݸ�ʽ��׼
    sendLength = recDatLength + 3;
    sendBytes[0] = 0xAE; sendBytes[1] = recDatLength; sendBytes[2+recDatLength] = 0xAF;

    // ���ý��ռ���
    GV_Uart1ReceiveDataCount = 0;

    // UART1 ����
    UART1_SendBytes(sendBytes, sendLength);
}


/**
 * ���߼���KedeAngleSenser.exe ��λ���е� ProductAutoTester ����̨ר��
 *
 * ע�⣺���ڸú��������� UART1 �Ľ��ռ���������ע������� UART1_MainXXX ��ͻ

 * ע�⣺UART1 ���յ����ݸ�ʽҪ�󣨼���λ���������԰�����ݸ�ʽҪ�󣩣�AE AF [DatLen] [Dat1] [Dat2] [Dat3] ... BE BF
 * ������UART1 ���ص����ݸ�ʽ��׼�������԰巢����λ�������ݸ�ʽ��׼����CE CF [DatLen] [Dat1] [Dat2] [Dat3] ... CE CF
 */
void UART1_MainProductAutoTester()
{
        // δ��ȡ������
    if( GV_Uart1ReceiveDataCount == 0 ) return;

    // ��ʼ��
    u8 dataLength, sendLength, sendBytes[40];  // ���͵����ݳ��Ⱥ��������飨�����ֽڳ��Ⱥ� GV_Uart1ReceiveDataLength һ�£� 
    
    // ��ʱ�ȴ����պ�����ֽ�
    delay_ms(20);

    // ��ʼ��
    u8 recHead1 = GV_Uart1ReceiveData[0];
    u8 recHead2 = GV_Uart1ReceiveData[1];
    u8 recDatLength = GV_Uart1ReceiveData[2];

    // ���������Լ��
    if( recHead1 != 0xAE || recHead2 != 0xAF ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( recDatLength > (GV_Uart1ReceiveDataLength-5) ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( GV_Uart1ReceiveData[recDatLength+3] != 0xBE || GV_Uart1ReceiveData[recDatLength+4] != 0xBF ){ GV_Uart1ReceiveDataCount = 0; return; }

    // ��ȡ������͹��ܸ�����
    u8 funcCodeX1    = GV_Uart1ReceiveData[3];
    u8 funcCodeAuxY1 = GV_Uart1ReceiveData[4];
    u8 funcCodeAuxY2 = GV_Uart1ReceiveData[5];

    // ��ʼ������
    sendBytes[0] = 0xCE;
    sendBytes[1] = 0xCF;
    sendBytes[2] = 0xFF;  // ��ʼ���ݳ��ȣ���������������ʼ/������
    sendBytes[3] = funcCodeX1;
    
    // ִ�й�����
    dataLength = UART1_MainProductAutoTesterFuncExe(funcCodeX1, funcCodeAuxY1, funcCodeAuxY2, sendBytes);

    // ��ȫ������
    dataLength = dataLength+1;       // �������ݳ��ȣ�+1 ��ʾ���Ϲ�����
    sendBytes[2] = dataLength;       // �������ݳ��ȣ���������������ʼ/������
    
    sendBytes[dataLength+3] = 0xDE;  // ���ý�����
    sendBytes[dataLength+4] = 0xDF;  // ���ý�����
    
    sendLength = dataLength+5;       // �������ͳ���
    
    
    // ���ý��ռ���
    GV_Uart1ReceiveDataCount = 0;

    // UART1 ����
    UART1_SendBytes(sendBytes, sendLength);
}
short UART1_MainProductAutoTesterFuncExe( u8 funcCodeX1, u8 funcCodeAuxY1, u8 funcCodeAuxY2, u8 *sendBytes )
{
    // ��ʼ��
    u8  dataLength = 0;
    u8  sendBytesStart = 4;  // �����ֽڼ�������ʼλ��ע��ǰ4λ�Ѿ���ʹ�ã��μ� UART1_MainProductAutoTester()
    u16 funcCodeAux;
    
    // ��ʼ��
    short error = 0;
    
    // �����룺���õ������
    if( funcCodeX1 == 0x01 ){
        error = MOTOR_ZDTUART_SetEnable();

        dataLength = 1;
        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }
    
    // �����룺���õ��������
    if( funcCodeX1 == 0x02 ){
        error = MOTOR_ZDTUART_SetDisable();

        dataLength = 1;
        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }
    
    // �����룺���õ�����
    if( funcCodeX1 == 0x03 ){
        MOTOR_ZDTUART_SetEnable();
        error = MOTOR_ZDTUART_SetZeroPoint(false);
        
        dataLength = 1;
        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }
    
    // �����룺�������
    if( funcCodeX1 == 0x04 ){
        MOTOR_ZDTUART_SetEnable();
        error = MOTOR_ZDTUART_Zero(funcCodeAuxY1);
        
        dataLength = 1;
        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }
    
    // �����룺���˳ʱ�����нǶ�
    if( funcCodeX1 == 0x05 ){
        MOTOR_ZDTUART_SetDir(MOTOR_ZDTUART_DIR_CW);
        MOTOR_ZDTUART_SetEnable();

        funcCodeAux = (funcCodeAuxY1<<8) | funcCodeAuxY2;
        error = MOTOR_ZDTUART_RunDegree(funcCodeAux);
        
        dataLength = 1;
        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }
    
    // �����룺�����ʱ�����нǶ�
    if( funcCodeX1 == 0x06 ){
        dataLength = 1;
        MOTOR_ZDTUART_SetDir(MOTOR_ZDTUART_DIR_CCW);
        MOTOR_ZDTUART_SetEnable();

        funcCodeAux = (funcCodeAuxY1<<8) | funcCodeAuxY2;
        error = MOTOR_ZDTUART_RunDegree(funcCodeAux);

        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }

    // �������ݳ���
    return dataLength;
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