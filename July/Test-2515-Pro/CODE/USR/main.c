//内容修改
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


/* -------------------------------------------------- */
// ● 主函数
/* -------------------------------------------------- */

int main()
{
    // 配置内部时钟：1分频
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);


    // 初始化
    // 串口优先初始化，以免未初始化前被调用导致单片机死机
    UART1_CInit();  //【适用模块：USB/WIFI/BLE、MOTOR-TMC-UART】
    //UART3_CInit();  //【适用模块：LIN、MOTOR-ZDT-UART】
 

    // 初始化
    //LIN_Init();


    // 基础库
    HC164_Init();  //【适用模块：CONNECT】
    HC165_Init();  //【适用模块：CONNECT、KEY】
    //HC595_Init();  //【适用模块：LCD、IOEXT、VPOWER】
    ADC2_CInit();  //【适用模块：ADC2】
    

    /* 外部中端 */
    //EXTI_Init();  //【适用模块：MOTOR】  


    /* IOEXT（IO扩展） */
    //IOEXT_Init(true);  //【适用模块：LCD、VPOWER】
    //IOEXT_VarPowerClose(1, true);
    //IOEXT_VarPowerClose(2, true);

    
    /* PWM 输出 */
    //TIM1_PWMO_Init(100);
    //TIM1_PWMO_Send(56, 56, 0);
    //TIM1_PWMO_Send(57, 57, 0);
    
    //TIM2_PWMO_Init(200);
    //TIM2_PWMO_Send(55, 0);

    //TIM3_PWMO_Init(300);
    //TIM3_PWMO_Send(54, 0);


    /* PWM 捕获 */   
    //TIM1_PWMI_Init();
    //TIM1_PWMI_Start();

    //TIM2_PWMI_Init();
    //TIM2_PWMI_Start();
    
    //TIM3_PWMI_Init();
    //TIM3_PWMI_Start();

    
    /* 蜂鸣器 */
    BUZZER_Init();
    //BUZZER_Open(1);
    //BUZZER_Close(1);
    

    /* LCD（液晶屏） */
    //LCD_MainInit();
    //LCD_MainLedSceneInit();
    //LCD_MSP3520_SpiOpenScreen();

    
    /* 启用总中断 */
    //enableInterrupts();  //【适用模块：LIN、TIM、MOTOR、MOTOR-TMC-UART、MOTOR-ZDT-UART、EXTI、UART1、UART3】


    /* MOTOR（电机） */
    //MOTOR_Init();  // 初始化：低细分、顺时针、电机启用、TIM2初始化
    //MOTOR_SetDir(MOTOR_DIR_CW);  // 设置 MOTOR 方向
    //MOTOR_SetDiv(MOTOR_DIV_HIGH);  // 设置 MOTOR 细分
    //MOTOR_InitSpeed(30);  // 初始化电机速度 
    //MOTOR_RunKeep();  // 持续运行
    

    /* MOTOR-TMC-UART（串口电机） */
    /* 注：以下代码因为牵涉到 UART1 数据中断接收，需要在 enableInterrupts() 之后执行 */
    //MOTOR_TMCUART_Init();                // 初始化：开启半双工、电机启用
    //MOTOR_TMCUART_SetIRunIHold(80, 10);  // 设置运行时最高电流百分比，静止时电流百分比

    //MOTOR_TMCUART_SetCoolStep(220);      // 开启 CoolStep 功能（智能节流）；无丝杆电机（2细分1000速度=200、4细分1000速度=100），有丝杆电机（2细分1000速度=130，8细分3000速度=70）
    //MOTOR_TMCUART_CloseCoolStep();       // 关闭 CoolStep 功能（智能节流）

    //MOTOR_TMCUART_SetMircoStep(1);               // 设置细分
    //MOTOR_TMCUART_SetMircoStep(MOTOR_GetDiv());  // 设置细分（基于 MOTOR）
    
    //MOTOR_TMCUART_MainRunKeep(2, 1000);  // 电机运行
    //MOTOR_TMCUART_MainRunKeep(0, 0);     // 电机停止


    /* MOTOR-ZDT-UART */
    /* 注：以下代码因为牵涉到 UART3 数据中断接收，需要在 enableInterrupts() 之后执行 */
    //delay_ms(500);  // 上电延时
    //MOTOR_ZDTUART_Init();  // 初始化：电机启用、设置细分/方向/速度
    //MOTOR_ZDTUART_SetZeroPoint(true);  // 设置零点
    //MOTOR_ZDTUART_SetZeroConfig(true);  // 设置配置


    /* MOTOR-ZDT-UART + STEP/DIR */
    //MOTOR_SetDiv(MOTOR_DIV_HIGH);
    //MOTOR_SetDir(MOTOR_ZDTUART_DIR_CW);  // 设置 MOTOR 方向
    //if( MOTOR_ZDTUART_SetDiv(MOTOR_DIV_HIGH,true) ){ MOTOR_ZDTUART_SetDisable(true); return 0; }  // 设置细分值；从而保证 UART 和 STEP/DIR 两个模式的数据一致
    
    
    // 主循环内容
    while(1){       
        /* PWM 输出 */
        //TIM1_PWMO_Send(56, 19, 0);
        //TIM1_PWMO_Send(57, 68, 0);
        
        //if( KEY_IsPress(51,true) == KEY_STATE_YES ) TIM1_PWMO_Send(56, 51, 0);
        //if( KEY_IsPress(50,true) == KEY_STATE_YES ) TIM1_PWMO_Stop(56);
        //if( KEY_IsPress(48,true) == KEY_STATE_YES ) TIM1_PWMO_Send(57, 48, 0);
        //if( KEY_IsPress(47,true) == KEY_STATE_YES ) TIM1_PWMO_Stop(57);

        //if( KEY_IsPress(51,true) == KEY_STATE_YES ) TIM2_PWMO_Send(51, 0);
        //if( KEY_IsPress(50,true) == KEY_STATE_YES ) TIM2_PWMO_Send(100, 0);
        //if( KEY_IsPress(48,true) == KEY_STATE_YES ) TIM2_PWMO_Send(48, 0);
        //if( KEY_IsPress(47,true) == KEY_STATE_YES ) TIM2_PWMO_Stop();
              
        //if( KEY_IsPress(51,true) == KEY_STATE_YES ) TIM3_PWMO_Send(51, 0);
        //if( KEY_IsPress(50,true) == KEY_STATE_YES ) TIM3_PWMO_Stop();
        //if( KEY_IsPress(48,true) == KEY_STATE_YES ) TIM3_PWMO_Send(48, 0);
        //if( KEY_IsPress(47,true) == KEY_STATE_YES ) TIM3_PWMO_Stop();
        
        
        /* PWM 捕获 */
        //printf("============================\r\n");
        //printf("J54: %dHz, Duty = %f%\r\n", TIM3_PWMI_GetFreq(54), TIM3_PWMI_GetDuty(54));
        //printf("J55: %dHz, Duty = %f%\r\n", TIM2_PWMI_GetFreq(55), TIM2_PWMI_GetDuty(55));
        //printf("J56: %dHz, Duty = %f%\r\n", TIM1_PWMI_GetFreq(56), TIM1_PWMI_GetDuty(56));
        //printf("J57: %dHz, Duty = %f%\r\n", TIM1_PWMI_GetFreq(57), TIM1_PWMI_GetDuty(57));
        
        
        /* VPOWER（可控电源） */
        //IOEXT_VarPowerOpen(1, true); delay_ms(2000); IOEXT_VarPowerClose(1, true); delay_ms(2000);
        //IOEXT_VarPowerOpen(2, true); delay_ms(2000); IOEXT_VarPowerClose(2, true); delay_ms(2000);
        //IOEXT_VarPowerOpen(3, true); delay_ms(2000); IOEXT_VarPowerClose(3, true); delay_ms(2000);
        
        
        /* LCD（液晶屏） */
        //LCD_MainAnimateIcons();  // 显示动态图标
        //LCD_MainMotorMonitor();
        //LCD_MSP3520_ShowInteger(0, 0, 5, 12, LCD_COLOR_RED, LCD_COLOR_WHITE, 1, 0);

        
        /* CONNECT（通断路） */
        //CONNECT_MainConnectMapLed();             // 连通点亮灯；依赖于 LCD_MainLedSceneInit()
        CONNECT_MainConnectAnyMapBuzzer();       // 任一路连通那么启动蜂鸣器
        //CONNECT_MainConnectMapLedFilterShort();  // 连通点亮灯并处理短路；依赖于 LCD_MainLedSceneInit()
  
        
        /* ADC2（电压采样） */ 
        //ADC2_MainPrintAllAdcValue();  // 显示所有ADC并输出
        
        
        /* MOTOR（电机） */
        //MOTOR_MainTwoPointRunByLength(10);  // 电机两点往返运行（按长度参数）
        //MOTOR_MainTwoPointRunByDegree(180);  // 电机两点往返运行（按角度参数）


        /* MOTOR-TMC-UART（串口电机） */
        //MOTOR_TMCUART_MainRoundTrip();            // 电机运行2秒往返（该函数内部会自动启动电机的运行）
        //MOTOR_TMCUART_MainMircoStepLoop();        // 循环执行全部细分值（该函数内部会自动启动电机的运行）
        //MOTOR_TMCUART_MainStallReverse(220*0.7);  // 电机堵转后反转（该函数内部不会自动启动电机的运行）
        //MOTOR_TMCUART_MainMonitorSGResult();      // 监视 SG_RESULT 值（该函数内部不会自动启动电机的运行）
        
        
        /* MOTOR-ZDT-UART（串口电机） */
        //MOTOR_ZDTUART_MainRunAndZero();  // 电机运行指定角度并回零
        //MOTOR_ZDTUART_MainRunAndZeroWithPulse();  // 电机运行指定角度并回零（UART+STEP/DIR 模式）
        //MOTOR_ZDTUART_MainTwoPointRunByDegree(90);  // 电机两点往返运行（按角度参数）

        
        /* UART1 */
        UART1_SendByte(0x0F);
        //UART1_MainProductAutoTester();
        //UART1_MainReceiveAndSendToLin();
        //UART1_MainReceiveAndSendToUsbByDirect();    // 接收数据并直接转发送给 USB（即发送给 UART1 自身）
        //UART1_MainReceiveAndSendToUsbByHeadFoot();  // 接收数据并发送给 USB（即发送给 UART1 自身）
        

        /* LIN */
        //LIN_MASTER_MainFindSlaveId(true);      // 查找从机的 ID 标识
        //LIN_MASTER_MainPrimaryReceiveToUsb();  // 发送主信号并接收从机的反馈信号，然后输出到 USB
        
        //LIN_MASTER_InitAuxSignal();       // 初始化下发的辅信号
        //LIN_MASTER_SetAuxSignalLLedOn();  // 辅信号：左使能灯亮
        //LIN_MASTER_SetAuxSignalRLedOn();  // 辅信号：右使能灯亮
        //LIN_MASTER_SetAuxSignalBLedOn();  // 辅信号：背光灯亮
        //LIN_MASTER_SendAuxSignal();       // 发送下发的辅信号

        
        /* KEY（按键） */
        //KEY_MainKeyMapLed();     // 按下点亮灯；依赖于 LCD_MainLedSceneInit()
        //KEY_MainKeyMapBuzzer();  // 按下蜂鸣器响
    }
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