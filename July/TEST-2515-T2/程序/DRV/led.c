#include "common.h"



/* -------------------------------------------------- */
// �� LED ���� 74HC595 ����
// �� ʹ��ǰ����ȶ� 74HC595 ���г�ʼ��
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * LED ��������
 */

//���������á�LED �Ʊ�ż���ֻ�н� LED �Ʊ�ż��뵽������������ʵ�ֶ�Ӧ LED �Ƶ���ع���
short GV_LedNumbers[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};   

//��ϵͳʹ�á�LED ��ʼ����״̬���������鳤�ȹ̶��������յ����͸� 74HC595 �����������������ݰ������±�����˳��ӳ�䵽��U2:Q0~Q7��U3:Q0~Q7��U4:Q0~Q7��U5:Q0~Q7
//��ϵͳ��ֵ��LED �Ʊ��ӳ���״̬�������������ڵƱ�ź͵�״̬û��ֱ�ӵĶ�Ӧ��ϵ������ͨ���ñ������й���������������+1����Ϊ LED �Ʊ��
short GV_LedStates[] = {0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0};  
const short GV_LedNumMapStateIndexs[] = {0,1,2,3,6,7,4,5,  8,9,10,11,12,13,14,15,  16,17,18,19,20,21,22,23,  24,25,26,27,28,29,30,31};  



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * LED �Ƴ�ʼ��
 */
void LED_Init( bool render )
{
    // ��ʼ�� LED ״̬
    for( int i=0; i < ARR_LENGTH(GV_LedStates); i++ ) GV_LedStates[i] = 0;
    if( render ) LED_Render();
}


/**
 * LED �Ƶ���
 *
 * @param short number  PCB�ϵı��
 * @param bool  render  �Ƿ���Ⱦ
 */
void LED_Open( short number, bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) if( GV_LedNumbers[i] == number ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = 1;
    if( render ) LED_Render();
}


/**
 * LED ��ȫ����
 *
 * @param bool render  �Ƿ���Ⱦ
 */
void LED_OpenAll( bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = 1;
    if( render ) LED_Render();
}


/**
 * LED ��Ϩ��
 *
 * @param short number  PCB�ϵı��
 * @param bool  render  �Ƿ���Ⱦ
 */
void LED_Close( short number, bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) if( GV_LedNumbers[i] == number ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = 0;
    if( render ) LED_Render();
}


/**
 * LED ��ȫϨ��
 *
 * @param bool render  �Ƿ���Ⱦ
 */
void LED_CloseAll( bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = 0;
    if( render ) LED_Render();
}


/**
 * LED �Ƶ�������Ϩ���л�
 *
 * @param short number  PCB�ϵı��
 * @param bool  render  �Ƿ���Ⱦ
 */
void LED_Reverse( short number, bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) if( GV_LedNumbers[i] == number ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] ? 0 : 1;
    if( render ) LED_Render();
}


/**
 * LED �Ƶ�������Ϩ��ȫ�л�
 *
 * @param bool render  �Ƿ���Ⱦ
 */
void LED_ReverseAll( bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] ? 0 : 1;
    if( render ) LED_Render();
}


/**
 * LED ��Ⱦ
 * ���� LED ���� 74HC595 ���ƣ�����ͨ��ȫ�ֱ��� GV_LedStates ��¼���Ƶ�״̬��Ȼ��һ���Է������ݵ�����Ϩ��
 */
void LED_Render()
{
    // ��ʼ��
    u8 byte;
    int i, ii;

    // �������ݲ��������ݸ� 74HC595
    for( i=(ARR_LENGTH(GV_LedStates)-1)/8; i >= 0; i-- ){
        byte = 0;
        
        for( ii = 0; ii < 8; ii++ ){
            byte <<= 1;
            byte |= GV_LedStates[i*8+ii];
        }
        
        // ���� LED �� 74HC595 ���Ϊ 2�����Բο�ԭ��ͼ
        HC595_SendByte(2, byte);
    }
    
    // 74HC595 ���
    HC595_SendOutput(2);
}



/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */

/**
 * ���߼��������
 *
 * @param short delay  �ӳ�ʱ�䣨���룩 
 */
void LED_MainHorseLamp( short delay )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ){
        LED_Open(GV_LedNumbers[i], true);
        delay_ms(delay);
    }
}
