#ifndef __MOTOR_ZDTUART_H_
#define __MOTOR_ZDTUART_H_



    /* -------------------------------------------------- */
    // ● 采用 ZDT UART 方式驱动（Emm_V5.0）
    // ● 使用前务必先对 UART3 进行初始化：UART3_CInit();
    // ● 之所以不使用 UART1，是因为 UART1 需要通过 USB 和电脑通讯，如果电机也采用 UART1 通讯，那么就会因为 CH340G 芯片导致无法收到电机驱动板的反馈数据  
    // ● v4_20231021 版本以后，必须手动将电机的串口和UART3进行焊接，具体参考原理图
    // 
    // ● motor_zdtuart.c 和 motor.c 是完全独立两套逻辑，没有依赖关系，各自使用各自的函数库
    // ● 当然 motor_zdtuart.c 和 motor.c 也可以混用，ZDT 支持 UART 和 STEP/DIR 混用，比如利用 UART 设置细分等参数，STEP/DIR 控制电机运行
    //
    // ● 该 ZDT UART 驱动库基于 0xF778 版本的驱动板编写，获取驱动板可使用函数：MOTOR_ZDTUART_GetVersion()
    // ● 版本号中，高8位表示软件版本：0xF4（Emm42_V5.0.0）、0xF7（Emm42_V5.0.3）
    // ● 版本号中，低8位表示硬件版本：0x78（ZDT_X42_V1.2）
    //
    // ● 注意：电机驱动板务必烧录 0xFA（Emm42_V5.0.6）程序，否则串口可能出现BUG
    // ● 注意：对于一些配置写入到驱动板的API接口，如果不进行延迟等待，那么会导致第二次发送串口命令执行失败且无反馈数据
    //         原因是写入配置到驱动板时，驱动板是利用定时器去刷新数据->关闭中断->修改配置值->存储到Flash->然后再初始化，所以需要一定的时间。而这一段时间内发送的新串口命令时无法执行的
    /* -------------------------------------------------- */


    // MOTOR 方向控制参数
    #define MOTOR_ZDTUART_DIR_CW        0x00  //【常量】电机 前进/顺时针 常量
    #define MOTOR_ZDTUART_DIR_CCW       0x01  //【常量】电机 后退/逆时针 常量
    #define MOTOR_ZDTUART_DIR_NEAR      0xFF  //【常量】电机 就近 常量

    #define MOTOR_ZDTUART_ZERO_DIR      0x01  //【常亮】电机单圈回零
    #define MOTOR_ZDTUART_ZERO_NEAR     0x00  //【常量】电机就近回零


    // MOTOR 电机相关参数
    #define MOTOR_ZDTUART_STEP_DEGREE   1.8  //【按需设置】电机 步进脚 度数（未细分）；可通过电机手册参数获取 + 细分



    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    short  MOTOR_ZDTUART_GetDiv();
    short  MOTOR_ZDTUART_GetDir();
    short  MOTOR_ZDTUART_GetSpeed();
    double MOTOR_ZDTUART_GetDegree360();
    double MOTOR_ZDTUART_GetRunDegree360Near( double currDegree, double distDegree );

    short  MOTOR_ZDTUART_GetStates();
    long   MOTOR_ZDTUART_GetVersion();
    
    short  MOTOR_ZDTUART_Init();
    short  MOTOR_ZDTUART_SetDiv( short div, bool delay );
    short  MOTOR_ZDTUART_SetDir( short dir );
    short  MOTOR_ZDTUART_SetSpeed( short speed );

    short  MOTOR_ZDTUART_SetEnable();
    short  MOTOR_ZDTUART_SetDisable();

    short  MOTOR_ZDTUART_RunDegree( double degree );
    short  MOTOR_ZDTUART_RunFinish( short states );
    
    short  MOTOR_ZDTUART_Zero( short mode );
    short  MOTOR_ZDTUART_ZeroFinish();
    short  MOTOR_ZDTUART_SetZeroPoint( bool delay );
    short  MOTOR_ZDTUART_SetZeroConfig( bool delay );
    short  MOTOR_ZDTUART_GetZeroConfig();



    /* -------------------------------------------------- */
    // ● 函数声明（主逻辑）
    /* -------------------------------------------------- */
    void MOTOR_ZDTUART_MainRunAndZero();  // 电机运行指定角度并回零
    void MOTOR_ZDTUART_MainRunAndZeroWithPulse();  // 电机运行指定角度并回零（结合脉冲模式）
    void MOTOR_ZDTUART_MainTwoPointRunByDegree( short degree );  // 电机两点往返运行（按角度参数）
    
    
    
#endif