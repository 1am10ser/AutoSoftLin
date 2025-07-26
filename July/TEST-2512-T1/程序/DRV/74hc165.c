#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * 74HC165 ��������
 */

//���������á�74HC165 ��ż���ֻ�н� 74HC165 ��ż��뵽������������ʵ�ֶ�Ӧ 74HC165 ����ع���
//���������������� U10,U11,U12,U13 �Ǽ����ģ�������д 10 �������ã�����Ҫ��д 11,12,13 ����������
short GV_HC165Numbers[] = {10,16};

//���������á�74HC165 ��ʼ������ֵ��
//�������������������ݶ�ӦоƬ�����ݣ�U16:D7~D0
//�������������������ݶ�ӦоƬ�����ݣ�U10:D7~D0, U11:D7~D0, U12:D7~D0, U13:D7~D0
short GV_HC165DataValues[] = {0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0};



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * 74HC165 ��ʼ��
 */
void HC165_Init()
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

    // ��ʼ�� 74HC165 ��Ӧ�� GPIO ���
    for( int i=0; i < ARR_LENGTH(GV_HC165Numbers); i++ ){
        // ʱ�ӽ�
        if( HC165_GetClockPort(GV_HC165Numbers[i]) == 0 ) continue;
        if( HC165_GetClockPin(GV_HC165Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC165_GetClockPort(GV_HC165Numbers[i]), HC165_GetClockPin(GV_HC165Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);

        // ���ݽ�
        if( HC165_GetDataPort(GV_HC165Numbers[i]) == 0 ) continue;
        if( HC165_GetDataPin(GV_HC165Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC165_GetDataPort(GV_HC165Numbers[i]), HC165_GetDataPin(GV_HC165Numbers[i]), GPIO_MODE_IN_PU_NO_IT);

        // ����װ�ؽ�
        if( HC165_GetLoadPort(GV_HC165Numbers[i]) == 0 ) continue;
        if( HC165_GetLoadPin(GV_HC165Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC165_GetLoadPort(GV_HC165Numbers[i]), HC165_GetLoadPin(GV_HC165Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
    }
}


/**
 * ��ȡ 74HC165 ��ӦоƬ��Ҫ�����λ������
 *
 * @param short number  PCB �ϵ� 74HC165 ���
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
short HC165_GetBitTotal( short number )
{
    // �������
    if( number == 10 ) return 32;
    if( number == 16 ) return 8;
    
    // ��Ч����
    return 0;
}


/**
 * ��ȡ 74HC165 ��ӦоƬ�ϵ���źţ����ݽ� Q7��
 *
 * @param short number  PCB �ϵ� 74HC165 ���
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC165_GetDataPin( short number )
{
    // �������
    if( number == 10 ) return GPIO_PIN_7;
    if( number == 16 ) return GPIO_PIN_5;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ 74HC165 ��ӦоƬ�ϵĶ˿ںţ����ݽ� Q7��
 *
 * @param short number  PCB �ϵ� 74HC165 ���
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* HC165_GetDataPort( short number )
{
    // ����˿�
    if( number == 10 ) return GPIOB;
    if( number == 16 ) return GPIOE;

    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}


/**
 * ��ȡ 74HC165 ��ӦоƬ�ϵ���źţ�����װ�ؽ� /PL��
 *
 * @param short number  PCB �ϵ� 74HC165 ���
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC165_GetLoadPin( short number )
{
    // �������
    if( number == 10 ) return GPIO_PIN_1;
    if( number == 16 ) return GPIO_PIN_0;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ 74HC165 ��ӦоƬ�ϵĶ˿ںţ�����װ�ؽ� /PL��
 * 
 * @param short number  PCB �ϵ� 74HC165 ���
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* HC165_GetLoadPort( short number )
{
    // ����˿�
    if( number == 10 ) return GPIOB;
    if( number == 16 ) return GPIOB;

    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}


/**
 * ��ȡ 74HC165 ��ӦоƬ�ϵ���źţ�ʱ�ӽ� CP��
 *
 * @param short number  PCB �ϵ� 74HC165 ���
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC165_GetClockPin( short number )
{
    // �������
    if( number == 10 ) return GPIO_PIN_6;
    if( number == 16 ) return GPIO_PIN_7;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ 74HC165 ��ӦоƬ�ϵĶ˿ںţ�ʱ�ӽ� CP��
 *
 * @param short number  PCB �ϵ� 74HC165 ���
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* HC165_GetClockPort( short number )
{
    // ����˿�
    if( number == 10 ) return GPIOB;
    if( number == 16 ) return GPIOE;

    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}


/**
 * ��ȡ 74HC165 ����
 * ���ݻ��浽ȫ�ֱ��� GV_HC165DataValues[] ��
 *
 * @param short number  PCB �ϵ� 74HC165 ���
 */
void HC165_ReadDataValues( short number )
{
    // ��ʼ��
    GPIO_TypeDef* dataPort = HC165_GetDataPort(number);
    if( dataPort == 0 ) return;

    GPIO_Pin_TypeDef dataPin = HC165_GetDataPin(number);
    if( dataPin == GPIO_PIN_ALL ) return;
    
    GPIO_TypeDef* loadPort = HC165_GetLoadPort(number);
    if( loadPort == 0 ) return;

    GPIO_Pin_TypeDef loadPin = HC165_GetLoadPin(number);
    if( loadPin == GPIO_PIN_ALL ) return;

    GPIO_TypeDef* clockPort = HC165_GetClockPort(number);
    if( clockPort == 0 ) return;

    GPIO_Pin_TypeDef clockPin = HC165_GetClockPin(number);
    if( clockPin == GPIO_PIN_ALL ) return;
    
    // ��������װ�ؽţ�ʹD0~D7�˵����ݽ�����λ�Ĵ���
    GPIO_WriteLow(loadPort, loadPin);
    
    // ��������װ�ؽţ���ס��λ�Ĵ����е�����
    // �����ڸߵ�ƽʱ���׸� 74HC165 �� Q7 ���������ָ�� D7
    // �����ڸߵ�ƽʱ����һ�������ݴ�DS���Ŵ��н���Ĵ��������ʱ�ӽ�ʹ�ã�ʱ�ӽ���һ�������أ���ô�����ƶ�һλ�����磺D6�����Ƶ�D7��D5�����ƶ���D6...��ʵ�ּ�����
    GPIO_WriteHigh(loadPort, loadPin);
    
    // ��ȡ���ݼ�
    for( u8 i = 0; i < HC165_GetBitTotal(number); i++ ){
        // ��ȡ���ݣ���ȡ��λ D7
        GV_HC165DataValues[i] = GPIO_ReadInputPin(dataPort, dataPin) == RESET ? 0 : 1;

        // ʱ�ӽŲ���һ�������أ��������ƶ�һλ�����磺D7������ʧ��D6�����Ƶ�D7��D5�����ƶ���D6...��ʵ�ּ�����
        GPIO_WriteLow(clockPort, clockPin);
        GPIO_WriteHigh(clockPort, clockPin);
    }
}


