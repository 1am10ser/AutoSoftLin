#include "common.h"



/* -------------------------------------------------- */
// �� ���� STEP/DIR ��ʽ����
// �� ���� TMC2209 оƬ����֧�� STEP/DIR �� UART ����
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * MOTOR ��ر���
 */
bool  GV_Motor = false;  //��ϵͳʹ�á��������

short GV_MotorDir = 0;  //��ϵͳʹ�á��������
short GV_MotorDiv = 0;  //��ϵͳʹ�á����ϸ��

long  GV_MotorSpeed     = 30;    //���������á���������ٶȣ���λ��RPM��Ȧ/���ӣ�
                                 //            ���ղ�����1.8�㣬ϸ��18����ôÿ������ 0.1��/Hz��1000Hz = 100��/��
                                 //            30RPM=0.5Ȧ/��=1800Hz��60RPM=1Ȧ/��=3600Hz��
long  GV_MotorSpeedFreq = 1800;  //��ϵͳʹ�á��������Ƶ��
                                 //            ��Ƶ��ֵ�� GV_MotorSpeed ֱ�ӹ������˴�Ԥ�����30RPMʱ��Ƶ�ʣ�ƽʱʹ��ʱ MOTOR_InitSpeed() �Զ�����

short GV_MotorState     = 0;  //��ϵͳʹ�á������ǰ״̬���μ� motor.h �е� MOTOR_STATE_XXX ״ֵ̬��
short GV_MotorStatePrev = 0;  //��ϵͳʹ�á������һ��״̬�����ֹͣ��ǰ״̬���Զ�����Ϊ��һ��״̬��

long  GV_MotorCountPulse  = 0;  //��ϵͳʹ�á����������ۼ�������
long  GV_MotorCountCircle = 0;  //��ϵͳʹ�á����������ߵ���Ȧ��������ʾ˳ʱ�룬����ʾ��ʱ�룩
long  GV_MotorCountDegree = 0;  //��ϵͳʹ�á����������ߵĵ�Ȧ����������ʾ˳ʱ�룬����ʾ��ʱ�룬ͬʱ����360���ͳ�Ƶ�Ȧ�������У��Ƕ�ֵ�Ŵ�10000���洢�������˸��㱣֤�˾��ȣ�



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ��ʼ��
 * Ĭ�ϵ�ϸ�֡�˳ʱ�롢������á�TIM2��ʼ��
 */
void MOTOR_Init()
{
    // ��ʼ��ϸ��
    GPIO_Init(MOTOR_DIV_PORT, MOTOR_DIV_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);
    MOTOR_SetDiv(MOTOR_DIV_LOW);

    // ��ʼ������
    MOTOR_InitCount();

    // ��ʼ���������
    // ����ԭ��ͼ������̬ʱ DIR ��Ϊ�ߵ�ƽ�� �͵�ƽʱ DIR ��Ϊ�͵�ƽ
    //
    // ���ǹ涨�ߵ�ƽʱ���˳ʱ����ת��������ǰ�ߣ����͵�ƽʱ�����ʱ����ת����������ߣ���
    // ������Ҫ����ʵ������������������оƬ������ߣ�ʹ֮���������Ĺ���
    GPIO_Init(MOTOR_DIR_PORT, MOTOR_DIR_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);  // ���ٿ�©�������̬
    MOTOR_SetDir(MOTOR_DIR_CW);

    // ��ʼ������
    GPIO_Init(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN, GPIO_MODE_OUT_PP_LOW_FAST);  // ������������͵�ƽ
    MOTOR_Enable();

    // ��ʼ���������Ƶ��
    TIM2_PWMO_Init(GV_MotorSpeedFreq);

    // ��ʼ�����
    GV_Motor = true;
}


/**
 * ��ʼ������
 */
void MOTOR_InitCount()
{
    GV_MotorCountPulse  = 0;
    GV_MotorCountCircle = 0;
    GV_MotorCountDegree = 0;
}


/**
 * �������
 */
void MOTOR_Enable()
{
    // 4988��TB67S109��TMC2209
    GPIO_WriteLow(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN);
}


/**
 * �������
 */
void MOTOR_Disable()
{
    // 4988��TB67S109��TMC2209
    GPIO_WriteHigh(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN);
}


/**
 * �������
 * ����һ��������ۼƣ������Զ����ݵ�������ϸ���ۼ���صļ���
 */
void MOTOR_CountStep()
{
    // ϸ�ֵĲ���ǣ��ȣ�
    // �Ƕ�ֵ��Ҫ���� 10000�������˸������㱣֤�˾���
    long divCircles = 3600000;
    short divStep = (short)(MOTOR_STEP_DEGREE*10000)/GV_MotorDiv;
    
    // �����ۼ�
    GV_MotorCountPulse++;
    
    // ��¼�����ۼ�
    if( GV_MotorDir == MOTOR_DIR_CW  ) GV_MotorCountDegree += divStep;
    if( GV_MotorDir == MOTOR_DIR_CCW ) GV_MotorCountDegree -= divStep;

    // ����תȦ��
    if( GV_MotorCountDegree >= divCircles || GV_MotorCountDegree <= -divCircles ) GV_MotorCountCircle += GV_MotorCountDegree/divCircles;
    GV_MotorCountDegree = GV_MotorCountDegree % divCircles;
}


/**
 * ��ȡ�ۼƵ�������
 */
long MOTOR_GetCountPulse()
{ 
    return GV_MotorCountPulse; 
}


/**
 * ��ȡ�ۼƵ�Ȧ��
 * ��ȡ�ۼƵ�Ȧ��ֵ��������ʾ˳ʱ�룬������ʾ��ʱ��
 */
long MOTOR_GetCountCircle()
{ 
    return GV_MotorCountCircle; 
}


/**
 * ��ȡ�ۼƵĽǶ�
 * ��ȡ��Ȧ�ĽǶ�ֵ��������ʾ˳ʱ�룬������ʾ��ʱ��
 */
double MOTOR_GetCountDegree()
{ 
    return GV_MotorCountDegree*1.0/10000; 
}


/**
 * ���õ��ϸ��
 * ���ڲ��԰��� IO �ڲ����ã����� A4988 �޷���������ȫ��ϸ��ģʽ
 * Ŀǰ�����ֻ�ܲ��ó�ʼһ��ϸ�֣�Ȼ������ٶ�̬��������һ��ϸ�֡����ڳ�ʼ�ĺͶ�̬���Ƶ�������ϸ����ҪԤ���� PCB ���Ͻ�����������
 *
 * @param short div  ϸ��ֵ��MOTOR_DIV_LOW, MOTOR_DIV_HIGH
 */
void MOTOR_SetDiv( short div )
{
    // ��Ч����
    if( !(div==MOTOR_DIV_LOW||div==MOTOR_DIV_HIGH) ) return;
    
    // ��ʼ��ϸ�֣�PE2�͵�ƽ��
    if( div == MOTOR_DIV_LOW ) GPIO_WriteLow(MOTOR_DIV_PORT, MOTOR_DIV_PIN);

    // ��ص�ϸ�֣�PE2����̬��
    if( div == MOTOR_DIV_HIGH ) GPIO_WriteHigh(MOTOR_DIV_PORT, MOTOR_DIV_PIN);

    // ����ȫ�ֱ���
    GV_MotorDiv = div;
}


/**
 * ��ȡ���ϸ��
 */
short MOTOR_GetDiv()
{
    return GV_MotorDiv;
}


/**
 * ���õ������
 *
 * @param short dir  ����MOTOR_DIR_CW��MOTOR_DIR_CCW
 */
void MOTOR_SetDir( short dir )
{
    if( dir == MOTOR_DIR_CW  ){ GPIO_WriteLow(MOTOR_DIR_PORT, MOTOR_DIR_PIN);  GV_MotorDir = dir; }  // �͵�ƽ
    if( dir == MOTOR_DIR_CCW ){ GPIO_WriteHigh(MOTOR_DIR_PORT, MOTOR_DIR_PIN); GV_MotorDir = dir; }  // ����̬
}


/**
 * ��ȡ�������
 */
short MOTOR_GetDir()
{
    return GV_MotorDir;
}


/**
 * ��ʼ����������ٶȣ�RPM��
 * �ú���������ɺ��ֱ����Ч����źţ��������ǰ����ֹ�������
 *
 * @param short speed  �����ٶȣ���λ��RPM��Ȧ/���ӣ�
 */
void MOTOR_InitSpeed( short speed )
{
    // �� speed תΪƵ��
    // Ƶ�� = ת�٣�Ȧ/�룩 * һȦ��Ƶ�ʸ���
    // Ƶ�� = (speed/60) * (360/(MOTOR_STEP_DEGREE/GV_MotorDiv))
    GV_MotorSpeedFreq = (long)((360*(u32)speed*GV_MotorDiv)/(60*MOTOR_STEP_DEGREE));

    // ��ʼ���������Ƶ��
    TIM2_PWMO_Init(GV_MotorSpeedFreq);
}


/**
 * ��ȡ��������ٶȣ�RPM��
 */
short MOTOR_GetSpeed()
{
    return GV_MotorSpeed;
}


/**
 * ���õ����ǰ״̬
 *
 * @param short state  ״̬���μ� motor.h �е� MOTOR_STATE_XXX
 */
void MOTOR_SetState( short state )
{
    GV_MotorState = state;
}


/**
 * ��ȡ�����ǰ״̬
 */
short MOTOR_GetState()
{
    return GV_MotorState;
}


/**
 * ���õ����һ��״̬
 *
 * @param short state  ״̬���μ� motor.h �е� MOTOR_STATE_XXX
 */
void MOTOR_SetStatePrev( short state )
{
    GV_MotorStatePrev = state;
}


/**
 * ��ȡ�����һ��״̬
 */
short MOTOR_GetStatePrev()
{
    return GV_MotorStatePrev;
}


/**
 * �жϵ���Ƿ��ѹ���
 * ��ȡ���㴥���ŵ�ʵʱֵ�жϵ�ǰ����Ƿ��ڹ���λ��
 */
bool MOTOR_IsZero()
{
    if( MOTOR_ZERO_EXTITYPE == 0 && EXTI_GetValue(MOTOR_ZERO_EXTINUM) == 0 ) return true;
    if( MOTOR_ZERO_EXTITYPE == 1 && EXTI_GetValue(MOTOR_ZERO_EXTINUM) == 1 ) return true;
    return false;
}


/**
 * ֹͣ���е��
 * �ú��������� GV_MotorStatePrev �� GV_MotorState = MOTOR_STATE_IDLE
 *
 * @return boolean  true ��ʾִ�гɹ���false ��ʾִ��ʧ��
 */
bool MOTOR_RunStop()
{
    // ��ֹ��������
    // �ú��������� GV_MotorStatePrev �� GV_MotorState = MOTOR_STATE_IDLE
    TIM2_PWMO_Stop();

    // ����
    return true;
}


/**
 * �������е��
 * �ú�����Ĭ�����õ������״̬Ϊ MOTOR_STATE_RUN
 * �����Ҫ�Զ�����������״̬����ô��ҪԤ�ȵ��� MOTOR_SetState(...) �������� 
 *
 * @return boolean  true ��ʾִ�гɹ���false ��ʾִ��ʧ��
 */
bool MOTOR_RunKeep()
{
    // ����Ĭ��״̬
    // ��״̬���� TIM2_PWMO_Stop() ����Ϊ MOTOR_STATE_IDLE
    if( GV_MotorState == MOTOR_STATE_IDLE ) GV_MotorState = MOTOR_STATE_RUN;
    
    // ��������
    return TIM2_PWMO_Send(50, 0);
}


/**
 * ���������е��
 * һ������һ��������
 *
 * �ú�����Ĭ�����õ������״̬Ϊ MOTOR_STATE_RUN
 * �����Ҫ�Զ�����������״̬����ô��ҪԤ�ȵ��� MOTOR_SetState(...) �������� 
 *
 * @param u32 number  ����
 * @return boolean  true ��ʾִ�гɹ���false ��ʾִ��ʧ��
 */
bool MOTOR_RunStep( u32 number )
{
    // ��Ч����
    if( number <= 0 ) return false;

    // ����Ĭ��״̬
    // ��״̬���� TIM2_PWMO_Stop() ����Ϊ MOTOR_STATE_IDLE
    if( GV_MotorState == MOTOR_STATE_IDLE ) GV_MotorState = MOTOR_STATE_RUN;
    
    // ��������
    return TIM2_PWMO_Send(50, number);
}


/**
 * ������ֵ���е��
 * �ú�����Ĭ�����õ������״̬Ϊ MOTOR_STATE_RUN
 * �����Ҫ�Զ�����������״̬����ô��ҪԤ�ȵ��� MOTOR_SetState(...) �������� 
 *
 * @param double length  ���ȣ���λ��mm
 * @return boolean  true ��ʾִ�гɹ���false ��ʾִ��ʧ��
 */
bool MOTOR_RunLength( double length )
{
    // ��Ч����
    if( length <= 0 ) return false;

    // ����������λ����
    double circlePulses = 360.0/(MOTOR_STEP_DEGREE/GV_MotorDiv);  // �����һȦ�������������
    double pulseLength = MOTOR_CIRCLE_LEN*1.0/circlePulses;  // һ���������ߵľ��룬��λ��mm

    // �������͵��������
    u32 sendLimit = (u32)(length/pulseLength);

    // ����Ĭ��״̬
    // ��״̬���� TIM2_PWMO_Stop() ����Ϊ MOTOR_STATE_IDLE
    if( GV_MotorState == MOTOR_STATE_IDLE ) GV_MotorState = MOTOR_STATE_RUN;
    
    // ��������
    return TIM2_PWMO_Send(50, sendLimit);
}


/**
 * ���Ƕ�ֵ���е��
 * �ú�����Ĭ�����õ������״̬Ϊ MOTOR_STATE_RUN
 * �����Ҫ�Զ�����������״̬����ô��ҪԤ�ȵ��� MOTOR_SetState(...) �������� 
 *
 * @param double degree  �Ƕȣ���λ����
 * @return boolean  true ��ʾִ�гɹ���false ��ʾִ��ʧ��
 */
bool MOTOR_RunDegree( double degree )
{
    // ��Ч����
    if( degree <= 0 ) return false;
 
    // �������͵��������
    u32 sendLimit = (int)(degree/(MOTOR_STEP_DEGREE/GV_MotorDiv));

    // ����Ĭ��״̬
    // ��״̬���� TIM2_PWMO_Stop() ����Ϊ MOTOR_STATE_IDLE
    if( GV_MotorState == MOTOR_STATE_IDLE ) GV_MotorState = MOTOR_STATE_RUN;
    
    // ��������
    return TIM2_PWMO_Send(50, sendLimit);
}



/* -------------------------------------------------- */
// �� �жϻص�����
//   ��������ڲμ� tim2.c �� extint.c
/* -------------------------------------------------- */

/**
 * ��������ж�
 * ÿ����һ�����嶼������������
 * ����� TIM2 �� PWM ���п��ƣ����Ը��жϺ����� TIM2 �� ��������ֵ������¡� �жϺ�����������
 * ���Ը��жϺ���ִ�е�Ƶ���ǻ�������Ϊ��λ�ģ���Ȼ����Ҳ�����������ƣ������δ����ʱ���жϺ������ᱻ���ã�
 */
void MOTOR_InterruptHandlerPulse()
{
    MOTOR_CountStep();
}


/**
 * �����������ж�
 * ��Ҫ���ڵ�������в�����ԭ��ʱ�ĵ��ֹͣ����
 *
 * @param short extiNum   �ж����ţ�J60 �� J61
 * @param short extiType  �ж����ͣ�0 ��ʾ�½��أ�1 ��ʾ������
 */
void MOTOR_InterruptHandlerZeroFinish( short extiNum, short extiType )
{
    // ƥ�����
    if( extiNum  != MOTOR_ZERO_EXTINUM  ) return;
    if( extiType != MOTOR_ZERO_EXTITYPE ) return;

    // ���ֹͣ
    // �ú��������� GV_MotorStatePrev �� GV_MotorState = MOTOR_STATE_IDLE
    MOTOR_RunStop();

    // ��ʼ������
    MOTOR_InitCount();
}



/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */

/**
 * ���߼������������������
 * 
 * @param short length  �������ȣ���λ��mm
 */
void MOTOR_MainTwoPointRunByLength( short length )
{
    if( GV_MotorState != MOTOR_STATE_IDLE ) return;
    delay_ms(2000);

    if( MOTOR_GetDir() == MOTOR_DIR_CW  ){ MOTOR_SetDir(MOTOR_DIR_CCW); MOTOR_RunLength(length); return; }
    if( MOTOR_GetDir() == MOTOR_DIR_CCW ){ MOTOR_SetDir(MOTOR_DIR_CW); MOTOR_RunLength(length); return;  }
}


/**
 * ���߼������������������
 * 
 * @param short degree  �����Ƕȣ���λ����
 */
void MOTOR_MainTwoPointRunByDegree( short degree )
{
    if( GV_MotorState != MOTOR_STATE_IDLE ) return;
    delay_ms(2000);
    
    if( MOTOR_GetDir() == MOTOR_DIR_CW  ){ MOTOR_SetDir(MOTOR_DIR_CCW); MOTOR_RunDegree(degree); return; }
    if( MOTOR_GetDir() == MOTOR_DIR_CCW ){ MOTOR_SetDir(MOTOR_DIR_CW); MOTOR_RunDegree(degree); return;  }
}
    

/**
 * ���߼����������Ƶ��(ʵʱ)
 * ������������ 43 ���Ƶ�� - ��ϸ��:˳ʱ��/��ǰ
 * ������������ 44 ���Ƶ�� - ��ϸ��:��ʱ��/���
 * ������������ 45 ���Ƶ�� - ��ϸ��:˳ʱ��/��ǰ
 * ������������ 46 ���Ƶ�� - ��ϸ��:��ʱ��/���
 */
void MOTOR_MainKeyControlMotorReal()
{
    // ��ʼ��
    short i;
    short flag, keys[4] = {43,44,45,46};
    
    // �������ݼ���
    KEY_DatLoad();
    
    // ����ȫ���ɿ�ʱ�����ֹͣ����
    flag = true; 
    for( i = 0; i < 4; i++) flag &= KEY_IsPress(keys[i],false) == KEY_STATE_NO;
    if( flag == true ){ MOTOR_RunStop(); return; }

    // ϸ�֣���ϸ�֣�����˳ʱ��/��ǰ
    // TODO...

    // ϸ�֣���ϸ�֣�������ʱ��/���
    // TODO...

    // ϸ�֣���ϸ�֣�����˳ʱ��/��ǰ
    // TODO...

    // ϸ�֣���ϸ�֣�������ʱ��/���
    // TODO...
}


/**
 * ���߼����������Ƶ��(������)
 * ������������ 43 ���Ƶ�� - ��ϸ��:˳ʱ��/��ǰ
 * ������������ 44 ���Ƶ�� - ��ϸ��:��ʱ��/���
 * ������������ 45 ���Ƶ�� - ��ϸ��:˳ʱ��/��ǰ
 * ������������ 46 ���Ƶ�� - ��ϸ��:��ʱ��/���
 *
 * @param double length  ���ȣ���λ��mm
 */
void MOTOR_MainKeyControlMotorByLength( double length )
{
    // TODO...
}