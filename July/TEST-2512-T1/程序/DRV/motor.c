#include "common.h"
#include "motor.h"

void motor_Init(void)
{
  GPIO_Init(GPIOE,GPIO_PIN_1,GPIO_MODE_OUT_PP_HIGH_SLOW);//DIR
  GPIO_Init(GPIOE,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_SLOW);//ϸ��
  
//  GPIO_Init(GPIOG,GPIO_PIN_0,GPIO_MODE_OUT_PP_HIGH_SLOW);
  
  GPIO_WriteLow(GPIOE,GPIO_PIN_1);
  GPIO_WriteLow(GPIOE,GPIO_PIN_2);

}

void motor_function(unsigned int f,unsigned short z,unsigned char s,unsigned char x)//f ��������z ռ�ձȣ�s �����1:1/16 0:1/2 ��x ת�� 1�� 0��
{  
//  GPIO_WriteHigh(GPIOG,GPIO_PIN_0);
  GPIO_WriteHigh(GPIOD,GPIO_PIN_5);
  if(s==1)
  {
    GPIO_WriteHigh(GPIOE,GPIO_PIN_2);
  }
  else
  {
    GPIO_WriteLow(GPIOE,GPIO_PIN_2);
  }
  if(x==1)
  {
    GPIO_WriteHigh(GPIOE,GPIO_PIN_1);
  }
  else
  {
    GPIO_WriteLow(GPIOE,GPIO_PIN_1);
  }
  TIME2_Cmd1zCHANNEL1(z);
  
  TIME2_Cmd1cCHANNEL1(f);
  
  
}

































































