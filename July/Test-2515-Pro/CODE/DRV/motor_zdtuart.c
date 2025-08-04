#include "common.h"



/* -------------------------------------------------- */
// �� ���� ZDT UART ��ʽ������Emm_V5.0��
// �� ʹ��ǰ����ȶ� UART3 ���г�ʼ����UART3_CInit();
// �� ֮���Բ�ʹ�� UART1������Ϊ UART1 ��Ҫͨ�� USB �͵���ͨѶ��������Ҳ���� UART1 ͨѶ����ô�ͻ���Ϊ CH340G оƬ�����޷��յ����������ķ�����Ϣ
//
// �� motor_zdtuart.c �� motor.c ����ȫ���������߼���û��������ϵ������ʹ�ø��Եĺ�����
// �� ��Ȼ motor_zdtuart.c �� motor.c Ҳ���Ի��ã�ZDT ֧�� UART �� STEP/DIR ���ã��������� UART ����ϸ�ֵȲ�����STEP/DIR ���Ƶ������
//
// �� �� ZDT UART ��������� 0xFA78 �汾���������д����ȡ�������ʹ�ú�����MOTOR_ZDTUART_GetVersion()
// �� �汾���У���8λ��ʾ����汾��0xF4��Emm42_V5.0.0����0xF7��Emm42_V5.0.3����0xFA��Emm_V5.0.6��
// �� �汾���У���8λ��ʾӲ���汾��0x78��ZDT_X42_V1.2��
//
// �� ע�⣺��������������¼ 0xFA��Emm_V5.0.6�����򣬷��򴮿ڿ��ܳ���BUG
// �� ע�⣺����һЩ����д�뵽�������API�ӿڣ�����������ӳٵȴ�����ô�ᵼ�µڶ��η��ʹ�������ִ��ʧ�����޷�������
//         ԭ����д�����õ�������ʱ�������������ö�ʱ��ȥˢ������->�ر��ж�->�޸�����ֵ->�洢��Flash->Ȼ���ٳ�ʼ����������Ҫһ����ʱ�䡣����һ��ʱ���ڷ��͵��´�������ʱ�޷�ִ�е�
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * MOTOR ��ر���
 */
short GV_MotorZdtDir   = 0;   //��ϵͳʹ�á��������
short GV_MotorZdtDiv   = 0;   //��ϵͳʹ�á����ϸ��
short GV_MotorZdtSpeed = 15;  //��ϵͳʹ�á�����ٶȣ�RPM,ת/ÿ���ӣ�



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ��ʼ������
 *
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_Init()
{
    // �������
    if( MOTOR_ZDTUART_SetEnable() ) return 1;
    
    // ����ϸ�֣�256ϸ��
    if( MOTOR_ZDTUART_SetDiv(256,true) ) return 2;
    
    // ���÷���
    if( MOTOR_ZDTUART_SetDir(MOTOR_ZDTUART_DIR_CW) ) return 3;
    
    // �����ٶ�
    if( MOTOR_ZDTUART_SetSpeed(15) ) return 4;

    // ����
    return 0;
}


/**
 * ���õ��ϸ��
 *
 * ���ڵ�����������ò���ϸ�֣������� UART ģʽ�����ò�ͬ��ϸ��ֵʵ������Ч�����󣬵��ǻ��ǽ������ô�һЩ��ϸ�֣����磺64��128��256��
 * ��Ϊ���ת���Ƕ���������������Ǹ���ϸ�ּ�������ģ���������������һ�����Σ����͵�ϸ�ֻᵼ�¼�����������ʱ�������룬����ת���ǶȻ���Щ����
 * 
 * ������� UART+STEP/DIR ��Ϸ�ʽȥ���Ƶ������ô���Ը���ʵ�������������ϸ��ֵ
 *
 * @param short dir    ϸ��: 0~256
 * @param bool  delay  ��ʱ���أ�������ȫд�뵽��������Ҫһ����ʱ��ȴ���������ŷ��͵Ĵ�������޷�ִ�У�
 *
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_SetDiv( short div, bool delay )
{
    // ��ʼ��
    u8 sendData[6] = {0x01, 0x84, 0x8A, 0x00, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0x84, 0x02, 0x6B}, *receiveData;

    // ������������
    // ���� STEP/DIR ģʽ���Ƶ���Ҫ��ϸ��ֵ���浽�����壬���Դ˴�Ĭ�ϱ���
    sendData[3] = 0x01;  // �Ƿ�洢��ϸ��ֵ�Ƿ�ֱ�Ӵ洢��������
    sendData[4] = div >= 256 ? 0 : div;

    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 6); delay_ms(2);

    // ����ִ�н��
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }

    // ����ϸ��ֵ
    GV_MotorZdtDiv = div; 
    
    // ���ý�������
    UART3_ResetReceiveData(); 

    // ��ʱ�ȴ�������ȫд��
    // Ϊ�˼� API �����ı��빤�������Ծͽ��ӳٺ�����ֱ��д����������������ڣ����øú���ʱ��Ҫע����������������һ��ʱ��
    // ԭ��д�����õ�������ʱ�������������ö�ʱ��ȥˢ������->�ر��ж�->�޸�����ֵ->�洢��Flash->Ȼ���ٳ�ʼ����������Ҫһ����ʱ�䡣����һ��ʱ���ڷ��͵��´�������ʱ�޷�ִ�е�
    // 
    // ���� Emm_V5.0.6 �汾�ĵ�����ѭ��ִ�з������ԣ��ӳ� 80ms �����������ݽ��գ�����ʹ�ó����������쳣�����Ը���ʵ������Ӵ��ӳ�ʱ��
    // ����1��MOTOR_ZDTUART_SetDiv() + MOTOR_ZDTUART_SetZeroPoint() ż���ᵼ�� MOTOR_ZDTUART_SetZeroPoint() ʧЧ��������ʱ�Ӵ� 150ms
    if( delay == true ) delay_ms(150);
    
    // ���سɹ�
    return 0;
}


/**
 * ��ȡ���ϸ��
 *
 * @return  ϸ��ֵ: 0~255, 0 ��ʾ 256ϸ��
 */
short MOTOR_ZDTUART_GetDiv()
{
    return GV_MotorZdtDiv;
}


/**
 * ���õ������
 * ������з����޷�ֱ�����õ��������ڲ��������淢���˶�����һ����
 *
 * @param short dir  ����MOTOR_ZDTUART_DIR_CW��MOTOR_ZDTUART_DIR_CCW
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_SetDir( short dir )
{
    // ���÷���
    GV_MotorZdtDir = dir;
    
    // ����
    return 0;
}


/**
 * ��ȡ�������
 *
 * @return  ����ֵ��MOTOR_ZDTUART_DIR_CW��MOTOR_ZDTUART_DIR_CCW
 */
short MOTOR_ZDTUART_GetDir()
{
    return GV_MotorZdtDir;
}


/**
 * ���õ���ٶ�
 *
 * @param short speed  �ٶȣ���λ��RPM��ת/ÿ���ӣ�
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_SetSpeed( short speed )
{
    // �����ٶ�
    GV_MotorZdtSpeed = speed;
    
    // ����
    return 0;
}


/**
 * ��ȡ����ٶ�
 *
 * @return  �ٶ�ֵ����λ��RPM��ת/ÿ���ӣ�
 */
short MOTOR_ZDTUART_GetSpeed()
{
    return GV_MotorZdtSpeed;
}


/**
 * ��ȡ���ʵʱ�Ƕȣ�����ʽ��
 * ����������ڵĽǶ���һ���ۼӵĲ����������ģ�
 * �ú����������ۼӽǶ�ֵ�ع����ع���ʽ������Բ�ο̶�˳ʱ������ľ��ԽǶ�ֵ���μ�PAT��װ�̶��̣�
 *
 * @return double  �Ƕ�ֵ��> 0 ��ʾ��Ч�ĽǶ�ֵ��-1 ��ʾ��Чֵ
 */
double MOTOR_ZDTUART_GetDegree360()
{
    // ��ʼ��
    u8 sendData[3] = {0x01, 0x36, 0x6B}, *receiveData;
    u32 degnum = 0; double degree = 0;

    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 3); delay_ms(4);

    // ���ִ�н��
    if( UART3_GetReceiveDataCount() < 8 ){ UART3_ResetReceiveData(); return -1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 2; i++ ) if( sendData[i] != receiveData[i] || receiveData[7] != 0x6B ){ UART3_ResetReceiveData(); return -1; }

    // �����Ƕ�
    degnum = degnum | receiveData[3]; degnum = degnum << 8;
    degnum = degnum | receiveData[4]; degnum = degnum << 8;
    degnum = degnum | receiveData[5]; degnum = degnum << 8;
    degnum = degnum | receiveData[6];

    degree = (double)degnum;
    degree = 360*(degree/65535);
    degree = degree*(receiveData[2]==1?-1:1);
    
    while( degree >  360 ) degree -= 360;
    while( degree < -360 ) degree += 360;
        
    // ���ý�������
    UART3_ResetReceiveData(); 

    // ����ִ�н��
    return degree > 0 ? degree : 360-degree;
}


/**
 * ��ȡ�ͽ����нǶ�
 * �ú����������ǶȲ���Ҫ�󣺻���Բ�ο̶�˳ʱ������ľ��ԽǶ�ֵ���μ�PAT��װ�̶��̣�
 *
 * @param double  currDegree  ��ǰ�Ƕ�
 * @param double  distDegree  Ŀ��Ƕ�
 *
 * @return  > 0 ��ʾ˳ʱ��Ҫ�����еĽǶȣ�< 0 ��ʾ��ʱ����Ҫ���еĽǶ�
 */
double MOTOR_ZDTUART_GetRunDegree360Near( double currDegree, double distDegree )
{
    // �ع�����
    while( currDegree >  360 ) currDegree -= 360;
    while( currDegree < -360 ) currDegree += 360;
    currDegree = currDegree > 0 ? currDegree : 360-currDegree;
    
    while( distDegree >  360 ) distDegree -= 360;
    while( distDegree < -360 ) distDegree += 360;
    distDegree = distDegree > 0 ? distDegree : 360-distDegree;
    
    // Ŀ��Ƕ� > ��ǰ�Ƕ�
    // distDegree=90    currDegree=40   ����  50��˳ʱ�룩
    // distDegree=350   currDegree=40   ���� -50����ʱ�룩
    if( distDegree > currDegree ){
       if( (distDegree-currDegree) <= (currDegree+360-distDegree) ) return distDegree-currDegree;
       else return -(currDegree+360-distDegree);
    }
     
    // Ŀ��Ƕ� < ��ǰ�Ƕ�
    // distDegree=10   currDegree=40    ����  -30����ʱ�룩
    // distDegree=30   currDegree=40    ����  -10����ʱ�룩
    // distDegree=10   currDegree=200   ����  170��˳ʱ�룩
    // distDegree=30   currDegree=200   ���� -170����ʱ�룩
    if( distDegree < currDegree ){
       if( (currDegree-distDegree) < (distDegree+360-currDegree) ) return -(currDegree-distDegree);
       else return distDegree+360-currDegree;
    }
    
    // ��ִ��
    return 0;
}


/**
 * ��ȡ������汾
 * 
 * return long -1 ��ʾ��ȡ�쳣
 *             ����16λ�С���8λ��ʾ����汾��0xF4��Emm42_V5.0.0����0xF7��Emm42_V5.0.3��
 *             ����16λ�С���8λ��ʾӲ���汾��0x78��ZDT_X42_V1.2��
 */
long MOTOR_ZDTUART_GetVersion()
{
    // ��ʼ��
    u8  sendData[3] = {0x01, 0x1F, 0x6B}, *receiveData;
    u16 softVersion, hardwareVersion;
        
    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 3); delay_ms(2);

    // ���ִ�н��
    if( UART3_GetReceiveDataCount() < 5 ){ UART3_ResetReceiveData(); return -1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 2; i++ ) if( sendData[i] != receiveData[i] || receiveData[4] != 0x6B ){ UART3_ResetReceiveData(); return -1; }

    // �����汾��
    softVersion = receiveData[2];
    hardwareVersion = receiveData[3];

    // ����������汾
    UART3_ResetReceiveData(); 
    return softVersion<<8|hardwareVersion;
}


/**
 * ���õ��ʹ��
 *
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_SetEnable()
{
    // ��ʼ��
    u8 sendData[6] = {0x01, 0xF3, 0xAB, 0x01, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0xF3, 0x02, 0x6B}, *receiveData;
        
    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 6); delay_ms(2);

    // ����ִ�н��
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    UART3_ResetReceiveData(); 

    return 0;
}


/**
 * ���õ������
 *
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_SetDisable()
{
    // ��ʼ��
    u8 sendData[6] = {0x01, 0xF3, 0xAB, 0x00, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0xF3, 0x02, 0x6B}, *receiveData;
        
    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 6); delay_ms(2);

    // ����ִ�н��
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    UART3_ResetReceiveData(); return 0;
}


/**
 * ��ȡ���״̬��
 * ����ֵ���������ʹ��״̬�������λ��־�������ת��־�������ת������־
 *
 *  @return ״̬����-1 ��ʾ��ȡ�쳣��������ʾ״̬��ֵ
 */
short MOTOR_ZDTUART_GetStates()
{
    // ��ʼ��
    u8 sendData[3] = {0x01, 0x3A, 0x6B}, *receiveData;
    short states;
    
    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 3); delay_ms(2);

    // ���ִ�н��
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return -1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 2; i++ ) if( sendData[i] != receiveData[i] || receiveData[3] != 0x6B ){ UART3_ResetReceiveData(); return -1; }
    
    // ����״̬��
    states = receiveData[2];

    // ���ý�������
    UART3_ResetReceiveData(); 
    
    // ����״̬��
    return states;
}


/**
 * ���Ƕ�ֵ���е��
 *
 * @param double degree  �Ƕȣ���λ����
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_RunDegree( double degree )
{
    // ��ʼ��
    u8 sendData[13] = {0x01, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0xFD, 0x02, 0x6B}, *receiveData;

    // �������͵��������
    u32 sendPulseTotal = (u32)(degree/(MOTOR_ZDTUART_STEP_DEGREE/GV_MotorZdtDiv));

    // ������������
    sendData[2]  = GV_MotorZdtDir;         // �����ת����
    sendData[3]  = GV_MotorZdtSpeed >> 8;  // ����ٶ� RPM����λ��
    sendData[4]  = GV_MotorZdtSpeed;       // ����ٶ� RPM����λ��
    sendData[5]  = 0;                      // ������ٶ�
    sendData[6]  = sendPulseTotal >> 24;   // ����������
    sendData[7]  = sendPulseTotal >> 16;   // ����������
    sendData[8]  = sendPulseTotal >> 8;    // ����������
    sendData[9]  = sendPulseTotal;         // ����������
    sendData[10] = 0;                      // �����ת�Ƕ�ģʽ��0 ��ʾ��ԽǶȣ�1��ʾ���ԽǶ�
    sendData[11] = 0;                      // ������ͬ��ģʽ��0 ��ʾ�رգ�1��ʾ����

    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 13); delay_ms(2);
    
    // ����ִ�н��
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    UART3_ResetReceiveData(); return 0;
}


/**
 * ����������״̬
 * ���������ɵı�־�����ݵ��״̬�� "��λ��־λ" �����ж�
 * �ڵ��������к�����ӳٸ� 10ms ����ȥִ����һ�����У�������ܳ��ֵ����粢ֹͣ
 * �²�ԭ�򣺵����������к��������ȥִ����һ�ε����У���ô�ܿ���������������������Ӧ���ӱ��϶�Ϊ��������ô������ǿ�ƻָ����������ֳ��
 *
 * @param u8 states  ���״̬����ͨ�� MOTOR_ZDTUART_GetStates() ��ȡ
 * @return ����״̬��-1 ��ʾ��ȡ�쳣��0 ��ʾ����δ��ɣ�1��ʾ�������
 */
short MOTOR_ZDTUART_RunFinish( short states )
{
    return states < 0 ? states : states & 0x02;
}


/**
 * �������
 * ִ��ʧ�ܿ���ԭ�򣺴����˶�ת���������ûʹ�ܡ���Ȧ��������λ��ֵ��Ч
 *
 * @param short mode  ����ģʽ��0x00 ��ʾ�ͽ����㣬0x01 ��ʾ��Ȧ�������
 *                    ��Ȧ�������ķ�����Ҫ�� MOTOR_ZDTUART_SetZeroConfig() �����ã�Ĭ��˳ʱ��
 *
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_Zero( short mode )
{
    // ��ʼ��
    u8 sendData[5] = {0x01, 0x9A, 0x00, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0x9A, 0x02, 0x6B}, *receiveData;

    // ������������
    sendData[2] = mode;   // ����ģʽ��0x00 ��ʾ������Ȧ�ͽ�����
                          // ����������0x01 ��ʾ������Ȧ�������
                          // ����������0x02 ��ʾ������Ȧ����λ��ײ����
                          // ����������0x03 ��ʾ������Ȧ����λ���ػ���
    
    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 5); delay_ms(2);
    
    // ���ִ�н��
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    UART3_ResetReceiveData(); return 0;
}


/**
 * ���õ�����
 *
 * @param bool delay  ��ʱ���أ�������ȫд�뵽��������Ҫһ����ʱ��ȴ���������ŷ��͵Ĵ�������޷�ִ�У�
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_SetZeroPoint( bool delay )
{
    // ��ʼ��
    u8 sendData[5] = {0x01, 0x93, 0x88, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0x93, 0x02, 0x6B}, *receiveData;

    // ������������
    sendData[3] = 0x00;  // �Ƿ�洢�����λ���Ƿ�ֱ�Ӵ洢��������

    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 5); delay_ms(2);
    
    // ���ִ�н��
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    
    // ���ý�������
    UART3_ResetReceiveData(); 
    
    // ��ʱ�ȴ�������ȫд��
    // Ϊ�˼� API �����ı��빤�������Ծͽ��ӳٺ�����ֱ��д����������������ڣ����øú���ʱ��Ҫע����������������һ��ʱ��
    // ԭ��д�����õ�������ʱ�������������ö�ʱ��ȥˢ������->�ر��ж�->�޸�����ֵ->�洢��Flash->Ȼ���ٳ�ʼ����������Ҫһ����ʱ�䡣����һ��ʱ���ڷ��͵��´�������ʱ�޷�ִ�е�
    // ���� Emm_V5.0.6 �汾�ĵ�����ѭ��ִ�з������ԣ��ӳ� 80ms �����������ݽ��գ�����ʹ�ó����������쳣�����Ը���ʵ������Ӵ��ӳ�ʱ��
    if( delay ) delay_ms(80); 
    
    // ����ִ�н��
    return 0;
}


/**
 * ��ȡ�����������
 * Ŀǰ�޷������������ݣ���������ִ�гɹ���ʧ�ܣ���ʱ��Ҫͨ�� �ϵ� �鿴
 *
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_GetZeroConfig()
{
    // ��ʼ��
    u8 sendData[5] = {0x01, 0x22, 0x6B}, *receiveData;

    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 3); delay_ms(4);
    
    // ���ִ�н��
    if( UART3_GetReceiveDataCount() < 18 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 2; i++ ) if( sendData[i] != receiveData[i] || receiveData[17] != 0x6B ){ UART3_ResetReceiveData(); return 2; }
    
    // ���ý�������
    UART3_ResetReceiveData(); 

    // ����ִ�н��
    return 0;
}


/**
 * ���õ����������
 *
 * @param bool delay  ��ʱ���أ�������ȫд�뵽��������Ҫһ����ʱ��ȴ���������ŷ��͵Ĵ�������޷�ִ�У�
 * @return �����룺0 ��ʾִ�гɹ�����0��ʾִ��ʧ��
 */
short MOTOR_ZDTUART_SetZeroConfig( bool delay )
{
    // ��ʼ��
    u8 sendData[20] = {0x01, 0x4C, 0xAE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0x4C, 0x02, 0x6B}, *receiveData;

    // ������������
    sendData[3]  = 0x00;  // �Ƿ�洢�����������Ƿ�ֱ�Ӵ洢��������
    sendData[4]  = 0x00;  // ����ģʽ��0x00 ��ʾ ��Ȧ�ͽ�����
    sendData[5]  = 0x00;  // ���㷽��0x00 ��ʾ ˳ʱ��(CW)

    sendData[6]  = 0x00;  // �����ٶȣ���λ��
    sendData[7]  = 0x1E;  // �����ٶȣ���λ����0x001E = 30��RPM��

    sendData[8]  = 0x00;  // ���㳬ʱʱ�䣨��λ��
    sendData[9]  = 0x00;  // ���㳬ʱʱ�䣨��λ��
    sendData[10] = 0x27;  // ���㳬ʱʱ�䣨��λ��
    sendData[11] = 0x10;  // ���㳬ʱʱ�䣨��λ����0x00002710 = 10000��ms��

    sendData[12] = 0x01;  // ����λ��ײ������ת�٣���λ��
    sendData[13] = 0x2C;  // ����λ��ײ������ת�٣���λ����0x012C = 300��RPM��

    sendData[14] = 0x03;  // ����λ��ײ�������������λ��
    sendData[15] = 0x20;  // ����λ��ײ�������������λ����0x012C = 800��mA��

    sendData[16] = 0x00;  // ����λ��ײ������ʱ�䣨��λ��
    sendData[17] = 0x3C;  // ����λ��ײ������ʱ�䣨��λ����0x003C = 60��ms��

    sendData[18] = 0x00;  // �ϵ��Զ��������㣺0x00 ��ʾ�رգ�0x01 ��ʾ����
    
    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 20); delay_ms(4);

    // ����ִ�н��
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    
    // ���ý�������
    UART3_ResetReceiveData(); 
        
    // ��ʱ�ȴ�������ȫд��
    // Ϊ�˼� API �����ı��빤�������Ծͽ��ӳٺ�����ֱ��д����������������ڣ����øú���ʱ��Ҫע����������������һ��ʱ��
    // ԭ��д�����õ�������ʱ�������������ö�ʱ��ȥˢ������->�ر��ж�->�޸�����ֵ->�洢��Flash->Ȼ���ٳ�ʼ����������Ҫһ����ʱ�䡣����һ��ʱ���ڷ��͵��´�������ʱ�޷�ִ�е�
    // ���� Emm_V5.0.6 �汾�ĵ�����ѭ��ִ�з������ԣ��ӳ� 80ms �����������ݽ��գ�����ʹ�ó����������쳣�����Ը���ʵ������Ӵ��ӳ�ʱ��
    if( delay ) delay_ms(80); 
    
    // ����ִ�н��
    return 0;
}


/**
 * ����������״̬
 *
 * @return  1 ��ʾ������ɣ�0 ��ʾ����δ��ɣ�-1 ��ʾ��ȡ�쳣
 */
short MOTOR_ZDTUART_ZeroFinish()
{
    // ��ʼ��
    u8 sendData[5] = {0x01, 0x3B, 0x6B}, *receiveData;
    short finish = 0;

    // ��������ӳٵȴ�ִ�н��
    // �ӳٵȴ�ԭ�򣺷���������������巴��������Ҫʱ��
    UART3_SendBytes(sendData, 3); delay_ms(2);

    // ���ִ�н��
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return -1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 2; i++ ) if( sendData[i] != receiveData[i]  || receiveData[3] != 0x6B ){ UART3_ResetReceiveData(); return -1; }

    // ������״̬
    if( (receiveData[2]&0x04) == 0x00 ) finish = 1;

    // ���ý�������
    UART3_ResetReceiveData(); 

    // �������״̬
    return finish;
}



/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */


/**
 * ���߼����������ָ���ǶȲ�����
 */
void MOTOR_ZDTUART_MainRunAndZero()
{
    // ������У����Ƕȣ�
    MOTOR_ZDTUART_RunDegree(170);
    while( MOTOR_ZDTUART_RunFinish(MOTOR_ZDTUART_GetStates()) == 0 ); 

    // ��ʱ 1 �����
    delay_ms(1000);

    // �������
    MOTOR_ZDTUART_Zero(MOTOR_ZDTUART_ZERO_NEAR); while( MOTOR_ZDTUART_ZeroFinish() == 0 );

    // ��ʱ 1 �뷵��
    delay_ms(1000);
}


/**
 * ���߼����������ָ���ǶȲ����㣨�������ģʽ��
 */
void MOTOR_ZDTUART_MainRunAndZeroWithPulse()
{
    // ������У����Ƕȣ�
    MOTOR_RunDegree(170);
    while( MOTOR_GetState() != MOTOR_STATE_IDLE );

    // ��ʱ 1 �����
    delay_ms(2000);

    // �������
    MOTOR_ZDTUART_Zero(MOTOR_ZDTUART_ZERO_NEAR); while( MOTOR_ZDTUART_ZeroFinish() == 0 );

    // ��ʱ 1 �뷵��
    delay_ms(2000);
    
}


/**
 * ���߼������������������
 * 
 * @param short degree  �����Ƕȣ���λ����
 */
void MOTOR_ZDTUART_MainTwoPointRunByDegree( short degree )
{
    MOTOR_ZDTUART_SetDir(MOTOR_ZDTUART_DIR_CW);
    MOTOR_ZDTUART_RunDegree(degree); 
    while( MOTOR_ZDTUART_RunFinish(MOTOR_ZDTUART_GetStates()) == 0 );
    delay_ms(1000);

    MOTOR_ZDTUART_SetDir(MOTOR_ZDTUART_DIR_CCW);
    MOTOR_ZDTUART_RunDegree(degree); while( MOTOR_ZDTUART_RunFinish(MOTOR_ZDTUART_GetStates()) == 0 );
    delay_ms(1000);
}
