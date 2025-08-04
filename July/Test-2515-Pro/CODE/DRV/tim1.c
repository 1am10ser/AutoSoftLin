#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * TIM1 PWM �����������
 * PCB ��·���� TIM1 ������Ϊ 56��57
 * ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM1_TimeBaseInit ���������໥���ţ�����ʹ�� TIM1 ������һ�����ܣ�PWM��� �� PWM���벶��
 */
short  GV_Tim1PwmoArr = 0;  //��ϵͳʹ�á���������װ��ֵ���Զ�����Ƶ�ʼ���
short  GV_Tim1PwmoNumbers[] = {56, 57};  //���������á�PWM��� ��ż���ֻ�н� PWM��� ��ż��뵽������������ʵ�ֶ�Ӧ PWM��� ����ع���

short  GV_Tim1PwmoSendSoon56     = 0;  //��ϵͳʹ�á��������ͱ��
double GV_Tim1PwmoSendSoon56Duty = 0;  //��ϵͳʹ�á��������͵�ռ�ձ�����
u32    GV_Tim1PwmoSendLimit56    = 0;  //��ϵͳʹ�á����͸�������
short  GV_Tim1PwmoSendCount56    = 0;  //��ϵͳʹ�á����͸����ۼ�
short  GV_Tim1PwmoSendSoon57     = 0;  //��ϵͳʹ�á��������ͱ��
double GV_Tim1PwmoSendSoon57Duty = 0;  //��ϵͳʹ�á��������͵�ռ�ձ�����
u32    GV_Tim1PwmoSendLimit57    = 0;  //��ϵͳʹ�á����͸�������
short  GV_Tim1PwmoSendCount57    = 0;  //��ϵͳʹ�á����͸����ۼ�


/**
 * TIM1 PWM ���벶���������
 * PCB ��·���� TIM1 ������Ϊ 56��57
 * ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM1_TimeBaseInit ���������໥���ţ�����ʹ�� TIM1 ������һ�����ܣ�PWM��� �� PWM���벶��
 */
short  GV_Tim1PwmiNumbers[] = {56, 57};  //���������á�PWM���� ��ż���ֻ�н� PWM��� ��ż��뵽������������ʵ�ֶ�Ӧ PWM��� ����ع���

u16    GV_Tim1PwmiFreq56 = 0;    //��ϵͳʹ�á���ʱ������� PWM Ƶ��
double GV_Tim1PwmiDuty56 = 0.0;  //��ϵͳʹ�á���ʱ������� PWM ��ռ�ձ�

u16    GV_Tim1PwmiFreq57 = 0;    //��ϵͳʹ�á���ʱ������� PWM Ƶ��
double GV_Tim1PwmiDuty57 = 0.0;  //��ϵͳʹ�á���ʱ������� PWM ��ռ�ձ�



/* -------------------------------------------------- */
// �� �������壨PWM�����
// �� PCB ��·���� TIM1 ������Ϊ 56��57
// �� ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM1_TimeBaseInit ���������໥���ţ�����ʹ�� TIM1 ������һ�����ܣ�PWM��� �� PWM���벶��
/* -------------------------------------------------- */

/**
 * ��ʼ�� TIM1 PWM ���
 * ��ʼ����˿ڲ�������� PWM �źţ���Ҫ���� TIM1_PWMO_Send(...) �������
 *
 * @param int   freq    PWM ���Ƶ�ʣ�100 < freq < 450000��16��Ƶ��������ȽϺò��Σ�ռ�ձȽ�׼��
 *                          ����������100 < freq <  40000��160��Ƶ��������ȽϺò��Σ�ռ�ձȽ�׼��
 */
void TIM1_PWMO_Init( u32 freq )
{
    // Ԥ��Ƶ
    u16 prescaler = 16;
    
    // ���ó�ʼ��
    TIM1_DeInit();
    
    // ��ʼ����������װ��ֵ
    // ����ϵͳʱ��Ƶ�ʡ���Ƶ�ʺ�����Ƶ�������������װ��ֵ
    GV_Tim1PwmoArr = 16000000/prescaler/freq;

    // ��ʼ�� TIM1 ʱ����Ԫ
    // 16Ԥ��Ƶ�����ϼ�����ÿ 1s/1M = 1us ������ֵ�� 1���ۼ� 1000 ����Ҫ 1 ���룩
    // ����˵����Ԥ����ֵ������ģʽ���Զ���װ��ֵ��������ֵ�� GV_Tim1PwmoArr ������װ�أ�����������ʼֵ��������ֵ����װ�غ��0��ʼ��
    TIM1_TimeBaseInit(prescaler-1, TIM1_COUNTERMODE_UP, GV_Tim1PwmoArr-1, 0);

    // �Զ�Ԥװ������λ
    // ARR��Auto-Reload Register
    TIM1_ARRPreloadConfig(ENABLE);

    // ��ʼ�� TIM1 ͨ��
    for( int i=0; i < ARR_LENGTH(GV_Tim1PwmoNumbers); i++ ){
        // ��ʼ��ͨ���ķ���/�Ƚ�
        // �˴��������� TIM1_OCMODE_PWM2+TIM1_OCPOLARITY_HIGH��ȷ����ʼʱΪ�͵�ƽ
        //
        // ����˵����
        //����1. PWM���ģʽ��TIM1_OCMODE_PWM1������ǰ�����Ч��ƽ����TIM1_OCMODE_PWM2������ǰ�����Ч��ƽ��
        //����      ��������������ǰ��ָ��ʱ��ֵС�ڱȽ����趨��ֵ
        //����2. ��ֹ������������Ƚ�
        //����3. ��ֹ�������û�������Ƚ�
        //����4. ������ֵ�������ʱ���Զ�װ������Ϊ 2000��������ֵ��������Ϊ 1000����ô����� 50% ��ռ�ձȡ�ͬʱ��ֵ���ڿ��Զ�̬����
        //����5. ����Ƚϼ��ԣ���Ҫ��� ����1 ȷ������ǰ����Ч��ƽ���μ�������ȭSTM8 39 ����PWM ���ʵ�顷����ͨ�� TIM1_OCMODE_PWM2+TIM1_OCPOLARITY_LOW �ȽϷ����˵ĳ���˼ά
        //����   ���������������˴�����ʹ�� TIM1_OCMODE_PWM2+TIM1_OCPOLARITY_HIGH ��ʽ����Ҫ��ȷ��PWM��ʼ��Ĭ������͵�ƽ����ģʽ��ͨ�� TIM1_SetCompare ���õ�ռ�ձ�ֵ���Ǹ�ռ�ձ��ˣ�����ʱע��
        //����6. ��������Ƚϼ��ԣ����������Ժ����о���
        //����7. ����ȽϿ���״̬�����������Ժ����о���
        //����8. ��������ȽϿ���״̬�����������Ժ����о���
        if( GV_Tim1PwmoNumbers[i] == 56 ) TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_DISABLE, TIM1_OUTPUTNSTATE_DISABLE, 0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);
        if( GV_Tim1PwmoNumbers[i] == 57 ) TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_DISABLE, TIM1_OUTPUTNSTATE_DISABLE, 0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);
    
        // �� TIM1_OCxInit ���ĸ�����Ϊ 0 ʱ����ô��ʼ�ĵ�ƽ�Ͳ��ǵ͵�ƽ������ͨ�����º���������һ�Ρ�
        if( GV_Tim1PwmoNumbers[i] == 56 ) TIM1_PWMO_SetDuty(56, 0);
        if( GV_Tim1PwmoNumbers[i] == 57 ) TIM1_PWMO_SetDuty(57, 0);
    }

    // ����жϱ�־
    TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
    TIM1_ClearFlag(TIM1_FLAG_UPDATE);
    
    // �����жϺ�����������ֵ�������
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
   
    // ���������
    // ����򿪣������޷����PWM
    TIM1_CtrlPWMOutputs(ENABLE);

    // ����ʱ��
    TIM1_Cmd(ENABLE);

    // ���벶��/�Ƚ����ʹ��
    // ���´���� TIM1_OCxInit() �еĵڶ�����������һ�£�һ���÷���ֱ���� TIM1_OCxInit() ��ʼʱֱ������Ϊ ENABLE
    // ����һ���÷����� TIM1_Cmd(ENABLE) ֮����� TIM1_CCxCmd(.., ENABLE) ����ʹ�ܣ���Ϊ TIM1_OCxInit() ��ʼ��ʱ��ֱ������Ϊ ENABLE �Ļ���
    // ��ô��λ��Ƭ��ʱ��TIM1 �Ϳ���������̵ĸߵ�ƽ����һ�ε����Ŀ�����������������Ŀ��������̵ĸߵ�ƽ���У���ô������ TIM1_Cmd(ENABLE) ֮������ TIM1_CCxCmd(.., ENABLE)
    //TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE); 
    //TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE); 
}


/**
 * ���� TIM1 ͨ���� PWM ��ռ�ձ�
 *
 * @param short number  PCB�ϵı�ţ�56, 57, TIM1_NUMBER_ALL
 * @param double duty   ��ռ�ձ�ֵ
 */
void TIM1_PWMO_SetDuty( short number, double duty )
{
    // ��ʼ����
    if( duty < 0 ) duty = 0;
    if( duty > 100 ) duty = 100;
 
    // ����ʹ���� TIM1_OCMODE_PWM2����ô PWM ����ǰ�Ľ����һ����Ч��ƽ��
    // ������ TIM1_OCPOLARITY_HIGH ���������ã���ָ������Ч��ƽ�Ǹߵ�ƽ����Ч��ƽ�ǵ͵�ƽ������ PWM ����ǰ�Ľ�����͵�ƽ
    // ��ô�����ʱ�������ò��� duty = 73��ʵ���Ͼ������ 73% �ĵ͵�ƽ����ռ�ձȣ�����������˵��ռ�ձ�����ռ�ձȡ�������Ҫ���� 100-duty ����ȡ��������
    duty = 100 - duty;
    
    // ����ռ�ձ�ֵ
    if( number == TIM1_NUMBER_ALL || number == 56 ) TIM1_SetCompare1(((u32)(duty*GV_Tim1PwmoArr))/100);  // duty/100 = x/GV_Tim1PwmoArr  =>  x = duty*GV_Tim1PwmoArr/100
    if( number == TIM1_NUMBER_ALL || number == 57 ) TIM1_SetCompare3(((u32)(duty*GV_Tim1PwmoArr))/100);  // duty/100 = x/GV_Tim1PwmoArr  =>  x = duty*GV_Tim1PwmoArr/100
}


/**
 * ���� TIM1 ͨ���� PWM ���
 * �������˷���������������ô������Զ��� ��ʱ�������ж� ���Ⲣ��������ֵ��ֹͣ ��ʱ��ͨ�� ���ʹ��
 *
 * �ú������ú󲻻��������� PWM�����ǳ�ʼ��صķ��Ͳ���������� ��ʱ�������ж� �������Щ�������� PWM ����
 * ��Ҫԭ�򣺸���ͨ����������ʱ�������ʱ TIM1_CNT ��Ϊ 0 ʱ����ô�ͻᵼ��PWM ��һ�����ڵĲ����쳣�������� ��ʱ�������ж� ���������;���ȷ�� TIM1_CNT Ϊ 0
 *
 * �������ú� GV_Tim1PwmoSendCountXX ��������Ϊ 1�����ʹ������ǰ������Ҫ���ǵ������������һ��ʱ����� GV_Tim1PwmoSendCountXX Ϊ 0����ô�ͱ�ʾ���巢����ϡ�
 *
 * @param short  number     PCB�ϵı�ţ�56, 57, TIM1_NUMBER_ALL
 * @param double duty       ��ռ�ձ�ֵ����������ռ�ձȣ�����������
 * @param u32    sendLimit  ���巢�͸������ƣ�0 ��ʾ������
 * 
 * @return boolean  true ��ʾ���ͳɹ���false ��ʾ����ʧ��
 */
bool TIM1_PWMO_Send( short number, double duty, u32 sendLimit )
{
    // PCB �ϵı�� 56 �Ķ˿� �� ȫ���˿�
    if( number == 56 || number == TIM1_NUMBER_ALL ){
        // ��ʼ������صı���
        // ��Ȼ��û���Ϸ��ͣ����Ǵ˴�ֱ�����÷��ͼ���Ϊ1��GV_Tim1PwmoSendCount56 = 1��
        // ��Ҫ���ǵ�������������ִ�����Ҫ���� GV_Tim1PwmoSendCount56 ����ֵ�����ж��Ƿ����ڷ��͵�״̬
        GV_Tim1PwmoSendSoon56 = 1;
        GV_Tim1PwmoSendSoon56Duty = duty;
        
        GV_Tim1PwmoSendCount56 = 1;
        GV_Tim1PwmoSendLimit56 = sendLimit;
        
        // ����ͨ��
        TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE); 
    }
    
    // PCB �ϵı�� 57 �Ķ˿� �� ȫ���˿�
    if( number == 57 || number == TIM1_NUMBER_ALL ){
        // ��ʼ������صı���
        // ��Ȼ��û���Ϸ��ͣ����Ǵ˴�ֱ�����÷��ͼ���Ϊ1��GV_Tim1PwmoSendCount57 = 1��
        // ��Ҫ���ǵ�������������ִ�����Ҫ���� GV_Tim1PwmoSendCount57 ����ֵ�����ж��Ƿ����ڷ��͵�״̬
        GV_Tim1PwmoSendSoon57 = 1;
        GV_Tim1PwmoSendSoon57Duty = duty;
        
        GV_Tim1PwmoSendCount57 = 1;
        GV_Tim1PwmoSendLimit57 = sendLimit;
                
        // ����ͨ��
        TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE); 
    }

    // ����
    return true;
}


/**
 * ��ֹ TIM1 ͨ���� PWM ���
 * �ú�����Ҫ���ý� ��ռ�ձ�����Ϊ0���͵�ƽ�� ��ģ�� ֹͣ��ʱ��ͨ��
 *
 * @param short number  PCB�ϵı�ţ�56, 57, TIM1_NUMBER_ALL
 */
void TIM1_PWMO_Stop( short number )
{
    // �ر� TIM1 CH1 ͨ��
    if( number == 56 || number == TIM1_NUMBER_ALL ){
        TIM1_PWMO_SetDuty(56, 0);
        // TIM1_CCxCmd(TIM1_CHANNEL_1, DISABLE); 

        GV_Tim1PwmoSendLimit56 = GV_Tim1PwmoSendCount56 = 0; 
        GV_Tim1PwmoSendSoon56 = 0; GV_Tim1PwmoSendSoon56Duty = 0;
    }
    
    // �ر� TIM1 CH2 ͨ��
    if( number == 57 || number == TIM1_NUMBER_ALL ){ 
        TIM1_PWMO_SetDuty(57, 0);
        // TIM1_CCxCmd(TIM1_CHANNEL_3, DISABLE); 
        
        GV_Tim1PwmoSendLimit57 = GV_Tim1PwmoSendCount57 = 0;
        GV_Tim1PwmoSendSoon57 = 0; GV_Tim1PwmoSendSoon57Duty = 0;
    }
}



/* -------------------------------------------------- */
// �� �������壨PWM ���벶��
// �� PCB ��·���� TIM1 ������Ϊ 56��57
// �� ���ǵ���ͬͨ�����/����ʱ��ͬʱ���� TIM1_TimeBaseInit ���������໥���ţ�����ʹ�� TIM1 ������һ�����ܣ�PWM��� �� PWM���벶��
/* -------------------------------------------------- */

/**
 * ��ʼ�� TIM1 PWM ����
 */
void TIM1_PWMI_Init()
{
    // ���ó�ʼ��
    TIM1_DeInit();
    
    // ��ʼ�� TIM1 ʱ����Ԫ
    // 16Ԥ��Ƶ�����ϼ�����ÿ 1s/1M = 1us ������ֵ�� 1���ۼ� 1000 ����Ҫ 1 ���룩
    // ����˵����Ԥ����ֵ������ģʽ���Զ���װ��ֵ��������ֵ�� 65536 ������װ�أ�����������ʼֵ��������ֵ����װ�غ��0��ʼ��
    TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 65536-1, 0);

    // ��ʼ�� TIM1 ͨ��
    for( int i=0; i < ARR_LENGTH(GV_Tim1PwmiNumbers); i++ ){
        if( GV_Tim1PwmiNumbers[i] == 56 ) TIM1_PWMI_Init56();
        if( GV_Tim1PwmiNumbers[i] == 57 ) TIM1_PWMI_Init57();
    }

}
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
    TIM1_PWMIConfig(TIM1_CHANNEL_1, TIM1_ICPOLARITY_RISING, TIM1_ICSELECTION_DIRECTTI, TIM1_ICPSC_DIV1, 0x02);

    // ���벶��/�Ƚ����ʹ��
    //������ʵ�ʲ��ԣ�����ʹ�ܴ������ע�͵�������Ϊ�˰��Ļ��Ǽ���ȥ��
    TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
    TIM1_CCxCmd(TIM1_CHANNEL_2, ENABLE);
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
    TIM1_ICInit(TIM1_CHANNEL_4, TIM1_ICPOLARITY_FALLING, TIM1_ICSELECTION_INDIRECTTI, TIM1_ICPSC_DIV1, 0x02);

    // ���벶��/�Ƚ����ʹ��
    //������ʵ�ʲ��ԣ�����ʹ�ܴ������ע�͵�������Ϊ�˰��Ļ��Ǽ���ȥ��
    TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE);
    TIM1_CCxCmd(TIM1_CHANNEL_4, ENABLE);
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
 * ��ʼ TIM1 PWM ����
 */
void TIM1_PWMI_Start()
{
    // ��շ����жϱ�־
    TIM1_ClearITPendingBit(TIM1_IT_CC1);
    TIM1_ClearFlag(TIM1_FLAG_CC1);

    TIM1_ClearITPendingBit(TIM1_IT_CC3);
    TIM1_ClearFlag(TIM1_FLAG_CC3);

    // �����жϺ���������Ƚ�ͨ��CH1�ж�
    TIM1_ITConfig(TIM1_IT_CC1, ENABLE);
    TIM1_ITConfig(TIM1_IT_CC3, ENABLE);

    // ����ʱ��
    //������ʵ�ʲ��ԣ���ʱ������δ����������� TIM1_IT_CC1 �ж����������룬���ǻ�ȡ�ļ�����ֵ��ȷ����
    TIM1_Cmd(ENABLE);
}


/**
 * ��ֹ TIM1 PWM ����
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
 * ��ȡ TIM1 ����� PWM Ƶ��
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
 * ��ȡ TIM1 ����� PWM ��ռ�ձ�
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
    if( TIM1_GetITStatus(TIM1_IT_UPDATE) ) TIM1_InterruptHandlerUpdate();
    
    // �жϣ�PWM ����
    // ���� PWM = 0 ���� 100 �Ĳ��������⣬��Ϊ������ PWM ֵ���޷������ж�
    if( TIM1_GetITStatus(TIM1_IT_CC1) != RESET ) TIM1_InterruptHandlerCapturePwm56();
    if( TIM1_GetITStatus(TIM1_IT_CC3) != RESET ) TIM1_InterruptHandlerCapturePwm57();
}


/**
 * TIM1 �жϣ�������ֵ�������
 */
void TIM1_InterruptHandlerUpdate()
{
    // �޼������͵����� �� ���÷�������
    if( GV_Tim1PwmoSendSoon56 == 0 && GV_Tim1PwmoSendLimit56 > 0 ){
        if( GV_Tim1PwmoSendCount56++ >= GV_Tim1PwmoSendLimit56 ) TIM1_PWMO_Stop(56);  // ������͸����������Ƹ�������ô��ͣ���
    }
    if( GV_Tim1PwmoSendSoon57 == 0 && GV_Tim1PwmoSendLimit57 > 0 ){
        if( GV_Tim1PwmoSendCount57++ >= GV_Tim1PwmoSendLimit57 ) TIM1_PWMO_Stop(57);  // ������͸����������Ƹ�������ô��ͣ���
    }

    // �м������͵�����
    if( GV_Tim1PwmoSendSoon56 == 1 ){
        if( GV_Tim1PwmoSendSoon56Duty >= 0 ) TIM1_PWMO_SetDuty(56, GV_Tim1PwmoSendSoon56Duty);
        GV_Tim1PwmoSendSoon56 = 0; GV_Tim1PwmoSendSoon56Duty = 0;
    }
    if( GV_Tim1PwmoSendSoon57 == 1 ){
        if( GV_Tim1PwmoSendSoon57Duty >= 0 ) TIM1_PWMO_SetDuty(57, GV_Tim1PwmoSendSoon57Duty);
        GV_Tim1PwmoSendSoon57 = 0; GV_Tim1PwmoSendSoon57Duty = 0;
    }
    
    // ����жϱ�־
    TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
    TIM1_ClearFlag(TIM1_FLAG_UPDATE);
}


/**
 * TIM1 �жϣ�PWM ����J56�ӿڣ�
 */
void TIM1_InterruptHandlerCapturePwm56()
{
    // ��ʼ����ʱ�����벶��ֵ
    // �ֱ��Ӧһ��PWM���ڵĵ�һ�������أ���һ���½��أ��ڶ���������
    static u16 icValueA, icValueB, icValueC;

    // ��ȡ��ʱ�����벶��ֵ
    if( icValueA == 0 ){
        icValueA = TIM1_GetCapture1();
    }else{
        icValueB = TIM1_GetCapture2();
        icValueC = TIM1_GetCapture1();

        // ����ʱ������ֵ������
        if( icValueC < icValueA ) icValueC = icValueC-icValueA+65535;
        else icValueC = icValueC - icValueA;
 
        if( icValueB < icValueA ) icValueB = icValueB-icValueA+65535;
        else icValueB = icValueB - icValueA;

        // ����Ƶ�ʺ�����
        // ����� 1000000 �ǻ��ڳ�ʼ�������е�ϵͳʱ�����ã�16Ԥ��Ƶ = 1000000��
        // ����� 100.0 С�����ʾ�����ʽ icValueB*100.0/icValueC �Ĺ��̺ͽ���Ը�����������
        GV_Tim1PwmiFreq56 = 1000000/icValueC;
        GV_Tim1PwmiDuty56 = icValueB*100.0/icValueC;
        
        // ռ�ձȲ���
        // �������ۣ�icValueC �ǿ϶����� icValueB������ʵ�ʲ����У�������ռ�ձ� <= 1 �� PWM ʱ��icValueB ����� icValueC����һ���Ѿ������������߼���
        // �� icValueB > icValueC������Ĵ�����������ռ�ձȽ���� > 100���������Ƕ�������� ���� ���㣬ʵ�ⷢ��ռ�ձ� <= 1 �ľ�����ȷ��
        GV_Tim1PwmiDuty56 = GV_Tim1PwmiDuty56 > 100 ? GV_Tim1PwmiDuty56-100.0 : GV_Tim1PwmiDuty56;

        // ���ö�ʱ�����벶��ֵ
        icValueA = icValueB = icValueC = 0;
    }

    // ����жϱ�־λ
    TIM1_ClearITPendingBit(TIM1_IT_CC1);    
    TIM1_ClearFlag(TIM1_FLAG_CC1);
}


/**
 * TIM1 �жϣ�PWM ����J57�ӿڣ�
 */
void TIM1_InterruptHandlerCapturePwm57()
{
    // ��ʼ����ʱ�����벶��ֵ
    // �ֱ��Ӧһ��PWM���ڵĵ�һ�������أ���һ���½��أ��ڶ���������
    static u16 icValueA, icValueB, icValueC;

    // ��ȡ��ʱ�����벶��ֵ
    if( icValueA == 0 ){
        icValueA = TIM1_GetCapture3();
    }else{
        icValueB = TIM1_GetCapture4();
        icValueC = TIM1_GetCapture3();
        
        // ����ʱ������ֵ������
        if( icValueC < icValueA ) icValueC = icValueC-icValueA+65535;
        else icValueC = icValueC - icValueA;
 
        if( icValueB < icValueA ) icValueB = icValueB-icValueA+65535;
        else icValueB = icValueB - icValueA;

        // ����Ƶ�ʺ�����
        // ����� 1000000 �ǻ��ڳ�ʼ�������е�ϵͳʱ�����ã�16Ԥ��Ƶ = 1000000��
        // ����� 100.0 С�����ʾ�����ʽ icValueB*100.0/icValueC �Ĺ��̺ͽ���Ը�����������
        GV_Tim1PwmiFreq57 = 1000000/icValueC;
        GV_Tim1PwmiDuty57 = icValueB*100.0/icValueC;
        
        // ռ�ձȲ���
        // �������ۣ�icValueC �ǿ϶����� icValueB������ʵ�ʲ����У�������ռ�ձ� <= 1 �� PWM ʱ��icValueB ����� icValueC����һ���Ѿ������������߼���
        // �� icValueB > icValueC������Ĵ�����������ռ�ձȽ���� > 100���������Ƕ�������� ���� ���㣬ʵ�ⷢ��ռ�ձ� <= 1 �ľ�����ȷ��
        GV_Tim1PwmiDuty57 = GV_Tim1PwmiDuty57 > 100 ? GV_Tim1PwmiDuty57-100.0 : GV_Tim1PwmiDuty57;

        // ���ö�ʱ�����벶��ֵ
        icValueA = icValueB = icValueC = 0;
    }

    // ����жϱ�־λ
    TIM1_ClearITPendingBit(TIM1_IT_CC3);    
    TIM1_ClearFlag(TIM1_FLAG_CC3);
}