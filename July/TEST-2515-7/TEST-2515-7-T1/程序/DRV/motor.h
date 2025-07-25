#ifndef __MOTOR_H_
#define __MOTOR_H_



    /* -------------------------------------------------- */
    // ● 采用 STEP/DIR 方式驱动
    // ● 对于 TMC2209 芯片，还支持 STEP/DIR 和 UART 混用
    /* -------------------------------------------------- */


    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    /**
     * MOTOR 常量
     */

    // 细分模式引脚
    // 由于测试版上 IO 口不够用，所以 电机驱动板 无法独立控制全部细分模式
    // 目前的设计是两种自由定义细分方案：初始一种细分（PE2低电平） + 软件控制另外一种细分（PE2高组态）。
    // 至于初始的和软件控制的是哪种细分需要预先在 PCB 板上进行跳线设置（PCB 板子丝印为 MS1、MS2、MS3 与 5V或PE2 自由跳线组合）
    //
    // === 【A4988】电机驱动板的PCB焊接和细分跳线设置如下：
    //     细分跳线：1 细分和 8  细分：MS1-[R]-5v, MS2-[R]-5v              |  MS1-PE2, MS2-PE2
    //     细分跳线：2 细分和 8  细分：MS1-[R]-5v, MS2-[R]-5v              |         , MS2-PE2
    //     细分跳线：4 细分和 8  细分：MS1-[R]-5v, MS2-[R]-5v              |  MS1-PE2,        
    //
    //     细分跳线：1 细分和 16 细分：MS1-[R]-5v, MS2-[R]-5v, MS3-[R]-5v  |  MS1-PE2, MS2-PE2, MS3-PE2
    //     细分跳线：2 细分和 16 细分：MS1-[R]-5v, MS2-[R]-5v, MS3-[R]-5v  |         , MS2-PE2, MS3-PE2
    //     细分跳线：4 细分和 16 细分：MS1-[R]-5v, MS2-[R]-5v, MS3-[R]-5v  |  MS1-PE2,        , MS3-PE2
    //
    //     PCB 焊接：R17和R30空置，5脚和6脚短接
    //     购买连接：https://item.taobao.com/item.htm?id=593072005558
    //
    //
    // === 【TB67S109】电机驱动板的PCB焊接和细分跳线设置如下：
    //     细分跳线：1 细分和 8  细分的跳线方式：MS1-[R]-5v,           , MS3-[R]-5v  |  MS1-PE2
    //     细分跳线：2 细分和 8  细分的跳线方式：MS1-[R]-5v,           , MS3-[R]-5v  |         ,        , MS3-PE2
    //     细分跳线：2 细分和 16 细分的跳线方式：MS1-[R]-5v, MS2-[R]-5v,             |  MS1-PE2
    //
    //     PCB 焊接：R17和R30空置
    //     购买连接：https://item.taobao.com/item.htm?id=558783722553
    //
    //
    // === 【TMC2209】电机驱动板的PCB焊接和细分跳线设置如下：
    //     细分跳线：8 细分和 16 细分的跳线方式：MS1-[R]-5v, MS2-[R]-5v,             |  MS1-PE2, MS2-PE2
    //     细分跳线：8 细分和 32 细分的跳线方式：MS1-[R]-5v, MS2-[R]-5v,             |  MS1-PE2
    //     细分跳线：8 细分和 64 细分的跳线方式：MS1-[R]-5v, MS2-[R]-5v,             |         , MS2-PE2
    //
    //     PCB 焊接：R17、R30空置，R212(MS3) 置空
    //     购买连接：https://item.taobao.com/item.htm?id=610229295552
    //     备注说明：TMC2208/TMC2226 支持2和4细分，具体参见：https://item.taobao.com/item.htm?id=677273237658


    // MOTOR 细分控制参数
    #define MOTOR_DIV_PIN         GPIO_PIN_2
    #define MOTOR_DIV_PORT        GPIOE
    #define MOTOR_DIV_LOW         8   //【按需设置】初始的细分（PE2低电平）
    #define MOTOR_DIV_HIGH        16  //【按需设置】软控的细分（PE2高组态）
 
    // MOTOR 方向控制参数
    #define MOTOR_DIR_PIN         GPIO_PIN_1
    #define MOTOR_DIR_PORT        GPIOE
    #define MOTOR_DIR_CW          0  //【常量】电机 前进/顺时针 常量
    #define MOTOR_DIR_CCW         1  //【常量】电机 后退/逆时针 常量
       
    // MOTOR 启禁控制参数
    #define MOTOR_ENABLE_PIN      GPIO_PIN_6
    #define MOTOR_ENABLE_PORT     GPIOA

    // MOTOR 电机相关参数
    #define MOTOR_CIRCLE_LEN      4    //【按需设置】电机走一圈前进的毫米值；可通过电机手册参数获取；该配置主要用于带丝杆的电机
    #define MOTOR_STEP_DEGREE     1.8  //【按需设置】电机 步进脚 度数（未细分）；可通过电机手册参数获取 + 细分

    // MOTOR 电机归零触发脚
    // 如果需要启用电机归零中断功能，那么需要加载 EXTI 模块
    // 电机归零中断功能主要用于电机运行中并触发原点时的电机停止操作
    #define MOTOR_ZERO_EXTINUM    60  //【按需设置】中断引脚：J60 或 J61
    #define MOTOR_ZERO_EXTITYPE   0   //【按需设置】中断类型：0 表示下降沿，1 表示上升沿

    // 电机常用状态
    // 状态为 0 时表示电机静止，状态非 0 时表示电机运行中
    // 以下所有的状态值都是独立的，并无包含关系，自己按需选用状态进行设置
    #define MOTOR_STATE_IDLE          0    // 空闲
    #define MOTOR_STATE_RUN           10   // 运行中（运行中）
    #define MOTOR_STATE_RUN_CW        11   // 运行中（顺时针运行中）
    #define MOTOR_STATE_RUN_CCW       12   // 运行中（逆时针运行中）
    #define MOTOR_STATE_RUN_FORWARD   13   // 运行中（向前运行中）
    #define MOTOR_STATE_RUN_BACKWARD  14   // 运行中（向后运行中）
    #define MOTOR_STATE_ZERO          20   // 运行中（归零）
    #define MOTOR_STATE_ZERO_CW       21   // 运行中（归零顺时针运行中）
    #define MOTOR_STATE_ZERO_CCW      22   // 运行中（归零顺时针运行中）
    #define MOTOR_STATE_ZERO_FORWARD  23   // 运行中（归零向前运行中）
    #define MOTOR_STATE_ZERO_BACKWARD 24   // 运行中（归零向后运行中）


       
    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void   MOTOR_Init();               // 初始化
    void   MOTOR_InitCount();          // 初始化计数

    void   MOTOR_SetDir( short dir );  // 设置电机方向
    short  MOTOR_GetDir();             // 获取电机方向
    
    void   MOTOR_SetDiv( short div );  // 设置电机细分
    short  MOTOR_GetDiv();             // 获取电机细分
    
    short  MOTOR_GetSpeed();                // 获取电机运行速度（RPM）
    void   MOTOR_InitSpeed( short speed );  // 设置电机运行速度（RPM）
    
    void   MOTOR_SetState( short state );      // 设置电机当前状态
    short  MOTOR_GetState();                   // 获取电机当前状态
    void   MOTOR_SetStatePrev( short state );  // 设置电机上一个状态
    short  MOTOR_GetStatePrev();               // 获取电机上一个状态
    
    void   MOTOR_Enable();     // 电机启用
    void   MOTOR_Disable();    // 电机禁用
    void   MOTOR_CountStep();  // 电机计数                          

    long   MOTOR_GetCountPulse();   // 获取累计的脉冲
    long   MOTOR_GetCountCircle();  // 获取累计的圈数
    double MOTOR_GetCountDegree();  // 获取累计的角度
    
    bool   MOTOR_RunStop();  // 电机停止
    bool   MOTOR_RunKeep();  // 保持运行电机
    bool   MOTOR_RunStep( u32 number );       // 按步数运行电机
    bool   MOTOR_RunLength( double length );  // 按长度值运行电机
    bool   MOTOR_RunDegree( double degree );  // 按角度值运行电机


       
    /* -------------------------------------------------- */
    // ● 函数声明（中断）
    /* -------------------------------------------------- */
    void   MOTOR_InterruptHandlerPulse();
    void   MOTOR_InterruptHandlerZeroFinish( short extiNum, short extiType );


       
    /* -------------------------------------------------- */
    // ● 函数声明（主逻辑）
    /* -------------------------------------------------- */
    void   MOTOR_MainTwoPointRunByLength( short length );  // 电机两点往返运行（按长度参数）
    void   MOTOR_MainTwoPointRunByDegree( short degree );  // 电机两点往返运行（按角度参数）
    
    void   MOTOR_MainKeyControlMotorReal();
    void   MOTOR_MainKeyControlMotorByLength( double length );
    
    
    
#endif