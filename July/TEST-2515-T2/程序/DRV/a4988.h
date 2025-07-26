#ifndef __A4988_H_
#define __A4988_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    /**
     * A4988 ����
     */

    // A4988 ����ģʽ����
    // ���ڲ��԰��� IO �ڲ����ã����� A4988 �޷���������ȫ��ϸ��ģʽ
    // Ŀǰ��������������ɶ���ϸ�ַ�������ʼһ��ϸ�� + �����������һ��ϸ�֡�
    //
    // ���ڳ�ʼ�ĺ�������Ƶ�������ϸ����ҪԤ���� PCB ���Ͻ����������ã�PCB ����˿ӡΪ MS1��MS2��MS3 �� 5V��PE2 ����������ϣ�
    // 1 ϸ�ֺ� 8  ϸ�ֵ����߷�ʽ��5v-[R]-MS1, 5v-[R]-MS2              |  MS1-PE2, MS2-PE2
    // 2 ϸ�ֺ� 8  ϸ�ֵ����߷�ʽ��5v-[R]-MS1, 5v-[R]-MS2              |         , MS2-PE2
    // 4 ϸ�ֺ� 8  ϸ�ֵ����߷�ʽ��5v-[R]-MS1, 5v-[R]-MS2              |  MS1-PE2,        
    //
    // 1 ϸ�ֺ� 16 ϸ�ֵ����߷�ʽ��5v-[R]-MS1, 5v-[R]-MS2, 5v-[R]-MS3  |  MS1-PE2, MS2-PE2, MS3-PE2
    // 2 ϸ�ֺ� 16 ϸ�ֵ����߷�ʽ��5v-[R]-MS1, 5v-[R]-MS2, 5v-[R]-MS3  |         , MS2-PE2, MS3-PE2
    // 4 ϸ�ֺ� 16 ϸ�ֵ����߷�ʽ��5v-[R]-MS1, 5v-[R]-MS2, 5v-[R]-MS3  |  MS1-PE2,        , MS3-PE2
    // ...
    //
    // ���Ϊ TesterProductA.v5_20210604  ��PCB���ϸ�ֿ���֧���� PCB ��ֱ�Ӻ��ӵ����ʼһ��ϸ�֣�Ҳ����ͨ���������ӵ� PE2 Ȼ�������������һ��ϸ��
    // ���Ϊ TesterProductA.v5_20210605+ ��PCB���ϸ�ֿ���֧���� PCB ��ֱ�Ӻ��ӵ����ʼһ��ϸ�֣�Ҳ����ͨ���������ӵ� PE2 Ȼ�������������һ��ϸ��
    #define A4988_MS_PIN               GPIO_PIN_2
    #define A4988_MS_PORT              GPIOE
    #define A4988_MS_MODE0_SUBDIV      2  //���������á���ʼ��ϸ�֣����ӵ��裬PE2 �͵�ƽ��
    #define A4988_MS_MODE1_SUBDIV      8  //���������á���ص�ϸ�֣�������ƣ�PE2 ����̬��

    // A4988 �����������
    #define A4988_DIR_PIN              GPIO_PIN_1
    #define A4988_DIR_PORT             GPIOE
    #define A4988_DIR_BACK             0  //����������� ����/��ʱ�� ����
    #define A4988_DIR_FRONT            1  //����������� ǰ��/˳ʱ�� ����

    // A4998 ��ӵĵ������
    #define A4988_MOTOR_PWM_FREQ       1000 //���������á����������������Ĭ��Ƶ�ʣ���ֵ��Ҫͨ�����ӵ��ʵ�ʲ���ȷ��
    #define A4988_MOTOR_STEP_DEGREE    1.8  //���������á���� ������ ������δϸ�֣�����ͨ������ֲ������ȡ + ϸ��

    #define A4988_MOTOR_CIRCLE_LEN     4    //���������á������һȦǰ���ĺ���ֵ����ͨ������ֲ������ȡ����������Ҫ���ڴ�˿�˵ĵ��
    #define A4988_MOTOR_CIRCLE_TIME    175  //���������á������Ĭ��Ƶ����һȦ����ĺ�������δϸ�֣�����ͨ��ʵ���ȡ����Ҫ���ڵ������ʱ�ȴ�
                                            //���������������øñ�����ͬ��ֵ��Ȼ��ͨ�����´��룬�鿴����Ƿ�����������һ�Σ�������������ɣ���ֵ�辡����ȷ������ϸ�ֺ������󣩣�
                                            //������������A4988_RunLength(A4988_DIR_BACK, A4988_MOTOR_CIRCLE_LENGTH); A4988_RunLengthWait(A4988_MOTOR_CIRCLE_LENGTH); A4988_RunLength(A4988_DIR_FRONT,A4988_MOTOR_CIRCLE_LENGTH); A4988_RunLengthWait(A4988_MOTOR_CIRCLE_LENGTH);  
                                            //������������A4988_RunDegree(A4988_DIR_BACK, 360); A4988_RunDegreeWait(360); A4988_RunDegree(A4988_DIR_FRONT,360); A4988_RunDegreeWait(360);
    // A4988 ��ӵĵ����λ������
    #define A4988_MOTOR_ZERO_PIN       0
    #define A4988_MOTOR_ZERO_PORT      0



    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void   A4988_Init();
    void   A4988_InitCount();

    void   A4988_SetDir( short dir );
    short  A4988_GetDir();
    
    void   A4988_SetSubdiv( short subdiv );
    short  A4988_GetSubdiv();
    
    void   A4988_CountStep();
    
    long   A4988_GetCountCircle();
    double A4988_GetCountDegree();
    
    bool   A4988_RunKeep( short dir, short runKeepProtectPrev );
    bool   A4988_RunKeeping( short dir );
    bool   A4988_RunKeepStop();
    
    bool   A4988_RunStep( short dir, u32 number );
    void   A4988_RunStepWait( u32 number );
    
    bool   A4988_RunLength( short dir, double length );
    void   A4988_RunLengthWait( double length );

    bool   A4988_RunDegree( short dir, double degree );
    void   A4988_RunDegreeWait( double degree );
    
    void   A4988_InterruptHandlerRuning();
    
    void   A4988_MainInit();
    void   A4988_MainKeyControlMotorReal();
    void   A4988_MainKeyControlMotorByLength( double length );
    
    
#endif