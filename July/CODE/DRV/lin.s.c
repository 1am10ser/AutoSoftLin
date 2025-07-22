#include "common.h"



/* -------------------------------------------------- */
// �� LIN �ӻ�ģʽ
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� ȫ�ֱ���
// �� ������ص��ź����ݻ��� ����X3 ����������
/* -------------------------------------------------- */

/**
 * LIN ��������
 */
u32 GV_LinSlaveBaudRate   = 19200;         // ������
LinState GV_LinSlaveState = LinStateIdle;  // LIN ��ǰ״̬


/**
 * LIN ������ر���
 */
short GV_LinSlaveDebugSendOutUart1      = 0;  // Debug ���͵���������� UART1
short GV_LinSlaveDebugSendOutPrintf     = 0;  // Debug ���͵���������� printf��ע�⣺��ϵͳ�п������� printf ��������� UART1��
short GV_LinSlaveDebugReceiveOutUart1   = 0;  // Debug ���յ���������� UART1
short GV_LinSlaveDebugReceiveOutPrintf  = 0;  // Debug ���յ���������� printf��ע�⣺��ϵͳ�п������� printf ��������� UART1��


/**
 * LIN �ź���ر���
 */
u8 GV_LinSlaveSignal[]                     = {0x2E, 0x09, 0x00, 0x00, 0x00};  // ���� ON ��ʱ����Ĭ���Ϸ����źţ�ID �� DATA������ CheckNum�������й��ܴ��������ڸ��źŹ����Ϸ�����
u8 GV_LinSlaveSignalResponse[]             = {0x00, 0x00, 0x00, 0x00, 0x00};  // ��ϵͳʹ�á���Ӧ���źţ����鳤�Ⱥ� GV_LinSlaveSignal һ�£���Ҫ���ڹ�������ż��������ź�
u8 GV_LinSlaveSignalChecknumStart          = 1;                               // ��ϵͳʹ�á��ź�У������ʼ���������� GV_LinSlaveSignal ������

u8 GV_LinSlaveReceiveSignal[]              = {0x00, 0x00, 0x00, 0x00};  // ��ϵͳʹ�á������ź����������� ECU �·������ݣ�
u8 GV_LinSlaveReceiveSignalSurplus         = 0;                         // ��ϵͳʹ�á������ź�ʣ������
u8 GV_LinSlaveReceiveSignalBLedOpen[]      = {0xD3, 0x44, 0xFF, 0xBB};  // ���������á�ID, DATA, CheckNum��ECU �·��ı���������ź�
u8 GV_LinSlaveReceiveSignalWLockLedOpen[]  = {0x14, 0xFD, 0xFF, 0x02};  // ���������á�ID, DATA, CheckNum��ECU �·��ĳ������������ź�


/**
 * ��ť��ر���
 * Ԥ��������ܰ�ť��Ӧ�� PCB KEY ���
 */
short GV_LinSlaveKeyMapSignalLFU       = 7;   // ��ǰ��
short GV_LinSlaveKeyMapSignalLFUA      = 2;   // ��ǰһ��������Ҫ�����ǰ����������ʵ�֣�
short GV_LinSlaveKeyMapSignalLFD       = 8;   // ��ǰ��
short GV_LinSlaveKeyMapSignalLFDA      = 2;   // ��ǰһ��������Ҫ�����ǰ����������ʵ�֣�

short GV_LinSlaveKeyMapSignalLBU       = 4;   // �����
short GV_LinSlaveKeyMapSignalLBUA      = 2;   // ���һ��������Ҫ����������������ʵ�֣�
short GV_LinSlaveKeyMapSignalLBD       = 3;   // ���
short GV_LinSlaveKeyMapSignalLBDA      = 2;   // ���һ��������Ҫ�����󽵰�������ʵ�֣�

short GV_LinSlaveKeyMapSignalRFU       = 6;   // ��ǰ��
short GV_LinSlaveKeyMapSignalRFUA      = 2;   // ��ǰһ��������Ҫ�����ǰ����������ʵ�֣�
short GV_LinSlaveKeyMapSignalRFD       = 9;   // ��ǰ��
short GV_LinSlaveKeyMapSignalRFDA      = 2;   // ��ǰһ��������Ҫ�����ǰ����������ʵ�֣�

short GV_LinSlaveKeyMapSignalRBU       = 5;   // �Һ���
short GV_LinSlaveKeyMapSignalRBUA      = 2;   // �Һ�һ��������Ҫ����Һ�����������ʵ�֣�
short GV_LinSlaveKeyMapSignalRBD       = 10;  // �Һ�
short GV_LinSlaveKeyMapSignalRBDA      = 2;   // �Һ�һ��������Ҫ����Һ󽵰�������ʵ�֣�

short GV_LinSlaveKeyMapSignalMirrOC    = 0;   // ���Ӿ��շ�
short GV_LinSlaveKeyMapSignalMirrSWR   = 0;   // �л����Һ��Ӿ�
short GV_LinSlaveKeyMapSignalMirrSWL   = 0;   // �л�������Ӿ���Ĭ�ϣ�

short GV_LinSlaveKeyMapSignalMirrUp    = 0;   // ���Ӿ�����
short GV_LinSlaveKeyMapSignalMirrDown  = 0;   // ���Ӿ�����
short GV_LinSlaveKeyMapSignalMirrLeft  = 0;   // ���Ӿ�����
short GV_LinSlaveKeyMapSignalMirrRight = 0;   // ���Ӿ�����

short GV_LinSlaveKeyMapSignalWinLock   = 1;   // ������



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ������Ӧ���ź�
 * Ԥ��������ܰ�ť��Ӧ�������ź�
 */
void LIN_SLAVE_SetSignalResponseLFUp()      { LIN_SLAVE_SetSignalResponse(2,'L',3,1); }  // �źţ���ǰ��
void LIN_SLAVE_SetSignalResponseLFUpA()     { LIN_SLAVE_SetSignalResponse(2,'L',7,1); }  // �źţ���ǰһ����
void LIN_SLAVE_SetSignalResponseLFUpAuto()  { LIN_SLAVE_SetSignalResponse(2,'L',4,1); }  // �źţ���ǰ�� + ��ǰһ����

void LIN_SLAVE_SetSignalResponseLFDown()    { LIN_SLAVE_SetSignalResponse(2,'L',1,1); }  // �źţ���ǰ��
void LIN_SLAVE_SetSignalResponseLFDownA()   { LIN_SLAVE_SetSignalResponse(2,'L',7,1); }  // �źţ���ǰһ����
void LIN_SLAVE_SetSignalResponseLFDownAuto(){ LIN_SLAVE_SetSignalResponse(2,'L',2,1); }  // �źţ���ǰ�� + ��ǰһ����

void LIN_SLAVE_SetSignalResponseLBUp()      { LIN_SLAVE_SetSignalResponse(2,'H',1,1); LIN_SLAVE_SetSignalResponse(2,'L',8,1); }  // �źţ������
void LIN_SLAVE_SetSignalResponseLBUpA()     { LIN_SLAVE_SetSignalResponse(2,'H',3,1); LIN_SLAVE_SetSignalResponse(2,'L',8,1); }  // �źţ����һ����
void LIN_SLAVE_SetSignalResponseLBUpAuto()  { LIN_SLAVE_SetSignalResponse(2,'H',2,1); }                                          // �źţ������ + ���һ����

void LIN_SLAVE_SetSignalResponseLBDown()    { LIN_SLAVE_SetSignalResponse(2,'L',8,1); }                                          // �źţ����
void LIN_SLAVE_SetSignalResponseLBDownA()   { LIN_SLAVE_SetSignalResponse(2,'H',3,1); LIN_SLAVE_SetSignalResponse(2,'L',8,1); }  // �źţ����һ����
void LIN_SLAVE_SetSignalResponseLBDownAuto(){ LIN_SLAVE_SetSignalResponse(2,'H',1,1); }                                          // �źţ���� + ���һ����

void LIN_SLAVE_SetSignalResponseRFUp()      { LIN_SLAVE_SetSignalResponse(3,'L',3,1); }  // �źţ���ǰ��
void LIN_SLAVE_SetSignalResponseRFUpA()     { LIN_SLAVE_SetSignalResponse(3,'L',7,1); }  // �źţ���ǰһ����
void LIN_SLAVE_SetSignalResponseRFUpAuto()  { LIN_SLAVE_SetSignalResponse(3,'L',4,1); }  // �źţ���ǰ�� + ��ǰһ����

void LIN_SLAVE_SetSignalResponseRFDown()    { LIN_SLAVE_SetSignalResponse(3,'L',1,1); }  // �źţ���ǰ��
void LIN_SLAVE_SetSignalResponseRFDownA()   { LIN_SLAVE_SetSignalResponse(3,'L',7,1); }  // �źţ���ǰһ����
void LIN_SLAVE_SetSignalResponseRFDownAuto(){ LIN_SLAVE_SetSignalResponse(3,'L',2,1); }  // �źţ���ǰ�� + ��ǰһ����

void LIN_SLAVE_SetSignalResponseRBUp()      { LIN_SLAVE_SetSignalResponse(3,'H',1,1); LIN_SLAVE_SetSignalResponse(3,'L',8,1); }   // �źţ��Һ���
void LIN_SLAVE_SetSignalResponseRBUpA()     { LIN_SLAVE_SetSignalResponse(3,'H',3,1); LIN_SLAVE_SetSignalResponse(3,'L',8,1); }   // �źţ��Һ�һ����
void LIN_SLAVE_SetSignalResponseRBUpAuto()  { LIN_SLAVE_SetSignalResponse(3,'H',2,1); }                                           // �źţ��Һ��� + �Һ�һ����

void LIN_SLAVE_SetSignalResponseRBDown()    { LIN_SLAVE_SetSignalResponse(3,'L',8,1); }                                           // �źţ��Һ�
void LIN_SLAVE_SetSignalResponseRBDownA()   { LIN_SLAVE_SetSignalResponse(3,'H',3,1); LIN_SLAVE_SetSignalResponse(3,'L',8,1); }   // �źţ��Һ�һ����
void LIN_SLAVE_SetSignalResponseRBDownAuto(){ LIN_SLAVE_SetSignalResponse(3,'H',1,1); }                                           // �źţ��Һ� + �Һ�һ����

void LIN_SLAVE_SetSignalResponseMirrOC()    { LIN_SLAVE_SetSignalResponse(1,'L', 5,1); LIN_SLAVE_SetSignalResponse(1,'L',8,0); }  // �źţ����Ӿ��շ�
void LIN_SLAVE_SetSignalResponseMirrSWR()   { LIN_SLAVE_SetSignalResponse(1,'L',10,1); LIN_SLAVE_SetSignalResponse(1,'L',1,0); }  // �źţ��л����Һ��Ӿ�

void LIN_SLAVE_SetSignalResponseMirrUp()    { LIN_SLAVE_SetSignalResponse(1,'H',4,1); }  // �źţ����Ӿ�����
void LIN_SLAVE_SetSignalResponseMirrDown()  { LIN_SLAVE_SetSignalResponse(1,'H',8,1); }  // �źţ����Ӿ�����
void LIN_SLAVE_SetSignalResponseMirrLeft()  { LIN_SLAVE_SetSignalResponse(1,'H',1,1); }  // �źţ����Ӿ�����
void LIN_SLAVE_SetSignalResponseMirrRight() { LIN_SLAVE_SetSignalResponse(1,'H',2,1); }  // �źţ����Ӿ�����

void LIN_SLAVE_SetSignalResponseWinLock()   { LIN_SLAVE_SetSignalResponse(3,'H',4,1); }  // �źţ�������



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ������Ӧ���ź�
 * �ú���ֱ������ȫ�ֱ��� GV_LinSlaveSignalResponse
 * �ú�������λ���Ƿ������磺1H40(��ʾ��LIN�����ݶε�1�������ֽڵĸ�λ4����Ϊ0)
 * ����������������������������2L11(��ʾ��LIN�����ݶε�2�������ֽڵĵ�λ1����Ϊ1)
 *
 * @param dataNum           ���ݱ�ţ��� 1 ��ʼ
 * @param dataHightLow      ���ݸߵ�λ��H��L
 * @param data8421Bit       ����λ���� 8421 Ϊ��׼
 * @param data8421BitValue  ����λֵ��0��1
 */
void LIN_SLAVE_SetSignalResponse( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue )
{
    // �ع�����λ
    if( dataHighLow == 'h' ) data8421Bit = data8421Bit << 4;
    if( dataHighLow == 'H' ) data8421Bit = data8421Bit << 4;

    // ������Ӧ���ź�
    if( data8421BitValue == 1 ) GV_LinSlaveSignalResponse[dataNum] |= data8421Bit;
    if( data8421BitValue == 0 ) GV_LinSlaveSignalResponse[dataNum] &= data8421Bit^0xFF;
}


/**
 * ��ʼ����Ӧ���ź�
 * �ú���ֱ�ӳ�ʼ��ȫ�ֱ��� GV_LinSlaveSignalResponse
 */
void LIN_SLAVE_InitSignalResponse()
{
    for( int i=0; i < ARR_LENGTH(GV_LinSlaveSignal); i++ ) GV_LinSlaveSignalResponse[i] = GV_LinSlaveSignal[i];
}


/**
 * ������Ӧ���ź�
 * �ú���ֱ�ӷ���ȫ�ֱ��� GV_LinSlaveSignalResponse �������
 */
void LIN_SLAVE_SendSignalResponse()
{
    // ��ʼ����Ӧ�ź�
    LIN_SLAVE_InitSignalResponse();

    // ������Ӧ���ź�
    // ��� PCB KEY ����״̬��������Ӧ��Ӧ������
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalLFU,true) == KEY_STATE_YES ) KEY_IsPress(GV_LinSlaveKeyMapSignalLFUA,true) == KEY_STATE_YES ? LIN_SLAVE_SetSignalResponseLFUpAuto()   : LIN_SLAVE_SetSignalResponseLFUp();    // ��ǰ��/һ��
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalLFD,true) == KEY_STATE_YES ) KEY_IsPress(GV_LinSlaveKeyMapSignalLFDA,true) == KEY_STATE_YES ? LIN_SLAVE_SetSignalResponseLFDownAuto() : LIN_SLAVE_SetSignalResponseLFDown();  // ��ǰ��/һ��
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalLBU,true) == KEY_STATE_YES ) KEY_IsPress(GV_LinSlaveKeyMapSignalLBUA,true) == KEY_STATE_YES ? LIN_SLAVE_SetSignalResponseLBUpAuto()   : LIN_SLAVE_SetSignalResponseLBUp();    // �����/һ��
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalLBD,true) == KEY_STATE_YES ) KEY_IsPress(GV_LinSlaveKeyMapSignalLBDA,true) == KEY_STATE_YES ? LIN_SLAVE_SetSignalResponseLBDownAuto() : LIN_SLAVE_SetSignalResponseLBDown();  // ���/һ��
    
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalRFU,true) == KEY_STATE_YES ) KEY_IsPress(GV_LinSlaveKeyMapSignalRFUA,true) == KEY_STATE_YES ? LIN_SLAVE_SetSignalResponseRFUpAuto()   : LIN_SLAVE_SetSignalResponseRFUp();    // ��ǰ��/һ��
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalRFD,true) == KEY_STATE_YES ) KEY_IsPress(GV_LinSlaveKeyMapSignalRFDA,true) == KEY_STATE_YES ? LIN_SLAVE_SetSignalResponseRFDownAuto() : LIN_SLAVE_SetSignalResponseRFDown();  // ��ǰ��/һ��
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalRBU,true) == KEY_STATE_YES ) KEY_IsPress(GV_LinSlaveKeyMapSignalRBUA,true) == KEY_STATE_YES ? LIN_SLAVE_SetSignalResponseRBUpAuto()   : LIN_SLAVE_SetSignalResponseRBUp();    // �Һ���/һ��
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalRBD,true) == KEY_STATE_YES ) KEY_IsPress(GV_LinSlaveKeyMapSignalRBDA,true) == KEY_STATE_YES ? LIN_SLAVE_SetSignalResponseRBDownAuto() : LIN_SLAVE_SetSignalResponseRBDown();  // �Һ�/һ��

    if( KEY_IsPress(GV_LinSlaveKeyMapSignalMirrOC ,true) == KEY_STATE_YES ) LIN_SLAVE_SetSignalResponseMirrOC();     // ���Ӿ��շ�
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalMirrSWR,true) == KEY_STATE_YES ) LIN_SLAVE_SetSignalResponseMirrSWR();    // �л����Һ��Ӿ�
    
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalMirrUp   ,true) == KEY_STATE_YES ) LIN_SLAVE_SetSignalResponseMirrUp();     // ���Ӿ�����
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalMirrDown ,true) == KEY_STATE_YES ) LIN_SLAVE_SetSignalResponseMirrDown();   // ���Ӿ�����
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalMirrLeft ,true) == KEY_STATE_YES ) LIN_SLAVE_SetSignalResponseMirrLeft();   // ���Ӿ�����
    if( KEY_IsPress(GV_LinSlaveKeyMapSignalMirrRight,true) == KEY_STATE_YES ) LIN_SLAVE_SetSignalResponseMirrRight();  // ���Ӿ�����

    if( KEY_IsPress(GV_LinSlaveKeyMapSignalWinLock,true)   == KEY_STATE_YES ) LIN_SLAVE_SetSignalResponseWinLock();    // ������
    
    // ������Ӧ���ź�
    // ������ 1 ��ʼ����Ϊ���� GV_LinSlaveSignalResponse �ĵ�һ������Ϊ ID ������ LIN ��Ӧ����ʱ���跢�� ID ���ݣ�ֱ�Ӵ� DATA ���ݶο�ʼ���ͼ���
    for( int i = 1; i < ARR_LENGTH(GV_LinSlaveSignal); i++ ) UART3_SendByte(GV_LinSlaveSignalResponse[i]);
    UART3_SendByte(LIN_CalcChecknum(GV_LinSlaveSignalResponse,ARR_LENGTH(GV_LinSlaveSignal),GV_LinSlaveSignalChecknumStart));
    
    //��DEBUG�� ͨ�� UART1 ���������Ӧ���ź�
    if( GV_LinSlaveDebugSendOutUart1 == 1 ){
        for( int i = 0; i < ARR_LENGTH(GV_LinSlaveSignal); i++ ) UART1_SendByte(GV_LinSlaveSignalResponse[i]);
        UART1_SendByte(LIN_CalcChecknum(GV_LinSlaveSignalResponse,ARR_LENGTH(GV_LinSlaveSignal),GV_LinSlaveSignalChecknumStart));
    }
    
    //��DEBUG�� ͨ�� Printf ���������Ӧ���ź�
    if( GV_LinSlaveDebugSendOutPrintf == 1 ){
        for( int i = 0; i < ARR_LENGTH(GV_LinSlaveSignal); i++ ) printf("%02X ", GV_LinSlaveSignalResponse[i]);
        printf("%02X\n", LIN_CalcChecknum(GV_LinSlaveSignalResponse,ARR_LENGTH(GV_LinSlaveSignal),GV_LinSlaveSignalChecknumStart));
    }
}



/* -------------------------------------------------- */
// �� LIN ���ݽ���
// �� ���� LIN Э���ʱ��
/* -------------------------------------------------- */

/**
 * ����֡���ճ�ʼ
 * ��������֡�����õ�����ر���
 */
void LIN_SLAVE_ReceiveInit()
{
    GV_LinSlaveReceiveSignalSurplus = 0;
}


/**
 * ����֡���ս���
 * ��������֡�����õ�����ر���
 */
void LIN_SLAVE_ReceiveOver()
{
    GV_LinSlaveReceiveSignalSurplus = 0;
    GV_LinSlaveState = LinStateIdle;
}


/**
 * ����֡��������
 * ������������֡���յ���ر�����ͨ���ڽ��յ� Break ��������øú���
 */
void LIN_SLAVE_ReceiveStart()
{
    LIN_SLAVE_ReceiveInit();
    GV_LinSlaveState = LinStateBreakReceived;
}


/**
 * ����֡���մ���
 */
void LIN_SLAVE_ReceiveHandle( u8 dataByte )
{
    // ��ʼ��
    short i;
    short match;
    short length;
    
    //��DEBUG��������յ��ź�
    if( GV_LinSlaveDebugReceiveOutUart1  == 1 ) UART1_SendByte(dataByte);
    if( GV_LinSlaveDebugReceiveOutPrintf == 1 ) printf("%02X ", dataByte);


    // LIN ״̬�߼�����
    switch( GV_LinSlaveState ){
        /* -------------------------------------------------- */
        // LIN ״̬��Idle
        /* -------------------------------------------------- */
        case LinStateIdle :
            // ��ʼ
            LIN_SLAVE_ReceiveInit();
 
            break;
            

        /* -------------------------------------------------- */
        // LIN ״̬���յ������
        /* -------------------------------------------------- */
        case LinStateBreakReceived :
            // ��ʼ
            LIN_SLAVE_ReceiveInit();

            // �Զ�������һ��״̬
            GV_LinSlaveState = LinStateSynchFieldWait;

            break;


        /* ---------------------------------------------------- */
        // LIN ״̬���ȴ�ͬ���� 
        /* ---------------------------------------------------- */
        case LinStateSynchFieldWait :
            // �Զ�������һ��״̬
            GV_LinSlaveState = dataByte == 0x55 ? LinStateIdWait : LinStateIdle;
  
            break;


        /* ------------------------------------------------ */
        // LIN ״̬���ȴ���ʶ����
        /* ------------------------------------------------ */
        case LinStateIdWait :
            // ����ʱ��
            // LIN �ӻ����� 281 ���Լо�
            if( dataByte == 0x42 ){ LIN_SLAVE_SendSignalResponse(); GV_LinSlaveState = LinStateIdle; }

            // IDƥ�䣺������Ӧ
            if( dataByte == GV_LinSlaveSignal[0] ){ LIN_SLAVE_SendSignalResponse(); GV_LinSlaveState = LinStateIdle; }

            // IDƥ�䣺�������
            if( dataByte == GV_LinSlaveReceiveSignalBLedOpen[0] ){
                GV_LinSlaveReceiveSignal[0] = dataByte;
                GV_LinSlaveReceiveSignalSurplus = ARR_LENGTH(GV_LinSlaveReceiveSignalBLedOpen)-1;
                GV_LinSlaveState = LinStateDataRevice;
            }

            // IDƥ�䣺��������
            if( dataByte == GV_LinSlaveReceiveSignalWLockLedOpen[0] ){
                GV_LinSlaveReceiveSignal[0] = dataByte;
                GV_LinSlaveReceiveSignalSurplus = ARR_LENGTH(GV_LinSlaveReceiveSignalWLockLedOpen)-1;
                GV_LinSlaveState = LinStateDataRevice;
            }

            break;

            
        /* ------------------------------------------------ */
        // LIN ״̬�����ݽ���
        /* ------------------------------------------------ */
        case LinStateDataRevice :
            // �������
            // �Զ�������һ��
            if( GV_LinSlaveReceiveSignalSurplus == 0 ){ LIN_SLAVE_ReceiveOver(); break; }

            // �������ݣ��������
            if( GV_LinSlaveReceiveSignal[0] == GV_LinSlaveReceiveSignalBLedOpen[0] ){
                length = ARR_LENGTH(GV_LinSlaveReceiveSignalBLedOpen)-GV_LinSlaveReceiveSignalSurplus;
                GV_LinSlaveReceiveSignal[length] = dataByte;
            }
            
            // �������ݣ���������
            if( GV_LinSlaveReceiveSignal[0] == GV_LinSlaveReceiveSignalWLockLedOpen[0] ){
                length = ARR_LENGTH(GV_LinSlaveReceiveSignalWLockLedOpen)-GV_LinSlaveReceiveSignalSurplus;
                GV_LinSlaveReceiveSignal[length] = dataByte;
            }

            // �������
            // �Զ�������һ��
            if( --GV_LinSlaveReceiveSignalSurplus == 0 ){
                // ���������Ӧ���������
                if( GV_LinSlaveReceiveSignal[0] == GV_LinSlaveReceiveSignalBLedOpen[0] ){
                    length = ARR_LENGTH(GV_LinSlaveReceiveSignalBLedOpen);
                    for( i = 0, match = 1; i < length; i++ ) match = match && (GV_LinSlaveReceiveSignal[i] == GV_LinSlaveReceiveSignalBLedOpen[i]);
                    //match ? BUZZER_Open(1) : BUZZER_Close(1);
                }
                
                // ���������Ӧ����������
                if( GV_LinSlaveReceiveSignal[0] == GV_LinSlaveReceiveSignalWLockLedOpen[0] ){
                    length = ARR_LENGTH(GV_LinSlaveReceiveSignalWLockLedOpen);
                    for( i = 0, match = 1; i < length; i++ ) match = match && (GV_LinSlaveReceiveSignal[i] == GV_LinSlaveReceiveSignalWLockLedOpen[i]);
                    //match ? LED_Open(2) : LED_Close(2);
                }

                // ���ݽ��ս���
                LIN_SLAVE_ReceiveOver();
            }

            break;
    }
}
