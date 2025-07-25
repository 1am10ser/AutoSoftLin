/* -------------------------------------------------- */
// ��Ŀ˵����
// ����.............
// ����..........................
//
//
// ʹ��ע�⣨TesterProductB.v2_20220702����
// ����1. KEY ģ��� PB0/PE7�˿� �� ADC2 ģ��� J68/J69 ���ã�ʹ��ʱע�⣻ͬһʱ��Ҫô���� KEY ģ�飬Ҫô���� ADC2-J68/J69��ע�� J68/J69 ��ص���ҲҪȥ������������ KEYģ�飩
// ����2. �� TIM2 ���� PWM ����ʱ��ͬʱ���� �ⲿ�жϽţ�TIM2_CH2/PD3�� �ⲿ�жϣ������ͻ�����Է���ʹ��
//
//
/* -------------------------------------------------- */

#include "common.h"


u16 Duty1 = 0;
u16 Freq1 = 0;

/* -------------------------------------------------- */
// �� ������
/* -------------------------------------------------- */
void LCD_ValueDisplay4_API(u16 Value,u16 X,u16 Y);
void LCD_DisplaysWaveform(void);

int main()
{
    // �����ڲ�ʱ�ӣ�1��Ƶ
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    // ������
    HC595_Init();  //������ģ�飺LCD��IOEXT��VPOWER��
    /* IOEXT��IO��չ�� */
    IOEXT_Init(true);  //������ģ�飺LCD��VPOWER��
    /* PWM ���� */   
    TIM1_PWMI_Init();
    TIM1_PWMI_Start();

    /* LCD��Һ������ */
    LCD_MainLedSceneInit();
    LCD_MSP3520_SpiOpenScreen();

    /* �������ж� */
    enableInterrupts();  //������ģ�飺LIN��TIM��MOTOR��MOTOR-TMC-UART��MOTOR-ZDT-UART��EXTI��UART1��UART3��

    // ��ѭ������
    while(1)
    {       
        Duty1 = 0;
        Freq1 = 0;
        TIM1_PWMI_InitData56();
        delay_ms(50);
      
        Freq1 = TIM1_PWMI_GetFreq(56);//��ȡƵ��
        Duty1 = (u16)TIM1_PWMI_GetDuty(56);//��ȡ��ռ�ձ�
        
        LCD_ValueDisplay4_API(Freq1,180+60,120);//��ʾƵ��
        LCD_ValueDisplay4_API(Duty1,180+60,150);//��ʾռ�ձ�
        
        LCD_DisplaysWaveform();
        if(Freq1 > 250 && Freq1 < 300 && Duty1 > 35 && Duty1 <65)
        {
            LCD_MSP3520_ShowChar(240 - 12,290,'O',24,LCD_COLOR_DBLUE,LCD_COLOR_WHITE,1);
            LCD_MSP3520_ShowChar(240     ,290,'K',24,LCD_COLOR_DBLUE,LCD_COLOR_WHITE,1);
        }
        else
        {
            LCD_MSP3520_ShowChar(240 - 12,290,'N',24,LCD_COLOR_WHITE,LCD_COLOR_RED,1);
            LCD_MSP3520_ShowChar(240     ,290,'O',24,LCD_COLOR_WHITE,LCD_COLOR_RED,1);
        }
    }
}


void LCD_MainDisplaysChineseCharacters(u16 x,u16 y,u8* titleChinese,u8 size,u16 color)
{
    LCD_MSP3520_ShowChinese(x,y,titleChinese,size,color,LCD_COLOR_WHITE,0);
}

/*
�������ܣ�     ��ʾ����ֵ
Value         ��Ҫ��ʾ��ֵ
X ��Y         ����
˵��������4λ������ʾ��û��С����ֻ����������Ҫ������ʾ���������ڴ�ĽǶ�
*/
void LCD_ValueDisplay4_API(u16 Value,u16 X,u16 Y)
{
  unsigned int GV_Displayi,GV_Displayk;//LDC��ʾ�����ĸ�������
   u16 end = 0;
  LCD_MSP3520_SpiOpenScreen();//��SPI
  
  LCD_MSP3520_Fill(X,Y,X+48,Y+24,LCD_COLOR_WHITE);
  
  if(Value >= 1000) end = 0;
  else if(Value < 1000 && Value >= 100) end = 1;
  else if(Value < 100 && Value >= 10) end = 2;
  else if(Value < 10) end = 3;
  
  for(GV_Displayi = end;GV_Displayi < 4;GV_Displayi ++)
  {
    switch(GV_Displayi) //ȷ��Xλ��
    {
    case 0:GV_Displayk = 1000;break;
    case 1:GV_Displayk = 100;break;
    case 2:GV_Displayk = 10;break;
    case 3:GV_Displayk = 0;break;
    }
    switch((Value / GV_Displayk % 10))//ȷ����ʾֵ
    {
    case 0:LCD_MSP3520_ShowChar((GV_Displayi * 12 + X),Y,'0',24,1,0,0);break;
    case 1:LCD_MSP3520_ShowChar((GV_Displayi * 12 + X),Y,'1',24,1,0,0);break;
    case 2:LCD_MSP3520_ShowChar((GV_Displayi * 12 + X),Y,'2',24,1,0,0);break;
    case 3:LCD_MSP3520_ShowChar((GV_Displayi * 12 + X),Y,'3',24,1,0,0);break;
    case 4:LCD_MSP3520_ShowChar((GV_Displayi * 12 + X),Y,'4',24,1,0,0);break;
    case 5:LCD_MSP3520_ShowChar((GV_Displayi * 12 + X),Y,'5',24,1,0,0);break;
    case 6:LCD_MSP3520_ShowChar((GV_Displayi * 12 + X),Y,'6',24,1,0,0);break;
    case 7:LCD_MSP3520_ShowChar((GV_Displayi * 12 + X),Y,'7',24,1,0,0);break;
    case 8:LCD_MSP3520_ShowChar((GV_Displayi * 12 + X),Y,'8',24,1,0,0);break;
    case 9:LCD_MSP3520_ShowChar((GV_Displayi * 12 + X),Y,'9',24,1,0,0);break;
    }
  }
  GV_Displayi = GV_Displayk = 0;//����ֵ����
  //LCD_MSP3520_SpiClose();
}

//��ʾ����
void LCD_DisplaysWaveform(void)
{
  LCD_MSP3520_SpiOpenScreen();//��SPI
  //��ֱ��
  LCD_MSP3520_Fill(180,220,425,270,LCD_COLOR_WHITE);//����ض�����
  u16 x = (Duty1/4);
  u16 x2 = 25 - x;
  
  LCD_MSP3520_DrawLine(180,220,180+x,220,LCD_COLOR_DGRAY);//�ڶ������θߵ�ƽ
  LCD_MSP3520_DrawLine(180+x,220,180+x,260,LCD_COLOR_DGRAY);//�ڶ������εĵ�ƽ�仯
  LCD_MSP3520_DrawLine(180+x,260,180+x+x2,260,LCD_COLOR_DGRAY);//�ڶ������ε͵�ƽ
  LCD_MSP3520_DrawLine(180+x+x2,260,180+x+x2,220,LCD_COLOR_DGRAY);//�ڶ������εĵ�ƽ�仯
  
  LCD_MSP3520_DrawLine(205,220,205+x,220,LCD_COLOR_DGRAY);//���������θߵ�ƽ
  LCD_MSP3520_DrawLine(205+x,220,205+x,260,LCD_COLOR_DGRAY);//���������εĵ�ƽ�仯
  LCD_MSP3520_DrawLine(205+x,260,205+x+x2,260,LCD_COLOR_DGRAY);//���������ε͵�ƽ
  LCD_MSP3520_DrawLine(205+x+x2,260,205+x+x2,220,LCD_COLOR_DGRAY);//���������εĵ�ƽ�仯
  
  LCD_MSP3520_DrawLine(230,220,230+x,220,LCD_COLOR_DGRAY);//���ĸ����θߵ�ƽ
  LCD_MSP3520_DrawLine(230+x,220,230+x,260,LCD_COLOR_DGRAY);//���ĸ����εĵ�ƽ�仯
  LCD_MSP3520_DrawLine(230+x,260,230+x+x2,260,LCD_COLOR_DGRAY);//���ĸ����ε͵�ƽ
  LCD_MSP3520_DrawLine(230+x+x2,260,230+x+x2,220,LCD_COLOR_DGRAY);//���ĸ����εĵ�ƽ�仯
  
  LCD_MSP3520_DrawLine(255,220,255+x,220,LCD_COLOR_DGRAY);//���ĸ����θߵ�ƽ
  LCD_MSP3520_DrawLine(255+x,220,255+x,260,LCD_COLOR_DGRAY);//���ĸ����εĵ�ƽ�仯
  LCD_MSP3520_DrawLine(255+x,260,255+x+x2,260,LCD_COLOR_DGRAY);//���ĸ����ε͵�ƽ
  LCD_MSP3520_DrawLine(255+x+x2,260,255+x+x2,220,LCD_COLOR_DGRAY);//���ĸ����εĵ�ƽ�仯
  
  LCD_MSP3520_DrawLine(280,220,280+x,220,LCD_COLOR_DGRAY);//���ĸ����θߵ�ƽ
  LCD_MSP3520_DrawLine(280+x,220,280+x,260,LCD_COLOR_DGRAY);//���ĸ����εĵ�ƽ�仯
  LCD_MSP3520_DrawLine(280+x,260,280+x+x2,260,LCD_COLOR_DGRAY);//���ĸ����ε͵�ƽ
  LCD_MSP3520_DrawLine(280+x+x2,260,280+x+x2,220,LCD_COLOR_DGRAY);//���ĸ����εĵ�ƽ�仯
  
}

/**
 * ���Ժ���
 * �ڱ�̵Ĺ�����Ϊ�����ṩ�������
 * STM8 �̶��ĺ���������ṩ�����������ܳ�������ο� stm8s_conf.h
 */
#ifdef USE_FULL_ASSERT
    void assert_failed(u8* file, u32 line)
    {
        while(1){
            // TODO
            // ������ ��ƫ����ѭ�� ʱ�����ڴ˴���ӡ�������ڲ���
        }
    }
#endif