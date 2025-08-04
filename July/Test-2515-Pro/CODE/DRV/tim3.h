#ifndef __TIM3_H_
#define __TIM3_H_



    /* -------------------------------------------------- */
    // �� ����������API��
    /* -------------------------------------------------- */
    void TIM3_PWMO_Init( u32 freq );  // ��ʼ�� TIM3 PWM ���

    void TIM3_PWMO_Stop();                              // ��ֹ TIM3 ͨ���� PWM ���
    bool TIM3_PWMO_Send( double duty, u32 sendLimit );  // ���� TIM3 ͨ���� PWM ���
    void TIM3_PWMO_SetDuty( double duty );              // ���� TIM3 ͨ���� PWM ��ռ�ձ�
    
    
    void TIM3_PWMI_Init();            // ��ʼ�� TIM3 PWM ����
    void TIM3_PWMI_InitData();        //
    
    void TIM3_PWMI_Stop();            // ��ֹ TIM3 PWM ����
    void TIM3_PWMI_Start();           // ��ʼ TIM3 PWM ����
    
    u16    TIM3_PWMI_GetFreq();       // ��ȡ TIM3 ����� PWM Ƶ��
    double TIM3_PWMI_GetDuty();       // ��ȡ TIM3 ����� PWM ��ռ�ձ�



    /* -------------------------------------------------- */
    // �� �����������жϣ�
    /* -------------------------------------------------- */
    void TIM3_InterruptHandler();            // TIM3 �ж�
    void TIM3_InterruptHandlerUpdate();      // TIM3 �жϣ�������ֵ�������
    void TIM3_InterruptHandlerCapturePwm();  // TIM3 �жϣ�PWM ����
    
    
    
#endif