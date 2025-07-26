/* -------------------------------------------------- */
// ��Ŀ˵����
// ����.............
// ����..........................
//     ����޸����ڣ�20220525 //��������λ��ƫ�� �ʴ������ж���� GV_DataDeviation ����������������
//
// ʹ��ע�⣨TEST-713-T1_20220525����
// ����1. �ɱ��Դģ�������ȱ�ݣ��޷�ʹ��
// ����2. KEYģ�� �� ADC2-J69 ���� PE7 �ţ�ͬһʱ��Ҫô���� KEYģ�飬Ҫô���� ADC2-J69��ע��J69��ص���ҲҪȥ������������ KEYģ�飩
//
//
/* -------------------------------------------------- */

#include "common.h"

#define FlashAddValue 0x00FBFF                                                 

//#define MOTOR_WakeUP GPIO_WriteLow(GPIOD,GPIO_PIN_1);
//#define MOTOR_Sleep GPIO_WriteHigh(GPIOD,GPIO_PIN_1);

unsigned char GV_PWMDutyVALUESZ[720];                                           //ʵʱֵ��¼����


unsigned char GV_Time1ms = 0;

unsigned int GV_InitialX = 0;                                                   //��ʼ����
unsigned int GV_InitialY = 270;                                                 //��ʼ����

unsigned int GV_DisplayX = 10;                                                  //��ʼ��ʾ����
unsigned int GV_DisplayY = 190;                                                 //��ʼ��ʾ����

unsigned int GV_ADDX = 60;                                                      //��������
unsigned int GV_ADDY = 48;                                                      //��������

unsigned int GV_ADDXY = 40;                                                     //�����С�������Σ�

unsigned int GV_StartXA = 35;                                                   //����X���꿪ʼֵ
unsigned int GV_StartYA = 290;                                                  //����Y���꿪ʼֵ

unsigned int GV_EndXA = 0;                                                      //����X�������ֵ
unsigned int GV_EndYA = 0;                                                      //����Y�������ֵ

unsigned int GV_Displayi = 0;
unsigned int GV_Displayk = 0;

unsigned char GV_TIME1Start = 0;                                                //��ʼ�ɼ�33-40��״̬

unsigned int GV_TIMEVlaue8[8];                                                  //���33-40��״̬

unsigned char GV_TIMEUPVlaue = 30;                                              //�͵�ƽ�������ٵĴ���

float GV_ADValue = 0;                                                           //AD����

unsigned int GV_ADValueUP = 275;                                                //оƬ���޵�ѹ
unsigned int GV_ADValueLO = 225;                                                //оƬ���޵�ѹ

unsigned char GV_ADValueComplete = 1;

/* -------------------------------------------------- */
// �� ������
/* -------------------------------------------------- */

int main()
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);                              //�����ڲ�ʱ�ӣ�1��Ƶ
    

    GPIO_Init(GPIOB,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);                          //����/��������
    GPIO_Init(GPIOB,GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOB,GPIO_PIN_4,GPIO_MODE_IN_PU_NO_IT); 
    GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_IN_PU_NO_IT);
    
    GPIO_Init(GPIOD,GPIO_PIN_0,GPIO_MODE_IN_PU_NO_IT);

    
    TIME1_Init();                                                               //��ʱ����ʼ��
    
    HC595_Init();                                                               //���� IOEXT ģ�飨������� VPOWER ģ�顢LCD ģ�飩
    HC165_Init();                                                               //����KEY
    HC164_Init();                                                               //����ͨ��·���
    
    IOEXT_Init(true);                                                           //���� VPOWER ģ�顢LCD ģ�� I/O��չ   
    
    CONNECT_Init();                                                             //ͨ��·��ʼ��
    
    LCD_MainInit();                                                             //LCD��ʼ��
    LCD_MainLedScene();                                                         //LCD���Ʊ���
    
    /* �������ж� */
    enableInterrupts();  
    
    //���� TIM ģ�顢A4988 ģ��
    
    TIM1_Cmd(ENABLE);
    GV_TIME1Start = 1;
    // ��ѭ������
    while(1)
    {
        
        
        

//        if((KEY_IsPress(51,true) == KEY_STATE_YES))                             //KEY1������Time1��ʱֵΪ�㣬����ǰ�����������У���ͬ
//        {
              CONNECT_DatIsConnect(1);                                          //ͨ��״̬�ɼ�
//            LCD_MSP3520_SpiOpenScreen();
//            LCD_MSP3520_Fill(36,90,500,289,LCD_COLOR565_WHITE);                 //����ض�����
//            LCD_MSP3520_SpiClose();
//            GV_MotorRunState = GV_PWMCollection = 0;
//            MOTOR_RESET();
//            delay_ms(100);
//            Start_MeasurePWM(); 
//        }
//        if(GV_TIME1Start == 1)                                                  //37-40״̬�ɼ�
//        {
//            KEY_DatLoad();                                                          //������ֵ
//        
//            if((KEY_IsPress(43,true) == KEY_STATE_YES))GV_TIMEVlaue8[4]++;
//            if((KEY_IsPress(44,true) == KEY_STATE_YES))GV_TIMEVlaue8[5]++;
//            if((KEY_IsPress(45,true) == KEY_STATE_YES))GV_TIMEVlaue8[6]++;
//            if((KEY_IsPress(46,true) == KEY_STATE_YES))GV_TIMEVlaue8[7]++;
            
//        }
        if(!GV_ADValueComplete)ADC2_MotorValue(0);                              //33-36״̬�ɼ�
//        if((KEY_IsPress(43,true) == KEY_STATE_YES))                             //KEY1������Time1��ʱֵΪ�㣬����ǰ�����������У���ͬ
//        {
//            FLASH_Unlock(FLASH_MEMTYPE_PROG);                                   //Flash����
//            for(unsigned int bc = 0;bc < 720;bc++)                              //ѭ��360��
//            {
//                FLASH_ProgramByte(FlashAddValue + bc,GV_PWMDutyVALUESZ[bc]); 
//            }
//            FLASH_Lock(FLASH_MEMTYPE_PROG);                                     //Flash����
//            LCD_MSP3520_SpiOpenScreen();                                        //����SPI
//            LCD_MSP3520_ShowChinese(400,200,"��",24,LCD_COLOR565_WHITE,LCD_COLOR565_DGREEN,1);//��ʾ�洢�����ʾ
//            LCD_MSP3520_ShowChinese(424,200,"��",24,LCD_COLOR565_WHITE,LCD_COLOR565_DGREEN,1); 
//            LCD_MSP3520_SpiClose();                                             //�ر�SPI
//        }
//        if((KEY_IsPress(44,true) == KEY_STATE_YES))                             //KEY1������Time1��ʱֵΪ�㣬����ǰ�����������У���ͬ
//        {
//             LCD_MSP3520_SpiOpenScreen();
//             for(unsigned int  hf =0;hf < 359; hf++)
//             {
//                                                                 //��SPI
//            AX = hf + GV_StartXA;                                                  //������ʼX����
//            AY = GV_StartYA - (FLASH_ReadByte(FlashAddValue + hf * 2) * 2);                               //������ʼY����
//            BX = hf + 1 + GV_StartXA;                                                      //���߽���X����
//            BY = GV_StartYA - (FLASH_ReadByte(FlashAddValue + (hf*2) + 2) * 2);                                   //���߽���Y����
//            LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_DYELLOW);                        //��ɫ
//
//            AY = GV_StartYA - (FLASH_ReadByte(FlashAddValue + hf * 2 + 1));                               //������ʼY����
//            BY = GV_StartYA - (FLASH_ReadByte(FlashAddValue + (hf*2) + 3));                                   //���߽���Y����
//            LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_GREEN);                        //��ɫ
//                                                      //�ر�SPI
//             }
//            LCD_MSP3520_SpiClose(); 
//        }

    }
}

void TIME1_INTERRUPTHANDLER11(void)                                             //Time1�жϷ�����
{
    GV_Time1ms = (GV_Time1ms + 1) % 100;                                         //��ʱֵ�Լ�
    if(GV_Time1ms == 0)
    {
        GV_TIME1Start = 0;
        
//        LCD_MSP3520_SpiOpenScreen();
//        LCD_MSP3520_Fill(424,75,448,145,LCD_COLOR565_WHITE);                 //����ض�����
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
            
//            LCD_ValueDisplay(GV_TIMEVlaue8[i],i - 4);                           //��ʾ����ֵ

            GV_TIMEVlaue8[i] = 0;
        }
        
        TIM1_Cmd(DISABLE); 
        GV_ADValueComplete = 0;
    }
    else 
    {
        KEY_DatLoad();                                                          //������ֵ
        if((KEY_IsPress(43,true) == KEY_STATE_YES))GV_TIMEVlaue8[4]++;
        if((KEY_IsPress(44,true) == KEY_STATE_YES))GV_TIMEVlaue8[5]++;
        if((KEY_IsPress(45,true) == KEY_STATE_YES))GV_TIMEVlaue8[6]++;
        if((KEY_IsPress(46,true) == KEY_STATE_YES))GV_TIMEVlaue8[7]++;
    }
    
    TIM1_ClearITPendingBit(TIM1_IT_UPDATE);                                     //���ϼ���
    TIM1_ClearFlag(TIM1_FLAG_UPDATE);                                           //���־
}

void ADC2_MotorValue(unsigned char J)                                           //��ȡADֵ
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
//        LCD_ValueDisplay(GV_TIMEVlaue8[i - 33],i - 33);                           //��ʾ����ֵ
        GV_TIMEVlaue8[i - 33] = 0;
    }
    GV_ADValueComplete = 1;
}

void MOTOR_RESET()
{
//    MOTOR_WakeUP
//    if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_0) == RESET)                            //��紫���� ����
//    {
//        motor_function(10000,50,GV_MotorPrescalerValue,1);                      //����500�����壬ռ�ձ�100%������� 1/16����ʱ����ת
//        delay_ms(1500);                                                         //��ʱ400ms��Ŀ�����õ���߳���紫�����ļ�ⷶΧ
//    }
//    while(GPIO_ReadInputPin(GPIOD,GPIO_PIN_0) != RESET)                         //��紫���� ����
//    {
//        
//        motor_function(1,50,GV_MotorPrescalerValue,0);                          //��������
//        if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_0) == RESET)                        //��紫���� ����
//        {
//            fe2 = 0;                                                            //Time2����ֵ����
//            TIM2_Cmd(DISABLE);                                                  //�ر�Time2
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
//                        LCD_MSP3520_SpiOpenScreen();                                                //��SPI
//                        AX = (GV_PWMCollection - 2) / 2 + GV_StartXA;                                                  //������ʼX����
//                        AY = GV_StartYA - (GV_PWMDutyVALUESZ[GV_PWMCollection - 2] * 2);                               //������ʼY����
//                        BX = (GV_PWMCollection / 2) + GV_StartXA;                                                      //���߽���X����
//                        BY = GV_StartYA - (GV_PWMDutyVALUESZ[GV_PWMCollection] * 2);                                   //���߽���Y����
//                        LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_DYELLOW);                        //��ɫ
//
////                        AX = (GV_PWMCollection - 2) / 2 + GV_StartXA;                                                  //������ʼX����
//                        AY = GV_StartYA - (GV_PWMDutyVALUESZ[GV_PWMCollection - 2 + 1]);                               //������ʼY����
////                        BX = (GV_PWMCollection / 2) + GV_StartXA;                                                      //���߽���X����
//                        BY = GV_StartYA - (GV_PWMDutyVALUESZ[GV_PWMCollection + 1]);                                   //���߽���Y����
//                        LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_GREEN);                        //��ɫ
//                        LCD_MSP3520_SpiClose();                                                     //��SPI
//                    }
//                    
//                    GV_PWMCollection = GV_PWMCollection + 2;
//                    
//                }
//            }
//}


void LCD_ValueDisplay(unsigned int Value,unsigned int Y)                        //��ʾ����ֵ�����Գ����ã�������λ�� 
{
    LCD_MSP3520_SpiOpenScreen();                                                //��SPI
    for(GV_Displayi = 0;GV_Displayi < 4;GV_Displayi ++)
    {
        switch(GV_Displayi)                                                     //ȷ��Xλ��
        {
            case 0:GV_Displayk = 1000;break;                                              
            case 1:GV_Displayk = 100;break;
            case 2:GV_Displayk = 10;break;
            case 3:GV_Displayk = 0;break;
        }
        switch(Y)                                                               //ȷ��Yλ��
        {
            case 0:Y = 70;break;
            case 1:Y = 85;break;
            case 2:Y = 100;break;
            case 3:Y = 115;break;
            case 4:Y = 130;break;
            case 5:Y = 145;break;
        }
        switch((Value / GV_Displayk % 10))                                      //ȷ����ʾֵ
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
    }GV_Displayi = GV_Displayk = 0;                                             //����ֵ����
    LCD_MSP3520_SpiClose();                                                     //��SPI
}

void Motor_Function()                                                           //�綯����λ����������
{

}

void Flash_360FlashValueDisplay(unsigned short s,unsigned short e,unsigned char g)//��ʾ���������� g����Ϊ4�ı��� e = 360 * 4 / g        g
{
//    unsigned short i,AX,AY,BX,BY;                                               //���帨������
//    
//    LCD_MSP3520_SpiOpenScreen();                                                //��SPI
//    for(i = s;i < e;i ++)                                                       //������
//    {
//        AX = (FLASH_ReadByte(GV_FlashAddValue + i * g + 5) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 4)));                  //������ʼX����
//        AY = (FLASH_ReadByte(GV_FlashAddValue + i * g + 7) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 6)) + GV_ADErrValue);  //������ʼY����
//        BX = (FLASH_ReadByte(GV_FlashAddValue + i * g + 9) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 8)));                  //���߽���X����
//        BY = (FLASH_ReadByte(GV_FlashAddValue + i * g + 11) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 10)) + GV_ADErrValue);//���߽���Y����
//        LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_LRED);                    //����ɫ
//    }
//    for(i = s;i < e;i ++)                                                       //������
//    {
//        AX = (FLASH_ReadByte(GV_FlashAddValue + i * g + 5) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 4)));                  //������ʼX����
//        AY = (FLASH_ReadByte(GV_FlashAddValue + i * g + 7) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 6)) - GV_ADErrValue);  //������ʼY����
//        BX = (FLASH_ReadByte(GV_FlashAddValue + i * g + 9) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 8)));                  //���߽���X����
//        BY = (FLASH_ReadByte(GV_FlashAddValue + i * g + 11) * 0x100 + (FLASH_ReadByte(GV_FlashAddValue + i * g + 10)) - GV_ADErrValue);//���߽���Y����
//        LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_LRED);                    //����ɫ
//    }
//    LCD_MSP3520_SpiClose();                                                     //��SPI
}

void AD_360ADValueDisplay(unsigned short t)                                     //��ʾ��ǰ���ݴ����Ĳ���
{
//    unsigned short AX,AY,BX,BY,CY,FY;                                           //���帨������
//    
//    t = t - 3;
//    
//    LCD_MSP3520_SpiOpenScreen();                                                //��SPI
//    AX = (GV_360ADVALUESZ[t * 4 + 1] * 0x100 + (GV_360ADVALUESZ[t * 4]));       //������ʼX����
//    AY = (GV_360ADVALUESZ[t * 4 + 3] * 0x100 + (GV_360ADVALUESZ[t * 4 + 2]));   //������ʼY����
//    BX = (GV_360ADVALUESZ[t * 4 + 5] * 0x100 + (GV_360ADVALUESZ[t * 4 + 4]));   //���߽���X����
//    BY = (GV_360ADVALUESZ[t * 4 + 7] * 0x100 + (GV_360ADVALUESZ[t * 4 + 6]));   //���߽���Y����
//    FY = FLASH_ReadByte(GV_FlashAddValue + t * 4 + 7) * 0x100 + FLASH_ReadByte(GV_FlashAddValue + t * 4 + 6);
//    if((BY > FY + GV_ADErrValue || BY < FY - GV_ADErrValue || (AY - BY) > 4) && ((BX < GV_SkipLower + GV_InitialX) || ((BX > GV_SkipLower + GV_InitialX + 20) && (BX < GV_SkipUpper + GV_InitialX - 10)) && !GV_Used1OR3))
//    {
//        CY = (GV_360ADVALUESZ[t * 4 + 11] * 0x100 + (GV_360ADVALUESZ[t * 4 + 10]));//���߽�����Y����
//        BY = (CY + AY) / 2;
//    }
//    else if((BY > FY + GV_ADErrValue || BY < FY - GV_ADErrValue || (BY - AY) > 4) && BX > GV_SkipUpper + GV_InitialX)
//    {
//        CY = (GV_360ADVALUESZ[t * 4 + 11] * 0x100 + (GV_360ADVALUESZ[t * 4 + 10]));//���߽�����Y����
//        BY = (CY + AY) / 2;
//    }
//    
//    LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_BLUE);                        //��ɫ
//    LCD_MSP3520_SpiClose();                                                     //��SPI
}

void AD_360ADTrueValueDisplay(unsigned short t)                                 //��ʾ��ǰ������ʵֵ
{
//    unsigned short AX,AY,BX,BY;                                                 //���帨������
//    
//    LCD_MSP3520_SpiOpenScreen();                                                //��SPI
//    AX = (GV_360ADVALUESZ[t * 4 + 1] * 0x100 + (GV_360ADVALUESZ[t * 4]));       //������ʼX����
//    AY = (GV_360ADVALUESZ[t * 4 + 3] * 0x100 + (GV_360ADVALUESZ[t * 4 + 2]));   //������ʼY����
//    BX = (GV_360ADVALUESZ[t * 4 + 5] * 0x100 + (GV_360ADVALUESZ[t * 4 + 4]));   //���߽���X����
//    BY = (GV_360ADVALUESZ[t * 4 + 7] * 0x100 + (GV_360ADVALUESZ[t * 4 + 6]));   //���߽���Y����
//    LCD_MSP3520_DrawLine(AX,AY,BX,BY,LCD_COLOR565_BLUE);                        //��ɫ
//    LCD_MSP3520_SpiClose();                                                     //��SPI
}

void Value_Judge()                                                              //�ж�ʵʱADֵ�Ƿ�ģ
{
//    unsigned short YA,YB;                                                       //������������
//    
//    for(unsigned short i = 0;i < 360 - GV_DataDeviation;i ++)                   //360������ �ж�Y�������Ƿ�ģ GV_DataDeviationΪX������ ��Ϊ360������
//    {
//        YA = GV_360ADVALUESZ[i * 4 + 3] * 0x100 + GV_360ADVALUESZ[i * 4 + 2];                                                                              //Y��ʵʱֵ
//        YB = FLASH_ReadByte(GV_FlashAddValue + (i + GV_DataDeviation) * 4 + 3) * 0x100 + FLASH_ReadByte(GV_FlashAddValue + (i + GV_DataDeviation) * 4 + 2);//Y�����ֵ
//        if(((YA > YB + GV_ADErrValue) || (YA < YB - GV_ADErrValue))/*&& i < 180 && i > 210*/)                                                              //����ֵ-���ֵ<ʵʱֵ<����ֵ+���ֵ 
//        {
//            GV_ValueJudge ++;                                                   //��¼��ģ���ݴ���
//        }
//    }
////    LCD_MSP3520_SpiOpenScreen();
////    LCD_ValueDisplay(GV_ValueJudge,0);                                          //��ʾ��ģ����
//    
//    LCD_MSP3520_SpiOpenScreen();
//    
//    if(GV_ValueJudge > GV_ADErrValueB)                                          //���� ��Ϊ���ϸ�
//    {
//        LCD_MSP3520_ShowChinese(LCD_MSP3520_SCREEN_LONG / 2 - 36,100,"��",24,LCD_COLOR565_WHITE,LCD_COLOR565_RED,1);
//        LCD_MSP3520_ShowChinese(LCD_MSP3520_SCREEN_LONG / 2 - 12,100,"��",24,LCD_COLOR565_WHITE,LCD_COLOR565_RED,1);
//        LCD_MSP3520_ShowChinese(LCD_MSP3520_SCREEN_LONG / 2 + 12,100,"��",24,LCD_COLOR565_WHITE,LCD_COLOR565_RED,1);
//        GV_ValueJudge = 0;
//    }
//    else
//    {
//        LCD_MSP3520_ShowChinese(LCD_MSP3520_SCREEN_LONG / 2 - 24,100,"��",24,LCD_COLOR565_WHITE,LCD_COLOR565_DGREEN,1);
//        LCD_MSP3520_ShowChinese(LCD_MSP3520_SCREEN_LONG / 2,100,"��",24,LCD_COLOR565_WHITE,LCD_COLOR565_DGREEN,1);
//        GV_ValueJudge = 0;
//    }
//    
//    LCD_MSP3520_SpiClose();
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