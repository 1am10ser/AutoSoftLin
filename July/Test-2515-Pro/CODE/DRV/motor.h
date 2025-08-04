#ifndef __MOTOR_H_
#define __MOTOR_H_



    /* -------------------------------------------------- */
    // �� ���� STEP/DIR ��ʽ����
    // �� ���� TMC2209 оƬ����֧�� STEP/DIR �� UART ����
    /* -------------------------------------------------- */


    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    /**
     * MOTOR ����
     */

    // ϸ��ģʽ����
    // ���ڲ��԰��� IO �ڲ����ã����� ��������� �޷���������ȫ��ϸ��ģʽ
    // Ŀǰ��������������ɶ���ϸ�ַ�������ʼһ��ϸ�֣�PE2�͵�ƽ�� + �����������һ��ϸ�֣�PE2����̬����
    // ���ڳ�ʼ�ĺ�������Ƶ�������ϸ����ҪԤ���� PCB ���Ͻ����������ã�PCB ����˿ӡΪ MS1��MS2��MS3 �� 5V��PE2 ����������ϣ�
    //
    // === ��A4988������������PCB���Ӻ�ϸ�������������£�
    //     ϸ�����ߣ�1 ϸ�ֺ� 8  ϸ�֣�MS1-[R]-5v, MS2-[R]-5v              |  MS1-PE2, MS2-PE2
    //     ϸ�����ߣ�2 ϸ�ֺ� 8  ϸ�֣�MS1-[R]-5v, MS2-[R]-5v              |         , MS2-PE2
    //     ϸ�����ߣ�4 ϸ�ֺ� 8  ϸ�֣�MS1-[R]-5v, MS2-[R]-5v              |  MS1-PE2,        
    //
    //     ϸ�����ߣ�1 ϸ�ֺ� 16 ϸ�֣�MS1-[R]-5v, MS2-[R]-5v, MS3-[R]-5v  |  MS1-PE2, MS2-PE2, MS3-PE2
    //     ϸ�����ߣ�2 ϸ�ֺ� 16 ϸ�֣�MS1-[R]-5v, MS2-[R]-5v, MS3-[R]-5v  |         , MS2-PE2, MS3-PE2
    //     ϸ�����ߣ�4 ϸ�ֺ� 16 ϸ�֣�MS1-[R]-5v, MS2-[R]-5v, MS3-[R]-5v  |  MS1-PE2,        , MS3-PE2
    //
    //     PCB ���ӣ�R17��R30���ã�5�ź�6�Ŷ̽�
    //     �������ӣ�https://item.taobao.com/item.htm?id=593072005558
    //
    //
    // === ��TB67S109������������PCB���Ӻ�ϸ�������������£�
    //     ϸ�����ߣ�1 ϸ�ֺ� 8  ϸ�ֵ����߷�ʽ��MS1-[R]-5v,           , MS3-[R]-5v  |  MS1-PE2
    //     ϸ�����ߣ�2 ϸ�ֺ� 8  ϸ�ֵ����߷�ʽ��MS1-[R]-5v,           , MS3-[R]-5v  |         ,        , MS3-PE2
    //     ϸ�����ߣ�2 ϸ�ֺ� 16 ϸ�ֵ����߷�ʽ��MS1-[R]-5v, MS2-[R]-5v,             |  MS1-PE2
    //
    //     PCB ���ӣ�R17��R30����
    //     �������ӣ�https://item.taobao.com/item.htm?id=558783722553
    //
    //
    // === ��TMC2209������������PCB���Ӻ�ϸ�������������£�
    //     ϸ�����ߣ�8 ϸ�ֺ� 16 ϸ�ֵ����߷�ʽ��MS1-[R]-5v, MS2-[R]-5v,             |  MS1-PE2, MS2-PE2
    //     ϸ�����ߣ�8 ϸ�ֺ� 32 ϸ�ֵ����߷�ʽ��MS1-[R]-5v, MS2-[R]-5v,             |  MS1-PE2
    //     ϸ�����ߣ�8 ϸ�ֺ� 64 ϸ�ֵ����߷�ʽ��MS1-[R]-5v, MS2-[R]-5v,             |         , MS2-PE2
    //
    //     PCB ���ӣ�R17��R30���ã�R212(MS3) �ÿ�
    //     �������ӣ�https://item.taobao.com/item.htm?id=610229295552
    //     ��ע˵����TMC2208/TMC2226 ֧��2��4ϸ�֣�����μ���https://item.taobao.com/item.htm?id=677273237658


    // MOTOR ϸ�ֿ��Ʋ���
    #define MOTOR_DIV_PIN         GPIO_PIN_2
    #define MOTOR_DIV_PORT        GPIOE
    #define MOTOR_DIV_LOW         8   //���������á���ʼ��ϸ�֣�PE2�͵�ƽ��
    #define MOTOR_DIV_HIGH        16  //���������á���ص�ϸ�֣�PE2����̬��
 
    // MOTOR ������Ʋ���
    #define MOTOR_DIR_PIN         GPIO_PIN_1
    #define MOTOR_DIR_PORT        GPIOE
    #define MOTOR_DIR_CW          0  //����������� ǰ��/˳ʱ�� ����
    #define MOTOR_DIR_CCW         1  //����������� ����/��ʱ�� ����
       
    // MOTOR �������Ʋ���
    #define MOTOR_ENABLE_PIN      GPIO_PIN_6
    #define MOTOR_ENABLE_PORT     GPIOA

    // MOTOR �����ز���
    #define MOTOR_CIRCLE_LEN      4    //���������á������һȦǰ���ĺ���ֵ����ͨ������ֲ������ȡ����������Ҫ���ڴ�˿�˵ĵ��
    #define MOTOR_STEP_DEGREE     1.8  //���������á���� ������ ������δϸ�֣�����ͨ������ֲ������ȡ + ϸ��

    // MOTOR ������㴥����
    // �����Ҫ���õ�������жϹ��ܣ���ô��Ҫ���� EXTI ģ��
    // ��������жϹ�����Ҫ���ڵ�������в�����ԭ��ʱ�ĵ��ֹͣ����
    #define MOTOR_ZERO_EXTINUM    60  //���������á��ж����ţ�J60 �� J61
    #define MOTOR_ZERO_EXTITYPE   0   //���������á��ж����ͣ�0 ��ʾ�½��أ�1 ��ʾ������

    // �������״̬
    // ״̬Ϊ 0 ʱ��ʾ�����ֹ��״̬�� 0 ʱ��ʾ���������
    // �������е�״ֵ̬���Ƕ����ģ����ް�����ϵ���Լ�����ѡ��״̬��������
    #define MOTOR_STATE_IDLE          0    // ����
    #define MOTOR_STATE_RUN           10   // �����У������У�
    #define MOTOR_STATE_RUN_CW        11   // �����У�˳ʱ�������У�
    #define MOTOR_STATE_RUN_CCW       12   // �����У���ʱ�������У�
    #define MOTOR_STATE_RUN_FORWARD   13   // �����У���ǰ�����У�
    #define MOTOR_STATE_RUN_BACKWARD  14   // �����У���������У�
    #define MOTOR_STATE_ZERO          20   // �����У����㣩
    #define MOTOR_STATE_ZERO_CW       21   // �����У�����˳ʱ�������У�
    #define MOTOR_STATE_ZERO_CCW      22   // �����У�����˳ʱ�������У�
    #define MOTOR_STATE_ZERO_FORWARD  23   // �����У�������ǰ�����У�
    #define MOTOR_STATE_ZERO_BACKWARD 24   // �����У�������������У�


       
    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    void   MOTOR_Init();               // ��ʼ��
    void   MOTOR_InitCount();          // ��ʼ������

    void   MOTOR_SetDir( short dir );  // ���õ������
    short  MOTOR_GetDir();             // ��ȡ�������
    
    void   MOTOR_SetDiv( short div );  // ���õ��ϸ��
    short  MOTOR_GetDiv();             // ��ȡ���ϸ��
    
    short  MOTOR_GetSpeed();                // ��ȡ��������ٶȣ�RPM��
    void   MOTOR_InitSpeed( short speed );  // ���õ�������ٶȣ�RPM��
    
    void   MOTOR_SetState( short state );      // ���õ����ǰ״̬
    short  MOTOR_GetState();                   // ��ȡ�����ǰ״̬
    void   MOTOR_SetStatePrev( short state );  // ���õ����һ��״̬
    short  MOTOR_GetStatePrev();               // ��ȡ�����һ��״̬
    
    void   MOTOR_Enable();     // �������
    void   MOTOR_Disable();    // �������
    void   MOTOR_CountStep();  // �������                          

    long   MOTOR_GetCountPulse();   // ��ȡ�ۼƵ�����
    long   MOTOR_GetCountCircle();  // ��ȡ�ۼƵ�Ȧ��
    double MOTOR_GetCountDegree();  // ��ȡ�ۼƵĽǶ�
    
    bool   MOTOR_RunStop();  // ���ֹͣ
    bool   MOTOR_RunKeep();  // �������е��
    bool   MOTOR_RunStep( u32 number );       // ���������е��
    bool   MOTOR_RunLength( double length );  // ������ֵ���е��
    bool   MOTOR_RunDegree( double degree );  // ���Ƕ�ֵ���е��


       
    /* -------------------------------------------------- */
    // �� �����������жϣ�
    /* -------------------------------------------------- */
    void   MOTOR_InterruptHandlerPulse();
    void   MOTOR_InterruptHandlerZeroFinish( short extiNum, short extiType );


       
    /* -------------------------------------------------- */
    // �� �������������߼���
    /* -------------------------------------------------- */
    void   MOTOR_MainTwoPointRunByLength( short length );  // ��������������У������Ȳ�����
    void   MOTOR_MainTwoPointRunByDegree( short degree );  // ��������������У����ǶȲ�����
    
    void   MOTOR_MainKeyControlMotorReal();
    void   MOTOR_MainKeyControlMotorByLength( double length );
    
    
    
#endif