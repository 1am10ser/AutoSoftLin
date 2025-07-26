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
u32 GV_LinMasterBaudRate     = 9600;  // ������
u32 GV_LinMasterSendInterval = 50;    // �ź��·����豸�ļ��ʱ�䣨���룩

/**
 * LIN ������ر���
 */
short GV_LinMasterDebugSendOutUart1            = 0;  // Debug ���͵���������� UART1
short GV_LinMasterDebugSendOutPrintf           = 0;  // Debug ���͵���������� printf��ע�⣺��ϵͳ�п������� printf ��������� UART1��
short GV_LinMasterDebugReceiveOutUart1         = 0;  // Debug ���յ���������� UART1
short GV_LinMasterDebugReceiveOutPrintf        = 0;  // Debug ���յ���������� printf��ע�⣺��ϵͳ�п������� printf ��������� UART1��
short GV_LinMasterDebugRecResponseChkOutUart1  = 0;  // Debug �������ݼ��ʱ����� UART1
short GV_LinMasterDebugRecResponseChkOutPrintf = 0;  // Debug �������ݼ��ʱ����� printf��ע�⣺��ϵͳ�п������� printf ��������� UART1��

/**
 * LIN ���ź���ر���
 * ���ź���ָ���յ��ӻ��������ź�
 */
u8 GV_LinMasterMainASignal[]               = {0x42, 0x00, 0x00, 0x00, 0xF2, 0xCA};  // ���� ON ��Ĭ�ϵ����źţ�ID��DATA��CheckNum��
                                                                                    // �ñ����������������͵�ID���ӻ�Ĭ����Ӧ������ �� У����
                                                                                    // ϵͳ�Զ�������Щ���ݷ���ID���Լ�ƥ�Դӻ��������ź��Ƿ�׼ȷ�ȹ���
                                                                                    // ���Բ�����KD-281-D1 ��{0x42, 0x40, 0x00, 0x20, 0x04, 0x59};  // ��·����δ����ͭƬ�����������ڴ���״̬
                                                                                    // ���Բ�����KD-281-D3 ��{0x42, 0x00, 0x00, 0x20, 0x04, 0x99};  // ��·����δ����ͭƬ�����������ڴ���״̬
                                                                                    // ��ԭ������KD-281-D27��{0x42, 0x00, 0x00, 0x00, 0xF2, 0xCA};
u8 GV_LinMasterMainASignalChecknumStart    = 0;                                     // ���������á����ź�У������ʼ���������� GV_LinMasterMainASignal ������  

u8 GV_LinMasterMainARecResponse[]          = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // ��ϵͳʹ�á����źŷ��ͺ��յ��ķ����������������� ID��DATA��CheckNum�����鳤����Ҫ�� GV_LinMasterMainASignal һ��
u8 GV_LinMasterMainARecResponseCnt         = 0;                                     // ��ϵͳʹ�á����� GV_LinMasterMainARecResponse ��������������ݵĸ���
u8 GV_LinMasterMainARecResponseWait        = 0;                                     // ��ϵͳʹ�á����źŵȴ������ı�־��ϵͳ�Զ����øñ�־���жϲ���ʽ�������źż�¼�� GV_LinMasterMainARecResponse
u8 GV_LinMasterMainARecResponseChks[]      = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // ��ϵͳʹ�á������źż�����ʱ�������������� ID��DATA��CheckNum�����鳤����Ҫ�� GV_LinMasterMainASignal һ��
u8 GV_LinMasterMainARecResponsePrev[]      = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // ��ϵͳʹ�á���һ���ź���ʱ�������������� ID��DATA��CheckNum�����鳤����Ҫ�� GV_LinMasterMainASignal һ��

u8 GV_LinMasterMainARecResponseToUsbMode   = LIN_TXUSB_MODE_DOWNUPONE;              // ���������á����źŷ��ͺ��յ��ķ����������ģʽ��LIN_TXUSB_MODE_DEFAULT, LIN_TXUSB_MODE_DOWNONE���ӻ����·���һ�Σ�, LIN_TXUSB_MODE_DOWNUPONE���ӻ����º��ɿ�������һ�Σ�
u8 GV_LinMasterMainARecResponseToUsbDnHead = 0xAE;                                  // ���������á��ӻ����·���һ���źŵ�����ͷ
u8 GV_LinMasterMainARecResponseToUsbDnFoot = 0xAF;                                  // ���������á��ӻ����·���һ���źŵ�����β
u8 GV_LinMasterMainARecResponseToUsbUpHead = 0xBE;                                  // ���������á��ӻ��ɿ�����һ���źŵ�����ͷ
u8 GV_LinMasterMainARecResponseToUsbUpFoot = 0xBF;                                  // ���������á��ӻ��ɿ�����һ���źŵ�����β


/**
 * LIN ���ź���ر���
 * ���źŽ�����������֪ͨ�ӻ�
 */
u8 GV_LinMasterDownASignal[]               = {0x80, 0x80, 0xFC};  // ���� ON ��Ĭ���·��ĸ��źţ�ID �� DATA������ CheckNum����һ���������� ECU ֪ͨ���ر������������ʹ�ܵ��������
u8 GV_LinMasterDownASignalBox[]            = {0x00, 0x00, 0x00};  // ��ϵͳʹ�á��·��ĸ��ź����������鳤�Ⱥ� GV_LinMasterDownASignal һ�£���Ҫ���ڹ�������ż������͸��ź�
u8 GV_LinMasterDownASignalChecknumStart    = 0;                   // �·��ĸ��ź�У������ʼ���������� GV_LinMasterDownASignal ������


/**
 * LIN �����ź���ر���
 */
u8 GV_LinMasterFindARecResponseCnt         = 0;  // ��ϵͳʹ�á����� GV_LinMasterFindARecResponse ��������������ݵĸ���
u8 GV_LinMasterFindARecResponseWait        = 0;  // ��ϵͳʹ�á����źŵȴ������ı�־��ϵͳ�Զ����øñ�־���жϲ���ʽ�������źż�¼�� GV_LinMasterFindARecResponse
u8 GV_LinMasterFindARecResponse[20];             // ��ϵͳʹ�á����źŷ��ͺ��յ��ķ����������������� ID��DATA��CheckNum



/* -------------------------------------------------- */
// �� �������壨���źţ�
/* -------------------------------------------------- */

/**
 * �����·������ź�
 * ���ź���ָ���յ��ӻ��������ź�
 *
 * �������ź� ID �󣬴ӻ��ᷴ�����ݣ��� STM8 �� "INTERRUPT_HANDLER(UART3_RX_IRQHandler,21)" �ж���
 * ������ UART3_InterruptHandlerRX()->LIN_MASTER_ReceiveMainARecResponse() ������¼���������ݵ�ȫ�ֱ��� GV_LinMasterMainARecResponse
 */
void LIN_MASTER_SendMainASignal()
{
    // ׼�����ݽ���
    GV_LinMasterMainARecResponseCnt  = 0;
    GV_LinMasterMainARecResponseWait = 1;
    
    // ���ͼ���Σ�ͬ���Σ�ID
    UART3_SendBreak();
    UART3_SendByte(0x55);
    UART3_SendByte(GV_LinMasterMainASignal[0]);
    
    // ʱ����
    delay_ms(GV_LinMasterSendInterval);
}


/**
 * ���ͷ������źŵ� USB
 */
void LIN_MASTER_SendMainARecResponseToUsb()
{
    // ��ʼ��
    u8 i, match = 1;
    u8 mode = GV_LinMasterMainARecResponseToUsbMode;
    u8 length = ARR_LENGTH(GV_LinMasterMainARecResponse);

    // ��Ĭ���źűȽ�
    match = 1; 
    for( i = 0; i < length; i++ ) match &= GV_LinMasterMainASignal[i] == GV_LinMasterMainARecResponse[i];

    // ��Ĭ���ź���ͬ����ģʽΪ��"���·�һ��" �� "���º��ɿ���һ��"
    // �ź���ͬ��������һ���źű����Ҳ����ͽ��ܵ��ķ����ź�
    // �����źź�Ĭ���ź���ͬ˵������·���ϴ���Ĭ��״̬��û�а������»�����Ĭ�����
    if( match == 1 && (mode==LIN_TXUSB_MODE_DOWNONE||mode==LIN_TXUSB_MODE_DOWNUPONE) ){
        // ��һ���ź��Ƿ�ȫ���� 0����ô˵����·����һ��״̬Ҳ��Ĭ��״̬
        // ˵��û�а������£���ôֱ���˳�
        match = 1;
        for( i = 0; i < length; i++ ) match &= GV_LinMasterMainARecResponsePrev[i] == 0x00;
        if( match == 1 ) return;
        
        // ��һ���źŷ� 0����ô��һ��״̬�������£���ô��ʱ�ĳ����߼������ɰ����ɿ�����
        if( mode == LIN_TXUSB_MODE_DOWNUPONE ){
            UART1_SendByte(GV_LinMasterMainARecResponseToUsbUpHead); UART1_SendByte(length);
            for( i = 0; i < length; i++ ) UART1_SendByte(GV_LinMasterMainARecResponsePrev[i]);
            UART1_SendByte(GV_LinMasterMainARecResponseToUsbUpHead);
        }
        
        // ������һ���ź�
        // �����ɿ��ˣ���ô������һ���ź�ȫ��ΪĬ��״̬
        for( i = 0; i < length; i++ ) GV_LinMasterMainARecResponsePrev[i] = 0x00;
        
        // ����
        return;
    }
    
    // ����һ���źűȽ�
    // �����źź���һ���ź���ͬ�򲻷��ͷ����ź�
    if( mode == LIN_TXUSB_MODE_DOWNONE || mode == LIN_TXUSB_MODE_DOWNUPONE ){
        match = 1; for( i = 0; i < length; i++ ) match &= GV_LinMasterMainARecResponsePrev[i] == GV_LinMasterMainARecResponse[i];
        if( match == 1 ) return;
    }

    
    // ���Ϳ�ͷ����
    UART1_SendByte(GV_LinMasterMainARecResponseToUsbDnHead);
    UART1_SendByte(length);
    
    // ���ͷ������ݵ� USB
    for( i = 0; i < length; i++ ) UART1_SendByte(GV_LinMasterMainARecResponse[i]);

    // ���浱ǰ���ݵ���һ���źŵı�����
    for( i = 0; i < length; i++ ) GV_LinMasterMainARecResponsePrev[i] = GV_LinMasterMainARecResponse[i];

    // ������β����
    UART1_SendByte(GV_LinMasterMainARecResponseToUsbDnFoot);
}


/**
 * �������źŰ������
 * �ú�������ȫ�ֱ��� GV_LinMasterMainARecResponse ֵΪ����
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
short LIN_MASTER_MainARecResponseContain( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue )
{
    // �ع�����λ
    if( dataHighLow == 'h' ) data8421Bit = data8421Bit << 4;
    if( dataHighLow == 'H' ) data8421Bit = data8421Bit << 4;

    if( data8421BitValue == 0 && (GV_LinMasterMainARecResponse[dataNum]&data8421Bit) == 0 ) return 1;
    if( data8421BitValue == 1 && (GV_LinMasterMainARecResponse[dataNum]&data8421Bit) == data8421Bit ) return 1;
    
    return 0;
}


/**
 * ���շ������ź�
 * �ú����� UART ���ж��б����ã������Զ�������ص����þ����Ƿ��¼����
 *
 * @param byte  8λ�ֽ�
 */
void LIN_MASTER_ReceiveMainARecResponse( u8 byte )
{
    // ��Ч����
    if( GV_LinMasterMainARecResponseWait == 0 ) return;
    if( GV_LinMasterMainARecResponseCnt >= ARR_LENGTH(GV_LinMasterMainARecResponse) ) return;

    // ���ռ���κ�ͬ���β�����
    if( GV_LinMasterMainARecResponseWait == 1 && byte == 0x00 ){ GV_LinMasterMainARecResponseWait = 2; return; }
    if( GV_LinMasterMainARecResponseWait == 2 && byte == 0x55 ){ GV_LinMasterMainARecResponseWait = 3; return; }
    if( GV_LinMasterMainARecResponseWait != 3 ) return;
    
    //��DEBUG�� ������յ��ķ����ź�
    if( GV_LinMasterDebugReceiveOutUart1  == 1 ) UART1_SendByte(byte);
    if( GV_LinMasterDebugReceiveOutPrintf == 1 ) printf("%02X ", byte);

    // ���ݸ�ֵ
    GV_LinMasterMainARecResponse[GV_LinMasterMainARecResponseCnt++] = byte;
}

/**
 * ���ý��շ����ź�
 */
void LIN_MASTER_ReceiveMainARecResponseReset()
{
    GV_LinMasterMainARecResponseCnt  = 0;
    GV_LinMasterMainARecResponseWait = 0;
}



/* -------------------------------------------------- */
// �� �������壨���źţ�
/* -------------------------------------------------- */

/**
 * ��ʼ���·��ĸ��ź�
 * �ú���ֱ�ӳ�ʼ��ȫ�ֱ��� GV_LinMasterDownASignalBox
 */
void LIN_MASTER_InitDownASignal()
{
    for( int i=0; i < ARR_LENGTH(GV_LinMasterDownASignal); i++ ) GV_LinMasterDownASignalBox[i] = GV_LinMasterDownASignal[i];
}


/**
 * �����·��ĸ��ź�
 * �ú���ֱ������ȫ�ֱ��� GV_LinMasterDownASignalBox
 * �ú�������λ���Ƿ������磺1H40(��ʾ��LIN�����ݶε�1�������ֽڵĸ�λ4����Ϊ0)
 * ����������������������������2L11(��ʾ��LIN�����ݶε�2�������ֽڵĵ�λ1����Ϊ1)
 *
 * @param dataNum           ���ݱ�ţ��� 1 ��ʼ
 * @param dataHightLow      ���ݸߵ�λ��H��L
 * @param data8421Bit       ����λ���� 8421 Ϊ��׼
 * @param data8421BitValue  ����λֵ��0��1
 */
void LIN_MASTER_SetDownASignal( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue )
{
    // �ع�����λ
    if( dataHighLow == 'h' ) data8421Bit = data8421Bit << 4;
    if( dataHighLow == 'H' ) data8421Bit = data8421Bit << 4;

    // �����·��ĸ��ź�
    if( data8421BitValue == 1 ) GV_LinMasterDownASignalBox[dataNum] |= data8421Bit;
    if( data8421BitValue == 0 ) GV_LinMasterDownASignalBox[dataNum] &= data8421Bit^0xFF;
}


/**
 * �����·��ĸ��ź�
 * �ú���ֱ�ӷ���ȫ�ֱ��� GV_LinMasterDownASignalBox �������
 */
void LIN_MASTER_SendDownASignal()
{
    // ��ʼ��
    u8 start  = GV_LinMasterDownASignalChecknumStart;
    u8 length = ARR_LENGTH(GV_LinMasterDownASignal);

    // ���� LIN ��־
    UART3_SendBreak();
    UART3_SendByte(0x55);

    // �����·��ĸ��ź�
    for( int i = 0; i < length; i++ ) UART3_SendByte(GV_LinMasterDownASignalBox[i]);
    UART3_SendByte(LIN_CalcChecknum(GV_LinMasterDownASignalBox,length,start));

    // ʱ����
    delay_ms(GV_LinMasterSendInterval);
    
    //��DEBUG�� ͨ�� UART1 ��������·��ĸ��ź�
    if( GV_LinMasterDebugSendOutUart1 == 1 ){
        for( int i = 0; i < length; i++ ) UART1_SendByte(GV_LinMasterDownASignalBox[i]);
        UART1_SendByte(LIN_CalcChecknum(GV_LinMasterDownASignalBox,length,start));
    }
    
    //��DEBUG�� ͨ�� Printf ��������·��ĸ��ź�
    if( GV_LinMasterDebugSendOutPrintf == 1 ){
        for( int i = 0; i < ARR_LENGTH(GV_LinMasterDownASignal); i++ ) printf("%02X ", GV_LinMasterDownASignalBox[i]);
        printf("%02X\n", LIN_CalcChecknum(GV_LinMasterDownASignalBox,length,start));
    }
}


/**
 * �����·��ĸ��ź�
 * �ú���ֱ������ȫ�ֱ��� GV_LinMasterDownASignalBox
 * ֱ�ӽ������ֽڼ���ֵ�� GV_LinMasterDownASignalBox������ֵǰ���� ID �Ƿ�һ�£�ͬʱ���� bytes �е� Checknum
 *
 * @param datBytes   �����ֽڼ�
 * @param datLength  �����ֽڳ���
 */
void LIN_MASTER_SetDownASignalBytes( u8 datBytes[], u8 datLength )
{
    // ��ʼ��
    u8 i;
    u8 length = ARR_LENGTH(GV_LinMasterDownASignal);

    // ��Ч ID
    if( datBytes[0] != GV_LinMasterDownASignal[0] ) return;

    // ��ֵ
    length = length > datLength ? datLength : length;
    for( i = 0; i < length; i++ ) GV_LinMasterDownASignalBox[i] = datBytes[i];
}


/**
 * �����·��ĸ��ź�
 * Ԥ��������ܶ�Ӧ�������ź�
 */
void LIN_MASTER_SetDownASignalLLedOn(){ LIN_MASTER_SetDownASignal(1, 'L', 4, 1); }  // �źţ���ʹ�ܵ���
void LIN_MASTER_SetDownASignalRLedOn(){ LIN_MASTER_SetDownASignal(1, 'L', 1, 1); }  // �źţ���ʹ�ܵ���



/* -------------------------------------------------- */
// �� �������壨LIN ID ������أ�
/* -------------------------------------------------- */

/**
 * �����·������ź�
 * ���ź���ָ���յ��ӻ��������ź�
 *
 * @param id  LIN ID �ֽ�
 */
void LIN_MASTER_SendFindASignal( u8 id )
{
    // ׼�����ݽ���
    GV_LinMasterFindARecResponseCnt  = 0;
    GV_LinMasterFindARecResponseWait = 1;
    
    // ���ͼ���Σ�ͬ���Σ�ID
    UART3_SendBreak();
    UART3_SendByte(0x55);
    UART3_SendByte(id);

    // ʱ����
    delay_ms(GV_LinMasterSendInterval);
}


/**
 * ���շ������ź�
 * �ú����� UART ���ж��б����ã������Զ�������ص����þ����Ƿ��¼����
 *
 * @param byte  8λ�ֽ�
 */
void LIN_MASTER_ReceiveFindARecResponse( u8 byte )
{
    // ��Ч����
    if( GV_LinMasterFindARecResponseWait == 0 ) return;
    if( GV_LinMasterFindARecResponseCnt >= ARR_LENGTH(GV_LinMasterFindARecResponse) ) return;

    // ���ռ���κ�ͬ���β�����
    if( GV_LinMasterFindARecResponseWait == 1 && byte == 0x00 ){ GV_LinMasterFindARecResponseWait = 2; return; }
    if( GV_LinMasterFindARecResponseWait == 2 && byte == 0x55 ){ GV_LinMasterFindARecResponseWait = 3; return; }
    if( GV_LinMasterFindARecResponseWait != 3 ) return;

    //��DEBUG�� ������յ��ķ����ź�
    if( GV_LinMasterDebugReceiveOutUart1  == 1 ) UART1_SendByte(byte);
    if( GV_LinMasterDebugReceiveOutPrintf == 1 ) printf("%02X ", byte);

    // ���ݸ�ֵ
    GV_LinMasterFindARecResponse[GV_LinMasterFindARecResponseCnt++] = byte;
}


/**
 * ���ͷ������źŵ� USB
 */
void LIN_MASTER_SendFindARecResponseToUsb()
{
    // ���ͷ������ݵ� USB
    for( int i = 0; i < GV_LinMasterFindARecResponseCnt; i++ ) UART1_SendByte(GV_LinMasterFindARecResponse[i]);
}


/**
 * ���ý��շ����ź�
 */
void LIN_MASTER_ReceiveFindARecResponseReset()
{
    GV_LinMasterFindARecResponseCnt  = 0;
    GV_LinMasterFindARecResponseWait = 0;
}



/* -------------------------------------------------- */
// �� �������壨���źż��ƥ����أ�
/* -------------------------------------------------- */

/**
 * ��ʼ�������źţ����ר�ã�
 */
void LIN_MASTER_MainARecResponseChkInit()
{
    for( int i=0; i < ARR_LENGTH(GV_LinMasterMainASignal); i++ ) GV_LinMasterMainARecResponseChks[i] = GV_LinMasterMainASignal[i];
}

/**
 * ���÷������źţ����ר�ã�
 * �ú���ֱ������ȫ�ֱ��� GV_LinMasterMainARecResponseChks
 * �ú�������λ���Ƿ������磺1H40(��ʾ�� LIN �����ݶε�1�������ֽڵĸ�λ4����Ϊ0)
 * ����������������������������2L11(��ʾ�� LIN �����ݶε�2�������ֽڵĵ�λ1����Ϊ1)
 *
 * @param dataNum           ���ݱ�ţ��� 1 ��ʼ
 * @param dataHightLow      ���ݸߵ�λ��H��L
 * @param data8421Bit       ����λ���� 8421 Ϊ��׼
 * @param data8421BitValue  ����λֵ��0��1
 */
void LIN_MASTER_MainARecResponseChkSet( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue )
{
    // �ع�����λ
    if( dataHighLow == 'h' ) data8421Bit = data8421Bit << 4;
    if( dataHighLow == 'H' ) data8421Bit = data8421Bit << 4;

    // ���÷������źţ����ר�ã�
    if( data8421BitValue == 1 ) GV_LinMasterMainARecResponseChks[dataNum] |= data8421Bit;
    if( data8421BitValue == 0 ) GV_LinMasterMainARecResponseChks[dataNum] &= data8421Bit^0xFF;
}

// ���Ӿ�����
short LIN_MASTER_MainARecResponseChkHasMirrUp()     { return LIN_MASTER_MainARecResponseContain(3,'L',1,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }  
void  LIN_MASTER_MainARecResponseChkSetMirrUp()     {         LIN_MASTER_MainARecResponseChkSet(3,'L',1,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }
 
// ���Ӿ�����
short LIN_MASTER_MainARecResponseChkHasMirrLeft()   { return LIN_MASTER_MainARecResponseContain(3,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }  
void  LIN_MASTER_MainARecResponseChkSetMirrLeft()   {         LIN_MASTER_MainARecResponseChkSet(3,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ���Ӿ�����
short LIN_MASTER_MainARecResponseChkHasMirrDown()   { return LIN_MASTER_MainARecResponseContain(3,'L',2,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetMirrDown()   {         LIN_MASTER_MainARecResponseChkSet(3,'L',2,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ���Ӿ�����
short LIN_MASTER_MainARecResponseChkHasMirrRight()  { return LIN_MASTER_MainARecResponseContain(3,'L',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetMirrRight()  {         LIN_MASTER_MainARecResponseChkSet(3,'L',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ���Ӿ��������������������շţ�����ʹ�ܣ�
short LIN_MASTER_MainARecResponseChkHasMirrAssist() { return LIN_MASTER_MainARecResponseContain(4,'L',8,1); }
void  LIN_MASTER_MainARecResponseChkSetMirrAssist() {         LIN_MASTER_MainARecResponseChkSet(4,'L',8,1); }

// ���Ӿ��������������������շţ�����ʹ�ܣ�
short LIN_MASTER_MainARecResponseChkHasMirrAssist2(){ return LIN_MASTER_MainARecResponseContain(2,'H',4,1); }
void  LIN_MASTER_MainARecResponseChkSetMirrAssist2(){         LIN_MASTER_MainARecResponseChkSet(2,'H',4,1); }

// ���Ӿ��շ�
short LIN_MASTER_MainARecResponseChkHasMirrOC()     { return LIN_MASTER_MainARecResponseContain(3,'H',1,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetMirrOC()     {         LIN_MASTER_MainARecResponseChkSet(3,'H',1,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ����Ӿ�ʹ��
short LIN_MASTER_MainARecResponseChkHasMirrSWL()    { return LIN_MASTER_MainARecResponseContain(2,'H',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetMirrSWL()    {         LIN_MASTER_MainARecResponseChkSet(2,'H',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// �Һ��Ӿ�ʹ��
short LIN_MASTER_MainARecResponseChkHasMirrSWR()    { return LIN_MASTER_MainARecResponseContain(2,'H',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetMirrSWR()    {         LIN_MASTER_MainARecResponseChkSet(2,'H',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰ��
short LIN_MASTER_MainARecResponseChkHasLFUp()       { return LIN_MASTER_MainARecResponseContain(1,'L',2,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetLFUp()       {         LIN_MASTER_MainARecResponseChkSet(1,'L',2,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰ��һ��
short LIN_MASTER_MainARecResponseChkHasLFUpA()      { return LIN_MASTER_MainARecResponseContain(1,'L',7,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetLFUpA()      {         LIN_MASTER_MainARecResponseChkSet(1,'L',7,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰһ����
short LIN_MASTER_MainARecResponseChkHasLFUpAuto()   { return LIN_MASTER_MainARecResponseContain(1,'L',6,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetLFUpAuto()   {         LIN_MASTER_MainARecResponseChkSet(1,'L',6,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰ��
short LIN_MASTER_MainARecResponseChkHasLFDown()     { return LIN_MASTER_MainARecResponseContain(1,'L',1,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetLFDown()     {         LIN_MASTER_MainARecResponseChkSet(1,'L',1,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰ��һ��
short LIN_MASTER_MainARecResponseChkHasLFDownA()    { return LIN_MASTER_MainARecResponseContain(1,'L',7,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetLFDownA()    {         LIN_MASTER_MainARecResponseChkSet(1,'L',7,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰһ����
short LIN_MASTER_MainARecResponseChkHasLFDownAuto() { return LIN_MASTER_MainARecResponseContain(1,'L',5,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }  
void  LIN_MASTER_MainARecResponseChkSetLFDownAuto() {         LIN_MASTER_MainARecResponseChkSet(1,'L',5,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰ��
short LIN_MASTER_MainARecResponseChkHasRFUp()       { return LIN_MASTER_MainARecResponseContain(1,'H',1,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFUp()       {         LIN_MASTER_MainARecResponseChkSet(1,'H',1,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰ��һ��
short LIN_MASTER_MainARecResponseChkHasRFUpA()      { return LIN_MASTER_MainARecResponseContain(1,'H',3,1) && LIN_MASTER_MainARecResponseContain(1,'L',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFUpA()      {         LIN_MASTER_MainARecResponseChkSet(1,'H',3,1);    LIN_MASTER_MainARecResponseChkSet(1,'L',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰһ����
short LIN_MASTER_MainARecResponseChkHasRFUpAuto()   { return LIN_MASTER_MainARecResponseContain(1,'H',3,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFUpAuto()   {         LIN_MASTER_MainARecResponseChkSet(1,'H',3,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰ��
short LIN_MASTER_MainARecResponseChkHasRFDown()     { return LIN_MASTER_MainARecResponseContain(1,'L',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFDown()     {         LIN_MASTER_MainARecResponseChkSet(1,'L',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰ��һ��
short LIN_MASTER_MainARecResponseChkHasRFDownA()    { return LIN_MASTER_MainARecResponseContain(1,'H',3,1) && LIN_MASTER_MainARecResponseContain(1,'L',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFDownA()    {         LIN_MASTER_MainARecResponseChkSet(1,'H',3,1);    LIN_MASTER_MainARecResponseChkSet(1,'L',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��ǰһ����
short LIN_MASTER_MainARecResponseChkHasRFDownAuto() { return LIN_MASTER_MainARecResponseContain(1,'H',2,1) && LIN_MASTER_MainARecResponseContain(1,'L',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFDownAuto() {         LIN_MASTER_MainARecResponseChkSet(1,'H',2,1);    LIN_MASTER_MainARecResponseChkSet(1,'L',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// ��������
short LIN_MASTER_MainARecResponseChkHasWinLock()    { return LIN_MASTER_MainARecResponseContain(3,'H',4,1); }
void  LIN_MASTER_MainARecResponseChkSetWinLock()    {         LIN_MASTER_MainARecResponseChkSet(3,'H',4,1); }  

// ��������
short LIN_MASTER_MainARecResponseChkHasWinUnLock()  { return LIN_MASTER_MainARecResponseContain(3,'H',8,1); }
void  LIN_MASTER_MainARecResponseChkSetWinUnLock()  {         LIN_MASTER_MainARecResponseChkSet(3,'H',8,1); }  



/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */

/**
 * LIN ���߼������Ҵӻ��� ID ��ʶ
 *
 * @param process  ��ʾ����
 */
void LIN_MASTER_MainFindASlaveId( bool process )
{
    // ��ʼ��
    int id;
    int finded = 0;
    
    // LIN ID ����
    for( id = 0; id <= 0xFF; id++ ){
        // �����������ź�
        LIN_MASTER_SendFindASignal((u8)id);

        // ����״̬
        finded = GV_LinMasterFindARecResponseCnt > 1 ? 1 : 0;

        // ���źŷ��ͺ�Է����źŴ���
        if( finded ){
            // �ӳ٣�ʹ��λ�������ȡ����ʱ���Զ�����
            if( process ) delay_ms(500); 
            
            // ���� LIN �źŵ� USB
            LIN_MASTER_SendFindARecResponseToUsb();
            
            // �ӳ٣�ʹ��λ�������ȡ����ʱ���Զ�����
            delay_ms(500); 
        }
        else if( process ){
            UART1_SendByte((short)id);
        }

        // ���÷����ź��Ա���´����ź�����
        LIN_MASTER_ReceiveFindARecResponseReset();
    }
}


/**
 * LIN ���߼����������źţ�MainA�������մӻ������źţ�Ȼ������� USB
 */
void LIN_MASTER_MainMainAReceiveSlaveToUsb()
{
    // �����������ź�
    LIN_MASTER_SendMainASignal();

    // ���źŷ��ͺ�Է����źŴ���
    LIN_MASTER_SendMainARecResponseToUsb();

    // ���÷����ź��Ա���´����ź�����
    LIN_MASTER_ReceiveMainARecResponseReset();
}


/**
 * LIN ���߼����������źţ�MainA�������մӻ������źţ�Ȼ����м��
 * �ú���һ�����ڵ�������·����
 */
void LIN_MASTER_MainMainAReceiveSlaveAndCheck()
{
    // ��ʼ��
    u8 signalMatch;
    u8 signalLength = ARR_LENGTH(GV_LinMasterMainASignal);
    u8 signalContain = 0;
    
    // �����Ƿ���Ĭ���ź�ƥ��
    // �������ź���Ĭ���ź�һ��˵������·��δ���°���
    signalMatch = 1; for( int i = 0; i < signalLength; i++ ) signalMatch &= GV_LinMasterMainASignal[i] == GV_LinMasterMainARecResponse[i];
    
    //��DEBUG��������յ��ķ����ź�
    if( GV_LinMasterDebugRecResponseChkOutUart1  == 1 ){ for( u8 i = 0; i < signalLength; i++ ) UART1_SendByte(GV_LinMasterMainARecResponse[i]); delay_ms(600); }
    if( GV_LinMasterDebugRecResponseChkOutPrintf == 1 ){ for( u8 i = 0; i < signalLength; i++ ) printf("==="); printf("\n"); for( u8 i = 0; i < signalLength; i++ ) printf("%02X ", GV_LinMasterMainARecResponse[i]); printf("\n"); }
    
    // ��Ĭ���ź�ƥ��
    //if( signalMatch == 1 ){ LED_Close(2); return; }

    
    // ����Ĭ���źŲ�ƥ���
    
    // ��ʼ�������źţ����ר�ã�
    LIN_MASTER_MainARecResponseChkInit();
    
    // ����Ƿ������ȷ�İ����ź�
    // ͬʱ���ݰ������¹��������ϵķ����źţ����ڼ��Աȣ�
    if( LIN_MASTER_MainARecResponseChkHasMirrUp()      == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrUp();      signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrLeft()    == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrLeft();    signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrDown()    == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrDown();    signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrRight()   == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrRight();   signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrAssist()  == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrAssist();  signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrAssist2() == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrAssist2(); signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrOC()      == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrOC();      signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrSWL()     == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrSWL();     signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrSWR()     == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrSWR();     signalContain |= 1; }

    if( LIN_MASTER_MainARecResponseChkHasLFUp()        == 1 ){ LIN_MASTER_MainARecResponseChkSetLFUp();        signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasLFUpA()       == 1 ){ LIN_MASTER_MainARecResponseChkSetLFUpA();       signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasLFUpAuto()    == 1 ){ LIN_MASTER_MainARecResponseChkSetLFUpAuto();    signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasLFDown()      == 1 ){ LIN_MASTER_MainARecResponseChkSetLFDown();      signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasLFDownA()     == 1 ){ LIN_MASTER_MainARecResponseChkSetLFDownA();     signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasLFDownAuto()  == 1 ){ LIN_MASTER_MainARecResponseChkSetLFDownAuto();  signalContain |= 1; }
    
    if( LIN_MASTER_MainARecResponseChkHasRFUp()        == 1 ){ LIN_MASTER_MainARecResponseChkSetRFUp();        signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasRFUpA()       == 1 ){ LIN_MASTER_MainARecResponseChkSetRFUpA();       signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasRFUpAuto()    == 1 ){ LIN_MASTER_MainARecResponseChkSetRFUpAuto();    signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasRFDown()      == 1 ){ LIN_MASTER_MainARecResponseChkSetRFDown();      signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasRFDownA()     == 1 ){ LIN_MASTER_MainARecResponseChkSetRFDownA();     signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasRFDownAuto()  == 1 ){ LIN_MASTER_MainARecResponseChkSetRFDownAuto();  signalContain |= 1; }
    
    if( LIN_MASTER_MainARecResponseChkHasWinLock()     == 1 ){ LIN_MASTER_MainARecResponseChkSetWinLock();     signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasWinUnLock()   == 1 ){ LIN_MASTER_MainARecResponseChkSetWinUnLock();   signalContain |= 1; }

    // ���������ź�У���루���ר�ã�
    GV_LinMasterMainARecResponseChks[signalLength-1] = LIN_CalcChecknum(GV_LinMasterMainARecResponseChks, signalLength-GV_LinMasterMainASignalChecknumStart-1, GV_LinMasterMainASignalChecknumStart);
      
    //��DEBUG��������ݰ������¹�������
    if( GV_LinMasterDebugRecResponseChkOutUart1  == 1 ){ UART1_SendBytes(GV_LinMasterMainARecResponseChks, signalLength); delay_ms(500); }
    if( GV_LinMasterDebugRecResponseChkOutPrintf == 1 ){ for( u8 i = 0; i < signalLength; i++ ) printf("%02X ", GV_LinMasterMainARecResponseChks[i]); printf("\n"); }
    
    // �ȽϽ��յ��ķ����źźͰ����۹����ķ����ź��Ƿ�һֱ
    signalMatch = 1; for( int i = 0; i < signalLength; i++ ) signalMatch &= GV_LinMasterMainARecResponse[i] == GV_LinMasterMainARecResponseChks[i];
    
    // �Ͱ�������ֵ�����źŲ�һ�£�˵�����յ����ź�������
    //if( signalMatch == 0 ){ LED_Open(2); return; }
  
    // ��������
    //if( signalContain ){ BUZZER_Open(1); delay_ms(5); BUZZER_Close(1); }
}
