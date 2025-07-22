#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * ��������ر���
 */

//���������á������� ��ż���ֻ�н� ������ ��ż��뵽������������ʵ�ֶ�Ӧ��������ع���
short GV_BuzzerNumbers[] = {1};



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ��ȡ��������ӦоƬ�ϵ���ź�
 *
 * @param short number  PCB �ϵķ��������
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef BUZZER_GetPin( short number )
{
    // �������
    if( number == 1 ) return GPIO_PIN_1;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ��������ӦоƬ�ϵĶ˿ں�
 *
 * @param short number  PCB �ϵķ��������
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* BUZZER_GetPort( short number )
{
    // ����˿�
    if( number == 1 ) return GPIOD;

    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}

    
/**
 * ��������ʼ��
 */
void BUZZER_Init()
{  
    // GPIO_MODE_OUT_OD_LOW_SLOW   ���ٿ�©����͵�ƽ
    // GPIO_MODE_OUT_PP_LOW_SLOW   ������������͵�ƽ
    // GPIO_MODE_OUT_OD_HIZ_SLOW   ���ٿ�©�������̬
    // GPIO_MODE_OUT_PP_HIGH_SLOW  ������������ߵ�ƽ
    
    // �����������Ŷ�Ӧ��оƬ�ڲ����� һ��NPN������(���缫C���ڽ�V+) �� һ��PNP������(���缫C���ڽ�V-)
    // �������ƣ�������NPN�����ܲ��ӷ����E����Ϊ����ڣ�V+��
    // �������죺������PNP�����ܲ��Ӽ��缫C����Ϊ����ڣ�V-��
    // ��������Ĵ���ԭ��Ӧ����ˣ�STMоƬ�������û�� V- ����ѹ���� 0V�����Ե͵�ƽ���� 0V�����������
    // �����������ֱ������ߵ�ƽ���������������͵�Դ�Ϳ�����������
    
    // ��©�������Ŷ�Ӧ��оƬ�ڲ�����һ��NPN�����ܣ����缫C��ʲô�����ӣ�ֱ����Ϊ����˿�
    // ��©���ֻ������͵�ƽ�͸���̬�����Ա��������������͵�Դ��������ߵ�ƽȥ�������أ���ѹ����ӵ�Դ����
  
    // ��ʼ����������Ӧ�� GPIO ���
    for( int i=0; i < ARR_LENGTH(GV_BuzzerNumbers); i++ ){
        if( BUZZER_GetPort(GV_BuzzerNumbers[i]) == 0 ) continue;
        if( BUZZER_GetPin(GV_BuzzerNumbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(BUZZER_GetPort(GV_BuzzerNumbers[i]), BUZZER_GetPin(GV_BuzzerNumbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
    }
}


/**
 * ����������
 *
 * @param number  PCB�ϵı��
 */
void BUZZER_Open( short number )
{
    // ��ʼ��
    GPIO_TypeDef* GPIO_Port = BUZZER_GetPort(number);
    if( GPIO_Port == 0 ) return;

    GPIO_Pin_TypeDef GPIO_Pin = BUZZER_GetPin(number);
    if( GPIO_Pin == GPIO_PIN_ALL ) return;

    // ����������
    GPIO_WriteHigh(GPIO_Port, GPIO_Pin);
}


/**
 * ������ȫ����
 */
void BUZZER_OpenAll()
{
    for( int i=0; i < ARR_LENGTH(GV_BuzzerNumbers); i++ ) BUZZER_Open(GV_BuzzerNumbers[i]);
}


/**
 * ����������
 *
 * @param number  PCB�ϵı��
 */
void BUZZER_Close( short number )
{
    // ��ʼ��
    GPIO_TypeDef* GPIO_Port = BUZZER_GetPort(number);
    if( GPIO_Port == 0 ) return;

    GPIO_Pin_TypeDef GPIO_Pin = BUZZER_GetPin(number);
    if( GPIO_Pin == GPIO_PIN_ALL ) return;

    // ����������
    GPIO_WriteLow(GPIO_Port, GPIO_Pin);
}


/**
 * ������ȫ����
 */
void BUZZER_CloseAll()
{
    for( int i=0; i < ARR_LENGTH(GV_BuzzerNumbers); i++ ) BUZZER_Close(GV_BuzzerNumbers[i]);
}


/**
 * �������������߾����л�
 *
 * @param number  PCB�ϵı��
 */
void BUZZER_Reverse( short number )
{
    // ��ʼ��
    GPIO_TypeDef* GPIO_Port = BUZZER_GetPort(number);
    if( GPIO_Port == 0 ) return;

    GPIO_Pin_TypeDef GPIO_Pin = BUZZER_GetPin(number);
    if( GPIO_Pin == GPIO_PIN_ALL ) return;

    // �л�������
    GPIO_WriteReverse(GPIO_Port, GPIO_Pin);
}


/**
 * ��������������Ϩ��ȫ�л�
 */
void BUZZER_ReverseAll()
{
    for( int i=0; i < ARR_LENGTH(GV_BuzzerNumbers); i++ ) BUZZER_Reverse(GV_BuzzerNumbers[i]);
}