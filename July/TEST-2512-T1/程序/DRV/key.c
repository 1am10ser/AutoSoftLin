#include "common.h"



/* -------------------------------------------------- */
// �� KEY ���� 74HC165 ����
// �� ʹ��ǰ����ȶ� 74HC165 ���г�ʼ��
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� �ⲿ����
/* -------------------------------------------------- */

/**
 * 74HC165 �ⲿ����
 * ���±��������� 74hc165.c �ļ���
 */
extern short GV_HC165DataValues[];



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * Key ��������
 */

//��ϵͳʹ�á�KEY ����״̬���������鳤�ȹ̶����� 74HC165 ���������ݴ��룬�������ݰ�����˳��洢�������ݽŵ�ֵ��U16:D7~D0
short GV_KeyDataStates[] = {1, 1, 1, 1, 1, 1, 1, 1};  // 0 ��ʾ���£�1 ��ʾ�ɿ������ζ�Ӧ�İ�����ţ�J51, J50, J48, J47, J46, J45, J44, J43



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * Key ����װ��
 * ���ڲ��� 74HC165 ��ȡ���ݣ�������ʹ�� Key ������ع���ʱ����Ҫʹ�øú�����������װ��
 */
void KEY_DatLoad()
{
    HC165_ReadDataValues(16); 
    for( int i=0; i < ARR_LENGTH(GV_KeyDataStates); i++ ) GV_KeyDataStates[i] = GV_HC165DataValues[i];
}


/**
 * Key �Ƿ���
 *
 * @param number    PCB�ϵı��
 * @param autoload  �Զ�װ������
 *
 * @return  KEY_STATE_YES ��ʾ��ͨ�У�KEY_STATE_NO ��ʾ�Ͽ�
 */
short KEY_IsPress( short number, bool autoload )
{
    // �Զ�װ������
    if( autoload ) KEY_DatLoad();

    // ����ָ�� Key ��������ͨ״̬
    if( number == 43 ) return GV_KeyDataStates[7] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 44 ) return GV_KeyDataStates[6] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 45 ) return GV_KeyDataStates[5] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 46 ) return GV_KeyDataStates[4] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 47 ) return GV_KeyDataStates[3] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 48 ) return GV_KeyDataStates[2] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 50 ) return GV_KeyDataStates[1] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 51 ) return GV_KeyDataStates[0] ? KEY_STATE_NO : KEY_STATE_YES;

    // Ĭ�Ϸ���
    return KEY_STATE_NO;
}