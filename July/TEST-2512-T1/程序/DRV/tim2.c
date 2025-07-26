#include "common.h"

unsigned int fe2;
TIM2_Prescaler_TypeDef Prescaler;

void TIME2_Init(void)
{
  GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);
  TIM2_DeInit();
  
  TIM2_Cmd(DISABLE);
  TIM2_CCxCmd(TIM2_CHANNEL_1,DISABLE);
  
//  if(GV_MotorPrescalerValue == 1)
//  {
//      GV_Time2OverflowValue = (((GV_MCUTime / GV_Time2PrescalerValue) * 1000) * GV_MotorOne) / 3200;
//  }
//  else if(GV_MotorPrescalerValue == 0)
//  {
//      GV_Time2OverflowValue = (((GV_MCUTime / GV_Time2PrescalerValue) * 1000) * GV_MotorOne) / 200;
//  }
//  
//  switch(GV_Time2PrescalerValue)
//  {
//      case 1:Prescaler = TIM2_PRESCALER_1;break;
//      case 2:Prescaler = TIM2_PRESCALER_2;break;
//      case 4:Prescaler = TIM2_PRESCALER_4;break;
//      case 8:Prescaler = TIM2_PRESCALER_8;break;
//      case 16:Prescaler = TIM2_PRESCALER_16;break;
//      case 32:Prescaler = TIM2_PRESCALER_32;break;
//      case 64:Prescaler = TIM2_PRESCALER_64;break;
//  default :Prescaler = TIM2_PRESCALER_16;break;
//  }
  
  
//  TIM2_TimeBaseInit(Prescaler,GV_Time2OverflowValue);
  TIM2_TimeBaseInit(TIM2_PRESCALER_16,160);
  TIM2_OC1Init(TIM2_OCMODE_PWM1,TIM2_OUTPUTSTATE_ENABLE,0,TIM2_OCPOLARITY_HIGH);
  TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);
  TIM2_OC1PreloadConfig(ENABLE);
  
  GPIO_WriteLow(GPIOG,GPIO_PIN_0);
}

void TIME2_INTERRUPTHANDLER13(void)
{
  if(fe2>0)fe2--;
  else 
  {  
    TIM2_Cmd(DISABLE);
    TIM2_CCxCmd(TIM2_CHANNEL_1,DISABLE);
    
    GPIO_WriteHigh(GPIOD,GPIO_PIN_1);
//    GPIO_WriteLow(GPIOD,GPIO_PIN_5);
    
    fe2=0;
  }
  TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
}

void TIME2_Cmd1cCHANNEL1(unsigned int c)
{
  if(c>0)
  {
    TIM2_Cmd(ENABLE);
    TIM2_CCxCmd(TIM2_CHANNEL_1,ENABLE);
    
    fe2=c;
  }
}

void TIME2_Cmd1oCHANNEL1(unsigned short o)
{
  delay_ms(o);
  TIM2_CCxCmd(TIM2_CHANNEL_1,ENABLE);
}

void TIME2_Cmd1zCHANNEL1(unsigned short z)
{
  TIM2->CCR1H = (uint8_t)(z >> 8);
  TIM2->CCR1L = (uint8_t)(z);
}