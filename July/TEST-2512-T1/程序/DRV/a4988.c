#include "common.h"



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * A4988 ��ر���
 */
bool  GV_A4988Motor       = false;  //��ϵͳʹ�á��������
bool  GV_A4988MotorRuning = false;  //��ϵͳʹ�á����������ʼ

long  GV_A4988MotorCountCircle = 0;  //��ϵͳʹ�á������λ���ߵ���Ȧ��������ʾ˳ʱ�룬����ʾ��ʱ�룩
long  GV_A4988MotorCountDegree = 0;  //��ϵͳʹ�á������λ���ߵĵ�Ȧ����������ʾ˳ʱ�룬����ʾ��ʱ�룬ͬʱ����360���ͳ�Ƶ�Ȧ�������У�

short GV_A4988MotorDir    = 0;  //��ϵͳʹ�á��������
short GV_A4988MotorSubdiv = 0;  //��ϵͳʹ�á����ϸ��

short GV_A4988MotorStateKeepRun    = -1;  //��ϵͳʹ�á����״̬���Ƿ񱣳�����
short GV_A4988MotorStateKeepRunDir = -1;  //��ϵͳʹ�á����״̬����������ʱ����



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ��ʼ��
 */
void A4988_Init()
{
    // ��ʼ��ϸ��
    GPIO_Init(A4988_MS_PORT, A4988_MS_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);
    A4988_SetSubdiv(A4988_MS_MODE0_SUBDIV);

    // ��ʼ������
    A4988_InitCount();

    // ��ʼ���������
    // ����ԭ��ͼ������̬ʱ DIR ��Ϊ�ߵ�ƽ�� �͵�ƽʱ DIR ��Ϊ�͵�ƽ
    //
    // ���ǹ涨�ߵ�ƽʱ���˳ʱ����ת��������ǰ�ߣ����͵�ƽʱ�����ʱ����ת����������ߣ���
    // ������Ҫ����ʵ������������������оƬ������ߣ�ʹ֮���������Ĺ���
    GPIO_Init(A4988_DIR_PORT, A4988_DIR_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW); // ���ٿ�©�������̬

    // ��ʼ���������Ƶ��
    TIM2_PWMO_Init(A4988_MOTOR_PWM_FREQ);

    // ��ʼ�����
    GV_A4988Motor = true;
}


/**
 * ��ʼ������
 */
void A4988_InitCount()
{
    GV_A4988MotorCountCircle = 0;
    GV_A4988MotorCountDegree = 0;
}


/**
 * �������
 * ����һ��������ۼƣ������Զ����ݵ�������ϸ���ۼ���صļ���
 */
void A4988_CountStep()
{
    // ϸ�ֵĲ���ǣ��ȣ�
    // ���� 10000 �ǲ������η�ʽ���渡����
    long subdivCircles = 3600000;
    short subdivStep = (short)(A4988_MOTOR_STEP_DEGREE*10000)/GV_A4988MotorSubdiv;
    
    // ��¼�����ۼ�
    if( GV_A4988MotorDir == A4988_DIR_BACK  ) GV_A4988MotorCountDegree -= subdivStep;
    if( GV_A4988MotorDir == A4988_DIR_FRONT ) GV_A4988MotorCountDegree += subdivStep;

    // ����תȦ��
    if( GV_A4988MotorCountDegree >= subdivCircles || GV_A4988MotorCountDegree <= -subdivCircles ) GV_A4988MotorCountCircle += GV_A4988MotorCountDegree/subdivCircles;
    GV_A4988MotorCountDegree = GV_A4988MotorCountDegree % subdivCircles;
}


/**
 * ��ȡͳ�Ƶ�Ȧ��
 * ��ȡ�ۼƵ�Ȧ��ֵ��������ʾ˳ʱ�룬������ʾ��ʱ��
 */
long A4988_GetCountCircle()
{ 
    return GV_A4988MotorCountCircle; 
}


/**
 * ��ȡͳ�ƵĽǶ�
 * ��ȡ��Ȧ�ĽǶ�ֵ��������ʾ˳ʱ�룬������ʾ��ʱ��
 */
double A4988_GetCountDegree()
{ 
    return GV_A4988MotorCountDegree*1.0/10000; 
}


/**
 * ���õ��ϸ��
 * ���ڲ��԰��� IO �ڲ����ã����� A4988 �޷���������ȫ��ϸ��ģʽ
 * Ŀǰ�����ֻ�ܲ��ó�ʼһ��ϸ�֣�Ȼ������ٶ�̬��������һ��ϸ�֡����ڳ�ʼ�ĺͶ�̬���Ƶ�������ϸ����ҪԤ���� PCB ���Ͻ�����������
 * 
 * ���Ϊ TesterProductA.v5_20210604  ��PCB���ϸ�ֿ���֧���� PCB ��ֱ�Ӻ��ӵ����ʼһ��ϸ�֣�Ҳ����ͨ���������ӵ� PE2 Ȼ�������������һ��ϸ��
 * ���Ϊ TesterProductA.v5_20210605+ ��PCB���ϸ�ֿ���֧���� PCB ��ֱ�Ӻ��ӵ����ʼһ��ϸ�֣�Ҳ����ͨ���������ӵ� PE2 Ȼ�������������һ��ϸ��
 *
 * @param short subdiv  ϸ��ֵ��A4988_MS_MODE0_SUBDIV, A4988_MS_MODE1_SUBDIV
 */
void A4988_SetSubdiv( short subdiv )
{
    // ��Ч����
    if( !(subdiv==A4988_MS_MODE0_SUBDIV||subdiv==A4988_MS_MODE1_SUBDIV) ) return;
    
    // ��ʼ��ϸ�֣����ӵ��裬�������ŵ͵�ƽ��
    if( subdiv == A4988_MS_MODE0_SUBDIV ) GPIO_WriteLow(A4988_MS_PORT, A4988_MS_PIN);

    // ��ص�ϸ�֣�������ƣ��������Ÿ���̬��
    if( subdiv == A4988_MS_MODE1_SUBDIV ) GPIO_WriteHigh(A4988_MS_PORT, A4988_MS_PIN);

    // ����ȫ�ֱ���
    GV_A4988MotorSubdiv = subdiv;
}


/**
 * ��ȡ���ϸ��
 */
short A4988_GetSubdiv()
{
    return GV_A4988MotorSubdiv;
}


/**
 * ���õ������
 *
 * @param short dir  ����A4988_DIR_FRONT��A4988_DIR_BACK
 */
void A4988_SetDir( short dir )
{
    if( dir == A4988_DIR_BACK  ){ GPIO_WriteLow(A4988_DIR_PORT, A4988_DIR_PIN); GV_A4988MotorDir = dir; } // �͵�ƽ
    if( dir == A4988_DIR_FRONT ){ GPIO_WriteHigh(A4988_DIR_PORT, A4988_DIR_PIN); GV_A4988MotorDir = dir; }  // ����̬
}


/**
 * ��ȡ�������
 */
short A4988_GetDir()
{
    return GV_A4988MotorDir;
}


/**
 * �������е��
 * ���������� �������� �У���ô�Զ����� runKeepProtectPrev �������Ա��ε��� ���� ��ֹ�ɵġ��������С���ִ���µ����� 
 *
 * @param short   dir                 ����A4988_DIR_FRONT��A4988_DIR_BACK
 * @param boolean runKeepProtectPrev  ��һ�� ���������С� �����������������ڱ��������У���ô�ٴε��øú�������ô������ִ��

 * @return boolean   true ��ʾִ�гɹ���false ��ʾִ��ʧ��
 */
bool A4988_RunKeep( short dir, short runKeepProtectPrev )
{
    // ������һ�� ���������С� ���� �� ������������У���ô�ú���������ִ��
    if( runKeepProtectPrev == true && A4988_RunKeeping(-1) == true ) return false;

    // ��� ����������� �� ����Ͳ���һ�£���ôֱ�ӷ��� true
    if( A4988_RunKeeping(dir) ) return true;
    
    // ֹͣ���е��
    A4988_RunKeepStop();
    
    // ��ʼ��
    bool result = true;

    // ���÷���
    A4988_SetDir(dir);
    
    // ��������
    result = TIM2_PWMO_Send(50, 0, true);

    // ����״ֵ̬
    if( result == true ){
        GV_A4988MotorStateKeepRun    = 1;
        GV_A4988MotorStateKeepRunDir = dir;
    }

    // ����
    return result;
}


/**
 * ֹͣ���е��
 * ������δ���У���ôֱ�ӷ��� true
 *
 * @return boolean  true ��ʾִ�гɹ���false ��ʾִ��ʧ��
 */
bool A4988_RunKeepStop()
{
    // ���δ��������״̬
    if( A4988_RunKeeping(-1) == false ) return true; 
    
    // ��ֹ��������
    TIM2_PWMO_Stop();

    // ����״ֵ̬
    GV_A4988MotorStateKeepRun    = -1;
    GV_A4988MotorStateKeepRunDir = -1;

    // ����
    return true;
}


/**
 * ����Ƿ��ڱ���������
 * ֧��ƥ�����ķ����飬-1 ��ʾ���Է�����м��
 *
 * @param short dir  ����-1��A4988_DIR_FRONT��A4988_DIR_BACK
 * @return boolean   true ��ʾ�����У�false ��ʾδ����
 */
bool A4988_RunKeeping( short dir )
{
    if( GV_A4988MotorStateKeepRun != 1 ) return false; 
    return dir == -1 || GV_A4988MotorStateKeepRunDir == dir ? true : false;
}


/**
 * ���������е��
 * һ������һ��������
 *
 * @param short dir     ����A4988_DIR_FRONT��A4988_DIR_BACK
 * @param u32   number  ����
 * 
 * @return boolean  true ��ʾִ�гɹ���false ��ʾִ��ʧ��
 */
bool A4988_RunStep( short dir, u32 number )
{
    // ��Ч����
    if( number <= 0 ) return false;

    // ��ʼ��
    bool result = true;

    // ���÷���
    A4988_SetDir(dir);

    // ��������
    result = TIM2_PWMO_Send(50, number, true);

    // ����
    return result;
}


/**
 * �ȴ�����������
 * �Զ����� ��ǰ������Ƶ�� �Լ� Ԥ��ĵ������ ��������������Ҫ�Ľ�Ϊ�����ʱ�䲢�ȴ�
 *
 * @param u32 number  ����
 */
void A4988_RunStepWait( u32 number )
{
    // ��ȡ�����һȦ���������
    // ���ݵ�ǰ��Ƶ�ʣ�����������һȦ���������
    int circleTime = (long)A4988_MOTOR_CIRCLE_TIME*GV_A4988MotorSubdiv;

    // ����� 1�� ����ĺ�����
    double msPerNumber = circleTime*1.0/(360.0/(A4988_MOTOR_STEP_DEGREE/GV_A4988MotorSubdiv));

    // ��ʱ�ȴ�
    delay_ms((int)(number*msPerNumber));
}


/**
 * ������ֵ���е��
 *
 * @param short  dir     ����A4988_DIR_FRONT��A4988_DIR_BACK
 * @param double length  ���ȣ���λ��mm
 * 
 * @return boolean  true ��ʾִ�гɹ���false ��ʾִ��ʧ��
 */
bool A4988_RunLength( short dir, double length )
{
    // ��Ч����
    if( length <= 0 ) return false;
    
    // ��ʼ��
    bool result = true;
    
    // ����������λ����
    double circlePulses = 360.0/(A4988_MOTOR_STEP_DEGREE/GV_A4988MotorSubdiv);  // �����һȦ�������
    double pulseLength = A4988_MOTOR_CIRCLE_LEN*1.0/circlePulses;  // һ���������ߵľ��룬��λ��mm
    
    // �������͵��������
    u32 sendLimit = (u32)(length/pulseLength);

    // ���÷���
    A4988_SetDir(dir);
    
    // ��������
    result = TIM2_PWMO_Send(50, sendLimit, true);

    // ����
    return result;
}


/**
 * �ȴ�����������
 * �Զ����� ��ǰ������Ƶ�� �Լ� Ԥ��ĵ������ ��������������Ҫ�Ľ�Ϊ�����ʱ�䲢�ȴ�
 *
 * @param double length  ���ȣ���λ��mm
 */
void A4988_RunLengthWait( double length )
{
    // ��ȡ�����һȦ���������
    // ���ݵ�ǰ��Ƶ�ʣ�����������һȦ���������
    int circleTime = (long)A4988_MOTOR_CIRCLE_TIME*GV_A4988MotorSubdiv;

    // ����� 1���� ����� ������
    double msPerMm = circleTime*1.0/A4988_MOTOR_CIRCLE_LEN;

    // ��ʱ�ȴ�
    delay_ms((int)(length*msPerMm));
}


/**
 * ���Ƕ�ֵ���е��
 *
 * @param short  dir     ����A4988_DIR_FRONT��A4988_DIR_BACK
 * @param double degree  �Ƕȣ���λ����
 * 
 * @return boolean  true ��ʾִ�гɹ���false ��ʾִ��ʧ��
 */
bool A4988_RunDegree( short dir, double degree )
{
    // ��Ч����
    if( degree <= 0 ) return false;
    
    // ��ʼ��
    bool result = true;

    // �������͵��������
    u32 sendLimit = (int)(degree/(A4988_MOTOR_STEP_DEGREE/GV_A4988MotorSubdiv));

    // ���÷���
    A4988_SetDir(dir);

    // ��������
    result = TIM2_PWMO_Send(50, sendLimit, true);

    // ����
    return result;
}


/**
 * �ȴ�����������
 * �Զ����� ��ǰ������Ƶ�� �Լ� Ԥ��ĵ������ ��������������Ҫ�Ľ�Ϊ�����ʱ�䲢�ȴ�
 *
 * @param double degree  �Ƕȣ���λ����
 */
void A4988_RunDegreeWait( double degree )
{
    // ��ȡ�����һȦ���������
    // ���ݵ�ǰ��Ƶ�ʣ�����������һȦ���������
    int circleTime = (long)A4988_MOTOR_CIRCLE_TIME*GV_A4988MotorSubdiv;

    // ����� 1�� ����ĺ�����
    double msPerDegree = circleTime*1.0/360;

    // ��ʱ�ȴ�
    delay_ms((int)(degree*msPerDegree));
}


/**
 * ��������ж�
 * ����� TIM2 �� PWM ���п��ƣ����Ը��жϺ����� TIM2 �� ��������ֵ������¡� �жϺ�����������
 * ���Ը��жϺ���ִ�е�Ƶ���ǻ�������Ϊ��λ�ģ���Ȼ����Ҳ�����������ƣ������δ����ʱ���жϺ������ᱻ���ã�
 */
void A4988_InterruptHandlerRuning()
{
    A4988_CountStep();
}


/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */


/**
 * ���߼�����ʼ��
 */
void A4988_MainInit()
{
    A4988_Init();
}


/**
 * ���߼����������Ƶ��(ʵʱ)
 * ������������ 43 ���Ƶ�� - MODE0:��ǰ
 * ������������ 44 ���Ƶ�� - MODE0:���
 * ������������ 45 ���Ƶ�� - MODE1:��ǰ
 * ������������ 46 ���Ƶ�� - MODE1:���
 */
void A4988_MainKeyControlMotorReal()
{
    // ��ʼ��
    short i;
    short flag, keys[4] = {43,44,45,46};
    
    // �������ݼ���
    KEY_DatLoad();
    
    // ����ȫ���ɿ�ʱ�����ֹͣ����
    flag = true; 
    for( i = 0; i < 4; i++) flag &= KEY_IsPress(keys[i],false) == KEY_STATE_NO;
    if( flag == true ){ A4988_RunKeepStop(); return; }

    // ϸ�֣�MODE0��������ǰ
    if( KEY_IsPress(43,false) == KEY_STATE_YES ){
        if( A4988_GetSubdiv() == A4988_MS_MODE0_SUBDIV && A4988_RunKeeping(A4988_DIR_FRONT) == true ) return;
        A4988_RunKeepStop(); A4988_SetSubdiv(A4988_MS_MODE0_SUBDIV); A4988_RunKeep(A4988_DIR_FRONT, false);
    }

    // ϸ�֣�MODE0���������
    if( KEY_IsPress(44,false) == KEY_STATE_YES ){
        if( A4988_GetSubdiv() == A4988_MS_MODE0_SUBDIV && A4988_RunKeeping(A4988_DIR_BACK) == true ) return;
        A4988_RunKeepStop(); A4988_SetSubdiv(A4988_MS_MODE0_SUBDIV); A4988_RunKeep(A4988_DIR_BACK, false);
    }

    // ϸ�֣�MODE1��������ǰ
    if( KEY_IsPress(45,false) == KEY_STATE_YES ){
        if( A4988_GetSubdiv() == A4988_MS_MODE1_SUBDIV && A4988_RunKeeping(A4988_DIR_FRONT) == true ) return;
        A4988_RunKeepStop(); A4988_SetSubdiv(A4988_MS_MODE1_SUBDIV); A4988_RunKeep(A4988_DIR_FRONT, false);
    }

    // ϸ�֣�MODE1���������
    if( KEY_IsPress(46,false) == KEY_STATE_YES ){
        if( A4988_GetSubdiv() == A4988_MS_MODE1_SUBDIV && A4988_RunKeeping(A4988_DIR_BACK) == true ) return;
        A4988_RunKeepStop(); A4988_SetSubdiv(A4988_MS_MODE1_SUBDIV); A4988_RunKeep(A4988_DIR_BACK, false);
    }
}


/**
 * ���߼����������Ƶ��(������)
 * ������������ 43 ���Ƶ�� - MODE0:��ǰ
 * ������������ 44 ���Ƶ�� - MODE0:���
 * ������������ 45 ���Ƶ�� - MODE1:��ǰ
 * ������������ 46 ���Ƶ�� - MODE1:���
 *
 * @param double length  ���ȣ���λ��mm
 */
void A4988_MainKeyControlMotorByLength( double length )
{
    // TODO...
}
