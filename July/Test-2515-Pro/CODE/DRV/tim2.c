#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * MOTOR �ⲿ����
 * ���±��������� motor.c �ļ���
 */
extern bool  GV_Motor;
extern short GV_MotorState;
extern short GV_MotorStatePrev;


/**
 * TIM2 PWM �����������
 * PCB ��·���� TIM2 ������Ϊ 55
 * ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM2_TimeBaseInit ���������໥���ţ�����ʹ�� TIM2 ������һ�����ܣ�PWM��� �� PWM���벶��
 */
short  GV_Tim2PwmoArr = 0;  //��ϵͳʹ�á���������װ��ֵ���Զ�����Ƶ�ʼ���

short  GV_Tim2PwmoSendSoon     = 0;  //��ϵͳʹ�á��������ͱ��
double GV_Tim2PwmoSendSoonDuty = 0;  //��ϵͳʹ�á��������͵�ռ�ձ�����
u32    GV_Tim2PwmoSendLimit    = 0;  //��ϵͳʹ�á����͸�������
short  GV_Tim2PwmoSendCount    = 0;  //��ϵͳʹ�á����͸����ۼ�


/**
 * TIM2 PWM ���벶���������
 * PCB ��·���� TIM2 ������Ϊ 55
 * ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM2_TimeBaseInit ���������໥���ţ�����ʹ�� TIM2 ������һ�����ܣ�PWM��� �� PWM���벶��
 */
u16    GV_Tim2PwmiFreq = 0;    //��ϵͳʹ�á���ʱ������� PWM Ƶ��
double GV_Tim2PwmiDuty = 0.0;  //��ϵͳʹ�á���ʱ������� PWM ��ռ�ձ�



/* -------------------------------------------------- */
// �� �������壨PWM�����
// �� PCB ��·���� TIM2 ������Ϊ 55
// �� ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM2_TimeBaseInit ���������໥���ţ�����ʹ�� TIM2 ������һ�����ܣ�PWM��� �� PWM���벶��
/* -------------------------------------------------- */

/**
 * ��ʼ�� TIM2 PWM ���
 * ��ʼ����˿ڲ�������� PWM �źţ���Ҫ���� TIM2_PWMO_Send(...) �������
 *
 * @param int freq  PWM ���Ƶ�ʣ�50 < freq < 450000��16��Ƶ��������ȽϺò��Σ�ռ�ձȽ�׼��
 */
void TIM2_PWMO_Init( u32 freq )
{
    // ���ó�ʼ��
    TIM2_DeInit();
    
    // ��ʼ����������װ��ֵ
    // ����ϵͳʱ��Ƶ�ʡ���Ƶ�ʺ�����Ƶ�������������װ��ֵ
    GV_Tim2PwmoArr = 16000000/16/freq;

    // ��ʼ�� TIME2 ʱ����Ԫ
    // 16Ԥ��Ƶ�����ϼ�����ÿ 1s/1M = 1us ������ֵ�� 1���ۼ� 1000 ����Ҫ 1 ���룩
    // ����˵����Ԥ����ֵ���Զ���װ��ֵ��������ֵ�� GV_Tim2PwmoArrNum ������װ�أ�
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, GV_Tim2PwmoArr-1);

    // �Զ�Ԥװ������λ
    // ARR��Auto-Reload Register
    TIM2_ARRPreloadConfig(ENABLE);

    // ��ʼ��ͨ���ķ���/�Ƚ�
    // �˴��������� TIM2_OCMODE_PWM2+TIM2_OCPOLARITY_HIGH��ȷ����ʼʱΪ�͵�ƽ
    //
    // ����˵����
    //����1. PWM���ģʽ��TIM2_OCMODE_PWM1������ǰ�����Ч��ƽ����TIM2_OCMODE_PWM2������ǰ�����Ч��ƽ��
    //����      ��������������ǰ��ָ��ʱ��ֵС�ڱȽ����趨��ֵ
    //����2. ��ֹ������������Ƚϣ�Ĭ�Ͻ��ã���Ϊʵ�ʲ����з������Ĭ�����ã���ô�ϵ��λ��Ƭ���ͻ����һ�����̵ĸߵ�ƽ�����ۺ��� TIM2_Cmd(..) �� ENABLE �� DISABLE
    //����3. ������ֵ�������ʱ���Զ�װ������Ϊ 2000��������ֵ��������Ϊ 1000����ô����� 50% ��ռ�ձȡ�ͬʱ��ֵ���ڿ��Զ�̬����
    //����4. ����Ƚϼ��ԣ���Ҫ��� ����1 ȷ������ǰ����Ч��ƽ���μ�������ȭSTM8 39 ����PWM ���ʵ�顷����ͨ�� TIM2_OCMODE_PWM2+TIM2_OCPOLARITY_LOW �ȽϷ����˵ĳ���˼ά
    //����   ���������������˴�����ʹ�� TIM2_OCMODE_PWM2+TIM2_OCPOLARITY_HIGH ��ʽ����Ҫ��ȷ��PWM��ʼ��Ĭ������͵�ƽ����ģʽ��ͨ�� TIM2_SetCompare ���õ�ռ�ձ�ֵ���Ǹ�ռ�ձ��ˣ�����ʱע��
    TIM2_OC1Init(TIM2_OCMODE_PWM2, TIM2_OUTPUTSTATE_DISABLE, 0, TIM2_OCPOLARITY_HIGH);

    // �� TIM2_OCxInit ����������Ϊ 0 ʱ����ô��ʼ�ĵ�ƽ�Ͳ��ǵ͵�ƽ������ͨ�����º���������һ�Ρ�
    TIM2_PWMO_SetDuty(0);

    // ����жϱ�־
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    
    // �����жϺ�����������ֵ�������
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    
    // ����ʱ��
    TIM2_Cmd(ENABLE);
    
    // ���벶��/�Ƚ����ʹ��
    // ���´���� TIM2_OC1Init() �еĵڶ�����������һ�£�����֮����û��ֱ���� TIM2_OC1Init() ��ʼʱֱ������Ϊ ENABLE������Ϊ��ʵ�ʵ��������Ŀ�з��֣�
    // ����� TIM2_OC1Init() ֱ�ӳ�ʼΪ ENABLE����ô��λ��Ƭ��ʱ����������̵ĸߵ�ƽ����ô�ͻ���ʹ�����һ������ǡ�������� ENABLE ���õ� TIM2_Cmd(ENABLE) ֮��
    // ��ô��λ��Ƭ��ʱ���Ͳ���������̵ĸߵ�ƽ������� TIM2 ��ʼ�����Ĭ������͵�ƽ��
    TIM2_CCxCmd(TIM2_CHANNEL_1, ENABLE); 
}


/**
 * ���� TIM2 ͨ���� PWM ��ռ�ձ�
 *
 * @param double duty  ��ռ�ձ�ֵ
 */
void TIM2_PWMO_SetDuty( double duty )
{
    // ��ʼ����
    if( duty < 0 ) duty = 0;
    if( duty > 100 ) duty = 100;
 
    // ����ʹ���� TIM2_OCMODE_PWM2����ô PWM ����ǰ�Ľ����һ����Ч��ƽ��
    // ������ TIM2_OCPOLARITY_HIGH ���������ã���ָ������Ч��ƽ�Ǹߵ�ƽ����Ч��ƽ�ǵ͵�ƽ������ PWM ����ǰ�Ľ�����͵�ƽ
    // ��ô�����ʱ�������ò��� duty = 73��ʵ���Ͼ������ 73% �ĵ͵�ƽ����ռ�ձȣ�����������˵��ռ�ձ�����ռ�ձȡ�������Ҫ���� 100-duty ����ȡ��������
    duty = 100 - duty;
    
    // ����ռ�ձ�ֵ
    TIM2_SetCompare1(((u32)(duty*GV_Tim2PwmoArr))/100);  // duty/100 = x/GV_Tim2PwmoArr  =>  x = duty*GV_Tim2PwmoArr/100
}


/**
 * ���� TIM2 ͨ���� PWM ���
 *
 * �ú������ú󲻻��������� PWM�����ǳ�ʼ��صķ��Ͳ���������� ��ʱ�������ж� �������Щ�������� PWM ����
 * ��Ҫԭ�򣺸���ͨ����������ʱ�������ʱ TIM2_CNT ��Ϊ 0 ʱ����ô�ͻᵼ��PWM ��һ�����ڵĲ����쳣�������� ��ʱ�������ж� ���������;���ȷ�� TIM2_CNT Ϊ 0
 *
 * �������ú� GV_Tim2PwmoSendCount ��������Ϊ 1�����ʹ������ǰ������Ҫ���ǵ������������һ��ʱ����� GV_Tim2PwmoSendCount Ϊ 0����ô�ͱ�ʾ���巢����ϡ�
 *
 * @param double duty       ��ռ�ձ�ֵ����������ռ�ձȣ�����������
 * @param u32    sendLimit  ���巢�͸������ƣ�0 ��ʾ������
 * 
 * @return boolean  true ��ʾ���ͳɹ���false ��ʾ����ʧ��
 */
bool TIM2_PWMO_Send( double duty, u32 sendLimit )
{
    // ��ʼ������صı���
    // ��Ȼ��û���Ϸ��ͣ����Ǵ˴�ֱ�����÷��ͼ���Ϊ1��GV_Tim2PwmoSendCount = 1��
    // ��Ҫ���ǵ�������������ִ�����Ҫ���� GV_Tim2PwmoSendCount ����ֵ�����ж��Ƿ����ڷ��͵�״̬
    GV_Tim2PwmoSendSoon = 1;
    GV_Tim2PwmoSendSoonDuty = duty;
    
    GV_Tim2PwmoSendCount = 1;
    GV_Tim2PwmoSendLimit = sendLimit;

    // ����
    return true;
}


/**
 * ��ֹ TIM2 ͨ���� PWM ���
 * �ú�����Ҫ���ý� ��ռ�ձ�����Ϊ0���͵�ƽ�� ��ģ�� ֹͣ��ʱ��ͨ��
 */
void TIM2_PWMO_Stop()
{
    // �������״̬
    if( GV_Motor == true ) GV_MotorStatePrev = GV_MotorState;
    if( GV_Motor == true ) GV_MotorState = MOTOR_STATE_IDLE;

    // ��ֹ PWM ���
    TIM2_PWMO_SetDuty(0);
    GV_Tim2PwmoSendLimit = GV_Tim2PwmoSendCount = 0; 
    GV_Tim2PwmoSendSoon = 0; GV_Tim2PwmoSendSoonDuty = 0;
}



/* -------------------------------------------------- */
// �� �������壨PWM ���벶��
// �� PCB ��·���� TIM2 ������Ϊ 55
// �� ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM2_TimeBaseInit ���������໥���ţ�����ʹ�� TIM2 ������һ�����ܣ�PWM��� �� PWM���벶��
/* -------------------------------------------------- */

/**
 * ��ʼ�� TIM2 PWM ����
 */
void TIM2_PWMI_Init()
{
    // ���ó�ʼ��
    TIM2_DeInit();

    // ��ʼ�� TIME1 ʱ����Ԫ
    // 16Ԥ��Ƶ�����ϼ�����ÿ 1s/1M = 1us ������ֵ�� 1���ۼ� 1000 ����Ҫ 1 ���룩
    // ����˵����Ԥ����ֵ���Զ���װ��ֵ��������ֵ�� 65536 ������װ�أ�
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 65536-1);

    // PWM��ͨ��1����
    // PWM��ͨ��2�������ã���Ϊ TIM1_PWMIConfig(...) �ڲ��Ѿ�����������
    //
    // ����˵����
    //����1. TIM2 ͨ��1
    //����2. ���ԣ������ز���
    //����3. ����ţ�TIM2_ICSELECTION_DIRECTTI ��ʾֱ�ӽ� T1 �źž��� TI1FP1 ���͸� IC1 ģ����д���
    //����4. Ԥ��Ƶ����˼�ǿ����ڶ��ٸ�����������һ�β��񣻡�����ʵ�ʲ��ԣ���λ����ģʽ�¸�ֵ��Ч�����ǰ���1��Ƶִ�С�
    //       ��������1M ϵͳʱ�ӣ����� 20Hz PWM һ��������Ҫ���� 50000  �Σ�1M/20=50000����TIM2 ��������65536�����Բ���
    //       ��������1M ϵͳʱ�ӣ����� 15Hz PWM һ��������Ҫ���� 66666  �Σ�1M/15=66666����TIM2 ��������65536���޷�����
    //����5. �˲�Ƶ�ʣ���������������ͬ�����������ȶ������Ƶ�ʲ���ż��ֵ���ԣ���ô�������� 0x02��4���¼���
    TIM2_PWMIConfig(TIM2_CHANNEL_1, TIM2_ICPOLARITY_RISING, TIM2_ICSELECTION_DIRECTTI, TIM2_ICPSC_DIV1, 0x00);

    // ���벶��/�Ƚ����ʹ��
    //������ʵ�ʲ��ԣ�����ʹ�ܴ������ע�͵�������Ϊ�˰��Ļ��Ǽ���ȥ��
    TIM2_CCxCmd(TIM2_CHANNEL_1, ENABLE);
    TIM2_CCxCmd(TIM2_CHANNEL_2, ENABLE);
}
void TIM2_PWMI_InitData()
{
    GV_Tim2PwmiFreq = 0;
    GV_Tim2PwmiDuty = 0.0;
}


/**
 * ��ʼ TIM2 PWM ����
 */
void TIM2_PWMI_Start()
{
    // ��շ����жϱ�־
    TIM2_ClearITPendingBit(TIM2_IT_CC1);
    TIM2_ClearFlag(TIM2_FLAG_CC1);
    
    // �����жϺ���������Ƚ�ͨ��CH1�ж�
    TIM2_ITConfig(TIM2_IT_CC1, ENABLE);

    // ����ʱ��
    //������ʵ�ʲ��ԣ���ʱ������δ����������� TIM2_IT_CC1 �ж����������룬���ǻ�ȡ�ļ�����ֵ��ȷ����
    TIM2_Cmd(ENABLE);
}


/**
 * ��ֹ TIM2 PWM ����
 * ���� TIM2_Cmd(DISABLE) �ر������ע����������������Ӱ�죬���磺TIM2_PWMI_Init
 */
void TIM2_PWMI_Stop()
{
    // �ر��жϺ���������Ƚ�ͨ��CH1�ж�
    TIM2_ITConfig(TIM2_IT_CC1, DISABLE);

    // �رն�ʱ��
    TIM2_Cmd(DISABLE);
}


/**
 * ��ȡ TIM2 ����� PWM Ƶ��
 *
 * @return integer  Ƶ��
 */
u16 TIM2_PWMI_GetFreq()
{
    return GV_Tim2PwmiFreq;
}


/**
 * ��ȡ TIM2 ����� PWM ��ռ�ձ�
 *
 * @return double  ռ�ձ�
 */
double TIM2_PWMI_GetDuty()
{
    return GV_Tim2PwmiDuty;
}



/* -------------------------------------------------- */
// �� �жϻص�����
//   ��������ڲμ� stm8s_it.c
/* -------------------------------------------------- */

/**
 * TIM2 �ж�
 */
void TIM2_InterruptHandler()
{
    // �жϣ�������ֵ�������
    if( TIM2_GetITStatus(TIM2_IT_UPDATE) ) TIM2_InterruptHandlerUpdate();
        
    // �жϣ�PWM ����
    // ���� PWM = 0 ���� 100 �Ĳ��������⣬��Ϊ������ PWM ֵ���޷������ж�
    if( TIM2_GetITStatus(TIM2_IT_CC1) != RESET ) TIM2_InterruptHandlerCapturePwm();
}


/**
 * TIM2 �жϣ�������ֵ�������
 */
void TIM2_InterruptHandlerUpdate()
{
    // �޼������͵����� �� ���÷�������
    if( GV_Tim2PwmoSendSoon == 0 && GV_Tim2PwmoSendLimit > 0 ){
        // ������͸����������Ƹ�������ô��ͣ���
        if( GV_Tim2PwmoSendCount++ >= GV_Tim2PwmoSendLimit ) TIM2_PWMO_Stop();  
    }

    // �м������͵�����
    if( GV_Tim2PwmoSendSoon == 1 ){
        // ������������
        if( GV_Tim2PwmoSendSoonDuty >= 0 ) TIM2_PWMO_SetDuty(GV_Tim2PwmoSendSoonDuty);
        GV_Tim2PwmoSendSoon = 0; GV_Tim2PwmoSendSoonDuty = 0;

        // ������б��
        // ��Ϊ����� TIM2 �� PWM ���ƣ�ͬʱ TIM2 ��һֱ���У��������ʱ������ռ�ձ� 0% ���壬��һֱ���ϵĽ��� TIM2_InterruptHandlerUpdate �жϺ�����
        // ���ǵ������ʱ�����Ǿ���Ϊ�����ͣ������������е�������жϣ����Ա�����һ�� ������жϿ�ʼ�� �ı�Ǳ������Ա���߳���Ҫ��ʼ�������жϺ�����
        if( GV_Motor == true && GV_MotorState == MOTOR_STATE_IDLE ) GV_MotorState = MOTOR_STATE_RUN;
    }

    // ��������ж�
    if( GV_Motor == true && GV_MotorState != MOTOR_STATE_IDLE ) MOTOR_InterruptHandlerPulse();

    // ����жϱ�־
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
}


/**
 * TIM2 �жϣ�PWM ����
 */
void TIM2_InterruptHandlerCapturePwm()
{
    // ��ʼ����ʱ�����벶��ֵ
    // �ֱ��Ӧһ��PWM���ڵĵ�һ�������أ���һ���½��أ��ڶ���������
    static u16 icValueA, icValueB, icValueC;

    // ��ȡ��ʱ�����벶��ֵ
    if( icValueA == 0 ){
        icValueA = TIM2_GetCapture1();
    }else{
        icValueB = TIM2_GetCapture2();
        icValueC = TIM2_GetCapture1();
        
        // ����ʱ������ֵ������
        if( icValueC < icValueA ) icValueC = icValueC-icValueA+65535;
        else icValueC = icValueC - icValueA;
 
        if( icValueB < icValueA ) icValueB = icValueB-icValueA+65535;
        else icValueB = icValueB - icValueA;

        // ����Ƶ�ʺ�����
        // ����� 1000000 �ǻ��ڳ�ʼ�������е�ϵͳʱ�����ã�16Ԥ��Ƶ = 1000000��
        // ����� 100.0 С�����ʾ�����ʽ icValueB*100.0/icValueC �Ĺ��̺ͽ���Ը�����������
        GV_Tim2PwmiFreq = 1000000/icValueC;
        GV_Tim2PwmiDuty = icValueB*100.0/icValueC;

        // ���ö�ʱ�����벶��ֵ
        icValueA = icValueB = icValueC = 0;
    }

    // ����жϱ�־λ
    TIM2_ClearITPendingBit(TIM2_IT_CC1);    
    TIM2_ClearFlag(TIM2_FLAG_CC1);
}