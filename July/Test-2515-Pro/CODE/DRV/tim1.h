#ifndef __TIM1_H_
#define __TIM1_H_



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    /**
     * Tim1 ����
     */
    #define TIM1_NUMBER_ALL   9999



    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    void TIM1_PWMO_Init( u32 freq );           // ��ʼ�� TIM1 PWM ���

    void TIM1_PWMO_Stop( short number );                              // ��ֹ TIM1 ͨ���� PWM ���
    bool TIM1_PWMO_Send( short number, double duty, u32 sendLimit );  // ���� TIM1 ͨ���� PWM ���
    void TIM1_PWMO_SetDuty( short number, double duty );              // ���� TIM1 ͨ���� PWM ��ռ�ձ�
    
    
    void TIM1_PWMI_Init();                     // ��ʼ�� TIM1 PWM ����
    void TIM1_PWMI_Init56();                   //
    void TIM1_PWMI_Init57();                   //
    void TIM1_PWMI_InitData56();               //
    void TIM1_PWMI_InitData57();               //
    
    void TIM1_PWMI_Stop();                     // ��ֹ TIM1 PWM ����
    void TIM1_PWMI_Start();                    // ��ʼ TIM1 PWM ����
    
    u16    TIM1_PWMI_GetFreq( short number );  // ��ȡ TIM1 ����� PWM Ƶ��
    double TIM1_PWMI_GetDuty( short number );  // ��ȡ TIM1 ����� PWM ��ռ�ձ�



    /* -------------------------------------------------- */
    // �� �����������жϣ�
    /* -------------------------------------------------- */
    void TIM1_InterruptHandler();              // TIM1 �ж�
    void TIM1_InterruptHandlerUpdate();        // TIM1 �жϣ�������ֵ�������
    void TIM1_InterruptHandlerCapturePwm56();  // TIM1 �жϣ�PWM ����J56�ӿڣ�
    void TIM1_InterruptHandlerCapturePwm57();  // TIM1 �жϣ�PWM ����J57�ӿڣ�
    
    
    
#endif