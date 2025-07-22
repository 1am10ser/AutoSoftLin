#ifndef __TIM2_H_
#define __TIM2_H_



    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    void TIM2_PWMO_Init( u32 freq );  // ��ʼ�� TIM2 PWM ���

    void TIM2_PWMO_Stop();                              // ��ֹ TIM2 ͨ���� PWM ���
    bool TIM2_PWMO_Send( double duty, u32 sendLimit );  // ���� TIM2 ͨ���� PWM ���
    void TIM2_PWMO_SetDuty( double duty );              // ���� TIM2 ͨ���� PWM ��ռ�ձ�
    
    
    void TIM2_PWMI_Init();            // ��ʼ�� TIM2 PWM ����
    void TIM2_PWMI_InitData();        // 
    
    void TIM2_PWMI_Stop();            // ��ֹ TIM2 PWM ����
    void TIM2_PWMI_Start();           // ��ʼ TIM2 PWM ����
    
    u16    TIM2_PWMI_GetFreq();       // ��ȡ TIM2 ����� PWM Ƶ��
    double TIM2_PWMI_GetDuty();       // ��ȡ TIM2 ����� PWM ��ռ�ձ�



    /* -------------------------------------------------- */
    // �� �����������жϣ�
    /* -------------------------------------------------- */
    void TIM2_InterruptHandler();            // TIM2 �ж�
    void TIM2_InterruptHandlerUpdate();      // TIM2 �жϣ�������ֵ�������
    void TIM2_InterruptHandlerCapturePwm();  // TIM2 �жϣ�PWM ����
    
    
    
#endif