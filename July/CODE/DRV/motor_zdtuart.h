#ifndef __MOTOR_ZDTUART_H_
#define __MOTOR_ZDTUART_H_



    /* -------------------------------------------------- */
    // �� ���� ZDT UART ��ʽ������Emm_V5.0��
    // �� ʹ��ǰ����ȶ� UART3 ���г�ʼ����UART3_CInit();
    // �� ֮���Բ�ʹ�� UART1������Ϊ UART1 ��Ҫͨ�� USB �͵���ͨѶ��������Ҳ���� UART1 ͨѶ����ô�ͻ���Ϊ CH340G оƬ�����޷��յ����������ķ�������  
    // �� v4_20231021 �汾�Ժ󣬱����ֶ�������Ĵ��ں�UART3���к��ӣ�����ο�ԭ��ͼ
    // 
    // �� motor_zdtuart.c �� motor.c ����ȫ���������߼���û��������ϵ������ʹ�ø��Եĺ�����
    // �� ��Ȼ motor_zdtuart.c �� motor.c Ҳ���Ի��ã�ZDT ֧�� UART �� STEP/DIR ���ã��������� UART ����ϸ�ֵȲ�����STEP/DIR ���Ƶ������
    //
    // �� �� ZDT UART ��������� 0xF778 �汾���������д����ȡ�������ʹ�ú�����MOTOR_ZDTUART_GetVersion()
    // �� �汾���У���8λ��ʾ����汾��0xF4��Emm42_V5.0.0����0xF7��Emm42_V5.0.3��
    // �� �汾���У���8λ��ʾӲ���汾��0x78��ZDT_X42_V1.2��
    //
    // �� ע�⣺��������������¼ 0xFA��Emm42_V5.0.6�����򣬷��򴮿ڿ��ܳ���BUG
    // �� ע�⣺����һЩ����д�뵽�������API�ӿڣ�����������ӳٵȴ�����ô�ᵼ�µڶ��η��ʹ�������ִ��ʧ�����޷�������
    //         ԭ����д�����õ�������ʱ�������������ö�ʱ��ȥˢ������->�ر��ж�->�޸�����ֵ->�洢��Flash->Ȼ���ٳ�ʼ����������Ҫһ����ʱ�䡣����һ��ʱ���ڷ��͵��´�������ʱ�޷�ִ�е�
    /* -------------------------------------------------- */


    // MOTOR ������Ʋ���
    #define MOTOR_ZDTUART_DIR_CW        0x00  //����������� ǰ��/˳ʱ�� ����
    #define MOTOR_ZDTUART_DIR_CCW       0x01  //����������� ����/��ʱ�� ����
    #define MOTOR_ZDTUART_DIR_NEAR      0xFF  //����������� �ͽ� ����

    #define MOTOR_ZDTUART_ZERO_DIR      0x01  //�������������Ȧ����
    #define MOTOR_ZDTUART_ZERO_NEAR     0x00  //������������ͽ�����


    // MOTOR �����ز���
    #define MOTOR_ZDTUART_STEP_DEGREE   1.8  //���������á���� ������ ������δϸ�֣�����ͨ������ֲ������ȡ + ϸ��



    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    short  MOTOR_ZDTUART_GetDiv();
    short  MOTOR_ZDTUART_GetDir();
    short  MOTOR_ZDTUART_GetSpeed();
    double MOTOR_ZDTUART_GetDegree360();
    double MOTOR_ZDTUART_GetRunDegree360Near( double currDegree, double distDegree );

    short  MOTOR_ZDTUART_GetStates();
    long   MOTOR_ZDTUART_GetVersion();
    
    short  MOTOR_ZDTUART_Init();
    short  MOTOR_ZDTUART_SetDiv( short div, bool delay );
    short  MOTOR_ZDTUART_SetDir( short dir );
    short  MOTOR_ZDTUART_SetSpeed( short speed );

    short  MOTOR_ZDTUART_SetEnable();
    short  MOTOR_ZDTUART_SetDisable();

    short  MOTOR_ZDTUART_RunDegree( double degree );
    short  MOTOR_ZDTUART_RunFinish( short states );
    
    short  MOTOR_ZDTUART_Zero( short mode );
    short  MOTOR_ZDTUART_ZeroFinish();
    short  MOTOR_ZDTUART_SetZeroPoint( bool delay );
    short  MOTOR_ZDTUART_SetZeroConfig( bool delay );
    short  MOTOR_ZDTUART_GetZeroConfig();



    /* -------------------------------------------------- */
    // �� �������������߼���
    /* -------------------------------------------------- */
    void MOTOR_ZDTUART_MainRunAndZero();  // �������ָ���ǶȲ�����
    void MOTOR_ZDTUART_MainRunAndZeroWithPulse();  // �������ָ���ǶȲ����㣨�������ģʽ��
    void MOTOR_ZDTUART_MainTwoPointRunByDegree( short degree );  // ��������������У����ǶȲ�����
    
    
    
#endif