#ifndef __LIN_H_
#define __LIN_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    #define LIN_MODE_SLAVE     1
    #define LIN_MODE_MASTER    2

    #define LIN_TXUSB_MODE_DEFAULT   1  // �ź�ת����USB��ģʽ������Ϸ��ͣ�Ĭ�ϣ�
    #define LIN_TXUSB_MODE_DOWNONE   2  // �ź�ת����USB��ģʽ�������·�һ��
    #define LIN_TXUSB_MODE_DOWNUPONE 3  // �ź�ת����USB��ģʽ�������º��ɿ���һ��



    /* -------------------------------------------------- */
    // �� �������Ͷ���
    /* -------------------------------------------------- */

    /**
     * LIN ״̬
     * �ñ���ָ���˵�ǰ LIN �����ڵ�״̬ö��
     */
    typedef enum{
        LinStateIdle,           // ����0������
        LinStateIdWait,         // ����3���ȴ���ʶ��
        LinStateBreakReceived,  // ����1���յ������
        LinStateSynchFieldWait, // ����2���ȴ�ͬ����
        LinStateDataRevice      // ����4�����ݽ���
    }LinState;



    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void LIN_Init();
    u8 LIN_CalcChecknum( u8 *bytes, short length, short start );

    
      
#endif