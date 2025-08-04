#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * LIN ��������
 */
u32 GV_LinMode = LIN_MODE_MASTER;  // LIN ģʽ��LIN_MODE_MASTER �� LIN_MODE_SLAVE



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * LIN ��ʼ��
 * ��ʼ�� LIN ��ص����ݣ�оƬ�� LIN ���ڵĳ�ʼ���ο� uart3.c �еĺ���
 */
void LIN_Init()
{
    // LIN ������س�ʼ��
    if( GV_LinMode == LIN_MODE_MASTER ){
        LIN_MASTER_InitAuxSignal();  // ��ʼ���·��ĸ��ź�
    }
    
    // LIN �ӻ���س�ʼ��
    if( GV_LinMode == LIN_MODE_SLAVE ){
        // TODO...
    }
}


/**
 * LIN ����У����
 *
 * @param data    ����ָ��
 * @param length  �������鳤��
 * @param start   ���ݼ������ʼ����
 *
 * @return u8     �����ֽ�
 */
u8 LIN_CalcChecknum( u8 *bytes, short length, short start )
{
    // ��ʼ��
    short checknum = 0;

    // ����У���벢����
    for( short i = start; i < length; i++ ) checknum += bytes[i];
    return (u8)(checknum%0xFF)^0xFF;
}