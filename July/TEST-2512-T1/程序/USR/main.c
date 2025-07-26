/* -------------------------------------------------- */
// 项目说明：
// 　　.............
// 　　..........................
//     最后修改日期：20220525 //由于新轴位置偏移 故此增加判断误差 GV_DataDeviation 并增加允许误差个数
//
// 使用注意（TEST-713-T1_20220525）：
// 　　1. 可变电源模块设计有缺陷，无法使用
// 　　2. KEY模块 和 ADC2-J69 共用 PE7 脚；同一时刻要么禁用 KEY模块，要么禁用 ADC2-J69（注意J69相关电阻也要去除，否则会干扰 KEY模块）
//
//
/* -------------------------------------------------- */

#include "common.h"

#define FlashAddValue 0x00FBFF                                                 

//#define MOTOR_WakeUP GPIO_WriteLow(GPIOD,GPIO_PIN_1);
//#define MOTOR_Sleep GPIO_WriteHigh(GPIOD,GPIO_PIN_1);

unsigned char GV_PWMDutyVALUESZ[720];                                           //实时值记录数组


unsigned char GV_Time1ms = 0;

unsigned int GV_InitialX = 0;                                                   //初始坐标
unsigned int GV_InitialY = 270;                                                 //初始坐标

unsigned int GV_DisplayX = 10;                                                  //初始显示坐标
unsigned int GV_DisplayY = 190;                                                 //初始显示坐标

unsigned int GV_ADDX = 60;                                                      //附加坐标
unsigned int GV_ADDY = 48;                                                      //附加坐标

unsigned int GV_ADDXY = 40;                                                     //长宽大小（正方形）

unsigned int GV_StartXA = 35;                                                   //画线X坐标开始值
unsigned int GV_StartYA = 290;                                                  //画线Y坐标开始值

unsigned int GV_EndXA = 0;                                                      //画线X坐标结束值
unsigned int GV_EndYA = 0;                                                      //画线Y坐标结束值

unsigned int GV_Displayi = 0;
unsigned int GV_Displayk = 0;

unsigned char GV_TIME1Start = 0;                                                //开始采集33-40的状态

unsigned int GV_TIMEVlaue8[8];                                                  //存放33-40的状态

unsigned char GV_TIMEUPVlaue = 30;                                              //低电平出现最少的次数

float GV_ADValue = 0;                                                           //AD变量

unsigned int GV_ADValueUP = 275;                                                //芯片上限电压
unsigned int GV_ADValueLO = 225;                                                //芯片下限电压

unsigned char GV_ADValueComplete = 1;

/* -------------------------------------------------- */
// ● 主函数
/* -------------------------------------------------- */

int main()
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);                              //配置内部时钟：1分频
    

    GPIO_Init(GPIOB,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);                          //悬空/上拉输入
    GPIO_Init(GPIOB,GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOB,GPIO_PIN_4,GPIO_MODE_IN_PU_NO_IT); 
    GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_IN_PU_NO_IT);
    
    GPIO_Init(GPIOD,GPIO_PIN_0,GPIO_MODE_IN_PU_NO_IT);

    
    TIME1_Init();                                                               //定时器初始化
    
    HC595_Init();                                                               //用于 IOEXT 模块（间接用于 VPOWER 模块、LCD 模块）
    HC165_Init();                                                               //用于KEY
    HC164_Init();                                                               //用于通断路检测
    
    IOEXT_Init(true);                                                           //用于 VPOWER 模块、LCD 模块 I/O扩展   
    
    CONNECT_Init();                                                             //通断路初始化
    
    LCD_MainInit();                                                             //LCD初始化
    LCD_MainLedScene();                                                         //LCD绘制标题
    
    /* 启用总中断 */
    enableInterrupts();  
    
    //用于 TIM 模块、A4988 模块
    
    TIM1_Cmd(ENABLE);
    GV_TIME1Start = 1;
    // 主循环内容
    while(1)
    {
        
        
        

//        if((KEY_IsPress(51,true) == KEY_STATE_YES))                             //KEY1按下且Time1定时值为零，即当前无任务在运行，下同
//        {
              CONNECT_DatIsConnect(1);                                          //通断状态采集
//            LCD_MSP3520_SpiOpenScreen();
//            LCD_MSP3520_Fill(36,90,500,289,LCD_COLOR565_WHITE);                 //清除特定区域
//            LCD_MSP3520_SpiClose();
//            GV_MotorRunState = GV_PWMCollection = 0;
//            MOTOR_RESET();
//            delay_ms(100);
//            Start_MeasurePWM(); 
//        }
//        if(GV_TIME1Start == 1)                                                  //37-40状态采集
//        {
//            KEY_DatLoad();                                                          //读按键值
//        
//            if((KEY_IsPress(43,true) == KEY_STATE_YES))GV_TIMEVlaue8[4]++;
//            if((KEY_IsPress(44,true) == KEY_STATE_YES))GV_TIMEVlaue8[5]++;
//            if((KEY_IsPress(45,true) == KEY_STATE_YES))GV_TIMEVlaue8[6]++;
//            if((KEY_IsPress(46,true) == KEY_STATE_YES))GV_TIMEVlaue8[7]++;
            
//        }
        if(!GV_ADValueComplete)ADC2_MotorValue(0);                              //33-36状态采集
//        if((KEY_IsPress(43,true) == KEY_STATE_YES))                             //KEY1按下且Time1定时值为零，即当前无任务在运行，下同
//        {
//            FLASH_Unlock(FLASH_MEMTYPE_PROG);                                   //Flash解锁
//            for(unsigned int bc = 0;bc < 720;bc++)                              //循环360次
//            {
//                FLASH_ProgramByte(FlashAddValue + bc,GV_PWMDutyVALUESZ[bc]); 
//            }
//            FLASH_Lock(FLASH_MEMTYPE_PROG);                                     //Flash上锁
//            LCD_MSP3520_SpiOpenScreen();                                        //开启SPI
//            LCD_MSP3520_ShowChinese(400,200,"完",24,LCD_COLOR565_WHITE,LCD_COLOR565_DGREEN,1);//显示存储完成提示
//            LCD_MSP3520_ShowChinese(424,200,"成",24,LCD_COLOR565_WHITE,LCD_COLOR565_DGREEN,1); 
//            LCD_MSP3520_SpiClose();                                             //关闭SPI
//        }
//        if((KEY_IsPress(44,true) == KEY_STATE_YES))                             //KEY1按下且Time1定时值为零，即当前无任务在运行，下同
//        {
//             LCD_MSP3520_SpiOpenScreen();
//             for(unsigned int  hf =0;hf < 359; hf++)
//             {
//                                                                 //开SPI
//            AX = hf + GV_StartXA;                                                  //画线起始X坐标
//            AY = GV_StartYA - (FLASH_ReadByte(FlashAddValue + hf * 2) * 2);                               //画线起始Y坐标
//            BX = hf + 1 + GV_StartXA;                                                      //画线结束X坐标
//            BY = GV_StartYA - (FLASH_ReadByte(FlashAddValue + (hf*2) + 2) * 2);                                   //画线结束Y坐标
//            LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_DYELLOW);                        //蓝色
//
//            AY = GV_StartYA - (FLASH_ReadByte(FlashAddValue + hf * 2 + 1));                               //画线起始Y坐标
//            BY = GV_StartYA - (FLASH_ReadByte(FlashAddValue + (hf*2) + 3));                                   //画线结束Y坐标
//            LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_GREEN);                        //蓝色
//                                                      //关闭SPI
//             }
//            LCD_MSP3520_SpiClose(); 
//        }

    }
}

void TIME1_INTERRUPTHANDLER11(void)                                             //Time1中断服务函数
{
    GV_Time1ms = (GV_Time1ms + 1) % 100;                                         //定时值自加
    if(GV_Time1ms == 0)
    {
        GV_TIME1Start = 0;
        
//        LCD_MSP3520_SpiOpenScreen();
//        LCD_MSP3520_Fill(424,75,448,145,LCD_COLOR565_WHITE);                 //清除特定区域
//        LCD_MSP3520_SpiClose();
        for(char i = 4;i < 8;i++)
        {
            if(GV_TIMEVlaue8[i] > GV_TIMEUPVlaue && GV_TIMEVlaue8[i] < GV_TIMEUPVlaue + 40)
            {
                CONNECT_LEDshuzu[33 + i] = 1;
            }
            else 
            {
                CONNECT_LEDshuzu[33 + i] = 0;
            }
            
//            LCD_ValueDisplay(GV_TIMEVlaue8[i],i - 4);                           //显示变量值

            GV_TIMEVlaue8[i] = 0;
        }
        
        TIM1_Cmd(DISABLE); 
        GV_ADValueComplete = 0;
    }
    else 
    {
        KEY_DatLoad();                                                          //读按键值
        if((KEY_IsPress(43,true) == KEY_STATE_YES))GV_TIMEVlaue8[4]++;
        if((KEY_IsPress(44,true) == KEY_STATE_YES))GV_TIMEVlaue8[5]++;
        if((KEY_IsPress(45,true) == KEY_STATE_YES))GV_TIMEVlaue8[6]++;
        if((KEY_IsPress(46,true) == KEY_STATE_YES))GV_TIMEVlaue8[7]++;
    }
    
    TIM1_ClearITPendingBit(TIM1_IT_UPDATE);                                     //向上计数
    TIM1_ClearFlag(TIM1_FLAG_UPDATE);                                           //清标志
}

void ADC2_MotorValue(unsigned char J)                                           //获取AD值
{
    for(char i = 33;i < 37;i ++)
    {
        GV_ADValue = ADC2_GetVoltValue(i) * 100;
        GV_TIMEVlaue8[i - 33] = (int)GV_ADValue;

        if(GV_TIMEVlaue8[i - 33] < GV_ADValueUP && GV_TIMEVlaue8[i - 33] > GV_ADValueLO)
        {
            CONNECT_LEDshuzu[i] = 1;
        }
        else 
        {
            CONNECT_LEDshuzu[i] = 0;
        }
//        LCD_ValueDisplay(GV_TIMEVlaue8[i - 33],i - 33);                           //显示变量值
        GV_TIMEVlaue8[i - 33] = 0;
    }
    GV_ADValueComplete = 1;
}

void MOTOR_RESET()
{
//    MOTOR_WakeUP
//    if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_0) == RESET)                            //光电传感器 灯灭
//    {
//        motor_function(10000,50,GV_MotorPrescalerValue,1);                      //发送500个脉冲，占空比100%，步距角 1/16，逆时针旋转
//        delay_ms(1500);                                                         //延时400ms，目的是让电机走出光电传感器的检测范围
//    }
//    while(GPIO_ReadInputPin(GPIOD,GPIO_PIN_0) != RESET)                         //光电传感器 灯亮
//    {
//        
//        motor_function(1,50,GV_MotorPrescalerValue,0);                          //发送脉冲
//        if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_0) == RESET)                        //光电传感器 灯灭
//        {
//            fe2 = 0;                                                            //Time2计数值清零
//            TIM2_Cmd(DISABLE);                                                  //关闭Time2
//            TIM2_CCxCmd(TIM2_CHANNEL_1,DISABLE);
//        }
//    }
//    while((GPIO_ReadOutputData(GPIOD) & 0x02));
//    MOTOR_Sleep
}

void Start_MeasurePWM()
{
//    if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_0) == RESET && GV_MotorRunState == 0)
//            {
//                GV_MotorRunState = 1;
//                GV_PWM1FrequencyERR = 0;
//                GV_PWM2FrequencyERR = 0;
//                GV_PWM3DutyCycleERR = 0;
//                GV_PWM4DutyCycleERR = 0;
//                GV_TIME33Vlaue = 0;
//                GV_TIME34Vlaue = 0;
                
//                MOTOR_WakeUP
//                motor_function(40000,50,1,1);
                
//                TIM1_PWMI_Start();
//                TIM3_Cmd(ENABLE);
            }
            
//            while(GV_MotorRunState == 1 || GV_MotorRunState == 2)
//            {
//                if((GPIO_ReadOutputData(GPIOD) & 0x02) && GV_MotorRunState == 1)
//                {
//                    
////                    TIM1_PWMI_Stop();
////                    TIM3_Cmd(DISABLE);
////                    GV_MotorRunState = 2;
////                    MOTOR_WakeUP
////                    motor_function(40000,50,1,0);
////                    TIM1_PWMI_Start();
////                    TIM3_Cmd(ENABLE);
//                }
//                else if((GPIO_ReadOutputData(GPIOD) & 0x02) && GV_MotorRunState == 2)
//                {
//                    GV_MotorRunState = 0;
//                    TIM1_PWMI_Stop();
////                    TIM3_Cmd(DISABLE);
//                    LCD_MSP3520_SpiOpenScreen();
//                    LCD_MSP3520_Fill(400,75,448,170,LCD_COLOR565_WHITE);
//                    LCD_ValueDisplay(GV_PWM1FrequencyERR,0);
//                    LCD_ValueDisplay(GV_PWM2FrequencyERR,1);
////                    LCD_ValueDisplay(GV_PWM3DutyCycleERR,2);
////                    LCD_ValueDisplay(GV_PWM4DutyCycleERR,3);
//                    LCD_ValueDisplay(GV_TIME33Vlaue,2);
//                    LCD_ValueDisplay(GV_TIME34Vlaue,3);
////                    LCD_ValueDisplay((unsigned int)(((double)GV_TIME33Vlaue/(double)GV_PWMCollection)*100),2);
////                    LCD_ValueDisplay((unsigned int)(((double)GV_TIME34Vlaue/(double)GV_PWMCollection)*100),3);
//                    LCD_ValueDisplay(GV_PWMCollection,4);
//                    LCD_MSP3520_SpiClose();
//                }
//                if(GV_MotorRunState != 0 && GV_PWMCollection < 720)
//                {
//                    if(TIM1_PWMI_GetFreq(56) > (GV_PWMFrequency + GV_PWMFrequencyRange) || TIM1_PWMI_GetFreq(56) < (GV_PWMFrequency - GV_PWMFrequencyRange))
//                    {
//                        GV_PWM1FrequencyERR++;
//                        GV_PWM1DutyCycleERR = 1;
////                        LCD_MSP3520_SpiOpenScreen();
////                        LCD_MSP3520_Fill(400,70,448,82,LCD_COLOR565_WHITE);
////                        LCD_ValueDisplay(TIM1_PWMI_GetFreq(56),0);
////                        LCD_MSP3520_SpiClose();
//                    }
//                    GV_PWMDutyVALUESZ[GV_PWMCollection] = (unsigned char)TIM1_PWMI_GetDuty(56);
//                    delay_ms(37);
//                    if(TIM1_PWMI_GetFreq(57) > (GV_PWMFrequency + GV_PWMFrequencyRange) || TIM1_PWMI_GetFreq(57) < (GV_PWMFrequency - GV_PWMFrequencyRange))
//                    {
//                        GV_PWM2FrequencyERR++;
//                        GV_PWM1DutyCycleERR = 1;
//                    }
//                    GV_PWMDutyVALUESZ[GV_PWMCollection + 1] = (unsigned char)TIM1_PWMI_GetDuty(57);
//                    
//                    if(GPIO_ReadInputPin(GPIOB,GPIO_PIN_2))GV_TIME33Vlaue++;
//                    if(GPIO_ReadInputPin(GPIOB,GPIO_PIN_3))GV_TIME34Vlaue++;
//                    
//                    if(GV_PWMCollection > 0)
//                    {
//                        LCD_MSP3520_SpiOpenScreen();                                                //开SPI
//                        AX = (GV_PWMCollection - 2) / 2 + GV_StartXA;                                                  //画线起始X坐标
//                        AY = GV_StartYA - (GV_PWMDutyVALUESZ[GV_PWMCollection - 2] * 2);                               //画线起始Y坐标
//                        BX = (GV_PWMCollection / 2) + GV_StartXA;                                                      //画线结束X坐标
//                        BY = GV_StartYA - (GV_PWMDutyVALUESZ[GV_PWMCollection] * 2);                                   //画线结束Y坐标
//                        LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_DYELLOW);                        //蓝色
//
////                        AX = (GV_PWMCollection - 2) / 2 + GV_StartXA;                                                  //画线起始X坐标
//                        AY = GV_StartYA - (GV_PWMDutyVALUESZ[GV_PWMCollection - 2 + 1]);                               //画线起始Y坐标
////                        BX = (GV_PWMCollection / 2) + GV_StartXA;                                                      //画线结束X坐标
//                        BY = GV_StartYA - (GV_PWMDutyVALUESZ[GV_PWMCollection + 1]);                                   //画线结束Y坐标
//                        LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_GREEN);                        //蓝色
//                        LCD_MSP3520_SpiClose();                                                     //关SPI
//                    }
//                    
//                    GV_PWMCollection = GV_PWMCollection + 2;
//                    
//                }
//            }
//}


void LCD_ValueDisplay(unsigned int Value,unsigned int Y)                        //显示变量值（调试程序用）变量，位置 
{
    LCD_MSP3520_SpiOpenScreen();                                                //开SPI
    for(GV_Displayi = 0;GV_Displayi < 4;GV_Displayi ++)
    {
        switch(GV_Displayi)                                                     //确定X位置
        {
            case 0:GV_Displayk = 1000;break;                                              
            case 1:GV_Displayk = 100;break;
            case 2:GV_Displayk = 10;break;
            case 3:GV_Displayk = 0;break;
        }
        switch(Y)                                                               //确定Y位置
        {
            case 0:Y = 70;break;
            case 1:Y = 85;break;
            case 2:Y = 100;break;
            case 3:Y = 115;break;
            case 4:Y = 130;break;
            case 5:Y = 145;break;
        }
        switch((Value / GV_Displayk % 10))                                      //确定显示值
        {
            case 0:LCD_MSP3520_ShowChar((GV_Displayi * 12 + 400),Y,'0',24,1,0,0);break;
            case 1:LCD_MSP3520_ShowChar((GV_Displayi * 12 + 400),Y,'1',24,1,0,0);break;
            case 2:LCD_MSP3520_ShowChar((GV_Displayi * 12 + 400),Y,'2',24,1,0,0);break;
            case 3:LCD_MSP3520_ShowChar((GV_Displayi * 12 + 400),Y,'3',24,1,0,0);break;
            case 4:LCD_MSP3520_ShowChar((GV_Displayi * 12 + 400),Y,'4',24,1,0,0);break;
            case 5:LCD_MSP3520_ShowChar((GV_Displayi * 12 + 400),Y,'5',24,1,0,0);break;
            case 6:LCD_MSP3520_ShowChar((GV_Displayi * 12 + 400),Y,'6',24,1,0,0);break;
            case 7:LCD_MSP3520_ShowChar((GV_Displayi * 12 + 400),Y,'7',24,1,0,0);break;
            case 8:LCD_MSP3520_ShowChar((GV_Displayi * 12 + 400),Y,'8',24,1,0,0);break;
            case 9:LCD_MSP3520_ShowChar((GV_Displayi * 12 + 400),Y,'9',24,1,0,0);break;
        }
    }GV_Displayi = GV_Displayk = 0;                                             //辅助值清零
    LCD_MSP3520_SpiClose();                                                     //关SPI
}

void Motor_Function()                                                           //电动机定位并发送脉冲
{

}

void Flash_360FlashValueDisplay(unsigned short s,unsigned short e,unsigned char g)//显示坐标上下限 g必须为4的倍数 e = 360 * 4 / g        g
{
//    unsigned short i,AX,AY,BX,BY;                                               //定义辅助变量
//    
//    LCD_MSP3520_SpiOpenScreen();                                                //开SPI
//    for(i = s;i < e;i ++)                                                       //画下限
//    {
//        AX = (FLASH_ReadByte(GV_FlashAddValue + i * g + 5) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 4)));                  //画线起始X坐标
//        AY = (FLASH_ReadByte(GV_FlashAddValue + i * g + 7) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 6)) + GV_ADErrValue);  //画线起始Y坐标
//        BX = (FLASH_ReadByte(GV_FlashAddValue + i * g + 9) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 8)));                  //画线结束X坐标
//        BY = (FLASH_ReadByte(GV_FlashAddValue + i * g + 11) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 10)) + GV_ADErrValue);//画线结束Y坐标
//        LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_LRED);                    //淡红色
//    }
//    for(i = s;i < e;i ++)                                                       //画上限
//    {
//        AX = (FLASH_ReadByte(GV_FlashAddValue + i * g + 5) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 4)));                  //画线起始X坐标
//        AY = (FLASH_ReadByte(GV_FlashAddValue + i * g + 7) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 6)) - GV_ADErrValue);  //画线起始Y坐标
//        BX = (FLASH_ReadByte(GV_FlashAddValue + i * g + 9) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 8)));                  //画线结束X坐标
//        BY = (FLASH_ReadByte(GV_FlashAddValue + i * g + 11) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 10)) - GV_ADErrValue);//画线结束Y坐标
//        LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_LRED);                    //淡红色
//    }
//    LCD_MSP3520_SpiClose();                                                     //关SPI
}

void AD_360ADValueDisplay(unsigned short t)                                     //显示当前数据处理后的波形
{
//    unsigned short AX,AY,BX,BY,CY,FY;                                           //定义辅助变量
//    
//    t = t - 3;
//    
//    LCD_MSP3520_SpiOpenScreen();                                                //开SPI
//    AX = (GV_360ADVALUESZ[t * 4 + 1] * 0x100 + (GV_360ADVALUESZ[t * 4]));       //画线起始X坐标
//    AY = (GV_360ADVALUESZ[t * 4 + 3] * 0x100 + (GV_360ADVALUESZ[t * 4 + 2]));   //画线起始Y坐标
//    BX = (GV_360ADVALUESZ[t * 4 + 5] * 0x100 + (GV_360ADVALUESZ[t * 4 + 4]));   //画线结束X坐标
//    BY = (GV_360ADVALUESZ[t * 4 + 7] * 0x100 + (GV_360ADVALUESZ[t * 4 + 6]));   //画线结束Y坐标
//    FY = FLASH_ReadByte(GV_FlashAddValue + t * 4 + 7) * 0x100 + FLASH_ReadByte(GV_FlashAddValue + t * 4 + 6);
//    if((BY > FY + GV_ADErrValue || BY < FY - GV_ADErrValue || (AY - BY) > 4) && ((BX < GV_SkipLower + GV_InitialX) || ((BX > GV_SkipLower + GV_InitialX + 20) && (BX < GV_SkipUpper + GV_InitialX - 10)) && !GV_Used1OR3))
//    {
//        CY = (GV_360ADVALUESZ[t * 4 + 11] * 0x100 + (GV_360ADVALUESZ[t * 4 + 10]));//画线结束后Y坐标
//        BY = (CY + AY) / 2;
//    }
//    else if((BY > FY + GV_ADErrValue || BY < FY - GV_ADErrValue || (BY - AY) > 4) && BX > GV_SkipUpper + GV_InitialX)
//    {
//        CY = (GV_360ADVALUESZ[t * 4 + 11] * 0x100 + (GV_360ADVALUESZ[t * 4 + 10]));//画线结束后Y坐标
//        BY = (CY + AY) / 2;
//    }
//    
//    LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_BLUE);                        //蓝色
//    LCD_MSP3520_SpiClose();                                                     //关SPI
}

void AD_360ADTrueValueDisplay(unsigned short t)                                 //显示当前数据真实值
{
//    unsigned short AX,AY,BX,BY;                                                 //定义辅助变量
//    
//    LCD_MSP3520_SpiOpenScreen();                                                //开SPI
//    AX = (GV_360ADVALUESZ[t * 4 + 1] * 0x100 + (GV_360ADVALUESZ[t * 4]));       //画线起始X坐标
//    AY = (GV_360ADVALUESZ[t * 4 + 3] * 0x100 + (GV_360ADVALUESZ[t * 4 + 2]));   //画线起始Y坐标
//    BX = (GV_360ADVALUESZ[t * 4 + 5] * 0x100 + (GV_360ADVALUESZ[t * 4 + 4]));   //画线结束X坐标
//    BY = (GV_360ADVALUESZ[t * 4 + 7] * 0x100 + (GV_360ADVALUESZ[t * 4 + 6]));   //画线结束Y坐标
//    LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_BLUE);                        //蓝色
//    LCD_MSP3520_SpiClose();                                                     //关SPI
}

void Value_Judge()                                                              //判断实时AD值是否超模
{
//    unsigned short YA,YB;                                                       //建立辅助变量
//    
//    for(unsigned short i = 0;i < 360 - GV_DataDeviation;i ++)                   //360个数据 判断Y轴数据是否超模 GV_DataDeviation为X轴误差补偿 均为360个数据
//    {
//        YA = GV_360ADVALUESZ[i * 4 + 3] * 0x100 + GV_360ADVALUESZ[i * 4 + 2];                                                                              //Y轴实时值
//        YB = FLASH_ReadByte(GV_FlashAddValue + (i + GV_DataDeviation) * 4 + 3) * 0x100 + FLASH_ReadByte(GV_FlashAddValue + (i + GV_DataDeviation) * 4 + 2);//Y轴对照值
//        if(((YA > YB + GV_ADErrValue) || (YA < YB - GV_ADErrValue))/*&& i < 180 && i > 210*/)                                                              //对照值-误差值<实时值<对照值+误差值 
//        {
//            GV_ValueJudge ++;                                                   //记录超模数据次数
//        }
//    }
////    LCD_MSP3520_SpiOpenScreen();
////    LCD_ValueDisplay(GV_ValueJudge,0);                                          //显示超模个数
//    
//    LCD_MSP3520_SpiOpenScreen();
//    
//    if(GV_ValueJudge > GV_ADErrValueB)                                          //大于 判为不合格
//    {
//        LCD_MSP3520_ShowChinese(LCD_MSP3520_SCREEN_LONG / 2 - 36,100,"不",24,LCD_COLOR565_WHITE,LCD_COLOR565_RED,1);
//        LCD_MSP3520_ShowChinese(LCD_MSP3520_SCREEN_LONG / 2 - 12,100,"合",24,LCD_COLOR565_WHITE,LCD_COLOR565_RED,1);
//        LCD_MSP3520_ShowChinese(LCD_MSP3520_SCREEN_LONG / 2 + 12,100,"格",24,LCD_COLOR565_WHITE,LCD_COLOR565_RED,1);
//        GV_ValueJudge = 0;
//    }
//    else
//    {
//        LCD_MSP3520_ShowChinese(LCD_MSP3520_SCREEN_LONG / 2 - 24,100,"合",24,LCD_COLOR565_WHITE,LCD_COLOR565_DGREEN,1);
//        LCD_MSP3520_ShowChinese(LCD_MSP3520_SCREEN_LONG / 2,100,"格",24,LCD_COLOR565_WHITE,LCD_COLOR565_DGREEN,1);
//        GV_ValueJudge = 0;
//    }
//    
//    LCD_MSP3520_SpiClose();
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