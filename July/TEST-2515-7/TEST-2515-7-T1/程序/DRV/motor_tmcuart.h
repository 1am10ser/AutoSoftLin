#ifndef __MOTOR_TMCUART_H_
#define __MOTOR_TMCUART_H_



    /* -------------------------------------------------- */
    // ● 采用 TMC UART 方式驱动
    // ● 使用前务必先对 UART1 进行初始化：UART1_CInit();
    // ● 所有的寄存器地址和电机工作逻辑基于 TMC2209 芯片和其数据手册
    // ● v4_20231021 版本以后，必须手动将电机的串口和UART1进行焊接，具体参考原理图
    // 
    // ● motor_tmcuart.c 和 motor.c 是完全独立两套逻辑，没有依赖关系，各自使用各自的函数库
    // ● 当然 motor_tmcuart.c 和 motor.c 也可以混用，TMC2209 支持 UART 和 STEP/DIR 混用，比如利用 UART 设置细分等参数，STEP/DIR 控制电机运行
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    // MOTOR TMC-UART 启禁控制参数
    #define MOTOR_TMCUART_ENABLE_PIN     GPIO_PIN_6
    #define MOTOR_TMCUART_ENABLE_PORT    GPIOA



    /* -------------------------------------------------- */
    // ● 寄存器地址
    /* -------------------------------------------------- */
    // 具体参见 TMC2209 数据手册 20+ 页

    // 默认寄存器
    #define REGADDR_IHOLD_IRUN 0x10  // 默认电流配置
    #define REGADDR_GCONF      0x00  // 通用配置（General Configuration Registers）
    #define REGADDR_GSTAT      0x01  // 全局状态寄存器（Global Status Flags）
    #define REGADDR_CHOPCONF   0x6C  // 斩波器配置（Chopper Configuration）
    #define REGADDR_VACTUAL    0x22  // 电机运行速度（Velocity Actual）
    #define REGADDR_TSTEP      0x12  // 微步时间（Time Step）：以 1/fCLK 为单位的步进输入信号上的两个 1/256 微步之间的实际测量时间
    #define REGADDR_DRV_STATUS 0x6F  // 驱动状态寄存器（Driver Status Flags）
    #define REGADDR_IFCNT      0x02  // UART写指令计数：每成功写入UART接口，该寄存器会增加；可以读出它来检查串行传输中是否有丢失的数据

    // 失速守卫（StallGuard Control）相关寄存器
    #define REGADDR_SG_RESULT  0x41  // 失速守卫的结果（StallGuard Result)：读取的 SG_RESULT 值越低那么表示此时电机负载很高，反之表示电机负载很低
    #define REGADDR_TPWMTHRS   0x13 
    #define REGADDR_TCOOLTHRS  0x14  // 节能模式的阈值、失速守卫失速时DIAG输出（CoolStep Threshold）:一般直接设置为最高值 1048575
    #define REGADDR_COOLCONF   0x42  // 智能能源控制（Smart Energy Control CoolStep）



    /* -------------------------------------------------- */
    // ● 函数声明（API）
    // ● 所有 MOTOR_TMCUART_SetXXX 开头的函数都会自动获取寄存器值然后再写入（使用时注意函数的效率，每调用此类函数都会先去获取源值再写入）
    // ● 所有 MOTOR_TMCUART_BuildXXX 开头的函数仅仅获取寄存器值
    /* -------------------------------------------------- */
    void MOTOR_TMCUART_Init();                                            // 初始化函数
    long MOTOR_TMCUART_ReadReg( u8 address );                             // 读取寄存器值
    void MOTOR_TMCUART_WriteReg( u8 address, long value );                // 写寄存器值

    void MOTOR_TMCUART_Enable();                                          // 电机启用（通过芯片外置的引脚控制）
    void MOTOR_TMCUART_Disable();                                         // 电机禁用（通过芯片外置的引脚控制）

    long MOTOR_TMCUART_GetCmdTotal();                                     // 获取UART写入成功的指令计数
    long MOTOR_TMCUART_GetStopFlag( long regDrvStatus, short getDelay );  // 获取电机静止标记
    long MOTOR_TMCUART_GetRunModeFlag( long regDrvStatus );               // 获取电机运行模式
    long MOTOR_TMCUART_GetReverseFlag( long regGConfValue );              // 获取电机反转标记
    
    void MOTOR_TMCUART_SetReverse();                                      // 设置电机反转
    long MOTOR_TMCUART_BuildReverse( long regGConfValue );                // 构建寄存器值：电机反转
    
    void MOTOR_TMCUART_SetCoolStep( long sgResult );                      // 设置智能电流功能
    long MOTOR_TMCUART_BuildCoolConf( u8 semin, u8 semax );               // 构建寄存器值：智能电流控制配置
    void MOTOR_TMCUART_CloseCoolStep();                                   // 关闭智能电流功能

    void MOTOR_TMCUART_SetIRunIHold( u8 irun, u8 ihold );                 // 设置电机基础电流
    long MOTOR_TMCUART_BuildIRunIHold( u8 irun, u8 ihold );               // 构建寄存器值：电机基础电流
    
    void MOTOR_TMCUART_SetMircoStep( short value );                                 // 设置细分
    long MOTOR_TMCUART_BuildMircoStep( long regChopConfValue, short value );        // 构建寄存器值：设置细分 
    long MOTOR_TMCUART_BuildMircoStepRegSelect( long regGConfValue, short value );  // 构建寄存器值：细分寄存器选择 

    void MOTOR_TMCUART_SetDriverEnable();                                           // 设置驱动启用（通过芯片寄存器配置控制）
    void MOTOR_TMCUART_SetDriverDisable();                                          // 设置驱动禁用（通过芯片寄存器配置控制）
    long MOTOR_TMCUART_BuildDriverEnable( long regChopConfValue, short value );     // 构建寄存器值：设置驱动启用/禁用
    
    u8 MOTOR_TMCUART_CalcCRC( u8 *data, u8 dataStart, u8 dataLength );              // 计算并设置 CRC 值
    


    /* -------------------------------------------------- */
    // ● 函数声明（主逻辑）
    /* -------------------------------------------------- */
    void MOTOR_TMCUART_MainInit();                              // 主逻辑：初始化
    void MOTOR_TMCUART_MainRunKeep( short mstep, long speed );  // 主逻辑：电机持续运行
    void MOTOR_TMCUART_MainRoundTrip();                         // 主逻辑：电机运行2秒往返
    void MOTOR_TMCUART_MainStallReverse( long stallSGResult );  // 主逻辑：电机堵转后反转
    void MOTOR_TMCUART_MainMircoStepLoop();                     // 主逻辑：循环执行全部细分值
    void MOTOR_TMCUART_MainMonitorSGResult();                   // 主逻辑：监视 SG_RESULT 值
    
    
#endif