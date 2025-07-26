#include "common.h"



/* -------------------------------------------------- */
// �� MAGNETIC ���� ADC2 �� 3 �ű��
// �� ʹ��ǰ����ȶ� ADC2 ���г�ʼ��
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * MAGNETIC ��������
 */

//��ϵͳʹ�á�MAGNETIC ʹ�õ� ADC2 ��� 
short GV_MagneticADC2Num = 3;

//��ϵͳʹ�á�MAGNETIC �� ADC2 ������ѹֵ 
float GV_MagneticADC2Volt = 0;



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * �ų���Ӧ��ʼ��
 */
void MAGNETIC_Init()
{
    // GPIO_MODE_OUT_OD_LOW_SLOW   ���ٿ�©����͵�ƽ
    // GPIO_MODE_OUT_PP_LOW_SLOW   ������������͵�ƽ
    // GPIO_MODE_OUT_OD_HIZ_SLOW   ���ٿ�©�������̬
    // GPIO_MODE_OUT_PP_HIGH_SLOW  ������������ߵ�ƽ
    
    // �����������Ŷ�Ӧ��оƬ�ڲ����� һ��NPN������(���缫C���ڽ�V+) �� һ��PNP������(���缫C���ڽ�V-)
    // �������ƣ�������NPN�����ܲ��ӷ����E����Ϊ����ڣ�V+��
    // �������죺������PNP�����ܲ��Ӽ��缫C����Ϊ����ڣ�V-��
    // ��������Ĵ���ԭ��Ӧ����ˣ�STMоƬ�������û�� V- ����ѹ���� 0V�����Ե͵�ƽ���� 0V�����������
    // �����������ֱ������ߵ�ƽ���������������͵�Դ�Ϳ�����������
    
    // ��©�������Ŷ�Ӧ��оƬ�ڲ�����һ��NPN�����ܣ����缫C��ʲô�����ӣ�ֱ����Ϊ����˿�
    // ��©���ֻ������͵�ƽ�͸���̬�����Ա��������������͵�Դ��������ߵ�ƽȥ�������أ���ѹ����ӵ�Դ����
  
    // ��ʼ��
    GPIO_Init(MAGNETIC_LED_PORT, MAGNETIC_LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
}


/**
 * �����ų���ѹֵ������
 */
float MAGNETIC_Sampling()
{
    GV_MagneticADC2Volt = ADC2_GetVoltValue(GV_MagneticADC2Num);
    return GV_MagneticADC2Volt;
}


/**
 * ��ȡ�ų��ż�
 *
 * @param bool sampling  �Ƿ���дų���ѹֵ����
 * @return char  N ��ʾ������S ��ʾ�ϼ���* ��ʾδ֪���ߴż�����
 */
char MAGNETIC_GetMagnetic( bool sampling )
{
    // �ų���ѹֵ����
    if( sampling ) MAGNETIC_Sampling();

    // ��ʼ�޴ų�ʱ��ѹ
    // ���� WSH315 оƬ������
    // ע��ÿ��оƬ�޴ų�ʱ���ܲ�ͬ������� 2.55 �ǲɼ��ڵ�ǰ�����ڰ�����оƬ��ͬʱ����������12V�������¼��5V���磬��ô��ֵ�᲻һ����
    float noMagneticVolt = 2.55;  // �޴ų�ʱ��ѹ
    float noMagneticVoltBound = 0.05;  // �޴ų���ѹֵ���
    
    // �ż��ж�
    if( GV_MagneticADC2Volt > (noMagneticVolt+noMagneticVoltBound) ) return 'S';
    if( GV_MagneticADC2Volt < (noMagneticVolt-noMagneticVoltBound) ) return 'N';
    
    // ����δ֪
    return '*';
}


/**
 * ��ȡ�ų���˹
 *
 * @param bool sampling  �Ƿ���дų���ѹֵ����
 * @return char  N ��ʾ������S ��ʾ�ϼ���* ��ʾδ֪���ߴż�����
 */
u16 MAGNETIC_GetMagneticGauss( bool sampling )
{
    // �ų���ѹֵ����
    if( sampling ) MAGNETIC_Sampling();

    // ÿ��˹�仯��Ӧ�ĵ�ѹֵ
    float voltPerGauss = 0.0015;  // 1.5mV/Gauss
    
    // ��ʼ�޴ų�ʱ��ѹ
    // ���� WSH315 оƬ������
    // ע��ÿ��оƬ�޴ų�ʱ���ܲ�ͬ������� 2.55 �ǲɼ��ڵ�ǰ�����ڰ�����оƬ��ͬʱ����������12V�������¼��5V���磬��ô��ֵ���� 2.64��
    float noMagneticVolt = 2.55;  // �޴ų�ʱ��ѹ
    float noMagneticVoltBound = 0.05;  // �޴ų���ѹֵ���
    
    // �ż��ж�
    if( GV_MagneticADC2Volt > (noMagneticVolt+noMagneticVoltBound) ) return (u16)((GV_MagneticADC2Volt-noMagneticVolt)/voltPerGauss);
    if( GV_MagneticADC2Volt < (noMagneticVolt+noMagneticVoltBound) ) return (u16)((noMagneticVolt-GV_MagneticADC2Volt)/voltPerGauss);
    
    // ����δ֪
    return 0;
}



/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */

/**
 * Magnetic ���߼����ϱ�����ʾ��Ӧ�ĵ�
 */
void MAGNETIC_MainMagneticShowLed()
{
    // ��ʼ��
    char magnetic = MAGNETIC_GetMagnetic(true);
    static u8 unknowMagnetic = 0;
    
    // �ż��жϲ�������Ӧ LED ��
    if( magnetic == 'N' ){ MAGNETIC_LED_N; unknowMagnetic = 0; return; }
    if( magnetic == 'S' ){ MAGNETIC_LED_S; unknowMagnetic = 0; return; }
    
    // δ֪�ż� LED ����˸
    if( unknowMagnetic++ > 200 ) unknowMagnetic = 1;
    if( unknowMagnetic%2 == 0 ){ MAGNETIC_LED_N; }else{ MAGNETIC_LED_S; }
}