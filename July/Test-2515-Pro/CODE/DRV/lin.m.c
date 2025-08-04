#include "common.h"



/* -------------------------------------------------- */
// �� LIN ����ģʽ
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� ȫ�ֱ���
// �� �����ź����ݻ��ڵ���������ECU
/* -------------------------------------------------- */

/**
 * LIN ��������
 */
u32 GV_LinMasterBaudRate     = 19200;  // �����ʣ����ò����ʣ�9600��19200
u32 GV_LinMasterSendInterval = 50;    // �ź��·����豸�ļ��ʱ�䣨���룩


/**
 * LIN ������ر���
 */
short GV_LinMasterDebugSendOutUart1        = 0;  // Debug ����ʱ��������� UART1
short GV_LinMasterDebugSendOutPrintf       = 0;  // Debug ����ʱ��������� printf��ע�⣺��ϵͳ�п������� printf ��������� UART1��
short GV_LinMasterDebugReceiveOutUart1     = 0;  // Debug ����ʱ��������� UART1
short GV_LinMasterDebugReceiveOutPrintf    = 0;  // Debug ����ʱ��������� printf��ע�⣺��ϵͳ�п������� printf ��������� UART1��
short GV_LinMasterDebugReceiveChkOutUart1  = 0;  // Debug �����źż��ʱ����� UART1
short GV_LinMasterDebugReceiveChkOutPrintf = 0;  // Debug �����źż��ʱ����� printf��ע�⣺��ϵͳ�п������� printf ��������� UART1��


/**
 * LIN ���ź���ر���
 * ���ź���ָ���յ��ӻ��������ź�
 */
u8 GV_LinMasterPrimarySignal[]            = {0x42, 0x00, 0x00, 0x00, 0x02, 0xBB};  // ���������á����� ON ��Ĭ�ϵ����źţ�ID��DATA��CheckNum��
                                                                                   // �������������ñ����������������͵�ID���ӻ�Ĭ����Ӧ������ �� У����
                                                                                   // ������������ϵͳ�Զ����ݸñ�����ȡ������ID���Լ�ƥ�Դӻ��������ź��Ƿ�׼ȷ�ȹ���
                                                                                   // ���Բ�����KD-281-1 ��{0x42, 0x40, 0x00, 0x20, 0x04, 0x59};  // ��·����δ����ͭƬ�����������ڴ���״̬
                                                                                   // ��ԭ������KD-281-2 ��{0x42, 0x00, 0x00, 0x00, 0x02, 0xBB};
                                                                                   // ���Բ�����KD-281-3 ��{0x42, 0x00, 0x00, 0x20, 0x04, 0x99};  // ��·����δ����ͭƬ�����������ڴ���״̬
                                                                                   // ��ԭ������KD-281-27��{0x42, 0x00, 0x00, 0x00, 0xF2, 0xCA};

u8 GV_LinMasterPrimaryReceive[]           = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // ��ϵͳʹ�á����źŷ��ͺ��յ��ķ����ź����������� ID��DATA��CheckNum�����鳤����Ҫ�� GV_LinMasterPrimarySignal һ��
u8 GV_LinMasterPrimaryReceiveCnt          = 0;                                     // ��ϵͳʹ�á����� GV_LinMasterPrimaryReceive ��������������ݵĸ���
u8 GV_LinMasterPrimaryReceiveWait         = 0;                                     // ��ϵͳʹ�á����źŵȴ������ı�־��ϵͳ�Զ����øñ�־���жϲ���ʽ�������źż�¼�� GV_LinMasterPrimaryReceive
u8 GV_LinMasterPrimaryReceiveChks[]       = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // ��ϵͳʹ�á������źż�����ʱ�������������� ID��DATA��CheckNum�����鳤����Ҫ�� GV_LinMasterPrimarySignal һ��
u8 GV_LinMasterPrimaryReceivePrev[]       = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // ��ϵͳʹ�á���һ���ź���ʱ�������������� ID��DATA��CheckNum�����鳤����Ҫ�� GV_LinMasterPrimarySignal һ��

u8 GV_LinMasterPrimaryReceiveToUsbMode    = LIN_TXUSB_MODE_DOWNUPONE;              // ���������á����źŷ��ͺ��յ��ķ����ź����ģʽ��LIN_TXUSB_MODE_DOWNONE���ӻ����·���һ�Σ�, LIN_TXUSB_MODE_DOWNUPONE���ӻ����º��ɿ�������һ�Σ�
u8 GV_LinMasterPrimaryReceiveToUsbDnHead  = 0xAE;                                  // ���������á��ӻ����·���һ���źŵ�����ͷ
u8 GV_LinMasterPrimaryReceiveToUsbDnFoot  = 0xAF;                                  // ���������á��ӻ����·���һ���źŵ�����β
u8 GV_LinMasterPrimaryReceiveToUsbUpHead  = 0xBE;                                  // ���������á��ӻ��ɿ�����һ���źŵ�����ͷ
u8 GV_LinMasterPrimaryReceiveToUsbUpFoot  = 0xBF;                                  // ���������á��ӻ��ɿ�����һ���źŵ�����β


/**
 * LIN ���ź���ر���
 * ���źŽ�����������֪ͨ�ӻ�
 */
u8 GV_LinMasterAuxSignal[]      = {0x80, 0x00, 0xF8};  // ���� ON ��Ĭ���·��ĸ��źţ�ID �� DATA������ CheckNum����һ���������� ECU ֪ͨ���ر������������ʹ�ܵ��������
u8 GV_LinMasterAuxSignalBox[]   = {0x00, 0x00, 0x00};  // ��ϵͳʹ�á��·��ĸ��ź����������鳤�Ⱥ� GV_LinMasterAuxSignal һ�£���Ҫ���ڹ�������ż������͸��ź�


/**
 * LIN �����ź���ر���
 */
u8 GV_LinMasterFindReceiveCnt   = 0;  // ��ϵͳʹ�á����� GV_LinMasterFindReceive ��������������ݵĸ���
u8 GV_LinMasterFindReceiveWait  = 0;  // ��ϵͳʹ�á����źŵȴ������ı�־��ϵͳ�Զ����øñ�־���жϲ���ʽ�������źż�¼�� GV_LinMasterFindReceive
u8 GV_LinMasterFindReceive[20];       // ��ϵͳʹ�á����źŷ��ͺ��յ��ķ����ź����������� ID��DATA��CheckNum



/* -------------------------------------------------- */
// �� �������壨���źţ�
/* -------------------------------------------------- */

/**
 * �����źš��������źŸ��ӻ�
 * ���ź���ָ���յ��ӻ��������ź�
 *
 * �������ź� ID �󣬴ӻ��ᷴ���źţ��� STM8 �� "INTERRUPT_HANDLER(UART3_RX_IRQHandler,21)" �ж���
 * ������ UART3_InterruptHandlerRX()->LIN_MASTER_InterruptHandlerRxPrimaryReceive() ���������淴�������ݵ�ȫ�ֱ��� GV_LinMasterPrimaryReceive
 */
void LIN_MASTER_SendPrimarySignal()
{
    // ׼�����ݽ���
    GV_LinMasterPrimaryReceiveCnt  = 0;
    GV_LinMasterPrimaryReceiveWait = 1;
    
    // ���ͼ���Σ�ͬ���Σ�ID
    UART3_SendBreak();
    UART3_SendByte(0x55);
    UART3_SendByte(GV_LinMasterPrimarySignal[0]);
    
    // ʱ����
    // �ȴ����շ����ź�
    delay_ms(GV_LinMasterSendInterval);
}


/**
 * �����źš����ͽ��յ��źŵ� USB
 * ֧����������ģʽ���ӻ����·�һ���źţ��ӻ����º��ɿ���һ�Ρ�����μ�������GV_LinMasterPrimaryReceiveToUsbMode
 */
void LIN_MASTER_SendPrimaryReceiveToUsb()
{
    // ��ʼ��
    u8 i, match;
    u8 mode = GV_LinMasterPrimaryReceiveToUsbMode;
    u8 length = ARR_LENGTH(GV_LinMasterPrimaryReceive);

    // ��ǰ���յ��ķ����ź� �� Ĭ���ź� �Ƚ�
    match = 1; for( i = 0; i < length; i++ ) match &= GV_LinMasterPrimarySignal[i] == GV_LinMasterPrimaryReceive[i];

    // ��ǰ���յ��ķ����ź� �� Ĭ���ź� ��ͬ��˵����ʱ�ӻ���û�а������»�����Ĭ�����
    if( match == 1 ){
        // ����û�а��£���ôֱ���˳�
        // ԭ����һ���źű���������ȫ���� 0����ô˵����·����һ��״̬Ҳ��Ĭ��״̬
        match = 1; for( i = 0; i < length; i++ ) match &= GV_LinMasterPrimaryReceivePrev[i] == 0x00;
        if( match == 1 ) return;
        
        // ��һ���źű��������ݷ�ȫ 0����ô˵����һ��״̬�������£���ô��ʱ�ĳ����߼������ɰ����ɿ�����
        if( mode == LIN_TXUSB_MODE_DOWNUPONE ){
            UART1_SendByte(GV_LinMasterPrimaryReceiveToUsbUpHead); UART1_SendByte(length);
            for( i = 0; i < length; i++ ) UART1_SendByte(GV_LinMasterPrimaryReceivePrev[i]);
            UART1_SendByte(GV_LinMasterPrimaryReceiveToUsbUpHead);
        }

        // ������һ���ź�
        for( i = 0; i < length; i++ ) GV_LinMasterPrimaryReceivePrev[i] = 0x00;

        // ����
        return;
    }
    
    // ��ǰ���յ��ķ����ź� �� Ĭ���ź� ����ͬ��˵����ʱ�ӻ��İ���������
    else{
        // ��ǰ���յ��ķ����źź���һ���ź���ͬ�򲻷��ͷ����ź�
        match = 1; for( i = 0; i < length; i++ ) match &= GV_LinMasterPrimaryReceivePrev[i] == GV_LinMasterPrimaryReceive[i];
        if( match == 1 ) return;
    }
    
    // ���Ϳ�ͷ����
    UART1_SendByte(GV_LinMasterPrimaryReceiveToUsbDnHead);
    UART1_SendByte(length);
    
    // ���ͷ����źŵ� USB
    for( i = 0; i < length; i++ ) UART1_SendByte(GV_LinMasterPrimaryReceive[i]);

    // ���浱ǰ���ݵ���һ���źŵı�����
    for( i = 0; i < length; i++ ) GV_LinMasterPrimaryReceivePrev[i] = GV_LinMasterPrimaryReceive[i];

    // ������β����
    UART1_SendByte(GV_LinMasterPrimaryReceiveToUsbDnFoot);
}


/**
 * �����źš����ý��շ����ź�
 */
void LIN_MASTER_PrimaryReceiveReset()
{
    GV_LinMasterPrimaryReceiveCnt  = 0;
    GV_LinMasterPrimaryReceiveWait = 0;
}


/**
 * �����źš������źŰ������
 * �ú�������ȫ�ֱ��� GV_LinMasterPrimaryReceive ֵΪ����
 * �ú�������λ���Ƿ������磺1H40(��ʾ��� LIN �����ݶε�1�������ֽڵĸ�λ4�Ƿ����0)
 * ����������������������������2L11(��ʾ��� LIN �����ݶε�2�������ֽڵĵ�λ1�Ƿ����1)
 *
 * @param dataNum           ���ݱ�ţ��� 1 ��ʼ
 * @param dataHightLow      ���ݸߵ�λ��H��L
 * @param data8421Bit       ����λ���� 8421 Ϊ��׼
 * @param data8421BitValue  ����λֵ��0��1
 *
 * @return 0 ��ʾ��������1 ��ʾ����
 */
short LIN_MASTER_PrimaryReceiveContain( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue )
{
    // �ع�����λ
    if( dataHighLow == 'h' ) data8421Bit = data8421Bit << 4;
    if( dataHighLow == 'H' ) data8421Bit = data8421Bit << 4;

    if( data8421BitValue == 0 && (GV_LinMasterPrimaryReceive[dataNum]&data8421Bit) == 0 ) return 1;
    if( data8421BitValue == 1 && (GV_LinMasterPrimaryReceive[dataNum]&data8421Bit) == data8421Bit ) return 1;
    
    return 0;
}



/* -------------------------------------------------- */
// �� �������壨���ź��жϣ�
/* -------------------------------------------------- */

/**
 * �����źš����շ������ź�
 * �ú����� UART ���ж��б����ã������Զ�������ص����þ����Ƿ��¼����
 *
 * @param byte  8λ�ֽ�
 */
void LIN_MASTER_InterruptHandlerRxPrimaryReceive( u8 byte )
{
    // ��Ч����
    if( GV_LinMasterPrimaryReceiveWait == 0 ) return;
    if( GV_LinMasterPrimaryReceiveCnt >= ARR_LENGTH(GV_LinMasterPrimaryReceive) ) return;

    // ���ռ���κ�ͬ���β�����
    if( GV_LinMasterPrimaryReceiveWait == 1 && byte == 0x00 ){ GV_LinMasterPrimaryReceiveWait = 2; return; }
    if( GV_LinMasterPrimaryReceiveWait == 2 && byte == 0x55 ){ GV_LinMasterPrimaryReceiveWait = 3; return; }
    if( GV_LinMasterPrimaryReceiveWait != 3 ) return;
    
    //��DEBUG�� ������յ��ķ����ź�
    if( GV_LinMasterDebugReceiveOutUart1  == 1 ) UART1_SendByte(byte);
    if( GV_LinMasterDebugReceiveOutPrintf == 1 ) printf("%02X ", byte);

    // ���ݸ�ֵ
    GV_LinMasterPrimaryReceive[GV_LinMasterPrimaryReceiveCnt++] = byte;
}



/* -------------------------------------------------- */
// �� �������壨���źţ�
/* -------------------------------------------------- */

/**
 * �����źš���ʼ���·��ĸ��ź�
 * �ú���ֱ�ӳ�ʼ��ȫ�ֱ��� GV_LinMasterAuxSignalBox
 */
void LIN_MASTER_InitAuxSignal()
{
    for( int i=0; i < ARR_LENGTH(GV_LinMasterAuxSignal); i++ ) GV_LinMasterAuxSignalBox[i] = GV_LinMasterAuxSignal[i];
}


/**
 * �����źš������·��ĸ��ź�
 * �ú���ֱ������ȫ�ֱ��� GV_LinMasterAuxSignalBox
 * �ú�������λ���Ƿ������磺1H40(��ʾ��LIN�����ݶε�1�������ֽڵĸ�λ4����Ϊ0)
 * ����������������������������2L11(��ʾ��LIN�����ݶε�2�������ֽڵĵ�λ1����Ϊ1)
 *
 * @param dataNum           ���ݱ�ţ��� 1 ��ʼ
 * @param dataHightLow      ���ݸߵ�λ��H��L
 * @param data8421Bit       ����λ��8��4��2��1
 * @param data8421BitValue  ����λֵ��0��1
 */
void LIN_MASTER_SetAuxSignal( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue )
{
    // �ع�����λ
    if( dataHighLow == 'h' ) data8421Bit = data8421Bit << 4;
    if( dataHighLow == 'H' ) data8421Bit = data8421Bit << 4;

    // �����·��ĸ��ź�
    if( data8421BitValue == 1 ) GV_LinMasterAuxSignalBox[dataNum] |= data8421Bit;
    if( data8421BitValue == 0 ) GV_LinMasterAuxSignalBox[dataNum] &= data8421Bit^0xFF;
}


/**
 * �����źš������·��ĸ��ź�
 * �ú���ֱ�ӷ���ȫ�ֱ��� GV_LinMasterAuxSignalBox �������
 */
void LIN_MASTER_SendAuxSignal()
{
    // ��ʼ��
    u8 length = ARR_LENGTH(GV_LinMasterAuxSignal);

    // ���� LIN ��־
    UART3_SendBreak();
    UART3_SendByte(0x55);

    // �����·��ĸ��ź�
    for( int i = 0; i < length; i++ ) UART3_SendByte(GV_LinMasterAuxSignalBox[i]);
    UART3_SendByte(LIN_CalcChecknum(GV_LinMasterAuxSignalBox,length,0));

    // ʱ����
    delay_ms(GV_LinMasterSendInterval);
    
    //��DEBUG�� ͨ�� UART1 ��������·��ĸ��ź�
    if( GV_LinMasterDebugSendOutUart1 == 1 ){
        for( int i = 0; i < length; i++ ) UART1_SendByte(GV_LinMasterAuxSignalBox[i]);
        UART1_SendByte(LIN_CalcChecknum(GV_LinMasterAuxSignalBox,length,0));
    }
    
    //��DEBUG�� ͨ�� Printf ��������·��ĸ��ź�
    if( GV_LinMasterDebugSendOutPrintf == 1 ){
        for( int i = 0; i < ARR_LENGTH(GV_LinMasterAuxSignal); i++ ) printf("%02X ", GV_LinMasterAuxSignalBox[i]);
        printf("%02X\n", LIN_CalcChecknum(GV_LinMasterAuxSignalBox,length,0));
    }
}


/**
 * �����źš������·��ĸ��ź�
 * �ú���ֱ������ȫ�ֱ��� GV_LinMasterAuxSignalBox
 * ֱ�ӽ������ֽڼ���ֵ�� GV_LinMasterAuxSignalBox������ֵǰ���� ID �Ƿ�һ�£�ͬʱ���� bytes �е� Checknum
 *
 * @param datBytes   �����ֽڼ�
 * @param datLength  �����ֽڳ���
 */
void LIN_MASTER_SetAuxSignalBytes( u8 datBytes[], u8 datLength )
{
    // ��ʼ��
    u8 i;
    u8 length = ARR_LENGTH(GV_LinMasterAuxSignal);

    // ��Ч ID
    if( datBytes[0] != GV_LinMasterAuxSignal[0] ) return;

    // ��ֵ
    length = length > datLength ? datLength : length;
    for( i = 0; i < length; i++ ) GV_LinMasterAuxSignalBox[i] = datBytes[i];
}


/**
 * �����źš������·��ĸ��ź�
 * Ԥ��������ܶ�Ӧ�������ź�
 */
void LIN_MASTER_SetAuxSignalBLedOn(){ LIN_MASTER_SetAuxSignal(1, 'H', 8, 1); }  // ���źţ��������
void LIN_MASTER_SetAuxSignalLLedOn(){ LIN_MASTER_SetAuxSignal(1, 'L', 4, 1); }  // ���źţ���ʹ�ܵ���
void LIN_MASTER_SetAuxSignalRLedOn(){ LIN_MASTER_SetAuxSignal(1, 'L', 1, 1); }  // ���źţ���ʹ�ܵ���



/* -------------------------------------------------- */
// �� �������壨LIN ID ���ң�
/* -------------------------------------------------- */

/**
 * ��LIN ID ���ҡ��������źŸ��ӻ�
 * ���ź���ָ���յ��ӻ��������ź�
 *
 * @param id  LIN ID �ֽ�
 */
void LIN_MASTER_SendFindSignal( u8 id )
{
    // ׼�����ݽ���
    GV_LinMasterFindReceiveCnt  = 0;
    GV_LinMasterFindReceiveWait = 1;
    
    // ���ͼ���Σ�ͬ���Σ�ID
    UART3_SendBreak();
    UART3_SendByte(0x55);
    UART3_SendByte(id);

    // ʱ����
    delay_ms(GV_LinMasterSendInterval);
}


/**
 * ��LIN ID ���ҡ����ͽ��յ��źŵ� USB
 */
void LIN_MASTER_SendFindReceiveToUsb()
{
    // ���ͷ����źŵ� USB
    for( int i = 0; i < GV_LinMasterFindReceiveCnt; i++ ) UART1_SendByte(GV_LinMasterFindReceive[i]);
}


/**
 * ��LIN ID ���ҡ����ý��շ����ź�
 */
void LIN_MASTER_FindReceiveReset()
{
    GV_LinMasterFindReceiveCnt  = 0;
    GV_LinMasterFindReceiveWait = 0;
}



/* -------------------------------------------------- */
// �� �������壨LIN ID �����жϣ�
/* -------------------------------------------------- */

/**
 * ��LIN ID ���ҡ����շ������ź�
 * �ú����� UART ���ж��б����ã������Զ�������ص����þ����Ƿ��¼����
 *
 * @param byte  8λ�ֽ�
 */
void LIN_MASTER_InterruptHandlerRxFindReceive( u8 byte )
{
    // ��Ч����
    if( GV_LinMasterFindReceiveWait == 0 ) return;
    if( GV_LinMasterFindReceiveCnt >= ARR_LENGTH(GV_LinMasterFindReceive) ) return;

    // ���ռ���κ�ͬ���β�����
    if( GV_LinMasterFindReceiveWait == 1 && byte == 0x00 ){ GV_LinMasterFindReceiveWait = 2; return; }
    if( GV_LinMasterFindReceiveWait == 2 && byte == 0x55 ){ GV_LinMasterFindReceiveWait = 3; return; }
    if( GV_LinMasterFindReceiveWait != 3 ) return;

    //��DEBUG�� ������յ��ķ����ź�
    if( GV_LinMasterDebugReceiveOutUart1  == 1 ) UART1_SendByte(byte);
    if( GV_LinMasterDebugReceiveOutPrintf == 1 ) printf("%02X ", byte);

    // ���ݸ�ֵ
    GV_LinMasterFindReceive[GV_LinMasterFindReceiveCnt++] = byte;
}



/* -------------------------------------------------- */
// �� �������壨���źż��ƥ����أ�
/* -------------------------------------------------- */

/**
 * ��ʼ�������źţ����ר�ã�
 */
void LIN_MASTER_PrimaryReceiveChkInit()
{
    for( int i=0; i < ARR_LENGTH(GV_LinMasterPrimarySignal); i++ ) GV_LinMasterPrimaryReceiveChks[i] = GV_LinMasterPrimarySignal[i];
}


/**
 * ���÷������źţ����ר�ã�
 * �ú���ֱ������ȫ�ֱ��� GV_LinMasterPrimaryReceiveChks
 * �ú�������λ���Ƿ������磺1H40(��ʾ�� LIN �����ݶε�1�������ֽڵĸ�λ4����Ϊ0)
 * ����������������������������2L11(��ʾ�� LIN �����ݶε�2�������ֽڵĵ�λ1����Ϊ1)
 *
 * @param dataNum           ���ݱ�ţ��� 1 ��ʼ
 * @param dataHightLow      ���ݸߵ�λ��H��L
 * @param data8421Bit       ����λ���� 8421 Ϊ��׼
 * @param data8421BitValue  ����λֵ��0��1
 */
void LIN_MASTER_PrimaryReceiveChkSet( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue )
{
    // �ع�����λ
    if( dataHighLow == 'h' ) data8421Bit = data8421Bit << 4;
    if( dataHighLow == 'H' ) data8421Bit = data8421Bit << 4;

    // ���÷������źţ����ר�ã�
    if( data8421BitValue == 1 ) GV_LinMasterPrimaryReceiveChks[dataNum] |= data8421Bit;
    if( data8421BitValue == 0 ) GV_LinMasterPrimaryReceiveChks[dataNum] &= data8421Bit^0xFF;
}


// ���Ӿ�����
short LIN_MASTER_PrimaryReceiveChkHasMirrUp()     { return LIN_MASTER_PrimaryReceiveContain(3,'L',1,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }  
void  LIN_MASTER_PrimaryReceiveChkSetMirrUp()     {         LIN_MASTER_PrimaryReceiveChkSet(3,'L',1,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }
 
// ���Ӿ�����
short LIN_MASTER_PrimaryReceiveChkHasMirrLeft()   { return LIN_MASTER_PrimaryReceiveContain(3,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }  
void  LIN_MASTER_PrimaryReceiveChkSetMirrLeft()   {         LIN_MASTER_PrimaryReceiveChkSet(3,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ���Ӿ�����
short LIN_MASTER_PrimaryReceiveChkHasMirrDown()   { return LIN_MASTER_PrimaryReceiveContain(3,'L',2,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetMirrDown()   {         LIN_MASTER_PrimaryReceiveChkSet(3,'L',2,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ���Ӿ�����
short LIN_MASTER_PrimaryReceiveChkHasMirrRight()  { return LIN_MASTER_PrimaryReceiveContain(3,'L',8,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetMirrRight()  {         LIN_MASTER_PrimaryReceiveChkSet(3,'L',8,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ���Ӿ��������������������շţ�����ʹ�ܣ�
short LIN_MASTER_PrimaryReceiveChkHasMirrAssist() { return LIN_MASTER_PrimaryReceiveContain(4,'L',8,1); }
void  LIN_MASTER_PrimaryReceiveChkSetMirrAssist() {         LIN_MASTER_PrimaryReceiveChkSet(4,'L',8,1); }

// ���Ӿ��������������������շţ�����ʹ�ܣ�
short LIN_MASTER_PrimaryReceiveChkHasMirrAssist2(){ return LIN_MASTER_PrimaryReceiveContain(2,'H',4,1); }
void  LIN_MASTER_PrimaryReceiveChkSetMirrAssist2(){         LIN_MASTER_PrimaryReceiveChkSet(2,'H',4,1); }

// ���Ӿ��շ�
short LIN_MASTER_PrimaryReceiveChkHasMirrOC()     { return LIN_MASTER_PrimaryReceiveContain(3,'H',1,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetMirrOC()     {         LIN_MASTER_PrimaryReceiveChkSet(3,'H',1,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ����Ӿ�ʹ��
short LIN_MASTER_PrimaryReceiveChkHasMirrSWL()    { return LIN_MASTER_PrimaryReceiveContain(2,'H',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetMirrSWL()    {         LIN_MASTER_PrimaryReceiveChkSet(2,'H',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// �Һ��Ӿ�ʹ��
short LIN_MASTER_PrimaryReceiveChkHasMirrSWR()    { return LIN_MASTER_PrimaryReceiveContain(2,'H',8,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetMirrSWR()    {         LIN_MASTER_PrimaryReceiveChkSet(2,'H',8,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰ��
short LIN_MASTER_PrimaryReceiveChkHasLFUp()       { return LIN_MASTER_PrimaryReceiveContain(1,'L',2,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetLFUp()       {         LIN_MASTER_PrimaryReceiveChkSet(1,'L',2,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰ��һ��
short LIN_MASTER_PrimaryReceiveChkHasLFUpA()      { return LIN_MASTER_PrimaryReceiveContain(1,'L',7,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetLFUpA()      {         LIN_MASTER_PrimaryReceiveChkSet(1,'L',7,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰһ����
short LIN_MASTER_PrimaryReceiveChkHasLFUpAuto()   { return LIN_MASTER_PrimaryReceiveContain(1,'L',6,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetLFUpAuto()   {         LIN_MASTER_PrimaryReceiveChkSet(1,'L',6,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰ��
short LIN_MASTER_PrimaryReceiveChkHasLFDown()     { return LIN_MASTER_PrimaryReceiveContain(1,'L',1,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetLFDown()     {         LIN_MASTER_PrimaryReceiveChkSet(1,'L',1,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰ��һ��
short LIN_MASTER_PrimaryReceiveChkHasLFDownA()    { return LIN_MASTER_PrimaryReceiveContain(1,'L',7,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetLFDownA()    {         LIN_MASTER_PrimaryReceiveChkSet(1,'L',7,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰһ����
short LIN_MASTER_PrimaryReceiveChkHasLFDownAuto() { return LIN_MASTER_PrimaryReceiveContain(1,'L',5,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }  
void  LIN_MASTER_PrimaryReceiveChkSetLFDownAuto() {         LIN_MASTER_PrimaryReceiveChkSet(1,'L',5,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰ��
short LIN_MASTER_PrimaryReceiveChkHasRFUp()       { return LIN_MASTER_PrimaryReceiveContain(1,'H',1,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetRFUp()       {         LIN_MASTER_PrimaryReceiveChkSet(1,'H',1,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰ��һ��
short LIN_MASTER_PrimaryReceiveChkHasRFUpA()      { return LIN_MASTER_PrimaryReceiveContain(1,'H',3,1) && LIN_MASTER_PrimaryReceiveContain(1,'L',8,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetRFUpA()      {         LIN_MASTER_PrimaryReceiveChkSet(1,'H',3,1);    LIN_MASTER_PrimaryReceiveChkSet(1,'L',8,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰһ����
short LIN_MASTER_PrimaryReceiveChkHasRFUpAuto()   { return LIN_MASTER_PrimaryReceiveContain(1,'H',3,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetRFUpAuto()   {         LIN_MASTER_PrimaryReceiveChkSet(1,'H',3,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰ��
short LIN_MASTER_PrimaryReceiveChkHasRFDown()     { return LIN_MASTER_PrimaryReceiveContain(1,'L',8,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetRFDown()     {         LIN_MASTER_PrimaryReceiveChkSet(1,'L',8,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰ��һ��
short LIN_MASTER_PrimaryReceiveChkHasRFDownA()    { return LIN_MASTER_PrimaryReceiveContain(1,'H',3,1) && LIN_MASTER_PrimaryReceiveContain(1,'L',8,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetRFDownA()    {         LIN_MASTER_PrimaryReceiveChkSet(1,'H',3,1);    LIN_MASTER_PrimaryReceiveChkSet(1,'L',8,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��ǰһ����
short LIN_MASTER_PrimaryReceiveChkHasRFDownAuto() { return LIN_MASTER_PrimaryReceiveContain(1,'H',2,1) && LIN_MASTER_PrimaryReceiveContain(1,'L',8,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',4,1) && LIN_MASTER_PrimaryReceiveContain(4,'L',2,0); }
void  LIN_MASTER_PrimaryReceiveChkSetRFDownAuto() {         LIN_MASTER_PrimaryReceiveChkSet(1,'H',2,1);    LIN_MASTER_PrimaryReceiveChkSet(1,'L',8,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',4,1);    LIN_MASTER_PrimaryReceiveChkSet(4,'L',2,0); }

// ��������
short LIN_MASTER_PrimaryReceiveChkHasWinLock()    { return LIN_MASTER_PrimaryReceiveContain(3,'H',4,1); }
void  LIN_MASTER_PrimaryReceiveChkSetWinLock()    {         LIN_MASTER_PrimaryReceiveChkSet(3,'H',4,1); }  

// ��������
short LIN_MASTER_PrimaryReceiveChkHasWinUnLock()  { return LIN_MASTER_PrimaryReceiveContain(3,'H',8,1); }
void  LIN_MASTER_PrimaryReceiveChkSetWinUnLock()  {         LIN_MASTER_PrimaryReceiveChkSet(3,'H',8,1); }  



/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */

/**
 * ���߼������Ҵӻ��� ID ��ʶ
 *
 * @param process  ��ʾ����
 */
void LIN_MASTER_MainFindSlaveId( bool process )
{
    // ��ʼ��
    int id;
    int finded = 0;
    
    // LIN ID ����
    for( id = 0; id <= 0xFF; id++ ){
        // ����ID����һ�Σ�
        // ��ʵ��ԭ���� KD-281-2 ��·������У���������2�����ϴ���ID + 1����ȷID�󣬷��ص����ݲ���ȫ��ȷ��������2��ID���ܷ�����ȫ��ȷ������
        LIN_MASTER_SendFindSignal((u8)id);
        LIN_MASTER_FindReceiveReset();
        
        // ����ID���ڶ��Σ�
        LIN_MASTER_SendFindSignal((u8)id);

        // �з����ź�
        finded = GV_LinMasterFindReceiveCnt > 1 ? 1 : 0;

        // �Է����źŴ���
        if( finded ){
            LIN_MASTER_SendFindReceiveToUsb();  // ���� LIN �źŵ� USB
        }
        else if( process ){
            UART1_SendByte((short)id);
        }

        // ���ý��շ����źţ��Ա���´����ź�����
        LIN_MASTER_FindReceiveReset();
    }
}


/**
 * ���߼����������źŲ����մӻ��ķ����źţ�Ȼ������� USB
 * ��������֧����������ģʽ���ӻ����·�һ���źţ��ӻ����º��ɿ���һ�Ρ�����μ�������GV_LinMasterPrimaryReceiveToUsbMode
 */
void LIN_MASTER_MainPrimaryReceiveToUsb()
{
    // �����������ź�
    LIN_MASTER_SendPrimarySignal();

    // ���ͽ��յ��źŵ� USB
    LIN_MASTER_SendPrimaryReceiveToUsb();  

    // ���ý��շ����źţ��Ա���´����ź�����
    LIN_MASTER_PrimaryReceiveReset();
}


/**
 * ���߼����������źŲ����մӻ��ķ����źţ�Ȼ����м��
 * �ú���һ�����ڵ�������·����
 */
void LIN_MASTER_MainPrimaryReceiveCheck()
{
    // ��ʼ��
    u8 signalMatch;
    u8 signalLength = ARR_LENGTH(GV_LinMasterPrimarySignal);
    u8 signalContain = 0;
    
    // ����Ƿ���Ĭ���ź�ƥ��
    // �������ź���Ĭ���ź�һ��˵������·��δ���°���
    signalMatch = 1; for( int i = 0; i < signalLength; i++ ) signalMatch &= GV_LinMasterPrimarySignal[i] == GV_LinMasterPrimaryReceive[i];
    
    //��DEBUG�������źż��ʱ��������յ��ķ����ź�
    if( GV_LinMasterDebugReceiveChkOutUart1  == 1 ){ for( u8 i = 0; i < signalLength; i++ ) UART1_SendByte(GV_LinMasterPrimaryReceive[i]); delay_ms(500); }
    if( GV_LinMasterDebugReceiveChkOutPrintf == 1 ){ for( u8 i = 0; i < signalLength; i++ ) printf("==="); printf("\n"); for( u8 i = 0; i < signalLength; i++ ) printf("%02X ", GV_LinMasterPrimaryReceive[i]); printf("\n"); }
    
    // ��Ĭ���ź�ƥ��
    //if( signalMatch == 1 ){ LED_Close(2); return; }

    
    // ����Ĭ���źŲ�ƥ���
    
    // ��ʼ�������źţ����ר�ã�
    LIN_MASTER_PrimaryReceiveChkInit();
    
    // ����Ƿ������ȷ�İ����ź�
    // ͬʱ���ݰ������¹��������ϵķ����źţ����ڼ��Աȣ�
    if( LIN_MASTER_PrimaryReceiveChkHasMirrUp()      == 1 ){ LIN_MASTER_PrimaryReceiveChkSetMirrUp();      signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasMirrLeft()    == 1 ){ LIN_MASTER_PrimaryReceiveChkSetMirrLeft();    signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasMirrDown()    == 1 ){ LIN_MASTER_PrimaryReceiveChkSetMirrDown();    signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasMirrRight()   == 1 ){ LIN_MASTER_PrimaryReceiveChkSetMirrRight();   signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasMirrAssist()  == 1 ){ LIN_MASTER_PrimaryReceiveChkSetMirrAssist();  signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasMirrAssist2() == 1 ){ LIN_MASTER_PrimaryReceiveChkSetMirrAssist2(); signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasMirrOC()      == 1 ){ LIN_MASTER_PrimaryReceiveChkSetMirrOC();      signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasMirrSWL()     == 1 ){ LIN_MASTER_PrimaryReceiveChkSetMirrSWL();     signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasMirrSWR()     == 1 ){ LIN_MASTER_PrimaryReceiveChkSetMirrSWR();     signalContain |= 1; }

    if( LIN_MASTER_PrimaryReceiveChkHasLFUp()        == 1 ){ LIN_MASTER_PrimaryReceiveChkSetLFUp();        signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasLFUpA()       == 1 ){ LIN_MASTER_PrimaryReceiveChkSetLFUpA();       signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasLFUpAuto()    == 1 ){ LIN_MASTER_PrimaryReceiveChkSetLFUpAuto();    signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasLFDown()      == 1 ){ LIN_MASTER_PrimaryReceiveChkSetLFDown();      signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasLFDownA()     == 1 ){ LIN_MASTER_PrimaryReceiveChkSetLFDownA();     signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasLFDownAuto()  == 1 ){ LIN_MASTER_PrimaryReceiveChkSetLFDownAuto();  signalContain |= 1; }
    
    if( LIN_MASTER_PrimaryReceiveChkHasRFUp()        == 1 ){ LIN_MASTER_PrimaryReceiveChkSetRFUp();        signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasRFUpA()       == 1 ){ LIN_MASTER_PrimaryReceiveChkSetRFUpA();       signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasRFUpAuto()    == 1 ){ LIN_MASTER_PrimaryReceiveChkSetRFUpAuto();    signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasRFDown()      == 1 ){ LIN_MASTER_PrimaryReceiveChkSetRFDown();      signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasRFDownA()     == 1 ){ LIN_MASTER_PrimaryReceiveChkSetRFDownA();     signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasRFDownAuto()  == 1 ){ LIN_MASTER_PrimaryReceiveChkSetRFDownAuto();  signalContain |= 1; }
    
    if( LIN_MASTER_PrimaryReceiveChkHasWinLock()     == 1 ){ LIN_MASTER_PrimaryReceiveChkSetWinLock();     signalContain |= 1; }
    if( LIN_MASTER_PrimaryReceiveChkHasWinUnLock()   == 1 ){ LIN_MASTER_PrimaryReceiveChkSetWinUnLock();   signalContain |= 1; }

    // ���������ź�У���루���ר�ã�
    GV_LinMasterPrimaryReceiveChks[signalLength-1] = LIN_CalcChecknum(GV_LinMasterPrimaryReceiveChks, signalLength-1, 0);
      
    //��DEBUG�������źż��ʱ����������ϵķ����ź�
    if( GV_LinMasterDebugReceiveChkOutUart1  == 1 ){ UART1_SendBytes(GV_LinMasterPrimaryReceiveChks, signalLength); delay_ms(500); }
    if( GV_LinMasterDebugReceiveChkOutPrintf == 1 ){ for( u8 i = 0; i < signalLength; i++ ) printf("%02X ", GV_LinMasterPrimaryReceiveChks[i]); printf("\n"); }
    
    // �Ƚ� ���յ��ķ����ź� �� �����ϵķ����ź� �Ƿ�һ��
    signalMatch = 1; for( int i = 0; i < signalLength; i++ ) signalMatch &= GV_LinMasterPrimaryReceive[i] == GV_LinMasterPrimaryReceiveChks[i];
    
    // �Ͱ�������ֵ�����źŲ�һ�£�˵�����յ����ź�������
    //if( signalMatch == 0 ){ LED_Open(2); return; }
  
    // ��������
    //if( signalContain ){ BUZZER_Open(1); delay_ms(5); BUZZER_Close(1); }
}
