#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * TIM3 PWM �����������
 * PCB ��·���� TIM3 ������Ϊ 54
 * ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM3_TimeBaseInit ���������໥���ţ�����ʹ�� TIM3 ������һ�����ܣ�PWM��� �� PWM���벶��
 */
short  GV_Tim3PwmoArr = 0;  //��ϵͳʹ�á���������װ��ֵ���Զ�����Ƶ�ʼ���

short  GV_Tim3PwmoSendSoon     = 0;  //��ϵͳʹ�á��������ͱ��
double GV_Tim3PwmoSendSoonDuty = 0;  //��ϵͳʹ�á��������͵�ռ�ձ�����
u32    GV_Tim3PwmoSendLimit    = 0;  //��ϵͳʹ�á����͸�������
short  GV_Tim3PwmoSendCount    = 0;  //��ϵͳʹ�á����͸����ۼ�


/**
 * TIM3 PWM ���벶���������
 * PCB ��·���� TIM3 ������Ϊ 54
 * ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM3_TimeBaseInit ���������໥���ţ�����ʹ�� TIM3 ������һ�����ܣ�PWM��� �� PWM���벶��
 */
u16    GV_Tim3PwmiFreq = 0;    //��ϵͳʹ�á���ʱ������� PWM Ƶ��
double GV_Tim3PwmiDuty = 0.0;  //��ϵͳʹ�á���ʱ������� PWM ��ռ�ձ�



/* -------------------------------------------------- */
// �� �������壨PWM�����
// �� PCB ��·���� TIM3 ������Ϊ 54
// �� ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM3_TimeBaseInit ���������໥���ţ�����ʹ�� TIM3 ������һ�����ܣ�PWM��� �� PWM���벶��
/* -------------------------------------------------- */

/**
 * ��ʼ�� TIM3 PWM ���
 * ��ʼ����˿ڲ�������� PWM �źţ���Ҫ���� TIM3_PWMO_Send(...) �������
 *
 * @param int freq  PWM ���Ƶ�ʣ�50 < freq < 450000��16��Ƶ��������ȽϺò��Σ�ռ�ձȽ�׼��
 */
void TIM3_PWMO_Init( u32 freq )
{
    // ���ó�ʼ��
    TIM3_DeInit();
    
    // ��ʼ����������װ��ֵ
    // ����ϵͳʱ��Ƶ�ʡ���Ƶ�ʺ�����Ƶ�������������װ��ֵ
    GV_Tim3PwmoArr = 16000000/16/freq;

    // ��ʼ�� TIME2 ʱ����Ԫ
    // 16Ԥ��Ƶ�����ϼ�����ÿ 1s/1M = 1us ������ֵ�� 1���ۼ� 1000 ����Ҫ 1 ���룩
    // ����˵����Ԥ����ֵ���Զ���װ��ֵ��������ֵ�� GV_Tim3PwmoArrNum ������װ�أ�
    TIM3_TimeBaseInit(TIM3_PRESCALER_16, GV_Tim3PwmoArr-1);

    // �Զ�Ԥװ������λ
    // ARR��Auto-Reload Register
    TIM3_ARRPreloadConfig(ENABLE);

    // ��ʼ��ͨ���ķ���/�Ƚ�
    // �˴��������� TIM3_OCMODE_PWM2+TIM3_OCPOLARITY_HIGH��ȷ����ʼʱΪ�͵�ƽ
    //
    // ����˵����
    //����1. PWM���ģʽ��TIM3_OCMODE_PWM1������ǰ�����Ч��ƽ����TIM3_OCMODE_PWM2������ǰ�����Ч��ƽ��
    //����      ��������������ǰ��ָ��ʱ��ֵС�ڱȽ����趨��ֵ
    //����2. ��ֹ������������Ƚ�
    //����3. ������ֵ�������ʱ���Զ�װ������Ϊ 2000��������ֵ��������Ϊ 1000����ô����� 50% ��ռ�ձȡ�ͬʱ��ֵ���ڿ��Զ�̬����
    //����4. ����Ƚϼ��ԣ���Ҫ��� ����1 ȷ������ǰ����Ч��ƽ���μ�������ȭSTM8 39 ����PWM ���ʵ�顷����ͨ�� TIM3_OCMODE_PWM2+TIM3_OCPOLARITY_LOW �ȽϷ����˵ĳ���˼ά
    //����   ���������������˴�����ʹ�� TIM3_OCMODE_PWM2+TIM3_OCPOLARITY_HIGH ��ʽ����Ҫ��ȷ��PWM��ʼ��Ĭ������͵�ƽ����ģʽ��ͨ�� TIM3_SetCompare ���õ�ռ�ձ�ֵ���Ǹ�ռ�ձ��ˣ�����ʱע��
    TIM3_OC1Init(TIM3_OCMODE_PWM2, TIM3_OUTPUTSTATE_ENABLE, 0, TIM3_OCPOLARITY_HIGH);

    // �� TIM3_OCxInit ����������Ϊ 0 ʱ����ô��ʼ�ĵ�ƽ�Ͳ��ǵ͵�ƽ������ͨ�����º���������һ�Ρ�
    TIM3_PWMO_SetDuty(0);

    // ����жϱ�־
    TIM3_ClearITPendingBit(TIM3_IT_UPDATE);
    TIM3_ClearFlag(TIM3_FLAG_UPDATE);
    
    // �����жϺ�����������ֵ�������
    TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
    
    // ����ʱ��
    TIM3_Cmd(ENABLE);
        
    // ���벶��/�Ƚ����ʹ��
    // ���´���� TIM3_OCxInit() �еĵڶ�����������һ�£�һ���÷���ֱ���� TIM3_OCxInit() ��ʼʱֱ������Ϊ ENABLE
    // ����һ���÷����� TIM3_Cmd(ENABLE) ֮����� TIM3_CCxCmd(.., ENABLE) �ٽ���ʹ�ܣ���Ϊ TIM3_OCxInit() ��ʼ��ʱ��ֱ������Ϊ ENABLE �Ļ���
    // ��ô��λ��Ƭ��ʱ��TIM3 �Ϳ���������̵ĸߵ�ƽ����һ�ε����Ŀ�����������������Ŀ��������̵ĸߵ�ƽ���У���ô������ TIM3_Cmd(ENABLE) ������ TIM3_CCxCmd(.., ENABLE)
    //TIM3_CCxCmd(TIM3_CHANNEL_1, ENABLE); 
}


/**
 * ���� TIM3 ͨ���� PWM ��ռ�ձ�
 *
 * @param double duty  ��ռ�ձ�ֵ
 */
void TIM3_PWMO_SetDuty( double duty )
{
    // ��ʼ����
    if( duty < 0 ) duty = 0;
    if( duty > 100 ) duty = 100;
 
    // ����ʹ���� TIM3_OCMODE_PWM2����ô PWM ����ǰ�Ľ����һ����Ч��ƽ��
    // ������ TIM3_OCPOLARITY_HIGH ���������ã���ָ������Ч��ƽ�Ǹߵ�ƽ����Ч��ƽ�ǵ͵�ƽ������ PWM ����ǰ�Ľ�����͵�ƽ
    // ��ô�����ʱ�������ò��� duty = 73��ʵ���Ͼ������ 73% �ĵ͵�ƽ����ռ�ձȣ�����������˵��ռ�ձ�����ռ�ձȡ�������Ҫ���� 100-duty ����ȡ��������
    duty = 100 - duty;
    
    // ����ռ�ձ�ֵ
    TIM3_SetCompare1(((u32)(duty*GV_Tim3PwmoArr))/100);  // duty/100 = x/GV_Tim3PwmoArr  =>  x = duty*GV_Tim3PwmoArr/100
}


/**
 * ���� TIM3 ͨ���� PWM ���
 * �������˷���������������ô������Զ��� ��ʱ�������ж� ���Ⲣ��������ֵ��ֹͣ ��ʱ��ͨ�� ���ʹ��
 *
 * �ú������ú󲻻��������� PWM�����ǳ�ʼ��صķ��Ͳ���������� ��ʱ�������ж� �������Щ�������� PWM ����
 * ��Ҫԭ�򣺸���ͨ����������ʱ�������ʱ TIM3_CNT ��Ϊ 0 ʱ����ô�ͻᵼ��PWM ��һ�����ڵĲ����쳣�������� ��ʱ�������ж� ���������;���ȷ�� TIM3_CNT Ϊ 0
 *
 * �������ú� GV_Tim3PwmoSendCount ��������Ϊ 1�����ʹ������ǰ������Ҫ���ǵ������������һ��ʱ����� GV_Tim3PwmoSendCount Ϊ 0����ô�ͱ�ʾ���巢����ϡ�
 *
 * @param double duty       ��ռ�ձ�ֵ����������ռ�ձȣ�����������
 * @param u32    sendLimit  ���巢�͸������ƣ�0 ��ʾ������
 * 
 * @return boolean  true ��ʾ���ͳɹ���false ��ʾ����ʧ��
 */
bool TIM3_PWMO_Send( double duty, u32 sendLimit )
{
    // ��ʼ������صı���
    // ��Ȼ��û���Ϸ��ͣ����Ǵ˴�ֱ�����÷��ͼ���Ϊ1��GV_Tim3PwmoSendCount = 1��
    // ��Ҫ���ǵ�������������ִ�����Ҫ���� GV_Tim3PwmoSendCount ����ֵ�����ж��Ƿ����ڷ��͵�״̬
    GV_Tim3PwmoSendSoon = 1;
    GV_Tim3PwmoSendSoonDuty = duty;
    
    GV_Tim3PwmoSendCount = 1;
    GV_Tim3PwmoSendLimit = sendLimit;

    // ����
    return true;
}


/**
 * ��ֹ TIM3 ͨ���� PWM ���
 * �ú�����Ҫ���ý� ��ռ�ձ�����Ϊ0���͵�ƽ�� ��ģ�� ֹͣ��ʱ��ͨ��
 */
void TIM3_PWMO_Stop()
{
    TIM3_PWMO_SetDuty(0);
    GV_Tim3PwmoSendLimit = GV_Tim3PwmoSendCount = 0; 
    GV_Tim3PwmoSendSoon = 0; GV_Tim3PwmoSendSoonDuty = 0;
}



/* -------------------------------------------------- */
// �� �������壨PWM ���벶��
// �� PCB ��·���� TIM3 ������Ϊ 54
// �� ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM3_TimeBaseInit ���������໥���ţ�����ʹ�� TIM3 ������һ�����ܣ�PWM��� �� PWM���벶��
/* -------------------------------------------------- */

/**
 * ��ʼ�� TIM3 PWM ����
 */
void TIM3_PWMI_Init()
{
    // ���ó�ʼ��
    TIM3_DeInit();

    // ��ʼ�� TIME3 ʱ����Ԫ
    // 16Ԥ��Ƶ�����ϼ�����ÿ 1s/1M = 1us ������ֵ�� 1���ۼ� 1000 ����Ҫ 1 ���룩
    // ����˵����Ԥ����ֵ���Զ���װ��ֵ��������ֵ�� 65536 ������װ�أ�
    TIM3_TimeBaseInit(TIM3_PRESCALER_16, 65536-1);

    // PWM��ͨ��1����
    // PWM��ͨ��2�������ã���Ϊ TIM3_PWMIConfig(...) �ڲ��Ѿ�����������
    //
    // ����˵����
    //����1. TIM3 ͨ��1
    //����2. ���ԣ������ز���
    //����3. ����ţ�TIM3_ICSELECTION_DIRECTTI ��ʾֱ�ӽ� T1 �źž��� TI1FP1 ���͸� IC1 ģ����д���
    //����4. Ԥ��Ƶ����˼�ǿ����ڶ��ٸ�����������һ�β��񣻡�����ʵ�ʲ��ԣ���λ����ģʽ�¸�ֵ��Ч�����ǰ���1��Ƶִ�С�
    //       ��������1M ϵͳʱ�ӣ����� 20Hz PWM һ��������Ҫ���� 50000  �Σ�1M/20=50000����TIM3 ��������65536�����Բ���
    //       ��������1M ϵͳʱ�ӣ����� 15Hz PWM һ��������Ҫ���� 66666  �Σ�1M/15=66666����TIM3 ��������65536���޷�����
    //����5. �˲�Ƶ�ʣ���������������ͬ�����������ȶ������Ƶ�ʲ���ż��ֵ���ԣ���ô�������� 0x02��4���¼���
    TIM3_PWMIConfig(TIM3_CHANNEL_1, TIM3_ICPOLARITY_RISING, TIM3_ICSELECTION_DIRECTTI, TIM3_ICPSC_DIV1, 0x00);
    
    // ���벶��/�Ƚ����ʹ��
    //������ʵ�ʲ��ԣ�����ʹ�ܴ������ע�͵�������Ϊ�˰��Ļ��Ǽ���ȥ��
    TIM3_CCxCmd(TIM3_CHANNEL_1, ENABLE);
    TIM3_CCxCmd(TIM3_CHANNEL_2, ENABLE);
}
void TIM3_PWMI_InitData()
{
    GV_Tim3PwmiFreq = 0;
    GV_Tim3PwmiDuty = 0.0;
}


/**
 * ��ʼ TIM3 PWM ����
 */
void TIM3_PWMI_Start()
{
    // ��շ����жϱ�־
    TIM3_ClearITPendingBit(TIM3_IT_CC1);
    TIM3_ClearFlag(TIM3_FLAG_CC1);
    
    // �����жϺ���������Ƚ�ͨ��CH1�ж�
    TIM3_ITConfig(TIM3_IT_CC1, ENABLE);

    // ����ʱ��
    //������ʵ�ʲ��ԣ���ʱ������δ����������� TIM3_IT_CC1 �ж����������룬���ǻ�ȡ�ļ�����ֵ��ȷ����
    TIM3_Cmd(ENABLE);
}


/**
 * ��ֹ TIM3 PWM ����
 * ���� TIM3_Cmd(DISABLE) �ر������ע����������������Ӱ�죬���磺TIM3_PWMI_Init
 */
void TIM3_PWMI_Stop()
{
    // �ر��жϺ���������Ƚ�ͨ��CH1�ж�
    TIM3_ITConfig(TIM3_IT_CC1, DISABLE);

    // �رն�ʱ��
    TIM3_Cmd(DISABLE);
}


/**
 * ��ȡ TIM3 ����� PWM Ƶ��
 *
 * @return integer  Ƶ��
 */
u16 TIM3_PWMI_GetFreq()
{
    return GV_Tim3PwmiFreq;
}


/**
 * ��ȡ TIM2 ����� PWM ��ռ�ձ�
 *
 * @return double  ռ�ձ�
 */
double TIM3_PWMI_GetDuty()
{
    return GV_Tim3PwmiDuty;
}



/* -------------------------------------------------- */
// �� �жϻص�����
//   ��������ڲμ� stm8s_it.c
/* -------------------------------------------------- */

/**
 * TIM3 �ж�
 */
void TIM3_InterruptHandler()
{
    // �жϣ�������ֵ�������
    if( TIM3_GetITStatus(TIM3_IT_UPDATE) ) TIM3_InterruptHandlerUpdate();
        
    // �жϣ�PWM ����
    // ���� PWM = 0 ���� 100 �Ĳ��������⣬��Ϊ������ PWM ֵ���޷������ж�
    if( TIM3_GetITStatus(TIM3_IT_CC1) != RESET ) TIM3_InterruptHandlerCapturePwm();
}


/**
 * TIM3 �жϣ�������ֵ�������
 */
void TIM3_InterruptHandlerUpdate()
{
    // �޼������͵����� �� ���÷�������
    if( GV_Tim3PwmoSendSoon == 0 && GV_Tim3PwmoSendLimit > 0 ){
        if( GV_Tim3PwmoSendCount++ >= GV_Tim3PwmoSendLimit ) TIM3_PWMO_Stop();  // ������͸����������Ƹ�������ô��ͣ���
    }

    // �м������͵�����
    if( GV_Tim3PwmoSendSoon == 1 ){
        if( GV_Tim3PwmoSendSoonDuty >= 0 ) TIM3_PWMO_SetDuty(GV_Tim3PwmoSendSoonDuty);
        GV_Tim3PwmoSendSoon = 0; GV_Tim3PwmoSendSoonDuty = 0;
    }
    
    // ����жϱ�־
    TIM3_ClearITPendingBit(TIM3_IT_UPDATE);
    TIM3_ClearFlag(TIM3_FLAG_UPDATE);
}


/**
 * TIM3 �жϣ�PWM ����
 */
void TIM3_InterruptHandlerCapturePwm()
{
    // ��ʼ����ʱ�����벶��ֵ
    // �ֱ��Ӧһ��PWM���ڵĵ�һ�������أ���һ���½��أ��ڶ���������
    static u16 icValueA, icValueB, icValueC;

    // ��ȡ��ʱ�����벶��ֵ
    if( icValueA == 0 ){
        icValueA = TIM3_GetCapture1();
    }else{
        icValueB = TIM3_GetCapture2();
        icValueC = TIM3_GetCapture1();
        
        // ����ʱ������ֵ������
        if( icValueC < icValueA ) icValueC = icValueC-icValueA+65535;
        else icValueC = icValueC - icValueA;
 
        if( icValueB < icValueA ) icValueB = icValueB-icValueA+65535;
        else icValueB = icValueB - icValueA;

        // ����Ƶ�ʺ�����
        // ����� 1000000 �ǻ��ڳ�ʼ�������е�ϵͳʱ�����ã�16Ԥ��Ƶ = 1000000��
        // ����� 100.0 С�����ʾ�����ʽ icValueB*100.0/icValueC �Ĺ��̺ͽ���Ը�����������
        GV_Tim3PwmiFreq = 1000000/icValueC;
        GV_Tim3PwmiDuty = icValueB*100.0/icValueC;

        // ���ö�ʱ�����벶��ֵ
        icValueA = icValueB = icValueC = 0;
    }

    // ����жϱ�־λ
    TIM3_ClearITPendingBit(TIM3_IT_CC1);    
    TIM3_ClearFlag(TIM3_FLAG_CC1);
}