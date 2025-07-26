#include "common.h"

/**
 * TIM1 PWM ���벶���������
 * PCB ��·���� TIM1 ������Ϊ 56��57
 * ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM1_TimeBaseInit ���������໥���ţ�����ʹ�� TIM1 ������һ�����ܣ�PWM��� �� PWM���벶��
 */
short  GV_Tim1PwmiNumbers[] = {56, 57};  //���������á�PWM���� ��ż���ֻ�н� PWM��� ��ż��뵽������������ʵ�ֶ�Ӧ PWM��� ����ع���

unsigned int GV_Tim1PwmiFreq56 = 0;    //��ϵͳʹ�á���ʱ������� PWM Ƶ��
double GV_Tim1PwmiDuty56 = 0.0;  //��ϵͳʹ�á���ʱ������� PWM ��ռ�ձ�

u16    GV_Tim1PwmiFreq57 = 0;    //��ϵͳʹ�á���ʱ������� PWM Ƶ��
double GV_Tim1PwmiDuty57 = 0.0;  //��ϵͳʹ�á���ʱ������� PWM ��ռ�ձ�


void TIME1_Init(void)
{ 

  TIM1_DeInit();
 
//  GV_Time1OverflowValue = (((GV_MCUTime / GV_Time1PrescalerValue) * 1000) * GV_MotorOne * 97 / 100) / 360;
//  
//  if(GV_Used1OR3 !=2)
//  {
//      if(GV_MotorOne > 2500)TIM1_TimeBaseInit(GV_Time1PrescalerValue - 1,TIM1_COUNTERMODE_UP,GV_Time1OverflowValue + ((GV_MotorOne - 2500) / 30),0);
//      else TIM1_TimeBaseInit(GV_Time1PrescalerValue - 1,TIM1_COUNTERMODE_UP,GV_Time1OverflowValue - 50,0);
//  }
//  else if(GV_Used1OR3 == 2)
//  {
//      if(GV_MotorOne > 3000)TIM1_TimeBaseInit(GV_Time1PrescalerValue - 1,TIM1_COUNTERMODE_UP,GV_Time1OverflowValue + ((GV_MotorOne - 3000) / 200),0);
//      else TIM1_TimeBaseInit(GV_Time1PrescalerValue - 1,TIM1_COUNTERMODE_UP,GV_Time1OverflowValue - 110,0);
//  }
  TIM1_TimeBaseInit(15,TIM1_COUNTERMODE_UP,65535 - 50000,0);

  TIM1_ARRPreloadConfig(ENABLE);

  TIM1_ITConfig(TIM1_IT_UPDATE,ENABLE);

  TIM1_Cmd(DISABLE);
}




/* -------------------------------------------------- */
// �� �������壨PWM ���벶��
// �� PCB ��·���� TIM1 ������Ϊ 56��57
// �� ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM1_TimeBaseInit ���������໥���ţ�����ʹ�� TIM1 ������һ�����ܣ�PWM��� �� PWM���벶��
/* -------------------------------------------------- */

/**
 * ��ʼ�� PWM
 */
//void TIM1_PWMI_Init()
//{
//    // ���ó�ʼ��
//    TIM1_DeInit();
//    
//    // ��ʼ�� TIM1 ʱ����Ԫ
//    // 16Ԥ��Ƶ�����ϼ�����ÿ 1s/1M = 1us ������ֵ�� 1���ۼ� 1000 ����Ҫ 1 ���룩
//    // ����˵����Ԥ����ֵ������ģʽ���Զ���װ��ֵ��������ֵ�� 65536 ������װ�أ�����������ʼֵ��������ֵ����װ�غ��0��ʼ��
//    TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 65536-1, 0);
//
//    // ��ʼ�� TIM1 ͨ��
////    for( int i=0; i < ARR_LENGTH(GV_Tim1PwmiNumbers); i++ ){
////        if( GV_Tim1PwmiNumbers[i] == 56 ) TIM1_PWMI_Init56();
////        if( GV_Tim1PwmiNumbers[i] == 57 ) TIM1_PWMI_Init57();
////    }
//
//}
void TIM1_PWMI_Init56()
{
    // PWM��ͨ��1����
    // PWM��ͨ��2�������ã���Ϊ TIM1_PWMIConfig(...) �ڲ��Ѿ�����������
    //
    // ����˵����
    //����1. TIM1 ͨ��1
    //����2. ���ԣ������ز���
    //����3. ����ţ�TIM1_ICSELECTION_DIRECTTI ��ʾֱ�ӽ� T1 �źž��� TI1FP1 ���͸� IC1 ģ����д���
    //����4. Ԥ��Ƶ����˼�ǿ����ڶ��ٸ�����������һ�β��񣻡�����ʵ�ʲ��ԣ���λ����ģʽ�¸�ֵ��Ч�����ǰ���1��Ƶִ�С�
    //       ��������1M ϵͳʱ�ӣ����� 20Hz PWM һ��������Ҫ���� 50000  �Σ�1M/20=50000����TIM1 ��������65536�����Բ���
    //       ��������1M ϵͳʱ�ӣ����� 15Hz PWM һ��������Ҫ���� 66666  �Σ�1M/15=66666����TIM1 ��������65536���޷�����
    //����5. �˲�Ƶ�ʣ���������������ͬ�����������ȶ������Ƶ�ʲ���ż��ֵ���ԣ���ô�������� 0x02��4���¼���
    TIM1_ICInit(TIM1_CHANNEL_1, TIM1_ICPOLARITY_RISING, TIM1_ICSELECTION_DIRECTTI, TIM1_ICPSC_DIV1, 0x02);
    
//    TIM1_SelectInputTrigger(TIM1_TS_TI1FP1);
//    TIM1_SelectSlaveMode(TIM1_SLAVEMODE_RESET);
    
    TIM1_ClearITPendingBit(TIM1_IT_CC1);
    TIM1_ClearFlag(TIM1_FLAG_CC1);


    // ���벶��/�Ƚ����ʹ��
    //������ʵ�ʲ��ԣ�����ʹ�ܴ������ע�͵�������Ϊ�˰��Ļ��Ǽ���ȥ��
//    TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
//    TIM1_CCxCmd(TIM1_CHANNEL_2, ENABLE);
}
void TIM1_PWMI_Init57()
{
    // PWM��ͨ��3����
    //
    // ����˵����
    //����1. TIM1 ͨ��3
    //����2. ���ԣ������ز���
    //����3. ����ţ�TIM1_ICSELECTION_DIRECTTI ��ʾֱ�ӽ� T3 �źž��� TI3FP3 ���͸� IC3 ģ����д���
    //����4. Ԥ��Ƶ����˼�ǿ����ڶ��ٸ�����������һ�β��񣻡�����ʵ�ʲ��ԣ���λ����ģʽ�¸�ֵ��Ч�����ǰ���1��Ƶִ�С�
    //       ��������1M ϵͳʱ�ӣ����� 20Hz PWM һ��������Ҫ���� 50000  �Σ�1M/20=50000����TIM1 ��������65536�����Բ���
    //       ��������1M ϵͳʱ�ӣ����� 15Hz PWM һ��������Ҫ���� 66666  �Σ�1M/15=66666����TIM1 ��������65536���޷�����
    //����5. �˲�Ƶ�ʣ���������������ͬ�����������ȶ������Ƶ�ʲ���ż��ֵ���ԣ���ô�������� 0x02��4���¼���
    TIM1_ICInit(TIM1_CHANNEL_3, TIM1_ICPOLARITY_RISING, TIM1_ICSELECTION_DIRECTTI, TIM1_ICPSC_DIV1, 0x02);
    
    // PWM��ͨ��4����
    //
    // ����˵����
    //����1. TIM1 ͨ��4
    //����2. ���ԣ������ز���
    //����3. ����ţ�TIM1_ICSELECTION_INDIRECTTI ��ʾֱ�ӽ� T3 �źž��� TI3FP4 ���͸� IC4 ģ����д���
    //����4. Ԥ��Ƶ����˼�ǿ����ڶ��ٸ�����������һ�β��񣻡�����ʵ�ʲ��ԣ���λ����ģʽ�¸�ֵ��Ч�����ǰ���1��Ƶִ�С�
    //       ��������1M ϵͳʱ�ӣ����� 20Hz PWM һ��������Ҫ���� 50000  �Σ�1M/20=50000����TIM1 ��������65536�����Բ���
    //       ��������1M ϵͳʱ�ӣ����� 15Hz PWM һ��������Ҫ���� 66666  �Σ�1M/15=66666����TIM1 ��������65536���޷�����
    //����5. �˲�Ƶ�ʣ���������������ͬ�����������ȶ������Ƶ�ʲ���ż��ֵ���ԣ���ô�������� 0x02��4���¼���
//    TIM1_ICInit(TIM1_CHANNEL_4, TIM1_ICPOLARITY_FALLING, TIM1_ICSELECTION_INDIRECTTI, TIM1_ICPSC_DIV1, 0x02);

    
    TIM1_ClearITPendingBit(TIM1_IT_CC3);
    TIM1_ClearFlag(TIM1_FLAG_CC3);
    
    // ���벶��/�Ƚ����ʹ��
    //������ʵ�ʲ��ԣ�����ʹ�ܴ������ע�͵�������Ϊ�˰��Ļ��Ǽ���ȥ��
//    TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE);
//    TIM1_CCxCmd(TIM1_CHANNEL_4, ENABLE);
}
void TIM1_PWMI_InitData56()
{
    GV_Tim1PwmiFreq56 = 0;
    GV_Tim1PwmiDuty56 = 0.0;
}
void TIM1_PWMI_InitData57()
{
    GV_Tim1PwmiFreq57 = 0;
    GV_Tim1PwmiDuty57 = 0.0;
}


/**
 * ��ʼ PWM ����
 */
void TIM1_PWMI_Start()
{
    // ��շ����жϱ�־


    // �����жϺ���������Ƚ�ͨ��CH1�ж�
    TIM1_ITConfig(TIM1_IT_CC1, ENABLE);
    TIM1_ITConfig(TIM1_IT_CC3, ENABLE);

    // ����ʱ��
    //������ʵ�ʲ��ԣ���ʱ������δ����������� TIM1_IT_CC1 �ж����������룬���ǻ�ȡ�ļ�����ֵ��ȷ����
    TIM1_Cmd(ENABLE);
}


/**
 * ��ֹ PWM ����
 * ���� TIM1_Cmd(DISABLE) �رղ���ע����������������Ӱ�죬���磺TIM1_PWMI_Init
 */
void TIM1_PWMI_Stop()
{
    // �ر��жϺ���������Ƚ�ͨ��CH1�ж�
    TIM1_ITConfig(TIM1_IT_CC1, DISABLE);
    TIM1_ITConfig(TIM1_IT_CC3, DISABLE);

    // �رն�ʱ��
    TIM1_Cmd(DISABLE);
}


/**
 * ��ȡ PWM Ƶ��
 *
 * @param short number  PCB�ϵı�ţ�56, 57
 * @return integer      Ƶ��
 */
u16 TIM1_PWMI_GetFreq( short number )
{
    // ����Ƶ��
    if( number == 56 ) return GV_Tim1PwmiFreq56;
    if( number == 57 ) return GV_Tim1PwmiFreq57;
        
    // ��Ч����
    return 0;
}


/**
 * ��ȡ PWM ��ռ�ձ�
 *
 * @param short number  PCB�ϵı�ţ�56, 57
 * @return double       ռ�ձ�
 */
double TIM1_PWMI_GetDuty( short number )
{
    // ����ռ�ձ�
    if( number == 56 ) return GV_Tim1PwmiDuty56;
    if( number == 57 ) return GV_Tim1PwmiDuty57;
        
    // ��Ч����
    return 0;
}



/* -------------------------------------------------- */
// �� �жϻص�����
//   ��������ڲμ� stm8s_it.c
/* -------------------------------------------------- */


/**
 * TIM1 �ж�
 */
void TIM1_InterruptHandler()
{
    // �жϣ�������ֵ�������
//    if( TIM1_GetITStatus(TIM1_IT_UPDATE) ) TIM1_InterruptHandlerUpdate();
    
    // �жϣ�PWM ����
    // ���� PWM = 0 ���� 100 �Ĳ��������⣬��Ϊ������ PWM ֵ���޷������ж�
//    if( TIM1_GetITStatus(TIM1_IT_CC1) != RESET ) TIM1_InterruptHandlerCapturePwm56();
//    if( TIM1_GetITStatus(TIM1_IT_CC3) != RESET ) TIM1_InterruptHandlerCapturePwm57();
}

/**
 * TIM1 �жϣ�PWM ����
 */
void TIM1_InterruptHandlerCapturePwm56()
{
    // ��ʼ����ʱ�����벶��ֵ
    // �ֱ��Ӧһ��PWM���ڵĵ�һ�������أ���һ���½��أ��ڶ���������
    static u16 icValueA, icValueB, icValueC;
    static u8 K = 0;
    // ��ȡ��ʱ�����벶��ֵ
    if( K == 0 )
    {
        icValueA = icValueB = icValueC = 0;
        icValueA = TIM1_GetCapture1();
        K = 1;
        
        TIM1->CCER1 |= TIM1_CCER1_CC1P;
    }
    else if(K == 1)
    {
        icValueB = TIM1_GetCapture1();
        K = 2;
        
        TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1P);
    }
    else
    {

        icValueC = TIM1_GetCapture1() - icValueA;
        GV_Tim1PwmiFreq56 = 16000000/icValueC;
        GV_Tim1PwmiDuty56 = (icValueB-icValueA)*100.0/icValueC;
        K = icValueA = icValueB = icValueC = 0;        
    }

    // ����жϱ�־λ
    TIM1_ClearITPendingBit(TIM1_IT_CC1);    
    TIM1_ClearFlag(TIM1_FLAG_CC1);
}
void TIM1_InterruptHandlerCapturePwm57()
{
    // ��ʼ����ʱ�����벶��ֵ
    // �ֱ��Ӧһ��PWM���ڵĵ�һ�������أ���һ���½��أ��ڶ���������
    static u16 icValueA3, icValueB3, icValueC3;
    static u8 K3 = 0;
    
    // ��ȡ��ʱ�����벶��ֵ
    if( K3 == 0 )
    {
        icValueA3 = icValueB3 = icValueC3 = 0;
        icValueA3 = TIM1_GetCapture3();
        K3 = 1;
        
        TIM1->CCER2 |= TIM1_CCER2_CC3P;
    }
    else if(K3 == 1)
    {
        icValueB3 = TIM1_GetCapture3();
        K3 = 2;
        
        TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC3P);
    }
    else
    {
        icValueC3 = TIM1_GetCapture3() - icValueA3;
        GV_Tim1PwmiFreq57 = 16000000/icValueC3;
        GV_Tim1PwmiDuty57 = (icValueB3-icValueA3)*100.0/icValueC3;
        K3 = icValueA3 = icValueB3 = icValueC3 = 0;
    }

    // ����жϱ�־λ
    TIM1_ClearITPendingBit(TIM1_IT_CC3);    
    TIM1_ClearFlag(TIM1_FLAG_CC3);
}