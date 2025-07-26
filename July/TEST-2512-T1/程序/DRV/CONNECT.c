#include "common.h"

/*【全局变量】*/
static unsigned long CONNECT_CtlValue = 0;
//static unsigned short CONNECT_Errors=0;
/*【建立数组】确定LED灯的位置，建立代数数组*/
unsigned long CONNECT_Ctlshuzu[34]={0x00000000,0x80000000,0x40000000,0x20000000,0x10000000,    //1-8
                                               0x01000000,0x02000000,0x04000000,0x08000000,    
                                              
                                               0x00800000,0x00400000,0x00200000,0x00100000,    //9-16
                                               0x00010000,0x00020000,0x00040000,0x00080000,
                                               
                                               0x00008000,0x00004000,0x00002000,0x00001000,    //17-24
                                               0x00000100,0x00000200,0x00000400,0x00000800,
                                               
                                               0x00000080,0x00000040,0x00000020,0x00000010,    //25-32
                                               0x00000008,0x00000004,0x00000002,0x00000001,
                                               0xffffffff};/*0,1,2,3,4,8,9,5,6,7,10,11,12,16,15,13,14,17,21*/
unsigned short CONNECT_Datshuzu[35]={0,1,2,3,4,8,7,6,5,12,11,10,9,16,15,14,13,20,19,18,17,24,23,22,21,28,27,26,25,32,31,30,29,33,34}; //通断路编号
unsigned short CONNECT_DatDATA[35];
unsigned short CONNECT_DDatDATA[35];
unsigned short CONNECT_Cwshuzu[42];
unsigned short CONNECT_LEDshuzu[42];

unsigned short CONNECT_LastLEDshuzu[42]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}; //2为初始值

unsigned char CONNECT_LEDQuantity = 32;


/*【74HC164、74HC165端口初始化】*/
void CONNECT_Init(void)
{
  GPIO_Init(GPIOA,GPIO_PIN_1,GPIO_MODE_OUT_PP_HIGH_FAST);//DATA
  GPIO_Init(GPIOA,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_FAST);//CP
   
  GPIO_Init(GPIOB,GPIO_PIN_1,GPIO_MODE_OUT_PP_HIGH_FAST);//PL
  GPIO_Init(GPIOB,GPIO_PIN_6,GPIO_MODE_OUT_PP_HIGH_FAST);//CP
   
  GPIO_Init(GPIOB,GPIO_PIN_7,GPIO_MODE_IN_FL_NO_IT);//Q7
}

/*【74HC164基础函数】*/
void CONNECT_CtlBasics(void)
{
  unsigned short i;
  
  for(i=0;i<CONNECT_LEDQuantity;i++)
  {
    if((CONNECT_CtlValue>>i) & 0x00000001)
      GPIO_WriteHigh(GPIOA,GPIO_PIN_1);
    else
      GPIO_WriteLow(GPIOA,GPIO_PIN_1);
    
    GPIO_WriteLow(GPIOA, GPIO_PIN_2);
    GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
  }
}

/*【74HC165基础函数】*/
void CONNECT_DatBasics(unsigned char X)
{
  unsigned short i;
  
  GPIO_WriteLow(GPIOB,GPIO_PIN_1);
  GPIO_WriteHigh(GPIOB,GPIO_PIN_1);
  
  for(i=0;i<CONNECT_LEDQuantity;i++)
  {
    if(X == 1)
    {
        CONNECT_DatDATA[i+1] = !GPIO_ReadInputPin(GPIOB,GPIO_PIN_7);
    }
    else if(X == 0)
    {
        CONNECT_DDatDATA[i+1] = !GPIO_ReadInputPin(GPIOB,GPIO_PIN_7);
    }
    GPIO_WriteLow(GPIOB, GPIO_PIN_6);
    GPIO_WriteHigh(GPIOB, GPIO_PIN_6);
  }
//  CONNECT_DatDATA[i+1] = !GPIO_ReadInputPin(GPIOB,GPIO_PIN_7);
}

/*
*【选择打开控制通道】
*【number】：选择控制的位置
*【render】：选择是否覆盖已有数据 1不覆盖 0覆盖
*            下同
*/
void CONNECT_CtlOpen(short number,short render)
{  
  if(render)
  {
    CONNECT_CtlValue |= CONNECT_Ctlshuzu[number];
  }
  else 
  {
    CONNECT_CtlValue = CONNECT_Ctlshuzu[number];
  }
  CONNECT_CtlBasics();
}

/*【打开全部控制通道】*/
void CONNECT_CtlOpenAll(short render)
{
  if(render)CONNECT_CtlOpen(CONNECT_LEDQuantity + 1,1);
}

/*【选择关闭控制通道】，参考【选择打开控制通道】*/
void CONNECT_CtlClose(short number,short render)
{
  if(render)
  {
    CONNECT_CtlValue &= ~(CONNECT_Ctlshuzu[number]);
  }
  else 
  {
    CONNECT_CtlValue = ~CONNECT_Ctlshuzu[number];
  }

  CONNECT_CtlBasics();
}

/*【关闭全部控制通道】*/
void CONNECT_CtlCloseAll(short render)
{
  if(render)CONNECT_CtlClose(CONNECT_LEDQuantity + 1,1);
}

/*【单路通断检测】，参考【选择打开控制通道】*/
void CONNECT_DatIsConnectOnly(short number,short render)
{
//  unsigned short i,k,j;
//  
//  CONNECT_CtlCloseAll(1);
//  j=0;
//  for(k=1;k<9;k++)
//  { 
//    CONNECT_CtlOpen(k,0);
//    delay_ms(2);
//    CONNECT_DatBasics();
//    if(CONNECT_DatDATA[CONNECT_Datshuzu[k]]!=RESET) 
//    {
//      for(i=k+1;i<12;i++)
//      {
//        if((CONNECT_DatDATA[CONNECT_Datshuzu[i]]!=RESET) && !(k==1&&i>8))
//        {
//          CONNECT_LEDshuzu[i]=0;
//          CONNECT_LEDshuzu[k]=0;
//          CONNECT_Cwshuzu[i]=1;
//          CONNECT_Cwshuzu[k]=1;
//          j++;
//        }
//        else if((CONNECT_DatDATA[CONNECT_Datshuzu[i]]==RESET) && !(k==1&&i>8))
//        {
//          CONNECT_Cwshuzu[i]=0;
//        }
//      }
//      
//      if(j==0)
//      {
//        CONNECT_Cwshuzu[k]=0;
//      }
//      else
//      {
//        CONNECT_Cwshuzu[k]=1;
//      }
//      
//    }
//    else
//    {
//      CONNECT_LEDshuzu[k]=0;
//    }
//    CONNECT_CtlClose(k,0);
//    j=0;
//  }

}

/*【多路通断短检测】（独立结果）*/
void CONNECT_DatIsConnect(short render)
{
  unsigned short i,k,j;
  unsigned int CZ,CK;

  CZ = GV_DisplayX;
  CK = GV_DisplayY;
  
  CONNECT_CtlOpenAll(1);
  CONNECT_DatBasics(1);
  
  for(i=1;i<CONNECT_LEDQuantity + 1;i++)
  {
    CONNECT_DDatDATA[CONNECT_Datshuzu[i]] = 0;
    CONNECT_CtlOpen(i,0);
    CONNECT_DatBasics(0);
    if(CONNECT_DDatDATA[CONNECT_Datshuzu[i]]!=RESET) 
    {
      for(k=1;k<CONNECT_LEDQuantity + 1;k++)
      {
        if((CONNECT_DDatDATA[CONNECT_Datshuzu[k]]!=RESET) && i!=k)
        {
          CONNECT_DDatDATA[CONNECT_Datshuzu[i]] = 0;
          CONNECT_DDatDATA[CONNECT_Datshuzu[k]] = 0;
          j++;
        }
      }
      k = 0;
      if(j==0)
      {
        CONNECT_DDatDATA[CONNECT_Datshuzu[i]] = 1;
      }
    }
    else
    {
      CONNECT_DDatDATA[CONNECT_Datshuzu[i]] = 0;
    }
    j = 0;
    CONNECT_DatDATA[CONNECT_Datshuzu[i]] = CONNECT_DatDATA[CONNECT_Datshuzu[i]] & CONNECT_DDatDATA[CONNECT_Datshuzu[i]];
  }
  
  for(i=1;i<CONNECT_LEDQuantity + 1;i++)
  {
    if(CONNECT_DatDATA[CONNECT_Datshuzu[i]]!=RESET)
    {
      CONNECT_LEDshuzu[i]=1;
    }
    else
    {
      CONNECT_LEDshuzu[i]=0;
    }
  }
  for(i=1;i<41;i++)
  {
    
    LCD_MSP3520_SpiOpenScreen();
        
        
    if(CONNECT_LEDshuzu[i] == 1 && CONNECT_LastLEDshuzu[i] != 1)
    {
        CONNECT_LastLEDshuzu[i] = CONNECT_LEDshuzu[i];
        /*if(i < CONNECT_LEDQuantity + 1)*/LCD_MSP3520_Fill(GV_InitialX + CZ,GV_InitialY - CK,GV_InitialX + GV_ADDXY + CZ,GV_InitialY - CK + GV_ADDXY,LCD_COLOR565_GREEN);
//        else if(i > CONNECT_LEDQuantity)LCD_MSP3520_DrawCircle(GV_InitialX + CZ,GV_InitialY - CK,20,LCD_COLOR565_GREEN,1,1);
        LCD_MSP3520_ShowChar(GV_InitialX + CZ + 6,GV_InitialY - CK + 8, '0' + (i / 10), 24, LCD_COLOR565_WHITE, LCD_COLOR565_GREEN, 0);
        LCD_MSP3520_ShowChar(GV_InitialX + CZ + 18,GV_InitialY - CK + 8, '0' + (i % 10), 24, LCD_COLOR565_WHITE, LCD_COLOR565_GREEN, 0);
    }
    else if(CONNECT_LEDshuzu[i] == 0 && CONNECT_LastLEDshuzu[i] != 0)
    {
        CONNECT_LastLEDshuzu[i] = CONNECT_LEDshuzu[i];
        /*if(i < CONNECT_LEDQuantity + 1)*/LCD_MSP3520_Fill(GV_InitialX + CZ,GV_InitialY - CK,GV_InitialX + GV_ADDXY + CZ,GV_InitialY - CK + GV_ADDXY,LCD_COLOR565_RED);
//        else if(i > CONNECT_LEDQuantity)LCD_MSP3520_DrawCircle(GV_InitialX + CZ,GV_InitialY - CK,20,LCD_COLOR565_RED,1,1);
        LCD_MSP3520_ShowChar(GV_InitialX + CZ + 6,GV_InitialY - CK + 8, '0' + (i / 10), 24, LCD_COLOR565_WHITE, LCD_COLOR565_RED, 0);
        LCD_MSP3520_ShowChar(GV_InitialX + CZ + 18,GV_InitialY - CK + 8, '0' + (i % 10), 24, LCD_COLOR565_WHITE, LCD_COLOR565_RED, 0);
    }
//    LCD_MSP3520_ShowChar(GV_InitialX + CZ + 10,GV_InitialY - CK + 8, '0' + i, 24, LCD_COLOR565_WHITE, LCD_COLOR565_RED, 1);
    CZ = CZ + GV_ADDX;
    if(CZ == GV_ADDX * 8 + GV_DisplayX)
    {
        CZ = GV_DisplayX;
        CK = CK - GV_ADDY;
    }
  }
  TIM1_Cmd(ENABLE);
  GV_TIME1Start = 1;
}
