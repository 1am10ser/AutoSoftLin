#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * 74HC164 ��������
 */
short GV_HC164Numbers[] = {6};   //���������á�74HC164 ��ż���ֻ�н� 74HC164 ��ż��뵽������������ʵ�ֶ�Ӧ 74HC164 ����ع���



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ��ȡ 74HC164 ��ӦоƬ�ϵ���źţ����ݽţ�
 *
 * @param short number  PCB �ϵ� 74HC164 ���
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC164_GetDataPin( short number )
{
    // �������
    if( number == 6 ) return GPIO_PIN_1;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ 74HC164 ��ӦоƬ�ϵĶ˿ںţ����ݽţ�
 *
 * @param short number  PCB �ϵ� 74HC164 ���
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* HC164_GetDataPort( short number )
{
    // ����˿�
    if( number == 6 ) return GPIOA;

    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}


/**
 * ��ȡ 74HC164 ��ӦоƬ�ϵ���źţ�ʱ�ӽţ�
 *
 * @param short number  PCB �ϵ� 74HC164 ���
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC164_GetClockPin( short number )
{
    // �������
    if( number == 6 ) return GPIO_PIN_2;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ 74HC164 ��ӦоƬ�ϵĶ˿ںţ�ʱ�ӽţ�
 *
 * @param short number  PCB �ϵ� 74HC164 ���
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* HC164_GetClockPort( short number )
{
    // ����˿�
    if( number == 6 ) return GPIOA;

    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}


/**
 * 74HC164 ��ʼ��
 */
void HC164_Init()
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

    // ��ʼ�� 74HC164 ��Ӧ�� GPIO ���
    for( int i=0; i < ARR_LENGTH(GV_HC164Numbers); i++ ){
        // ���ݽ�
        if( HC164_GetDataPort(GV_HC164Numbers[i]) == 0 ) continue;
        if( HC164_GetDataPin(GV_HC164Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC164_GetDataPort(GV_HC164Numbers[i]), HC164_GetDataPin(GV_HC164Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
        
        // ʱ�ӽ�
        if( HC164_GetClockPort(GV_HC164Numbers[i]) == 0 ) continue;
        if( HC164_GetClockPin(GV_HC164Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC164_GetClockPort(GV_HC164Numbers[i]), HC164_GetClockPin(GV_HC164Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
    }
}


/**
 * 74HC164 ��������
 * �ú���Ϊ 74HC164 �����ֽڵĳ���,�ó������ȷ��͵������λ
 * ��ע������һ���ֽڣ�����������λ���ս���Q7�������������������λ����Q0�����
 *
 * ��������Ϊ����1��+��2�źϲ�Ϊ�������ţ�����9��Ϊ��λ��ͨ��ֱ�ӽ���VCC��
 * ���ж�Ƭ 74HC164 ����ʱ��ֻ�ж�ε��ø÷��ͳ����ȷ����ֽڽ����Զ��������һ�� 74HC164
 */
void HC164_SendByte( short number, u8 byte )
{
    // ��ʼ��
    u8 i, dat;
    
    // ��ʼ��
    GPIO_TypeDef* dataPort = HC164_GetDataPort(number);
    if( dataPort == 0 ) return;

    GPIO_Pin_TypeDef dataPin = HC164_GetDataPin(number);
    if( dataPin == GPIO_PIN_ALL ) return;

    GPIO_TypeDef* clockPort = HC164_GetClockPort(number);
    if( clockPort == 0 ) return;

    GPIO_Pin_TypeDef clockPin = HC164_GetClockPin(number);
    if( clockPin == GPIO_PIN_ALL ) return;

    // �����ֽ�����
    for( i = 0; i < 8; i++ ){
        // ȡ�ֽ����λ
        dat = byte & 0x01; 
    
        // �����ֽ�λ
        // ʱ�ӽ�������һ�Σ�74HC164 ��������ݽŶ�ȡһλ����
        GPIO_WriteLow(clockPort, clockPin);
        dat ? GPIO_WriteHigh(dataPort, dataPin) : GPIO_WriteLow(dataPort, dataPin);
        GPIO_WriteHigh(clockPort, clockPin);

        // �����͵��ֽ�����1λ
        byte >>= 1;
    }
}

