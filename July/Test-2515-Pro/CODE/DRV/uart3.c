#include "common.h"



/* -------------------------------------------------- */
// �� �ⲿ����
/* -------------------------------------------------- */

/**
 * LIN �ⲿ����
 * ���±��������� lin.c, lin.m.c, lin.s.c �ļ���
 */
extern u32 GV_LinMode;
extern u32 GV_LinSlaveBaudRate;
extern u32 GV_LinMasterBaudRate;



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */
u8 GV_Uart3Lin = false;             // �Ƿ����� LIN
u8 GV_Uart3ReceiveData[40];         // ���ݽ�������
u8 GV_Uart3ReceiveDataCount  = 0;   // ���ݽ���ʵ�ʳ���
u8 GV_Uart3ReceiveDataLength = 40;  // ���ݽ�����������



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * UART3 ��ʼ��
 */
void UART3_CInit()
{
    if( GV_Uart3Lin == false ) UART3_CInitUart();
    if( GV_Uart3Lin == true && GV_LinMode == LIN_MODE_SLAVE  ) UART3_CInitLinSlave();
    if( GV_Uart3Lin == true && GV_LinMode == LIN_MODE_MASTER ) UART3_CInitLinMaster();
}


/**
 * UART3 ���ڳ�ʼ��
 */
void UART3_CInitUart()
{
    // ���ô��ڲ���Ϊ��������115200��8λ����λ��1λֹͣλ����У�飬������շ���
    UART3_Init((u32)115200, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);

    // ���� UART3 �ж�
    // UART3_IT_RXNE_OR ��ʾ �������ݼĴ����ǿջ����������ʱ �����ж�
    UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
    
    // ���� UART3
    UART3_Cmd(ENABLE);
}


/**
 * UART3 LIN ���óɴӽڵ�
 */
void UART3_CInitLinSlave()
{
    // ��ʼ UART3
    UART3_DeInit();
    UART3_Init((u32)GV_LinSlaveBaudRate, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);
    
    // ���� UART3 LIN
    // UART3_LIN_AUTOSYNC_ENABLE ��ʾÿ���� Synch Field ֮�����в����ʵ���
    UART3_LINConfig(UART3_LIN_MODE_SLAVE, UART3_LIN_AUTOSYNC_ENABLE, UART3_LIN_DIVUP_NEXTRXNE);
    UART3_LINBreakDetectionConfig(UART3_LINBREAKDETECTIONLENGTH_11BITS);

    // ���� UART3 ���жϣ����� Lin Break �ж�
    UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
    UART3_ITConfig(UART3_IT_LBDF, ENABLE);
    
    // ���� UART3 LIN
    UART3_LINCmd(ENABLE);
}


/**
 * UART3 LIN ���ó����ڵ�
 */
void UART3_CInitLinMaster()
{
    // ��ʼ UART3
    UART3_DeInit();
    UART3_Init((u32)GV_LinMasterBaudRate, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);
    
    // ���� UART3 LIN
    // UART3_LIN_AUTOSYNC_ENABLE ��ʾÿ���� Synch Field ֮�����в����ʵ���
    UART3_LINConfig(UART3_LIN_MODE_MASTER, UART3_LIN_AUTOSYNC_DISABLE,  UART3_LIN_DIVUP_LBRR1);
    UART3_LINBreakDetectionConfig(UART3_LINBREAKDETECTIONLENGTH_11BITS);

    // ���� UART3 ���жϣ��ر� Lin Break �ж�
    UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
    UART3_ITConfig(UART3_IT_LBDF, DISABLE);
    
    // ���� UART3 LIN
    UART3_LINCmd(ENABLE);
}


/**
 * UART3 �����ֽ�
 *
 * @param byte  8λ�ֽ�
 */
void UART3_SendByte( u8 byte )
{
    // �����ֽ�
    UART3_SendData8(byte);
    
    // �ȴ��������
    // RESET = 0; SET = 1; 
    // UART3_FLAG_TXE ��ʾ �������ݼĴ����ձ�־������ʾUART3û�з��ͣ���
    // �������� 1����ô��ʾ URAR1 ������ɻ��������У��������� 0����ô��ʾ UART3 ���ڷ���
    while( UART3_GetFlagStatus(UART3_FLAG_TXE) == RESET );
}


/**
 * UART3 �����ֽڼ�
 *
 * @param bytes   8λ�ֽڼ�
 * @param length  �ֽڼ�����
 */
void UART3_SendBytes( u8 *bytes, short length )
{
    for( short i = 0; i < length; i++ ) UART3_SendByte(bytes[i]);
}


/**
 * ��ȡ���յ�������
 *
 * return u8*  ����ָ��
 */
u8* UART3_GetReceiveData()
{
    return GV_Uart3ReceiveData;
}


/**
 * ��ȡ���յ������ݳ���
 *
 * return u8
 */
u8 UART3_GetReceiveDataCount()
{
    return GV_Uart3ReceiveDataCount;
}


/**
 * ���ƽ��յ�������
 *
 * @param u8 *data  ���飬�������ֽ��������� >= GV_Uart3ReceiveDataLength
 * return u8 ���������ݸ���
 */
u8 UART3_CopyReceiveData( u8 *data )
{
    // ��������
    for( u8 i = 0; i < GV_Uart3ReceiveDataCount; i++ ) data[i] = GV_Uart3ReceiveData[i];

    // ����
    return GV_Uart3ReceiveDataCount;
}


/**
 * ���ý��յ�������
 */
void UART3_ResetReceiveData()
{
    // ���ñ���
    GV_Uart3ReceiveDataCount = 0;
    for( u8 i = 0; i < GV_Uart3ReceiveDataLength; i++ ) GV_Uart3ReceiveData[i] = 0;
}



/* -------------------------------------------------- */
// �� �жϻص�����
// �� ���������μ� stm8s.it.c
/* -------------------------------------------------- */

/**
 * UART3 �����жϻص�����
 */
void UART3_InterruptHandlerRX()
{
    if( GV_Uart3Lin == false ) UART3_InterruptHandlerRXUart();
    if( GV_Uart3Lin == true && GV_LinMode == LIN_MODE_SLAVE  ) UART3_InterruptHandlerRXTriggerLinSlave();
    if( GV_Uart3Lin == true && GV_LinMode == LIN_MODE_MASTER ) UART3_InterruptHandlerRXTriggerLinMaster();
}


/**
 * UART3 ���жϻص����� - �������ݽ���
 */
void UART3_InterruptHandlerRXUart()
{
    // ��ֹ OR ���ش����²�ͣ�Ľ�����жϣ�OR Դ�� UART3_IT_RXNE_OR��
    // STM8 �ڲ����ƣ��ȶ�ȡ UART_SR���ٶ�ȡ UART_DR �ɰ� OR λ�ж�״̬����
    // ��ͨ��PC�˵Ĵ�������� STM8 һ���Է��Ͷ��ֽ�����ʱ�����磺01 02 03������ô STM8 �ͻ�һֱ��ͣ�Ľ�����жϣ�������ѭ��
    if( UART3_GetITStatus(UART3_IT_RXNE) == RESET ) return;
    
    // ��ȡ����
    u8 byte = UART3_ReceiveData8();
    
    // �������ݣ��������������
    if( GV_Uart3ReceiveDataCount < GV_Uart3ReceiveDataLength ) GV_Uart3ReceiveData[GV_Uart3ReceiveDataCount++] = byte;
}


/**
 * UART3 ���жϻص����� - LIN �ӻ����յ���������������
 */
void UART3_InterruptHandlerRXTriggerLinSlave()
{
    // LIN DATA �ж�
    // LIN Break Detection Interrupt
    if( UART3_GetITStatus(UART3_IT_LBDF) != RESET ){
        // ����ҵ��
        LIN_SLAVE_ReceiveStart();
        LIN_SLAVE_ReceiveHandle(0x00);

        // ����ж�
        // ���������ñ��λ��ôϵͳ�ͻ�һֱ���� LIN DATA �ж��߼�
        UART3_ClearITPendingBit(UART3_IT_LBDF);
    }

    // LIN ͬ���α�־
    // Ĭ������� STM8S �� UART3 RX �ж����޷���ȡ�� LIN ��ͬ���� 0x55 ֵ
    // ������������ͬ���α�־����ô UART3 RX �Ϳ���������ȡ�� LIN ��ͬ���� 0x55 ֵ
    // 
    // ���磺����ï�оߣ�281-3��Ĭ���·� 80,80 FC,02 �� 42
    // �����������ͬ���α�־��������·�����Ϊ��55,80,80 FC,02 �� 55,42
    // �����������ͬ���α�־��������·�����Ϊ��80,80 FC,02 �� 42
    // �ܽ᣺���Ͻ������Թ���ï�ļоߣ����廹��Ҫ�Լ���ʵ���Ͻ��вɼ�����
    // �����������ͬ���α�־������£�ͬ���� 0x55 ֵ�ܽ��յ���ʵ���������ͬ���α�־�����յ�0x55���ݣ�
    if( UART3_GetFlagStatus(UART3_FLAG_LSF) != RESET ){
        UART3_ClearFlag(UART3_FLAG_LSF);
    }

    // LIN DATA ���ݶ�ȡ
    if( UART3_GetITStatus(UART3_IT_RXNE) != RESET ){
        LIN_SLAVE_ReceiveHandle(UART3_ReceiveData8());
        UART3_ClearITPendingBit(UART3_IT_RXNE);
    }
}


/**
 * UART3 ���жϻص����� - LIN �������յ��ӻ�����Ӧ����
 */
void UART3_InterruptHandlerRXTriggerLinMaster()
{
    // ��ʼ��
    u8 byte = UART3_ReceiveData8();
    
    // ���շ������ź�
    LIN_MASTER_InterruptHandlerRxFindReceive(byte);
    LIN_MASTER_InterruptHandlerRxPrimaryReceive(byte);
}


/**
 * UART3 �����жϻص�����
 */
void UART3_InterruptHandlerTX()
{
}
