/* -------------------------------------------------- */
// 项目说明：
// 　　.............
// 　　..........................
//
//
// 使用注意（TesterProductB.v2_20220702）：
// 　　1. KEY 模块的 PB0/PE7端口 和 ADC2 模块的 J68/J69 共用，使用时注意；同一时刻要么禁用 KEY 模块，要么禁用 ADC2-J68/J69（注意 J68/J69 相关电阻也要去除，否则会干扰 KEY模块）
// 　　2. 当 TIM2 进行 PWM 捕获时，同时启用 外部中断脚（TIM2_CH2/PD3） 外部中断，不会冲突，可以放心使用
//
//
/* -------------------------------------------------- */

#include "common.h"


u16 Duty1 = 0;
u16 Freq1 = 0;

/* -------------------------------------------------- */
// ● 主函数
/* -------------------------------------------------- */
void LCD_ValueDisplay4_API(u16 Value,u16 X,u16 Y);
void LCD_DisplaysWaveform(void);

int main()
{
    // 配置内部时钟：1分频
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    // 基础库
    HC595_Init();  //【适用模块：LCD、IOEXT、VPOWER】
    /* IOEXT（IO扩展） */
    IOEXT_Init(true);  //【适用模块：LCD、VPOWER】
    /* PWM 捕获 */   
    TIM1_PWMI_Init();
    TIM1_PWMI_Start();

    /* LCD（液晶屏） */
    LCD_MainLedSceneInit();
    LCD_MSP3520_SpiOpenScreen();

    /* 启用总中断 */
    enableInterrupts();  //【适用模块：LIN、TIM、MOTOR、MOTOR-TMC-UART、MOTOR-ZDT-UART、EXTI、UART1、UART3】

    // 主循环内容
    while(1)
    {       
        Duty1 = 0;
        Freq1 = 0;
        TIM1_PWMI_InitData56();
        delay_ms(50);
      
        Freq1 = TIM1_PWMI_GetFreq(56);//获取频率
        Duty1 = (u16)TIM1_PWMI_GetDuty(56);//获取正占空比
        
        LCD_ValueDisplay4_API(Freq1,180+60,120);//显示频率
        LCD_ValueDisplay4_API(Duty1,180+60,150);//显示占空比
        
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
函数功能：     显示变量值
Value         需要显示的值
X ，Y         坐标
说明：进行4位数据显示，没有小数，只有整数。主要用于显示最大误差所在打的角度
*/
void LCD_ValueDisplay4_API(u16 Value,u16 X,u16 Y)
{
  unsigned int GV_Displayi,GV_Displayk;//LDC显示变量的辅助变量
   u16 end = 0;
  LCD_MSP3520_SpiOpenScreen();//开SPI
  
  LCD_MSP3520_Fill(X,Y,X+48,Y+24,LCD_COLOR_WHITE);
  
  if(Value >= 1000) end = 0;
  else if(Value < 1000 && Value >= 100) end = 1;
  else if(Value < 100 && Value >= 10) end = 2;
  else if(Value < 10) end = 3;
  
  for(GV_Displayi = end;GV_Displayi < 4;GV_Displayi ++)
  {
    switch(GV_Displayi) //确定X位置
    {
    case 0:GV_Displayk = 1000;break;
    case 1:GV_Displayk = 100;break;
    case 2:GV_Displayk = 10;break;
    case 3:GV_Displayk = 0;break;
    }
    switch((Value / GV_Displayk % 10))//确定显示值
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
  GV_Displayi = GV_Displayk = 0;//辅助值清零
  //LCD_MSP3520_SpiClose();
}

//显示波形
void LCD_DisplaysWaveform(void)
{
  LCD_MSP3520_SpiOpenScreen();//开SPI
  //画直线
  LCD_MSP3520_Fill(180,220,425,270,LCD_COLOR_WHITE);//清除特定区域
  u16 x = (Duty1/4);
  u16 x2 = 25 - x;
  
  LCD_MSP3520_DrawLine(180,220,180+x,220,LCD_COLOR_DGRAY);//第二个波形高电平
  LCD_MSP3520_DrawLine(180+x,220,180+x,260,LCD_COLOR_DGRAY);//第二个波形的电平变化
  LCD_MSP3520_DrawLine(180+x,260,180+x+x2,260,LCD_COLOR_DGRAY);//第二个波形低电平
  LCD_MSP3520_DrawLine(180+x+x2,260,180+x+x2,220,LCD_COLOR_DGRAY);//第二个波形的电平变化
  
  LCD_MSP3520_DrawLine(205,220,205+x,220,LCD_COLOR_DGRAY);//第三个波形高电平
  LCD_MSP3520_DrawLine(205+x,220,205+x,260,LCD_COLOR_DGRAY);//第三个波形的电平变化
  LCD_MSP3520_DrawLine(205+x,260,205+x+x2,260,LCD_COLOR_DGRAY);//第三个波形低电平
  LCD_MSP3520_DrawLine(205+x+x2,260,205+x+x2,220,LCD_COLOR_DGRAY);//第三个波形的电平变化
  
  LCD_MSP3520_DrawLine(230,220,230+x,220,LCD_COLOR_DGRAY);//第四个波形高电平
  LCD_MSP3520_DrawLine(230+x,220,230+x,260,LCD_COLOR_DGRAY);//第四个波形的电平变化
  LCD_MSP3520_DrawLine(230+x,260,230+x+x2,260,LCD_COLOR_DGRAY);//第四个波形低电平
  LCD_MSP3520_DrawLine(230+x+x2,260,230+x+x2,220,LCD_COLOR_DGRAY);//第四个波形的电平变化
  
  LCD_MSP3520_DrawLine(255,220,255+x,220,LCD_COLOR_DGRAY);//第四个波形高电平
  LCD_MSP3520_DrawLine(255+x,220,255+x,260,LCD_COLOR_DGRAY);//第四个波形的电平变化
  LCD_MSP3520_DrawLine(255+x,260,255+x+x2,260,LCD_COLOR_DGRAY);//第四个波形低电平
  LCD_MSP3520_DrawLine(255+x+x2,260,255+x+x2,220,LCD_COLOR_DGRAY);//第四个波形的电平变化
  
  LCD_MSP3520_DrawLine(280,220,280+x,220,LCD_COLOR_DGRAY);//第四个波形高电平
  LCD_MSP3520_DrawLine(280+x,220,280+x,260,LCD_COLOR_DGRAY);//第四个波形的电平变化
  LCD_MSP3520_DrawLine(280+x,260,280+x+x2,260,LCD_COLOR_DGRAY);//第四个波形低电平
  LCD_MSP3520_DrawLine(280+x+x2,260,280+x+x2,220,LCD_COLOR_DGRAY);//第四个波形的电平变化
  
}

/**
 * 断言函数
 * 在编程的过程中为程序提供参数检查
 * STM8 固定的函数，最好提供，否则编译可能出错；具体参考 stm8s_conf.h
 */
#ifdef USE_FULL_ASSERT
    void assert_failed(u8* file, u32 line)
    {
        while(1){
            // TODO
            // 当程序 跑偏、死循环 时可以在此处打印出来用于查阅
        }
    }
#endif