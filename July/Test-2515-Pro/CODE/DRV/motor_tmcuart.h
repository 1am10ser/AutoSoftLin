#ifndef __MOTOR_TMCUART_H_
#define __MOTOR_TMCUART_H_



    /* -------------------------------------------------- */
    // �� ���� TMC UART ��ʽ����
    // �� ʹ��ǰ����ȶ� UART1 ���г�ʼ����UART1_CInit();
    // �� ���еļĴ�����ַ�͵�������߼����� TMC2209 оƬ���������ֲ�
    // �� v4_20231021 �汾�Ժ󣬱����ֶ�������Ĵ��ں�UART1���к��ӣ�����ο�ԭ��ͼ
    // 
    // �� motor_tmcuart.c �� motor.c ����ȫ���������߼���û��������ϵ������ʹ�ø��Եĺ�����
    // �� ��Ȼ motor_tmcuart.c �� motor.c Ҳ���Ի��ã�TMC2209 ֧�� UART �� STEP/DIR ���ã��������� UART ����ϸ�ֵȲ�����STEP/DIR ���Ƶ������
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    // MOTOR TMC-UART �������Ʋ���
    #define MOTOR_TMCUART_ENABLE_PIN     GPIO_PIN_6
    #define MOTOR_TMCUART_ENABLE_PORT    GPIOA



    /* -------------------------------------------------- */
    // �� �Ĵ�����ַ
    /* -------------------------------------------------- */
    // ����μ� TMC2209 �����ֲ� 20+ ҳ

    // Ĭ�ϼĴ���
    #define REGADDR_IHOLD_IRUN 0x10  // Ĭ�ϵ�������
    #define REGADDR_GCONF      0x00  // ͨ�����ã�General Configuration Registers��
    #define REGADDR_GSTAT      0x01  // ȫ��״̬�Ĵ�����Global Status Flags��
    #define REGADDR_CHOPCONF   0x6C  // ն�������ã�Chopper Configuration��
    #define REGADDR_VACTUAL    0x22  // ��������ٶȣ�Velocity Actual��
    #define REGADDR_TSTEP      0x12  // ΢��ʱ�䣨Time Step������ 1/fCLK Ϊ��λ�Ĳ��������ź��ϵ����� 1/256 ΢��֮���ʵ�ʲ���ʱ��
    #define REGADDR_DRV_STATUS 0x6F  // ����״̬�Ĵ�����Driver Status Flags��
    #define REGADDR_IFCNT      0x02  // UARTдָ�������ÿ�ɹ�д��UART�ӿڣ��üĴ��������ӣ����Զ���������鴮�д������Ƿ��ж�ʧ������

    // ʧ��������StallGuard Control����ؼĴ���
    #define REGADDR_SG_RESULT  0x41  // ʧ�������Ľ����StallGuard Result)����ȡ�� SG_RESULT ֵԽ����ô��ʾ��ʱ������غܸߣ���֮��ʾ������غܵ�
    #define REGADDR_TPWMTHRS   0x13 
    #define REGADDR_TCOOLTHRS  0x14  // ����ģʽ����ֵ��ʧ������ʧ��ʱDIAG�����CoolStep Threshold��:һ��ֱ������Ϊ���ֵ 1048575
    #define REGADDR_COOLCONF   0x42  // ������Դ���ƣ�Smart Energy Control CoolStep��



    /* -------------------------------------------------- */
    // �� ����������API��
    // �� ���� MOTOR_TMCUART_SetXXX ��ͷ�ĺ��������Զ���ȡ�Ĵ���ֵȻ����д�루ʹ��ʱע�⺯����Ч�ʣ�ÿ���ô��ຯ��������ȥ��ȡԴֵ��д�룩
    // �� ���� MOTOR_TMCUART_BuildXXX ��ͷ�ĺ���������ȡ�Ĵ���ֵ
    /* -------------------------------------------------- */
    void MOTOR_TMCUART_Init();                                            // ��ʼ������
    long MOTOR_TMCUART_ReadReg( u8 address );                             // ��ȡ�Ĵ���ֵ
    void MOTOR_TMCUART_WriteReg( u8 address, long value );                // д�Ĵ���ֵ

    void MOTOR_TMCUART_Enable();                                          // ������ã�ͨ��оƬ���õ����ſ��ƣ�
    void MOTOR_TMCUART_Disable();                                         // ������ã�ͨ��оƬ���õ����ſ��ƣ�

    long MOTOR_TMCUART_GetCmdTotal();                                     // ��ȡUARTд��ɹ���ָ�����
    long MOTOR_TMCUART_GetStopFlag( long regDrvStatus, short getDelay );  // ��ȡ�����ֹ���
    long MOTOR_TMCUART_GetRunModeFlag( long regDrvStatus );               // ��ȡ�������ģʽ
    long MOTOR_TMCUART_GetReverseFlag( long regGConfValue );              // ��ȡ�����ת���
    
    void MOTOR_TMCUART_SetReverse();                                      // ���õ����ת
    long MOTOR_TMCUART_BuildReverse( long regGConfValue );                // �����Ĵ���ֵ�������ת
    
    void MOTOR_TMCUART_SetCoolStep( long sgResult );                      // �������ܵ�������
    long MOTOR_TMCUART_BuildCoolConf( u8 semin, u8 semax );               // �����Ĵ���ֵ�����ܵ�����������
    void MOTOR_TMCUART_CloseCoolStep();                                   // �ر����ܵ�������

    void MOTOR_TMCUART_SetIRunIHold( u8 irun, u8 ihold );                 // ���õ����������
    long MOTOR_TMCUART_BuildIRunIHold( u8 irun, u8 ihold );               // �����Ĵ���ֵ�������������
    
    void MOTOR_TMCUART_SetMircoStep( short value );                                 // ����ϸ��
    long MOTOR_TMCUART_BuildMircoStep( long regChopConfValue, short value );        // �����Ĵ���ֵ������ϸ�� 
    long MOTOR_TMCUART_BuildMircoStepRegSelect( long regGConfValue, short value );  // �����Ĵ���ֵ��ϸ�ּĴ���ѡ�� 

    void MOTOR_TMCUART_SetDriverEnable();                                           // �����������ã�ͨ��оƬ�Ĵ������ÿ��ƣ�
    void MOTOR_TMCUART_SetDriverDisable();                                          // �����������ã�ͨ��оƬ�Ĵ������ÿ��ƣ�
    long MOTOR_TMCUART_BuildDriverEnable( long regChopConfValue, short value );     // �����Ĵ���ֵ��������������/����
    
    u8 MOTOR_TMCUART_CalcCRC( u8 *data, u8 dataStart, u8 dataLength );              // ���㲢���� CRC ֵ
    


    /* -------------------------------------------------- */
    // �� �������������߼���
    /* -------------------------------------------------- */
    void MOTOR_TMCUART_MainInit();                              // ���߼�����ʼ��
    void MOTOR_TMCUART_MainRunKeep( short mstep, long speed );  // ���߼��������������
    void MOTOR_TMCUART_MainRoundTrip();                         // ���߼����������2������
    void MOTOR_TMCUART_MainStallReverse( long stallSGResult );  // ���߼��������ת��ת
    void MOTOR_TMCUART_MainMircoStepLoop();                     // ���߼���ѭ��ִ��ȫ��ϸ��ֵ
    void MOTOR_TMCUART_MainMonitorSGResult();                   // ���߼������� SG_RESULT ֵ
    
    
#endif