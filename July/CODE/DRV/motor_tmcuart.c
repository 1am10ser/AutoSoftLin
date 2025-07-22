#include "common.h"



/* -------------------------------------------------- */
// �� ���� TMC UART ��ʽ����
// �� ʹ��ǰ����ȶ� UART1 ���г�ʼ����UART1_CInit();
// �� ���еļĴ�����ַ�͵�������߼����� TMC2209 оƬ���������ֲ�
//
// �� motor_tmcuart.c �� motor.c ����ȫ���������߼���û��������ϵ������ʹ�ø��Եĺ�����
// �� ��Ȼ motor_tmcuart.c �� motor.c Ҳ���Ի��ã�TMC2209 ֧�� UART �� STEP/DIR ���ã��������� UART ����ϸ�ֵȲ�����STEP/DIR ���Ƶ������
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */

/**
 * ��ʼ������
 */
void MOTOR_TMCUART_Init()
{
    // ��ʼ��
    long regval;
    
    // ������˫��
    UART1_HalfDuplexCmd(ENABLE);
    
    // ��ʼ������
    GPIO_Init(MOTOR_TMCUART_ENABLE_PORT, MOTOR_TMCUART_ENABLE_PIN, GPIO_MODE_OUT_PP_LOW_FAST);  // ������������͵�ƽ
    MOTOR_TMCUART_Enable();
    
    // ��ʼȫ������
    regval = MOTOR_TMCUART_ReadReg(REGADDR_GCONF);
    regval = regval | 0x00000040;  // ���� GCONFI.pdn_disable = 1�����ڹر� PDN_UART �ⲿ����
    MOTOR_TMCUART_WriteReg(REGADDR_GCONF, regval);
}


/**
 * �������
 * ͨ��оƬ���õ����ſ���
 */
void MOTOR_TMCUART_Enable()
{
    GPIO_WriteLow(MOTOR_TMCUART_ENABLE_PORT, MOTOR_TMCUART_ENABLE_PIN);
}


/**
 * �������
 * ͨ��оƬ���õ����ſ���
 */
void MOTOR_TMCUART_Disable()
{
    GPIO_WriteHigh(MOTOR_TMCUART_ENABLE_PORT, MOTOR_TMCUART_ENABLE_PIN);
}


/**
 * ��ȡ�Ĵ���ֵ
 *
 * param u8 address  �Ĵ�����ַ����� motor_tmcuart.h ����� REGADDR_XXX �궨��
 * return long  �Ĵ���ֵ��-1 ��ʾ������Ч
 */
long MOTOR_TMCUART_ReadReg( u8 address )
{
    // ��ʼ��
    u8 i, crc;
    u8 recData[20], recDataValid;
    u8 sendCommand[4] = {0x05,0x00,0x00,0x00};
    long recDatValue = 0;

    // �Ĵ�����ַ
    sendCommand[2] = address;
    
    // ������������ CRC ֵ
    MOTOR_TMCUART_CalcCRC(sendCommand, 0, 4);
    
    // ���ý��յ�������
    UART1_ResetReceiveData();
    
    // ��������
    UART1_SendBytes(sendCommand, 4); 
    
    // �ӳٵȴ����ݽ������
    delay_ms(10);

    // ��ȡ���յ�������
    UART1_CopyReceiveData(recData);

    // ���յ���������Ч�Լ�⣨���͵������⣩
    recDataValid = 1;
    for( i = 0; i < 4; i++ ) recDataValid &= recData[i] == sendCommand[i];
    if( recDataValid == 0 ) return -1;
    
    // ���յ���������Ч�Լ�⣨��Ӧ������ͷ��⣩
    if( recData[4] != 0x05 ) return -1;
    if( recData[5] != 0xFF ) return -1;
    if( recData[6] != sendCommand[2] ) return -1;

    // ���յ���������Ч�Լ�⣨CRC��⣩
    crc = recData[11];
    if( crc != MOTOR_TMCUART_CalcCRC(recData,4,8) ) return -1;
    
    // ��������λֵ
    recDatValue |= (long)recData[7] << 24;
    recDatValue |= (long)recData[8] << 16;
    recDatValue |= (long)recData[9] <<  8;
    recDatValue |= (long)recData[10];

    // ����ֵ
    return recDatValue;
}


/**
 * д�Ĵ���ֵ
 *
 * param u8   address  �Ĵ�����ַ����� motor_tmcuart.h ����� REGADDR_XXX �궨��
 * param long value    �Ĵ���ֵ
 */
void MOTOR_TMCUART_WriteReg( u8 address, long value )
{
    // ��ʼ��
    u8 sendCommand[8] = {0x00};
    
    // ��������
    sendCommand[0] = 0x05;
    sendCommand[1] = 0x00;                   // оƬ��ţ�Ĭ�� 0x00
    sendCommand[2] = address | 0x80;         // Ҫд�����ݵļĴ�����ַ
    sendCommand[3] = (value >> 24) & 0xFF;   // & 0xFF ��Ҫ���þ��ǵ� value Ϊ����ʱ���������ƶ�����ô���������ݿ�λ�����Զ���� 0xF��ʹ��ʱ���׳�����������Ҫ�����ݿ�λ���Ϊ 0
    sendCommand[4] = (value >> 16) & 0xFF;
    sendCommand[5] = (value >> 8 ) & 0xFF;
    sendCommand[6] = (value      ) & 0xFF;
        
    // ������������ CRC ֵ
    MOTOR_TMCUART_CalcCRC(sendCommand, 0, 8);
    
    // ��������
    UART1_SendBytes(sendCommand, 8);
}


/**
 * ��ȡUARTд��ɹ���ָ�����
 * 
 * return ����ֵ��0~255��ѭ����
 */
long MOTOR_TMCUART_GetCmdTotal()
{
    return MOTOR_TMCUART_ReadReg(REGADDR_IFCNT);
}


/**
 * ���õ����ת
 * �ú�����ϸ����غ��������˷�װ������ʹ�ò���
 */
void MOTOR_TMCUART_SetReverse()
{
    // ��ʼ��
    long regval;
    
    // ���õ����ת
    regval = MOTOR_TMCUART_ReadReg(REGADDR_GCONF);
    regval = MOTOR_TMCUART_BuildReverse(regval);
    MOTOR_TMCUART_WriteReg(REGADDR_GCONF, regval);
}


/**
 * �����Ĵ���ֵ�������ת
 * ���� GCONF �Ĵ������ GCONF.shaft ����λ��1bit��
 *
 * �� TMC2209 Ϊ�ο���GCONF�Ĵ���ֵ�е����� �����ת ������λ�ǵ�4λ
 * 
 * @param long regGConfValue  ԴGCONF�Ĵ���ֵ
 * @return long  GCONF�µ�ֵ
 */
long MOTOR_TMCUART_BuildReverse( long regGConfValue )
{
    // ��ȡ GCONF.shaft ����λ��ֵ
    u8 shaft = (u8)(regGConfValue>>3) & 0x01;

    // ������ GCONF ֵ
    if( shaft == 1 ) regGConfValue &= 0xFFFFFFF7;
    if( shaft == 0 ) regGConfValue |= 0x00000008;

    // ������ GCONF ֵ
    return regGConfValue;
}


/**
 * ����ϸ��
 * �ú�����ϸ����غ��������˷�װ������ʹ�ò���
 *
 * @param short value  ϸ��ֵ��1, 2, 4, 8, 16, 32, 64, 128, 256
 */
void MOTOR_TMCUART_SetMircoStep( short value )
{
    // ��ʼ��
    long regval;
    
    // ����ϸ�ּĴ���
    regval = MOTOR_TMCUART_ReadReg(REGADDR_GCONF);
    regval = MOTOR_TMCUART_BuildMircoStepRegSelect(regval, 1);
    MOTOR_TMCUART_WriteReg(REGADDR_GCONF, regval);
    
    // ����ϸ��
    regval = MOTOR_TMCUART_ReadReg(REGADDR_CHOPCONF);
    regval = MOTOR_TMCUART_BuildMircoStep(regval, value);
    MOTOR_TMCUART_WriteReg(REGADDR_CHOPCONF, regval);
}


/**
 * �����Ĵ���ֵ������ϸ�� 
 * ���� CHOPCONF �Ĵ������ CHOPCONF.MRES ����λ��4bit��
 *
 * ע����Ҫ�� GCONF.mstep_reg_select ��ֵ����Ϊ 1���������� CHOPCONF.MRES ��Ż���Ч����MOTOR_TMCUART_BuildMircoStepRegSelect(..., 1)
 * 
 * @param long  regChopConfValue  ԴCHOPCONF�Ĵ���ֵ
 * @param short value             ϸ��ֵ��1, 2, 4, 8, 16, 32, 64, 128, 256
 *
 * @return long  CHOPCONF�Ĵ����µ�ֵ
 */
long MOTOR_TMCUART_BuildMircoStep( long regChopConfValue, short value )
{
    // ��ʼ��
    u8 byte = 0;
    
    // ���ø�8λ�еĵ�4λ���������µĵ�4λֵ
    if( value == 256 ) byte |= 0x00;
    if( value == 128 ) byte |= 0x01;
    if( value == 64  ) byte |= 0x02;
    if( value == 32  ) byte |= 0x03;
    if( value == 16  ) byte |= 0x04;
    if( value == 8   ) byte |= 0x05;
    if( value == 4   ) byte |= 0x06;
    if( value == 2   ) byte |= 0x07;
    if( value == 1   ) byte |= 0x08;

    // �����¼Ĵ���ֵ
    regChopConfValue = regChopConfValue & 0xF0FFFFFF;  // �� CHOPCONF.MRES ��4λ����Ϊ 0
    regChopConfValue = regChopConfValue | ((long)byte<<24);  // �����µ� CHOPCONF.MRES ����λֵ

    // �����µļĴ���ֵ
    return regChopConfValue;
}


/**
 * �����Ĵ���ֵ��ϸ�ּĴ���ѡ�� 
 * ���� GCONF �Ĵ������ GCONF.mstep_reg_select ����λ��1bit��
 * 
 * �� TMC2209 Ϊ�ο���GCONF�Ĵ���ֵ�е����� ϸ�ּĴ��� ������λ�ǵ�8λ
 * - mstep_reg_select = 0 ʱ����ôʹ�� MS1/MS2 ����оƬ������ȷ�����ϸ��
 * - mstep_reg_select = 1 ʱ����ôʹ�� MSTEP �Ĵ���ֵ��ȷ�����ϸ��
 * 
 * @param long  regGConfValue  ԴGCONF�Ĵ���ֵ
 * @param short value          �Ĵ���ѡ��0 ��ʾ MS1/MS2 оƬ���ţ�1 ��ʾʹ�� MSTEP �Ĵ���
 *
 * @return long  GCONF�Ĵ����µ�ֵ
 */
long MOTOR_TMCUART_BuildMircoStepRegSelect( long regGConfValue, short value )
{
    // �����µļĴ���ֵ
    if( value == 0 ) regGConfValue = regGConfValue & 0xFFFFFF7F;
    if( value != 0 ) regGConfValue = regGConfValue | 0x00000080;
    
    // �����µļĴ���ֵ
    return regGConfValue;
}


/**
 * ������������
 * ͨ��оƬ�Ĵ������ÿ���
 * �ú���������������غ��������˷�װ������ʹ�ò���
 *
 * ͨ�� TMC оƬ���õ��������õ������
 */
void MOTOR_TMCUART_SetDriverEnable()
{
    // ��ʼ��
    long regval;

    // ������������
    regval = MOTOR_TMCUART_ReadReg(REGADDR_CHOPCONF);
    regval = MOTOR_TMCUART_BuildDriverEnable(regval, 1);
    MOTOR_TMCUART_WriteReg(REGADDR_CHOPCONF, regval);
}


/**
 * ������������
 * ͨ��оƬ�Ĵ������ÿ���
 * �ú���������������غ��������˷�װ������ʹ�ò���
 *
 * ͨ�� TMC оƬ���õ����ý��õ������
 */
void MOTOR_TMCUART_SetDriverDisable()
{
    // ��ʼ��
    long regval;

    // ������������
    regval = MOTOR_TMCUART_ReadReg(REGADDR_CHOPCONF);
    regval = MOTOR_TMCUART_BuildDriverEnable(regval, 0);
    MOTOR_TMCUART_WriteReg(REGADDR_CHOPCONF, regval);
}


/**
 * �����Ĵ���ֵ��������������/����
 * 
 * @param long  regChopConfValue  ԴCHOPCONF�Ĵ���ֵ
 * @param short value             ������ֵ��0, 1
 *
 * @return long  CHOPCONF�Ĵ����µ�ֵ
 */
long MOTOR_TMCUART_BuildDriverEnable( long regChopConfValue, short value )
{
    // �����µļĴ���ֵ
    if( value == 0 ) regChopConfValue = regChopConfValue & 0xFFFFFFF0;
    if( value != 0 ) regChopConfValue = regChopConfValue | 0x0000000F;

    // �����µļĴ���ֵ
    return regChopConfValue;
}


/**
 * �������ܵ�������
 * �ú�����ϸ����غ��������˷�װ������ʹ�ò���
 *
 * ע���������ʱ CoolStep ���ܽ��޷����ã����磺������渺���²�õ� SG_RESULT ֵС�� 100 �ģ���ô���ܵ������ܻ������޷���Ч��
 *
 * @param long sgResult  ��ת�����ֵ
 *                       ��ֵ�����ڵ����������ʱ��ȡ�����������������ȡ������MOTOR_TMCUART_ReadReg(REGADDR_SG_RESULT)
 */
void MOTOR_TMCUART_SetCoolStep( long sgResult )
{
    // ��ʼ��
    u8 semin;
    long regval;
    
    // �������ܵ��������������ֵ
    MOTOR_TMCUART_WriteReg(REGADDR_TPWMTHRS , 0x00);  // ����Ϊ��Сֵ����������ٶȣ�TSTEP����Զ���ڸ�ֵ������ͻ����ø��ٷ���ģʽ����ʹ CoolStep ���ܽ���
    MOTOR_TMCUART_WriteReg(REGADDR_TCOOLTHRS, 0xFFFFF);  // ����Ϊ���ֵ����������ٶȣ�TSTEP����ԶС�ڸ�ֵ���ͻ�һֱ���� CoolStep ����

    // ���� SEMIN ֵ
    // SG_RESULT < SEMIN*32�����������������Ϊ0ʱ������ CoolStep
    semin = (u8)(sgResult*0.6/32);
    semin = semin ? semin : 1;
    
    // ���� SG_RESULT ֵ�������ܵ�������Ԥ����
    regval = MOTOR_TMCUART_BuildCoolConf(semin, 0);
    MOTOR_TMCUART_WriteReg(REGADDR_COOLCONF, regval);
}


/**
 * �ر����ܵ�������
 */
void MOTOR_TMCUART_CloseCoolStep()
{
    MOTOR_TMCUART_WriteReg(REGADDR_TCOOLTHRS, 0);
}


/**
 * �����Ĵ���ֵ�����ܵ�����������
 * ���� COOLCONF �Ĵ���
 * 
 * @param u8 semin  SG_RESULT < SEMIN*32�����������������Ϊ0ʱ������ CoolStep
 *                  �Ƽ�ֵ��SG_RESULT*60%/32
 * @param u8 semax  SG_RESULT > (SEMIN+SEMAX+1)*32��������������С
 *                  �Ƽ�ֵ��0/1����ֵ���˹�����Ϊ SG_RESULT ֵͨ���ǵ��û��̫����������»�ȡ���ģ�
 *                  ��������   ��������� semax ̫����ô�ͻ���Զʹ��SG_RESULT < (SEMIN+SEMAX+1)*32��
 *                  ��������   �������شӴ�->С��SG_RESULTֵ���С->�󣩣��� SG_RESULT ֵ�Ծ� < (SEMIN+SEMAX+1)*32�������Ͳ����ټ�����
 *                               
 *
 * @return long  COOLCONF�Ĵ���ֵ
 */
long MOTOR_TMCUART_BuildCoolConf( u8 semin, u8 semax )
{
    // ��ʼ��
    u8 seup, sedn;
    long regCoolConfValue = 0;
    
    // ���ܵ������Ƶ���С����
    regCoolConfValue |= 0x8000;  // 0x0000 ��ʾ 1/2*IRUN��0x8000 ��ʾ 1/4*IRUN;
    
    // ���ܵ������Ƶ� StallGuard2 ����ֵ
    semax = semax & 0x0F;
    regCoolConfValue |= (long)semax << 8;

    // �����½�����
    // sedn �����ֵԽС�������½�����Խ��
    // Ҳ����˵�����شӴ�ָ�������ʱ����ô���������ٻָ�
    sedn = 0x11;  // 0x00/0x01/0x10/0x11 �ֱ���� StallGuard4ֵ ÿ���� 32/8/2/1���������� 1��StallGuard4ֵӦ����ָ SG_RESULT ֵ��
    regCoolConfValue |= (long)sedn << 12;
    
    // ������������
    //
    // ֵԽ���������Խ��
    // ֵԽ��ĺô��������׶�������Ϊ���������죬��λʱ����Ť��������
    // ֵԽ��Ļ�����������������ô���������
    // ֵԽ��Ļ������������Ӳ���ϰ��ﵼ�¼��ٶ�ת����ô�����������ܿ죬һ���������ߺ���ܵ���оƬ���������쳣
    // ����������������һ�������ת��ת����Ŀ�У����ֵ��������������²����ˣ���Ϊ������ͳ������ʱ����� SG_RESULT �ǲ�һ������������е��ʱ�� SG_RESULT ���ܱ�С������ת��ֵ��ҪС
    //
    // ֵԽ������ó������޼��ٶ�ת����Ŀ
    // ֵԽС�����ó������м��ٶ�ת����Ŀ
    seup = 0x11;  // 0x00/0x01/0x10/0x11 �ֱ���� 1/2/4/8
    regCoolConfValue |= (long)seup << 4;

    // ���ܵ������Ƶ���С Stallguard 2 ֵ�����ܵ���ʹ��
    // 0 ��ʾ ���ܵ������� coolStep �ر�
    semin = semin & 0x0F;
    regCoolConfValue |= (long)semin;
   
    // ����
    return regCoolConfValue;
}


/**
 * ���õ����������
 *
 * ����ʵ�����ԣ����е����и�����޶���� irun=10%������޶��� 25%����ô�Ծ��� 25% �ĵ������У��ֲ�Ҳ�ᵽ��߽���75%���ң�
 * �����������������Ǵ˴����õ� irun ������ CoolStep���� ���ֵ����������ر��ʱ���������������޾����������õ� irun ֵ��CoolStep���ܵĵ������ֵԼ�� 75%���ң�
 *
 * param u8 irun   ���ȫ�������ǵ����ٷֱȣ�0 ~ 100��ֵԽ�����Խ���Ƽ� 50 ~ 100
 * param u8 ihold  ��ֹ״̬�µ�������ٷֱȣ�0 ~ 100��ֵԽ�����Խ��
 */
void MOTOR_TMCUART_SetIRunIHold( u8 irun, u8 ihold )
{
    long regval = MOTOR_TMCUART_BuildIRunIHold(irun, ihold);
    MOTOR_TMCUART_WriteReg(REGADDR_IHOLD_IRUN, regval);
}


/**
 * �����Ĵ���ֵ�������������
 * ���� IHOLD_IRUN �Ĵ���
 *
 * param u8 irun   ���ȫ�������ǵ����ٷֱȣ�0 ~ 100��ֵԽ�����Խ���Ƽ� 50 ~ 100
 * param u8 ihold  ��ֹ״̬�µ�������ٷֱȣ�0 ~ 100��ֵԽ�����Խ��
 */
long MOTOR_TMCUART_BuildIRunIHold( u8 irun, u8 ihold )
{
    // ��ʼ��
    u8 iholdDelay;
    long regIHoldIRun = 0;

    // ���þ�ֹ״̬�µ������
    ihold = (u8)((long)ihold*31/100);
    ihold = ihold & 0x1F;  // ȥ����3λ����
    regIHoldIRun |= (long)ihold;

    // ���õ��ȫ�������ǵ���
    irun = (u8)((long)irun*31/100);
    irun = irun & 0x1F;  // ȥ����3λ����
    regIHoldIRun |= (long)irun << 8;

    // ������˶�����ֹ��һ��ʱ�����½�ģʽ
    // ��Ҫ�����Ǳ�����û������ͣ����ʱ�����仯��ɵĵ������
    // 0 ��ʾ˲�併������1~15 ��ʾ ÿ 2^18 ��ʱ�ӱ�����ʱ��˥������
    iholdDelay = 0x00;
    regIHoldIRun |= (long)iholdDelay << 16;

    // ����
    return regIHoldIRun;
}


/**
 * ��ȡ�����ֹ���
 * ע�⣺�����ͣת���� 160ms ����ܻ�ȡ����ʵ�ĵ����ֹ���ֵ
 * �ֲ�˵����DRV_STATUS.stst ��־��ʾ�����ÿ������ģʽ�±��־�ֹ���ⷢ�������һ������֮��� 2^20 ʱ��
 *
 * param long  regDrvStatus  ԴDRV_STATUS�Ĵ���ֵ��-1 ʱ��ʾ�Զ���ȡ
 * param short getDelay      ��ʱ�������ȡ���
 */
long MOTOR_TMCUART_GetStopFlag( long regDrvStatus, short getDelay )
{
    // ��ʱ
    if( getDelay ) delay_ms(getDelay);
    
    // �Զ���ȡԴ DRV_STATUS �Ĵ���ֵ
    regDrvStatus = regDrvStatus == -1 ? MOTOR_TMCUART_ReadReg(REGADDR_DRV_STATUS) : regDrvStatus;
    regDrvStatus = regDrvStatus == -1 ? 0 : regDrvStatus;

    // ����
    // & 0x01 ��Ҫ���þ��ǵ� regDrvStatus Ϊ����ʱ���������ƶ�����ô���������ݿ�λ�����Զ���� 0xF��ʹ��ʱ���׳�����������Ҫ�����ݿ�λ���Ϊ 0
    return (regDrvStatus >> 31) & 0x01; 
}


/**
 * ��ȡ�����ת���
 *
 * param long regGConfValue  ԴGCONF�Ĵ���ֵ��-1 ʱ��ʾ�Զ���ȡ
 */
long MOTOR_TMCUART_GetReverseFlag( long regGConfValue )
{
    // �Զ���ȡԴ GCONF �Ĵ���ֵ
    regGConfValue = regGConfValue == -1 ? MOTOR_TMCUART_ReadReg(REGADDR_GCONF) : regGConfValue;
    regGConfValue = regGConfValue == -1 ? 0 : regGConfValue;

    // ����
    // & 0x01 ��Ҫ���þ��ǵ� regGConfValue Ϊ����ʱ���������ƶ�����ô���������ݿ�λ�����Զ���� 0xF��ʹ��ʱ���׳�����������Ҫ�����ݿ�λ���Ϊ 0
    return (regGConfValue >> 3) & 0x01; 
}


/**
 * ��ȡ�������ģʽ
 *
 * param long regDrvStatus  ԴDRV_STATUS�Ĵ���ֵ��-1 ʱ��ʾ�Զ���ȡ
 * return long  0 ��ʾ SpreadCycleģʽ��1 ��ʾ StealthChop ģʽ
 */
long MOTOR_TMCUART_GetRunModeFlag( long regDrvStatus )
{
    // �Զ���ȡԴ DRV_STATUS �Ĵ���ֵ
    regDrvStatus = regDrvStatus == -1 ? MOTOR_TMCUART_ReadReg(REGADDR_DRV_STATUS) : regDrvStatus;
    regDrvStatus = regDrvStatus == -1 ? 0 : regDrvStatus;

    // ����
    // & 0x01 ��Ҫ���þ��ǵ� regDrvStatus Ϊ����ʱ���������ƶ�����ô���������ݿ�λ�����Զ���� 0xF��ʹ��ʱ���׳�����������Ҫ�����ݿ�λ���Ϊ 0
    return (regDrvStatus >> 30) & 0x01; 
}


/**
 * ���㲢���� CRC ֵ
 * ���� *data ���������λ�ֽ��� CRC Ԥ��λ���ú�������� CRC ֵ��ḳֵ�����λ�ֽڣ�ͬʱ���� CRC ֵ
 *
 * ������Ϣ 0x05 0x00 0x00����ȷ�� CRC �� 0x48�����Դ��룺MOTOR_TMCUART_CalcCRC([0x05,0x00,0x00,0x00], 0, 4);
 * ������Ϣ 0x05 0x00 0x06����ȷ�� CRC �� 0x6F�����Դ��룺MOTOR_TMCUART_CalcCRC([0x05,0x00,0x06,0x00], 0, 4);
 *
 * @param u8* data        ��������ָ�루���������һλ����Ԥ������Ϊ�������Ҳ���� CRC ���㣬Ԥ��ֵΪ 0��
 * @param u8  dataStart   ��������������ʼ���±�� 0 ��ʼ��
 * @param u8  dataLength  �����������ݳ��ȣ��������һλԤ���ֽڣ�
 *
 * @return u8  CRC ֵ
 */
u8 MOTOR_TMCUART_CalcCRC( u8 *data, u8 dataStart, u8 dataLength )
{
    // ��ʼ��
    int i, j;
    u8 *crc = data + (dataStart + dataLength - 1);   // ��λ���������������һ���ֽڣ�ָ�룩
    u8 currentByte;
    
    // ��ʼԤ��λ
    *crc = 0;

    // ����ȫ�������ֽ�
    for( i = 0; i < (dataLength - 1); i++ ){
        currentByte = data[dataStart+i];
      
        for( j = 0; j < 8; j++ ){
            if( (*crc >> 7) ^ (currentByte & 0x01) ){
                *crc = (*crc << 1) ^ 0x07;
            }else{
                *crc = (*crc << 1);
            }
            
            currentByte = currentByte >> 1;
        }
    }
    
    // ���� CRC ֵ
    return data[dataStart+dataLength-1];
}



/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */

/**
 * ���߼�����ʼ��
 */
void MOTOR_TMCUART_MainInit()
{
    MOTOR_TMCUART_Init();
}


/**
 * ���߼����������2������
 * �ú����ڲ����Զ��������������
 */
void MOTOR_TMCUART_MainRoundTrip()
{
    // ��ʼ��
    static u8 numloop = 0;
    
    // ��������
    if( numloop > 10 ) numloop = 1;

    
    /* ���÷��� �����ٶ�ֵ ���� �������ת */
    if( numloop++ % 2 == 0 ){
        MOTOR_TMCUART_WriteReg(REGADDR_VACTUAL, 1000); 
    }else{
        MOTOR_TMCUART_WriteReg(REGADDR_VACTUAL, -1000);
    }


    /* ���÷��� ��ת�Ĵ�ֵ ���� ������ת */
    /*
    if( numloop == 0 ){
        MOTOR_TMCUART_WriteReg(REGADDR_VACTUAL, 500);
    }

    if( numloop++ % 2 == 0 ){
        MOTOR_TMCUART_SetReverse();
    }
    */

    
    // ��ʱ
    delay_ms(2000);
}


/**
 * ���߼��������ת��ת
 * ʵʱ��� SG_RESULT ֵ�����ֶ�ת��ת
 *��δ������⡿���ڸô���Ӧ���� main.c �� while �����ڣ���ô���ܴ��ڼ�� SG_RESULT ֵ����ʵʱ�����ܼ�� XX �����ִ��һ�Σ���
 *����������������ô���п��ܵ����תҲ���� XX ���룬ʵ���з��ֶ�ת�ҵ������������󣬵�����о��쳣���쳣������ȥ��CoolStep����ʧЧ�ˣ�
 *��������������# ��������������������������ܵ���CoolStep���ܵ���Ŀ�У�ͬʱ���е���������60%����
 *
 * param long  stallSGResult  �����ת�ĸ�����ֵ  
 */
void MOTOR_TMCUART_MainStallReverse( long stallSGResult )
{
    // δ�ﵽ��ת�Ķ�תֵ
    if( MOTOR_TMCUART_ReadReg(REGADDR_SG_RESULT) > stallSGResult ) return;
    
    // �����ת
    MOTOR_TMCUART_SetReverse();

    // ��ʱ���ȴ������ת��������� 200ms �������ȥ��� SG_RESULT ֵ
    // ��Ҫ�����ǵ��ĵ����ת˲�� SG_RESULT ���ȶ��������߼��жϳ�����
    delay_ms(200);  
}


/**
 * ���߼���ѭ��ִ��ȫ��ϸ��ֵ
 * �ú����ڲ����Զ��������������
 */
void MOTOR_TMCUART_MainMircoStepLoop()
{
    // ��ʼ��
    short i;
    long regval, regvalChopconf;
    static u8 initFlag = 0;
    
    // ���μ���
    if( initFlag++ == 0 ){
        // ����ϸ�ּĴ���
        regval = MOTOR_TMCUART_ReadReg(REGADDR_GCONF);
        regval = MOTOR_TMCUART_BuildMircoStepRegSelect(regval, 1);
        MOTOR_TMCUART_WriteReg(REGADDR_GCONF, regval);
        
        // ��ʼ���е��
        MOTOR_TMCUART_WriteReg(REGADDR_VACTUAL,  1000);
    }

    // ��ȡ CHOPCONF �Ĵ���ֵ
    regvalChopconf = MOTOR_TMCUART_ReadReg(REGADDR_CHOPCONF);
    
    // ѭ��ִ��ϸ��ֵ  
    for( i = 1; i <= 256; i=i*2 ){
        regvalChopconf = MOTOR_TMCUART_BuildMircoStep(regvalChopconf, i);
        MOTOR_TMCUART_WriteReg(REGADDR_CHOPCONF, regvalChopconf); 

        delay_ms(4000);
    }
}


/**
 * ���߼��������������
 *
 * param short mstep  ϸ��ֵ��1, 2, 4, 8, 16, 32, 64, 128, 256��0 ��ʾ������ϸ��
 * param long  speed  �ٶ�ֵ
 */
void MOTOR_TMCUART_MainRunKeep( short mstep, long speed )
{
    if( mstep ) MOTOR_TMCUART_SetMircoStep(mstep);
    MOTOR_TMCUART_WriteReg(REGADDR_VACTUAL, speed);
}


/**
 * ���߼������� SG_RESULT ֵ
 * �����ʼ��ֹʱ SG_RESULT ֵΪ 0~10
 * ������к�ֹ SG_RESULT ֵΪ 0~10����Ҫ��ֹһС�κ���ʱ���
 *
 * ͨ�� LCD ��ʾ SG_RESULT ֵ��ʹ��ǰ��Ҫ��
 * ����1.��ʼ�� LCD    ��LCD_MainInit()
 * ����2.������Ļ��SPI ��LCD_MSP3520_SpiOpenScreen()
 * ����3.�����������  ��MOTOR_TMCUART_MainRunKeep()
 */
void MOTOR_TMCUART_MainMonitorSGResult()
{
    // ��ʼ��
    long regval, line = 0;
    static long max = 0, min = 0x0FFFFFFF;

    // ��ʾ SG_RESULT ʵʱֵ
    regval = MOTOR_TMCUART_ReadReg(REGADDR_SG_RESULT);
    LCD_MSP3520_ShowInteger(45, 5+15*(line++), regval, 12, LCD_COLOR_BLACK, LCD_COLOR_WHITE, true, 3);

    // ��ʾ SG_RESULT ��Сֵ
    min = min > regval ? regval : min;
    LCD_MSP3520_ShowString(20, 5+15*line, "MIN:", 12, LCD_COLOR_RED, LCD_COLOR_WHITE, false);
    LCD_MSP3520_ShowInteger(45, 5+15*(line++), min, 12, LCD_COLOR_RED, LCD_COLOR_WHITE, true, 3);
    
    // ��ʾ SG_RESULT ���ֵ
    max = regval > max ? regval : max;
    LCD_MSP3520_ShowString(20, 5+15*line, "MAX:", 12, LCD_COLOR_BLUE, LCD_COLOR_WHITE, false);
    LCD_MSP3520_ShowInteger(45, 5+15*(line++), max, 12, LCD_COLOR_BLUE, LCD_COLOR_WHITE, true, 3);
}