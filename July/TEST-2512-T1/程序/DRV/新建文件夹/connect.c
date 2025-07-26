#include "common.h"



/* -------------------------------------------------- */
// �� CONNECT ���� 74HC164 �� 74HC165 ����
// �� ʹ��ǰ����ȶ� 74HC164 �� 74HC165 ���г�ʼ��
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� �ⲿ����
/* -------------------------------------------------- */

/**
 * 74HC165 �ⲿ����
 * ���±��������� 74hc165.c �ļ���
 */
//extern short GV_HC165DataValues[];



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * CONNECT ��������
 */

//���������á�Connect ��ż���ֻ�н� Connect ��ż��뵽������������ʵ�ֶ�Ӧ Connect ����ع���
//short GV_ConnectNumbers[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};   
short GV_ConnectNumbers[] = {1,2,3,4,5,6,7,8,9,10};   

//��ϵͳʹ�á�Connect ����״̬���������鳤�ȹ̶����� 74HC165 ���������ݴ��룬�������ݰ�����˳��洢�������ݽŵ�ֵ��U10:D7~D0��U11:D7~D0��U12:D7~D0��U13:D7~D0
//��ϵͳ��ֵ��Connect ���ӳ�����ݼ������������� Connect ��ź����ݽ�û��ֱ�ӵĶ�Ӧ��ϵ������ͨ���ñ������й���������������+1����Ϊ Connect ���
short GV_ConnectDataStates[] = {0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0};  
const short GV_ConnectNumMapDataStateIndexs[] = {0,1,2,3,7,6,5,4,  11,10,9,8,15,14,13,12,  19,18,17,16,23,22,21,20,  27,26,25,24,31,30,29,28};  

//��ϵͳʹ�á�Connect ����״̬���������鳤�ȹ̶��������յ����͸� 74HC164 �����������������ݰ�˳��ӳ�䵽��U6:Q0~Q7��U7:Q0~Q7��U8:Q0~Q7��U9:Q0~Q7
//��ϵͳ��ֵ��Connect ���ӳ��״̬�������������� Connect ��źͿ��ƽ�û��ֱ�ӵĶ�Ӧ��ϵ������ͨ���ñ������й���������������+1����Ϊ Connect ���
short GV_ConnectControlStates[] = {0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0};  
const short GV_ConnectNumMapControlStateIndexs[] = {0,1,2,3,7,6,5,4,  8,9,10,11,15,14,13,12,  16,17,18,19,23,22,21,20,  24,25,26,27,28,29,30,31};  

// ��ϵͳʹ�á�Connect ����״̬������
short GV_ConnectControlStatesCache[] = {-1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1};  



///* -------------------------------------------------- */
//// �� ��������
///* -------------------------------------------------- */
//
///**
// * Connect ������ͨ
// * ע�⣺ʹ�øú����򿪿��ƿ��غ�ǵõ��� CONNECT_CtlRender() ������Ⱦ
// *
// * @param short number  PCB�ϵı��
// * @param bool  render  �Ƿ���Ⱦ
// */
//void CONNECT_CtlOpen( short number, bool render )
//{
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ) if( GV_ConnectNumbers[i] == number ) GV_ConnectControlStates[GV_ConnectNumMapControlStateIndexs[GV_ConnectNumbers[i]-1]] = 1;
//    if( render ) CONNECT_CtlRender();
//}
//
//
///**
// * Connect ����ȫ��ͨ
// * ע�⣺ʹ�øú����򿪿��ƿ��غ�ǵõ��� CONNECT_CtlRender() ������Ⱦ
// *
// * @param bool  render  �Ƿ���Ⱦ
// */
//void CONNECT_CtlOpenAll( bool render )
//{
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ) CONNECT_CtlOpen(GV_ConnectNumbers[i], false);
//    if( render ) CONNECT_CtlRender();
//}
//
//
///**
// * Connect ���ƶϿ�
// * ע�⣺ʹ�øú����򿪿��ƿ��غ�ǵõ��� CONNECT_CtlRender() ������Ⱦ
// *
// * @param short number  PCB�ϵı��
// * @param bool  render  �Ƿ���Ⱦ
// */
//void CONNECT_CtlClose( short number, bool render )
//{
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ) if( GV_ConnectNumbers[i] == number ) GV_ConnectControlStates[GV_ConnectNumMapControlStateIndexs[GV_ConnectNumbers[i]-1]] = 0;
//    if( render ) CONNECT_CtlRender();
//}
//
//
///**
// * Connect ����ȫ�Ͽ�
// * ע�⣺ʹ�øú����򿪿��ƿ��غ�ǵõ��� CONNECT_CtlRender() ������Ⱦ
// */
//void CONNECT_CtlCloseAll( bool render )
//{
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ) CONNECT_CtlClose(GV_ConnectNumbers[i], false);
//    if( render ) CONNECT_CtlRender();
//}
//
//
///**
// * Connect ������Ⱦ
// * ���� Connect ���� ���� 74HC164 ���ƣ�����ͨ��ȫ�ֱ��� GV_ConnectControlStates ��¼�����Ƶ��״̬��Ȼ��һ���Է������ݴ򿪻�ر� Connect �Ŀ��ƿ���
// */
//void CONNECT_CtlRender()
//{
//    // ��ʼ��
//    int i, ii;
//    u8 byte, same = 1;
// 
//    // ��ֹ�ظ�����
//    for( i = 0; i < ARR_LENGTH(GV_ConnectControlStates); i++ ) same &= GV_ConnectControlStates[i] == GV_ConnectControlStatesCache[i];
//    if( same ) return;
//
//    // �������ݲ��������ݸ� 74HC164
//    for( i=(ARR_LENGTH(GV_ConnectControlStates)-1)/8; i >= 0; i-- ){
//        byte = 0;
//        
//        for( ii = 0; ii < 8; ii++ ){
//            byte <<= 1;
//            byte |= GV_ConnectControlStates[i*8+ii];
//        }
//        
//        // ���� Connect �� 74HC164 ���Ϊ 6�����Բο�ԭ��ͼ
//        HC164_SendByte(6, byte);
//    }
//        
//    // ����
//    for( i = 0; i < ARR_LENGTH(GV_ConnectControlStates); i++ ) GV_ConnectControlStatesCache[i] = GV_ConnectControlStates[i];
//}
//
//
///**
// * Connect ����װ��
// * ���ڲ��� 74HC165 ��ȡ���ݣ�������ʹ�� Connect ������ع���ʱ����Ҫʹ�øú�����������װ��
// */
//void CONNECT_DatLoad()
//{
//    HC165_ReadDataValues(10); 
//    for( int i=0; i < ARR_LENGTH(GV_ConnectDataStates); i++ ) GV_ConnectDataStates[i] = GV_HC165DataValues[i];
//}
//
//
///**
// * Connect �����Ƿ���ͨ
// * ע�⣺ʹ��ʱ�ǵ�ͨ������ CONNECT_CtlOpen �򿪻�ر� Connect ��Ŷ�Ӧ�Ŀ��ƿ���
// * ע�⣺ʹ�øú����ж� Connect �Ƿ���ͨǰ�ǵõ��� CONNECT_DatLoad() ��������װ��
// *
// * @param number    PCB�ϵı��
// * @param autoload  �Զ�װ������
// *
// * @return  CONNECT_STATE_YES ��ʾ��ͨ�У�CONNECT_STATE_NO ��ʾ�Ͽ�
// */
//short CONNECT_DatIsConnect( short number, bool autoload )
//{
//    // �Զ�װ������
//    if( autoload ) CONNECT_DatLoad();
//    
//    // ����ָ�� Connect ��������ͨ״̬
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ) if( GV_ConnectNumbers[i] == number ) return GV_ConnectDataStates[GV_ConnectNumMapDataStateIndexs[GV_ConnectNumbers[i]-1]] ? CONNECT_STATE_NO : CONNECT_STATE_YES;
//
//    // Ĭ�Ϸ���
//    return CONNECT_STATE_NO;
//}
//
//
///**
// * Connect �����Ƿ���ͨ������·��ͨ��
// * ָ��·��ͨ������·ȫ���Ͽ�����ô���� CONNECT_STATE_YES�����򷵻� CONNECT_STATE_NO��
// *
// * ע�⣺�ú�������ȥ�ж� Connect ���ƿ����Ƿ�ʹ�ܣ�ֱ�Ӷ� Connect ���ݽ����ж���ʹ��ǰ���а������ Connect ���ƿ���
// * ע�⣺ʹ�øú����ж� Connect �Ƿ���ͨǰ�ǵõ��� CONNECT_DatLoad() ��������װ��
// *
// * @param number    PCB�ϵı��
// * @param autoload  �Զ�װ������
// *
// * @return  CONNECT_STATE_YES ��ʾ��ͨ�У�CONNECT_STATE_NO ��ʾ�Ͽ�
// */
//short CONNECT_DatIsConnectOnly( short number, bool autoload )
//{
//    // ��ʼ��
//    short connectState;
//    short connectStateSelf   = 0;
//    short connectStateOthers = 0;
//    
//    // �Զ�װ������
//    if( autoload ) CONNECT_DatLoad();
//
//    // ���������״̬����
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ){
//        connectState = CONNECT_DatIsConnect(GV_ConnectNumbers[i], false);
//        if( connectState == CONNECT_STATE_INVALID ) return CONNECT_STATE_INVALID;
//        
//        if( number == GV_ConnectNumbers[i] ) connectStateSelf = connectState == CONNECT_STATE_YES ? 1 : 0;
//        else connectStateOthers |= connectState == CONNECT_STATE_YES ? 1 : 0;
//    }
//    
//    // ����
//    return connectStateSelf == 1 && connectStateOthers == 0 ? CONNECT_STATE_YES : CONNECT_STATE_NO;
//}
//
//
///**
// * Connect �����Ƿ���ͨ���κ�һ·��
// * �κ�ĳһ·��ͨ����ô���� CONNECT_STATE_YES�����򷵻� CONNECT_STATE_NO��
// *
// * ע�⣺�ú�������ȥ�ж� Connect ���ƿ����Ƿ�ʹ�ܣ�����ֱ�Ӷ� Connect ���ݽ����ж���ʹ��ǰ���а������ Connect ���ƿ���
// * ע�⣺ʹ�øú����ж� Connect �Ƿ���ͨǰ�ǵõ��� CONNECT_DatLoad() ��������װ��
// *
// * @param autoload  �Զ�װ������
// * @return  CONNECT_STATE_YES ��ʾ��ͨ�У�CONNECT_STATE_NO ��ʾ�Ͽ�
// */
//short CONNECT_DatIsConnectAny( bool autoload )
//{
//    // �Զ�װ������
//    if( autoload ) CONNECT_DatLoad();
//    
//    // �������
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ){
//        if( CONNECT_DatIsConnect(GV_ConnectNumbers[i],false) == CONNECT_STATE_YES ) return CONNECT_STATE_YES;
//    }
//    
//    // ����δ��ͨ
//    return CONNECT_STATE_NO;
//}
//
//
//
///* -------------------------------------------------- */
//// �� ���߼�����
///* -------------------------------------------------- */
//
///**
// * Connect ���߼�����ͨ������
// * Connect ��ŵ���ͨ����ô������ͬ��� LED ��
// *
// * �ú���������Һ�����Լ�Һ����LED������ʹ��ǰ��Ҫ��Һ�����ͳ������г�ʼ����LCD_MainInit(); LCD_MainLedScene();
// */
//void CONNECT_MainConnectMapLed()
//{
//    // ����װ��
//    CONNECT_DatLoad();
//    
//    // ����ȫ������
//    CONNECT_CtlOpenAll(true);
//
//    // ������ͨ��ͬʱ���������Ӧ��ŵĵ�
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ){
//        if( CONNECT_DatIsConnect(GV_ConnectNumbers[i],false) == CONNECT_STATE_YES ){
//            LCD_MainLedSceneOpen(GV_ConnectNumbers[i]);
//        }else{
//            LCD_MainLedSceneClose(GV_ConnectNumbers[i]);
//        }
//    }
//}
//
//
///**
// * Connect ���߼�����ͨ�����Ʋ������·
// * Connect ��ŵ���ͨ����ô������ͬ��� LED �ƣ������·��ô������
// *
// * ע�⣺������ȫ�ֱ��� GV_ConnectNumbers ��ֵ��δ���õ� Connect �����Ҫɾ����������ﲻ��Ԥ��Ч��
// * �ú���������Һ�����Լ�Һ����LED������ʹ��ǰ��Ҫ��Һ�����ͳ������г�ʼ����LCD_MainInit(); LCD_MainLedScene();
// */
//void CONNECT_MainConnectMapLedFilterShort()
//{
//    // ������ͨ��ͬʱ���������Ӧ��ŵĵ�
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ){  
//        // �ر�ȫ������
//        CONNECT_CtlCloseAll(false);
//
//        // ����
//        CONNECT_CtlOpen(GV_ConnectNumbers[i], true);
//
//        // Connect ��·��Ⲣ����
//        if( CONNECT_DatIsConnectOnly(GV_ConnectNumbers[i],true) == CONNECT_STATE_YES ){
//            LCD_MainLedSceneOpen(GV_ConnectNumbers[i]);
//        }else{
//            LCD_MainLedSceneClose(GV_ConnectNumbers[i]);
//        }
//    }
//}
//
//
///**
// * Connect ��ʾ����һ·��ͨ��ô����������
// * ע�⣺������ȫ�ֱ��� GV_ConnectNumbers ��ֵ��δ���õ� Connect �����Ҫɾ����������ﲻ��Ԥ��Ч��
// */
//void CONNECT_MainConnectAnyMapBuzzer()
//{
//    // �ر�ȫ������
//    CONNECT_CtlOpenAll(true);
//
//    // �ж���һ��ͨ
////    if( CONNECT_DatIsConnectAny(true) == CONNECT_STATE_YES ){
////        BUZZER_Open(1);
////    }else{
////        BUZZER_Close(1);
////    }
//}


