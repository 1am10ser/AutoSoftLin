#ifndef __A4988_H_
#define __A4988_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    /**
     * A4988 常量
     */

    // A4988 驱动模式引脚
    // 由于测试版上 IO 口不够用，所以 A4988 无法独立控制全部细分模式
    // 目前的设计是两种自由定义细分方案：初始一种细分 + 软件控制另外一种细分。
    //
    // 至于初始的和软件控制的是哪种细分需要预先在 PCB 板上进行跳线设置（PCB 板子丝印为 MS1、MS2、MS3 与 5V或PE2 自由跳线组合）
    // 1 细分和 8  细分的跳线方式：5v-[R]-MS1, 5v-[R]-MS2              |  MS1-PE2, MS2-PE2
    // 2 细分和 8  细分的跳线方式：5v-[R]-MS1, 5v-[R]-MS2              |         , MS2-PE2
    // 4 细分和 8  细分的跳线方式：5v-[R]-MS1, 5v-[R]-MS2              |  MS1-PE2,        
    //
    // 1 细分和 16 细分的跳线方式：5v-[R]-MS1, 5v-[R]-MS2, 5v-[R]-MS3  |  MS1-PE2, MS2-PE2, MS3-PE2
    // 2 细分和 16 细分的跳线方式：5v-[R]-MS1, 5v-[R]-MS2, 5v-[R]-MS3  |         , MS2-PE2, MS3-PE2
    // 4 细分和 16 细分的跳线方式：5v-[R]-MS1, 5v-[R]-MS2, 5v-[R]-MS3  |  MS1-PE2,        , MS3-PE2
    // ...
    //
    // 编号为 TesterProductA.v5_20210604  的PCB板的细分控制支持由 PCB 上直接焊接电阻初始一种细分，也可以通过飞线连接到 PE2 然后软件控制另外一种细分
    // 编号为 TesterProductA.v5_20210605+ 的PCB板的细分控制支持由 PCB 上直接焊接电阻初始一种细分，也可以通过焊盘连接到 PE2 然后软件控制另外一种细分
    #define A4988_MS_PIN               GPIO_PIN_2
    #define A4988_MS_PORT              GPIOE
    #define A4988_MS_MODE0_SUBDIV      2  //【按需设置】初始的细分（焊接电阻，PE2 低电平）
    #define A4988_MS_MODE1_SUBDIV      8  //【按需设置】软控的细分（软件控制，PE2 高组态）

    // A4988 方向控制引脚
    #define A4988_DIR_PIN              GPIO_PIN_1
    #define A4988_DIR_PORT             GPIOE
    #define A4988_DIR_BACK             0  //【常量】电机 后退/逆时针 常量
    #define A4988_DIR_FRONT            1  //【常量】电机 前进/顺时针 常量

    // A4998 外接的电机参数
    #define A4988_MOTOR_PWM_FREQ       1000 //【按需设置】能正常驱动电机的默认频率；该值需要通过连接电机实际测试确定
    #define A4988_MOTOR_STEP_DEGREE    1.8  //【按需设置】电机 步进脚 度数（未细分）；可通过电机手册参数获取 + 细分

    #define A4988_MOTOR_CIRCLE_LEN     4    //【按需设置】电机走一圈前进的毫米值；可通过电机手册参数获取；该配置主要用于带丝杆的电机
    #define A4988_MOTOR_CIRCLE_TIME    175  //【按需设置】电机以默认频率走一圈所需的毫秒数（未细分）；可通过实测获取；主要用于电机运行时等待
                                            //　　　　　　设置该变量不同的值，然后通过以下代码，查看电机是否能正常来回一次，如果能正常即可（该值需尽量精确，否则细分后误差会变大）：
                                            //　　　　　　A4988_RunLength(A4988_DIR_BACK, A4988_MOTOR_CIRCLE_LENGTH); A4988_RunLengthWait(A4988_MOTOR_CIRCLE_LENGTH); A4988_RunLength(A4988_DIR_FRONT,A4988_MOTOR_CIRCLE_LENGTH); A4988_RunLengthWait(A4988_MOTOR_CIRCLE_LENGTH);  
                                            //　　　　　　A4988_RunDegree(A4988_DIR_BACK, 360); A4988_RunDegreeWait(360); A4988_RunDegree(A4988_DIR_FRONT,360); A4988_RunDegreeWait(360);
    // A4988 外接的电机复位触发脚
    #define A4988_MOTOR_ZERO_PIN       0
    #define A4988_MOTOR_ZERO_PORT      0



    /* -------------------------------------------------- */
    // ● 函数声明
    /* -------------------------------------------------- */
    void   A4988_Init();
    void   A4988_InitCount();

    void   A4988_SetDir( short dir );
    short  A4988_GetDir();
    
    void   A4988_SetSubdiv( short subdiv );
    short  A4988_GetSubdiv();
    
    void   A4988_CountStep();
    
    long   A4988_GetCountCircle();
    double A4988_GetCountDegree();
    
    bool   A4988_RunKeep( short dir, short runKeepProtectPrev );
    bool   A4988_RunKeeping( short dir );
    bool   A4988_RunKeepStop();
    
    bool   A4988_RunStep( short dir, u32 number );
    void   A4988_RunStepWait( u32 number );
    
    bool   A4988_RunLength( short dir, double length );
    void   A4988_RunLengthWait( double length );

    bool   A4988_RunDegree( short dir, double degree );
    void   A4988_RunDegreeWait( double degree );
    
    void   A4988_InterruptHandlerRuning();
    
    void   A4988_MainInit();
    void   A4988_MainKeyControlMotorReal();
    void   A4988_MainKeyControlMotorByLength( double length );
    
    
#endif