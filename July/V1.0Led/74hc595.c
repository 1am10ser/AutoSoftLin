#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * 74HC595 ��������
 */

//���������á�74HC595 ��ż���ֻ�н� 74HC595 ��ż��뵽������������ʵ�ֶ�Ӧ 74HC595 ����ع���
short GV_HC595Numbers[] = {2};



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ��ȡ 74HC595 ��ӦоƬ�ϵ���źţ����ݽţ�
 *
 * @param short number  PCB �ϵ� 74HC595 ���
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC595_GetDataPin( short number )
{
    // �������
    if( number == 2 ) return GPIO_PIN_3;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ 74HC595 ��ӦоƬ�ϵĶ˿ںţ����ݽţ�
 *
 * @param short number  PCB �ϵ� 74HC595 ���
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* HC595_GetDataPort( short number )
{
    // ����˿�
    if( number == 2 ) return GPIOA;

    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}


/**
 * ��ȡ 74HC595 ��ӦоƬ�ϵ���źţ���λʱ�ӽţ�
 *
 * @param short number  PCB �ϵ� 74HC595 ���
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC595_GetBitClockPin( short number )
{
    // �������
    if( number == 2 ) return GPIO_PIN_6;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ 74HC595 ��ӦоƬ�ϵĶ˿ںţ���λʱ�ӽţ�
 *
 * @param short number  PCB �ϵ� 74HC595 ���
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* HC595_GetBitClockPort( short number )
{
    // ����˿�
    if( number == 2 ) return GPIOA;

    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}


/**
 * ��ȡ 74HC595 ��ӦоƬ�ϵ���źţ��洢ʱ�ӽţ�
 *
 * @param short number  PCB �ϵ� 74HC595 ���
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC595_GetStoreClockPin( short number )
{
    // �������
    if( number == 2 ) return GPIO_PIN_7;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ 74HC595 ��ӦоƬ�ϵĶ˿ںţ��洢ʱ�ӽţ�
 *
 * @param short number  PCB �ϵ� 74HC595 ���
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* HC595_GetStoreClockPort( short number )
{
    // ����˿�
    if( number == 2 ) return GPIOD;

    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}


/**
 * 74HC595 ��ʼ��
 */
void HC595_Init()
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

    // ��ʼ�� 74HC595 ��Ӧ�� GPIO ���
    for( int i=0; i < ARR_LENGTH(GV_HC595Numbers); i++ ){
        // ���ݽ�
        if( HC595_GetDataPort(GV_HC595Numbers[i]) == 0 ) continue;
        if( HC595_GetDataPin(GV_HC595Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC595_GetDataPort(GV_HC595Numbers[i]), HC595_GetDataPin(GV_HC595Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
        
        // ��λʱ�ӽ�
        if( HC595_GetBitClockPort(GV_HC595Numbers[i]) == 0 ) continue;
        if( HC595_GetBitClockPin(GV_HC595Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC595_GetBitClockPort(GV_HC595Numbers[i]), HC595_GetBitClockPin(GV_HC595Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
                
        // �洢ʱ�ӽ�
        if( HC595_GetStoreClockPort(GV_HC595Numbers[i]) == 0 ) continue;
        if( HC595_GetStoreClockPin(GV_HC595Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC595_GetStoreClockPort(GV_HC595Numbers[i]), HC595_GetStoreClockPin(GV_HC595Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
    }
}


/**
 * 74HC595 ��������
 * �ú���Ϊ 74HC595 �����ֽڵĳ���,�ó����ȷ��͵������λ
 * ��ע������һ���ֽڣ�����������λ���ս���Q7�������������������λ����Q0�����
 *
 * ���ж�Ƭ 74HC595 ����ʱ��ֻ�ж�ε��ø÷��ͳ����ȷ����ֽڽ����Զ��������һ�� 74HC595
 * ���� 74HC595 ��������ƣ�����ͨ���ú����������ݺ󲢲�������������ݵ����пڣ���Ҫ���� HC595_SendOutput() �������
 */
void HC595_SendByte( short number, u8 byte )
{
    // ��ʼ��
    u8 i, dat;
    
    // ��ʼ��
    GPIO_TypeDef* dataPort = HC595_GetDataPort(number);
    if( dataPort == 0 ) return;

    GPIO_Pin_TypeDef dataPin = HC595_GetDataPin(number);
    if( dataPin == GPIO_PIN_ALL ) return;

    GPIO_TypeDef* clockPort = HC595_GetBitClockPort(number);
    if( clockPort == 0 ) return;

    GPIO_Pin_TypeDef clockPin = HC595_GetBitClockPin(number);
    if( clockPin == GPIO_PIN_ALL ) return;

    // �����ֽ�����
    for( i = 0; i < 8; i++ ){
        // ȡ�ֽ����λ
        dat = byte & 0x01; 
    
        // �����ֽ�λ
        // ʱ�ӽ�������һ�Σ�74HC595 ��������ݽŶ�ȡһλ����
        GPIO_WriteLow(clockPort, clockPin);
        dat ? GPIO_WriteHigh(dataPort, dataPin) : GPIO_WriteLow(dataPort, dataPin);
        GPIO_WriteHigh(clockPort, clockPin);

        // �����͵��ֽ�����1λ
        byte >>= 1;
    }
}


/**
 * 74HC595 ��������
 * ���� 74HC595 ��������ƣ�����ͨ�� HC595_SendByte() �����������ݺ󲢲�������������ݵ����пڣ���Ҫ���øú����������
 */
void HC595_SendOutput( short number )
{
    // ��ʼ��
    GPIO_TypeDef* clockPort = HC595_GetStoreClockPort(number);
    if( clockPort == 0 ) return;

    GPIO_Pin_TypeDef clockPin = HC595_GetStoreClockPin(number);
    if( clockPin == GPIO_PIN_ALL ) return;

    // �������
    // 74HC595 �� 13��OE����Ϊ�͵�ƽʱ��Ȼ������ 12��STCP������һ�������غ�ͻ����
    GPIO_WriteLow(clockPort, clockPin);
    GPIO_WriteHigh(clockPort, clockPin);
}






