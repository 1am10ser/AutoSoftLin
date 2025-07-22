#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * ADC2 ��������
 */
short GV_Adc2Numbers[] = {68,69,90,91,92,93};   //���������á�ADC2 �ӿڱ�ż���ֻ�н� ADC2 �ӿڱ�ż��뵽������������ʵ�ֶ�Ӧ ADC2 �ӿڵ���ع���


/**
 * ADC2 ������ر���
 */
short GV_Adc2SamplingAvgCount  = 5;  // ADC2 ����ƽ��ֵ������������Խ��ƽ��ֵԽ׼ȷ
float GV_Adc2SamplingBaseVolt  = 5;  // ADC2 �����Ļ�׼��ѹ



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ��ȡ ADC2 �ӿڶ�ӦоƬ�ϵ���ź�
 *
 * @param short number  PCB �ϵ� ADC2 �ӿڱ��
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef ADC2_GetPin( short number )
{
    // �������
    if( number == 68 ) return GPIO_PIN_0;
    if( number == 69 ) return GPIO_PIN_7;
    if( number == 90 ) return GPIO_PIN_5;
    if( number == 91 ) return GPIO_PIN_4;
    if( number == 92 ) return GPIO_PIN_3;
    if( number == 93 ) return GPIO_PIN_2;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ ADC2 �ӿڶ�ӦоƬ�ϵĶ˿ں�
 *
 * @param short number  PCB �ϵ� ADC2 �ӿڱ��
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* ADC2_GetPort( short number )
{
    // ����˿�
    if( number == 68 ) return GPIOB;
    if( number == 69 ) return GPIOE;
    if( number == 90 ) return GPIOB;
    if( number == 91 ) return GPIOB;
    if( number == 92 ) return GPIOB;
    if( number == 93 ) return GPIOB;
    
    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}


/**
 * ��ȡ ADC2 �ӿڶ�ӦоƬ�ϵ�ͨ����
 *
 * @param short number  PCB �ϵ� ADC2 �ӿڱ��
 * @return ADC2_Channel_TypeDef  ��Ч����ʱ���� 0xFF
 */
ADC2_Channel_TypeDef ADC2_GetChannel( short number )
{
    // ����˿�
    if( number == 68 ) return ADC2_CHANNEL_0;
    if( number == 69 ) return ADC2_CHANNEL_8;
    if( number == 90 ) return ADC2_CHANNEL_5;
    if( number == 91 ) return ADC2_CHANNEL_4;
    if( number == 92 ) return ADC2_CHANNEL_3;
    if( number == 93 ) return ADC2_CHANNEL_2;

    // ��Ч����
    return (ADC2_Channel_TypeDef)0xFF;
}


/**
 * ��ȡ ADC2 �ӿڶ�ӦоƬ�ϵ�ʩ����ͨ����
 *
 * @param short number  PCB �ϵ� ADC2 �ӿڱ��
 * @return ADC2_SchmittTrigg_TypeDef  ��Ч����ʱ���� 0xFF
 */
ADC2_SchmittTrigg_TypeDef ADC2_GetSchmittChannel( short number )
{
    // ����˿�
    if( number == 68 ) return ADC2_SCHMITTTRIG_CHANNEL0;
    if( number == 69 ) return ADC2_SCHMITTTRIG_CHANNEL8;
    if( number == 90 ) return ADC2_SCHMITTTRIG_CHANNEL5;
    if( number == 91 ) return ADC2_SCHMITTTRIG_CHANNEL4;
    if( number == 92 ) return ADC2_SCHMITTTRIG_CHANNEL3;
    if( number == 93 ) return ADC2_SCHMITTTRIG_CHANNEL2;
    
    // ��Ч����
    return (ADC2_SchmittTrigg_TypeDef)0xFF;
}


/**
 * ADC2 ��ʼ��
 */
void ADC2_CInit()
{
    // ��ʼ�� ADC2 GPIO
    // GPIO_MODE_IN_FL_IT     �����������жϹ���
    // GPIO_MODE_IN_PU_IT     �����������жϹ���
    // GPIO_MODE_IN_FL_NO_IT  �����������жϹ���
    // GPIO_MODE_IN_PU_NO_IT  �����������жϹ���
    for( int i=0; i < ARR_LENGTH(GV_Adc2Numbers); i++ ){
        if( ADC2_GetPort(GV_Adc2Numbers[i]) == 0 ) continue;
        if( ADC2_GetPin(GV_Adc2Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(ADC2_GetPort(GV_Adc2Numbers[i]), ADC2_GetPin(GV_Adc2Numbers[i]), GPIO_MODE_IN_FL_NO_IT);
    }
    
    // ���� ADC2 ����
    ADC2_Cmd(ENABLE);
}


/**
 * ��ȡ ADC2 ��ѹֵ
 *
 * @param number  PCB�ϵı��
 * @return float  ��ѹֵ
 */
float ADC2_GetVoltValue( short number )
{
    // ��Ч����
    if( ADC2_GetChannel(number) == 0xFF ) return ADC2_INVALID_VOLT;
    if( ADC2_GetSchmittChannel(number) == 0xFF ) return ADC2_INVALID_VOLT;
    
    // ��ʼ��
    int adc2SamplingTotalValue = 0;
    
    // ADC2_Init ����˵����
    // ����1. ת��ģʽ�����λ�����ת����
    // ����2. ͨ����0~9��
    // ����3. ʱ�ӷ�Ƶ��
    // ����4. �ⲿ����ת���¼�
    // ����5. ����/�ر��ⲿ����ת���¼�
    // ����6. ���ݶ��뷽�����������Ҷ��룩
    // ����7. ָ��ͨ����ʩ���ش�����
    // ����8. ���û��߹ر�ָ��ͨ����ʩ���ش�����
    //
    //        1                           2                        3                      4                 5        6                 7                               8
    ADC2_Init(ADC2_CONVERSIONMODE_SINGLE, ADC2_GetChannel(number), ADC2_PRESSEL_FCPU_D18, ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_GetSchmittChannel(number), DISABLE);

    // ��ȡ ADC2 ����ֵ�ۼ�ֵ
    for( int ii = 0; ii < GV_Adc2SamplingAvgCount; ii++ ){
        // �ɼ����ۼ�
        ADC2_StartConversion();
        adc2SamplingTotalValue += ADC2_GetConversionValue(); 
        
        // �ӳ٣���ߵ�ѹ�ɼ��ȶ���
        //delay_ms(10);
    }

    // ����
    return (float)adc2SamplingTotalValue/GV_Adc2SamplingAvgCount/1024*GV_Adc2SamplingBaseVolt;
}



/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */

/**
 * ���߼�����ʾ����ADC�����
 */
void ADC2_MainPrintAllAdcValue()
{
    // ��ʼ��
    short i;
    float volts[10];
    
    // �ɼ����ݼ�
    for( i=0; i < ARR_LENGTH(GV_Adc2Numbers); i++ ){
        volts[i] = ADC2_GetVoltValue(GV_Adc2Numbers[i]);
    }

    // ������ݼ�
    for( i=0; i < ARR_LENGTH(GV_Adc2Numbers); i++ ){
        printf("J%d = ", GV_Adc2Numbers[i]);
        if( volts[i] == ADC2_INVALID_VOLT ) printf("Invalid Volt\n");
        else printf("%.2f V\n", volts[i]);
    }
    
    // ����ָ���
    printf("==========\n");
}