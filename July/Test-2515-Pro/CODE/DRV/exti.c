#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * MOTOR �ⲿ����
 * ���±��������� motor.c �ļ���
 */
extern bool  GV_Motor;


/**
 * �ⲿ�ж���ر���
 */

//���������á��ⲿ�жϱ�ż���ֻ�н��ⲿ�жϱ�ż��뵽������������ʵ�ֶ�Ӧ�ⲿ�жϵ���ع���
short GV_ExtiNumbers[] = {60, 61};   

//��ϵͳʹ�á��ⲿ�ж���һ�ε�ֵ��
short GV_ExtiPrevValues[] = {-1, -1};

//���������á��ⲿ�жϴ���Դ��0 ��ʾ�͵�ƽ������1��ʾ�ߵ�ƽ�������ñ����� GV_ExtiNumbers һһ��Ӧ
//�������������ⲿ�жϴ���Դ�ĸߵ͵�ƽ������·����Ӳ�������ģ����Ծ�����Ҫ�μ�PCBԪ������ԭ��ͼ�����ô˴���ֵ
short GV_ExtiTriggerSources[] = {0, 0};



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ��ȡ�ⲿ�ж϶�ӦоƬ�ϵ���ź�
 *
 * @param short number  PCB �ϵ��ⲿ�жϱ��
 * @return GPIO_Pin_TypeDef  ��Ч����ʱ���� GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef EXTI_GetPin( short number )
{
    // �������
    if( number == 60 ) return GPIO_PIN_0;
    if( number == 61 ) return GPIO_PIN_3;

    // ��Ч����
    return GPIO_PIN_ALL;
}


/**
 * ��ȡ�ⲿ�ж϶�ӦоƬ�ϵĶ˿ں�
 *
 * @param short number  PCB �ϵ��ⲿ�жϱ��
 * @return GPIO_TypeDef*  ��Ч����ʱ���� 0
 */
GPIO_TypeDef* EXTI_GetPort( short number )
{
    // ����˿�
    if( number == 60 ) return GPIOD;
    if( number == 61 ) return GPIOD;

    // ��Ч����
    return (GPIO_TypeDef*)0x0000;
}


/**
 * ��ȡ�ⲿ�ж�ʱ�ĵ�ѹ״̬
 *
 * @param short number  PCB �ϵ��ⲿ�жϱ��
 * @return char  0��ʾ�͵�ƽ��1��ʾ�ߵ�ƽ��-1��ʾδ֪
 */
char EXTI_GetValue( short number )
{
    if( EXTI_GetPort(number) == 0 ) return -1;
    if( EXTI_GetPin(number) == GPIO_PIN_ALL ) return -1;
    return GPIO_ReadInputPin(EXTI_GetPort(number),EXTI_GetPin(number)) == RESET ? 0 : 1;
}


/**
 * �ⲿ�жϳ�ʼ��
 */
void EXTI_Init()
{
    // ��ʼ���ⲿ�ж϶�Ӧ�� GPIO ���
    for( int i=0; i < ARR_LENGTH(GV_ExtiNumbers); i++ ){
        if( EXTI_GetPort(GV_ExtiNumbers[i]) == 0 ) continue;
        if( EXTI_GetPin(GV_ExtiNumbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(EXTI_GetPort(GV_ExtiNumbers[i]), EXTI_GetPin(GV_ExtiNumbers[i]), GV_ExtiTriggerSources[i]==1?GPIO_MODE_IN_FL_IT:GPIO_MODE_IN_PU_IT);
    }

    // �ⲿ�����������غ��½���
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_FALL);  
}



/* -------------------------------------------------- */
// �� �жϻص�����
//   ��������ڲμ� stm8s_it.c
/* -------------------------------------------------- */

/**
 * EXTI �ж�
 */
void EXTI_InterruptHandler()
{
    // ��ʼ��
    short value1, value2;
    
    // �����ⲿ�жϱ�ż�
    for( int i=0; i < ARR_LENGTH(GV_ExtiNumbers); i++ ){
        if( EXTI_GetPort(GV_ExtiNumbers[i]) == 0 ) continue;
        if( EXTI_GetPin(GV_ExtiNumbers[i]) == GPIO_PIN_ALL ) continue;
        
        // ��ȡ���ֵ
        value1 = EXTI_GetValue(GV_ExtiNumbers[i]);

        // �͵�ƽ�������½��أ�
        if( GV_ExtiTriggerSources[i] == 0 ){
            // ��һ��ֵΪ-1����1�����ҵ�ǰֵΪ0����ô�����½���
            if( (GV_ExtiPrevValues[i]==-1||GV_ExtiPrevValues[i]==1) && value1 == 0 ){
                // ��������ȡIO��ֵ
                if( EXTI_INT_JITTER ) delay_ms(EXTI_INT_JITTER); 
                value2 = EXTI_GetValue(GV_ExtiNumbers[i]);
                
                // �����Զ��庯��
                if( value1 == value2 ) EXTI_InterruptHandlerFall(GV_ExtiNumbers[i]);
                else value1 = value2;
            }
        }
        
        // �ߵ�ƽ�����������أ�
        if( GV_ExtiTriggerSources[i] == 1 ){
            // ��һ��ֵΪ-1����0�����ҵ�ǰֵΪ1����ô����������
            if( (GV_ExtiPrevValues[i]==-1||GV_ExtiPrevValues[i]==0) && value1 == 1 ){
                // ��������ȡIO��ֵ
                if( EXTI_INT_JITTER ) delay_ms(EXTI_INT_JITTER); 
                value2 = EXTI_GetValue(GV_ExtiNumbers[i]);
                
                // �����Զ��庯��
                if( value1 == value2 ) EXTI_InterruptHandlerRise(GV_ExtiNumbers[i]);
                else value1 = value2;
            }
        }
        
        // ������һ��ֵ
        GV_ExtiPrevValues[i] = value1;
    }
}


/**
 * EXTI �жϣ��½��أ�
 */
void EXTI_InterruptHandlerFall( short number )
{
    // �����������ж�
    if( GV_Motor ) MOTOR_InterruptHandlerZeroFinish(number, 0);
    
    
    //���߼����롿 J60 �½���
    if( number == 60 ){
        // ...
        //���½��ء�J60 �˿ڱ�Ϊ�͵�ƽ��GV_ExtiTriggerSources[0] == 0����ҵ���߼�����...
        // ...
    }
    
    
    //���߼����롿 J61 �½���
    if( number == 61 ){
        // ...
        //���½��ء�J61 �˿ڱ�Ϊ�͵�ƽ��GV_ExtiTriggerSources[1] == 0����ҵ���߼�����...
        // ...
    }
}


/**
 * EXTI �жϣ������أ�
 */
void EXTI_InterruptHandlerRise( short number )
{
    // �����������ж�
    if( GV_Motor ) MOTOR_InterruptHandlerZeroFinish(number, 1);
    

    //���߼����롿 J60 ������
    if( number == 60 ){
        // ...
        //�������ء�J60 �˿ڱ�Ϊ�ߵ�ƽ��GV_ExtiTriggerSources[0] == 1����ҵ���߼�����...
        // ......
    }
    
    
    //���߼����롿 J61 ������
    if( number == 61 ){
        // ...
        //�������ء�J61 �˿ڱ�Ϊ�ߵ�ƽ��GV_ExtiTriggerSources[1] == 1����ҵ���߼�����...
        // ...
    }
}