#include "common.h"



/* -------------------------------------------------- */
// ● 采用 TMC UART 方式驱动
// ● 使用前务必先对 UART1 进行初始化：UART1_CInit();
// ● 所有的寄存器地址和电机工作逻辑基于 TMC2209 芯片和其数据手册
//
// ● motor_tmcuart.c 和 motor.c 是完全独立两套逻辑，没有依赖关系，各自使用各自的函数库
// ● 当然 motor_tmcuart.c 和 motor.c 也可以混用，TMC2209 支持 UART 和 STEP/DIR 混用，比如利用 UART 设置细分等参数，STEP/DIR 控制电机运行
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 初始化函数
 */
void MOTOR_TMCUART_Init()
{
    // 初始化
    long regval;
    
    // 开启半双工
    UART1_HalfDuplexCmd(ENABLE);
    
    // 初始化启禁
    GPIO_Init(MOTOR_TMCUART_ENABLE_PORT, MOTOR_TMCUART_ENABLE_PIN, GPIO_MODE_OUT_PP_LOW_FAST);  // 低速推挽输出低电平
    MOTOR_TMCUART_Enable();
    
    // 初始全局配置
    regval = MOTOR_TMCUART_ReadReg(REGADDR_GCONF);
    regval = regval | 0x00000040;  // 设置 GCONFI.pdn_disable = 1，用于关闭 PDN_UART 外部输入
    MOTOR_TMCUART_WriteReg(REGADDR_GCONF, regval);
}


/**
 * 电机启用
 * 通过芯片外置的引脚控制
 */
void MOTOR_TMCUART_Enable()
{
    GPIO_WriteLow(MOTOR_TMCUART_ENABLE_PORT, MOTOR_TMCUART_ENABLE_PIN);
}


/**
 * 电机禁用
 * 通过芯片外置的引脚控制
 */
void MOTOR_TMCUART_Disable()
{
    GPIO_WriteHigh(MOTOR_TMCUART_ENABLE_PORT, MOTOR_TMCUART_ENABLE_PIN);
}


/**
 * 读取寄存器值
 *
 * param u8 address  寄存器地址，详见 motor_tmcuart.h 里面的 REGADDR_XXX 宏定义
 * return long  寄存器值，-1 表示数据无效
 */
long MOTOR_TMCUART_ReadReg( u8 address )
{
    // 初始化
    u8 i, crc;
    u8 recData[20], recDataValid;
    u8 sendCommand[4] = {0x05,0x00,0x00,0x00};
    long recDatValue = 0;

    // 寄存器地址
    sendCommand[2] = address;
    
    // 设置命令数据 CRC 值
    MOTOR_TMCUART_CalcCRC(sendCommand, 0, 4);
    
    // 重置接收到的数据
    UART1_ResetReceiveData();
    
    // 发送数据
    UART1_SendBytes(sendCommand, 4); 
    
    // 延迟等待数据接收完成
    delay_ms(10);

    // 获取接收到的数据
    UART1_CopyReceiveData(recData);

    // 接收到的数据有效性检测（发送的命令检测）
    recDataValid = 1;
    for( i = 0; i < 4; i++ ) recDataValid &= recData[i] == sendCommand[i];
    if( recDataValid == 0 ) return -1;
    
    // 接收到的数据有效性检测（响应的数据头检测）
    if( recData[4] != 0x05 ) return -1;
    if( recData[5] != 0xFF ) return -1;
    if( recData[6] != sendCommand[2] ) return -1;

    // 接收到的数据有效性检测（CRC检测）
    crc = recData[11];
    if( crc != MOTOR_TMCUART_CalcCRC(recData,4,8) ) return -1;
    
    // 构建数据位值
    recDatValue |= (long)recData[7] << 24;
    recDatValue |= (long)recData[8] << 16;
    recDatValue |= (long)recData[9] <<  8;
    recDatValue |= (long)recData[10];

    // 返回值
    return recDatValue;
}


/**
 * 写寄存器值
 *
 * param u8   address  寄存器地址，详见 motor_tmcuart.h 里面的 REGADDR_XXX 宏定义
 * param long value    寄存器值
 */
void MOTOR_TMCUART_WriteReg( u8 address, long value )
{
    // 初始化
    u8 sendCommand[8] = {0x00};
    
    // 构建命令
    sendCommand[0] = 0x05;
    sendCommand[1] = 0x00;                   // 芯片编号，默认 0x00
    sendCommand[2] = address | 0x80;         // 要写入数据的寄存器地址
    sendCommand[3] = (value >> 24) & 0xFF;   // & 0xFF 主要作用就是当 value 为负数时且数据右移动，那么产生的数据空位填充会自动填充 0xF，使用时容易出问题所以需要将数据空位填充为 0
    sendCommand[4] = (value >> 16) & 0xFF;
    sendCommand[5] = (value >> 8 ) & 0xFF;
    sendCommand[6] = (value      ) & 0xFF;
        
    // 设置命令数据 CRC 值
    MOTOR_TMCUART_CalcCRC(sendCommand, 0, 8);
    
    // 发送数据
    UART1_SendBytes(sendCommand, 8);
}


/**
 * 获取UART写入成功的指令计数
 * 
 * return 返回值：0~255（循环）
 */
long MOTOR_TMCUART_GetCmdTotal()
{
    return MOTOR_TMCUART_ReadReg(REGADDR_IFCNT);
}


/**
 * 设置电机反转
 * 该函数对细分相关函数进行了封装，简化了使用步骤
 */
void MOTOR_TMCUART_SetReverse()
{
    // 初始化
    long regval;
    
    // 设置电机反转
    regval = MOTOR_TMCUART_ReadReg(REGADDR_GCONF);
    regval = MOTOR_TMCUART_BuildReverse(regval);
    MOTOR_TMCUART_WriteReg(REGADDR_GCONF, regval);
}


/**
 * 构建寄存器值：电机反转
 * 基于 GCONF 寄存器里的 GCONF.shaft 数据位（1bit）
 *
 * 以 TMC2209 为参考，GCONF寄存器值中的配置 电机反转 的数据位是第4位
 * 
 * @param long regGConfValue  源GCONF寄存器值
 * @return long  GCONF新的值
 */
long MOTOR_TMCUART_BuildReverse( long regGConfValue )
{
    // 获取 GCONF.shaft 数据位的值
    u8 shaft = (u8)(regGConfValue>>3) & 0x01;

    // 构建新 GCONF 值
    if( shaft == 1 ) regGConfValue &= 0xFFFFFFF7;
    if( shaft == 0 ) regGConfValue |= 0x00000008;

    // 返回新 GCONF 值
    return regGConfValue;
}


/**
 * 设置细分
 * 该函数对细分相关函数进行了封装，简化了使用步骤
 *
 * @param short value  细分值：1, 2, 4, 8, 16, 32, 64, 128, 256
 */
void MOTOR_TMCUART_SetMircoStep( short value )
{
    // 初始化
    long regval;
    
    // 激活细分寄存器
    regval = MOTOR_TMCUART_ReadReg(REGADDR_GCONF);
    regval = MOTOR_TMCUART_BuildMircoStepRegSelect(regval, 1);
    MOTOR_TMCUART_WriteReg(REGADDR_GCONF, regval);
    
    // 设置细分
    regval = MOTOR_TMCUART_ReadReg(REGADDR_CHOPCONF);
    regval = MOTOR_TMCUART_BuildMircoStep(regval, value);
    MOTOR_TMCUART_WriteReg(REGADDR_CHOPCONF, regval);
}


/**
 * 构建寄存器值：设置细分 
 * 基于 CHOPCONF 寄存器里的 CHOPCONF.MRES 数据位（4bit）
 *
 * 注：需要将 GCONF.mstep_reg_select 的值设置为 1，这样配置 CHOPCONF.MRES 后才会有效果：MOTOR_TMCUART_BuildMircoStepRegSelect(..., 1)
 * 
 * @param long  regChopConfValue  源CHOPCONF寄存器值
 * @param short value             细分值：1, 2, 4, 8, 16, 32, 64, 128, 256
 *
 * @return long  CHOPCONF寄存器新的值
 */
long MOTOR_TMCUART_BuildMircoStep( long regChopConfValue, short value )
{
    // 初始化
    u8 byte = 0;
    
    // 重置高8位中的低4位，并设置新的低4位值
    if( value == 256 ) byte |= 0x00;
    if( value == 128 ) byte |= 0x01;
    if( value == 64  ) byte |= 0x02;
    if( value == 32  ) byte |= 0x03;
    if( value == 16  ) byte |= 0x04;
    if( value == 8   ) byte |= 0x05;
    if( value == 4   ) byte |= 0x06;
    if( value == 2   ) byte |= 0x07;
    if( value == 1   ) byte |= 0x08;

    // 构建新寄存器值
    regChopConfValue = regChopConfValue & 0xF0FFFFFF;  // 将 CHOPCONF.MRES 的4位重置为 0
    regChopConfValue = regChopConfValue | ((long)byte<<24);  // 设置新的 CHOPCONF.MRES 的四位值

    // 返回新的寄存器值
    return regChopConfValue;
}


/**
 * 构建寄存器值：细分寄存器选择 
 * 基于 GCONF 寄存器里的 GCONF.mstep_reg_select 数据位（1bit）
 * 
 * 以 TMC2209 为参考，GCONF寄存器值中的配置 细分寄存器 的数据位是第8位
 * - mstep_reg_select = 0 时，那么使用 MS1/MS2 两个芯片引脚来确定电机细分
 * - mstep_reg_select = 1 时，那么使用 MSTEP 寄存器值来确定电机细分
 * 
 * @param long  regGConfValue  源GCONF寄存器值
 * @param short value          寄存器选择：0 表示 MS1/MS2 芯片引脚，1 表示使用 MSTEP 寄存器
 *
 * @return long  GCONF寄存器新的值
 */
long MOTOR_TMCUART_BuildMircoStepRegSelect( long regGConfValue, short value )
{
    // 构建新的寄存器值
    if( value == 0 ) regGConfValue = regGConfValue & 0xFFFFFF7F;
    if( value != 0 ) regGConfValue = regGConfValue | 0x00000080;
    
    // 返回新的寄存器值
    return regGConfValue;
}


/**
 * 设置驱动启用
 * 通过芯片寄存器配置控制
 * 该函数对驱动启用相关函数进行了封装，简化了使用步骤
 *
 * 通过 TMC 芯片内置的配置启用电机驱动
 */
void MOTOR_TMCUART_SetDriverEnable()
{
    // 初始化
    long regval;

    // 设置驱动禁用
    regval = MOTOR_TMCUART_ReadReg(REGADDR_CHOPCONF);
    regval = MOTOR_TMCUART_BuildDriverEnable(regval, 1);
    MOTOR_TMCUART_WriteReg(REGADDR_CHOPCONF, regval);
}


/**
 * 设置驱动禁用
 * 通过芯片寄存器配置控制
 * 该函数对驱动禁用相关函数进行了封装，简化了使用步骤
 *
 * 通过 TMC 芯片内置的配置禁用电机驱动
 */
void MOTOR_TMCUART_SetDriverDisable()
{
    // 初始化
    long regval;

    // 设置驱动禁用
    regval = MOTOR_TMCUART_ReadReg(REGADDR_CHOPCONF);
    regval = MOTOR_TMCUART_BuildDriverEnable(regval, 0);
    MOTOR_TMCUART_WriteReg(REGADDR_CHOPCONF, regval);
}


/**
 * 构建寄存器值：设置驱动启用/禁用
 * 
 * @param long  regChopConfValue  源CHOPCONF寄存器值
 * @param short value             启禁用值：0, 1
 *
 * @return long  CHOPCONF寄存器新的值
 */
long MOTOR_TMCUART_BuildDriverEnable( long regChopConfValue, short value )
{
    // 构建新的寄存器值
    if( value == 0 ) regChopConfValue = regChopConfValue & 0xFFFFFFF0;
    if( value != 0 ) regChopConfValue = regChopConfValue | 0x0000000F;

    // 返回新的寄存器值
    return regChopConfValue;
}


/**
 * 设置智能电流功能
 * 该函数对细分相关函数进行了封装，简化了使用步骤
 *
 * 注：电机低速时 CoolStep 功能将无法启用，例如：电机常规负载下测得的 SG_RESULT 值小于 100 的，那么智能电流功能基本就无法生效了
 *
 * @param long sgResult  堵转检测结果值
 *                       该值可以在电机正常运行时获取并常量化到程序里，获取方法：MOTOR_TMCUART_ReadReg(REGADDR_SG_RESULT)
 */
void MOTOR_TMCUART_SetCoolStep( long sgResult )
{
    // 初始化
    u8 semin;
    long regval;
    
    // 启用智能电流控制所需的阈值
    MOTOR_TMCUART_WriteReg(REGADDR_TPWMTHRS , 0x00);  // 设置为最小值，这样电机速度（TSTEP）永远大于该值，否则就会启用高速防抖模式而是使 CoolStep 功能禁用
    MOTOR_TMCUART_WriteReg(REGADDR_TCOOLTHRS, 0xFFFFF);  // 设置为最大值，这样电机速度（TSTEP）永远小于该值，就会一直启用 CoolStep 功能

    // 构建 SEMIN 值
    // SG_RESULT < SEMIN*32，电机电流将会增大，为0时不启用 CoolStep
    semin = (u8)(sgResult*0.6/32);
    semin = semin ? semin : 1;
    
    // 根据 SG_RESULT 值进行智能电流控制预配置
    regval = MOTOR_TMCUART_BuildCoolConf(semin, 0);
    MOTOR_TMCUART_WriteReg(REGADDR_COOLCONF, regval);
}


/**
 * 关闭智能电流功能
 */
void MOTOR_TMCUART_CloseCoolStep()
{
    MOTOR_TMCUART_WriteReg(REGADDR_TCOOLTHRS, 0);
}


/**
 * 构建寄存器值：智能电流控制配置
 * 基于 COOLCONF 寄存器
 * 
 * @param u8 semin  SG_RESULT < SEMIN*32，电机电流将会增大，为0时不启用 CoolStep
 *                  推荐值：SG_RESULT*60%/32
 * @param u8 semax  SG_RESULT > (SEMIN+SEMAX+1)*32，电机电流将会变小
 *                  推荐值：0/1；改值不宜过大，因为 SG_RESULT 值通常是电机没有太多阻力情况下获取到的，
 *                  　　　　   　所以如果 semax 太大，那么就会永远使：SG_RESULT < (SEMIN+SEMAX+1)*32，
 *                  　　　　   　当负载从大->小（SG_RESULT值则从小->大），但 SG_RESULT 值仍就 < (SEMIN+SEMAX+1)*32，电流就不会再减少了
 *                               
 *
 * @return long  COOLCONF寄存器值
 */
long MOTOR_TMCUART_BuildCoolConf( u8 semin, u8 semax )
{
    // 初始化
    u8 seup, sedn;
    long regCoolConfValue = 0;
    
    // 智能电流控制的最小电流
    regCoolConfValue |= 0x8000;  // 0x0000 表示 1/2*IRUN，0x8000 表示 1/4*IRUN;
    
    // 智能电流控制的 StallGuard2 磁滞值
    semax = semax & 0x0F;
    regCoolConfValue |= (long)semax << 8;

    // 电流下降速率
    // sedn 代表的值越小，电流下降速率越快
    // 也就是说当负载从大恢复到正常时，那么电流将快速恢复
    sedn = 0x11;  // 0x00/0x01/0x10/0x11 分别代表 StallGuard4值 每增加 32/8/2/1，电流减少 1（StallGuard4值应该是指 SG_RESULT 值）
    regCoolConfValue |= (long)sedn << 12;
    
    // 电流上升步长
    //
    // 值越大电流上升越快
    // 值越大的好处：不容易丢步，因为电流上升快，单位时间内扭矩上升快
    // 值越大的坏处：电流上升大，那么电机会有振荡
    // 值越大的坏处：如果遇到硬的障碍物导致急速堵转，那么电流就上升很快，一旦电流过高后可能导致芯片工作持续异常
    // 　　　　　　　在一个电机堵转反转的项目中，发现电流急速上升后下不来了：因为大电流和常规电流时电机的 SG_RESULT 是不一样，大电流运行电机时的 SG_RESULT 可能比小电流堵转的值还要小
    //
    // 值越大的适用场景：无急速堵转的项目
    // 值越小的适用场景：有急速堵转的项目
    seup = 0x11;  // 0x00/0x01/0x10/0x11 分别代表 1/2/4/8
    regCoolConfValue |= (long)seup << 4;

    // 智能电流控制的最小 Stallguard 2 值及智能电流使能
    // 0 表示 智能电流控制 coolStep 关闭
    semin = semin & 0x0F;
    regCoolConfValue |= (long)semin;
   
    // 返回
    return regCoolConfValue;
}


/**
 * 设置电机基础电流
 *
 * 经过实践测试，运行电流有个最低限额，比如 irun=10%，最低限额是 25%，那么仍旧以 25% 的电流运行（手册也提到最高节能75%左右）
 * 　　　　　　　我们此处设置的 irun 参数是 CoolStep功能 最高值（即电机负载变大时，电流上升的上限就是我们设置的 irun 值，CoolStep功能的电流最低值约是 75%左右）
 *
 * param u8 irun   电机全速运行是电流百分比：0 ~ 100，值越大电流越大；推荐 50 ~ 100
 * param u8 ihold  静止状态下电机电流百分比：0 ~ 100，值越大电流越大
 */
void MOTOR_TMCUART_SetIRunIHold( u8 irun, u8 ihold )
{
    long regval = MOTOR_TMCUART_BuildIRunIHold(irun, ihold);
    MOTOR_TMCUART_WriteReg(REGADDR_IHOLD_IRUN, regval);
}


/**
 * 构建寄存器值：电机基础电流
 * 基于 IHOLD_IRUN 寄存器
 *
 * param u8 irun   电机全速运行是电流百分比：0 ~ 100，值越大电流越大；推荐 50 ~ 100
 * param u8 ihold  静止状态下电机电流百分比：0 ~ 100，值越大电流越大
 */
long MOTOR_TMCUART_BuildIRunIHold( u8 irun, u8 ihold )
{
    // 初始化
    u8 iholdDelay;
    long regIHoldIRun = 0;

    // 设置静止状态下电机电流
    ihold = (u8)((long)ihold*31/100);
    ihold = ihold & 0x1F;  // 去掉高3位数据
    regIHoldIRun |= (long)ihold;

    // 设置电机全速运行是电流
    irun = (u8)((long)irun*31/100);
    irun = irun & 0x1F;  // 去掉高3位数据
    regIHoldIRun |= (long)irun << 8;

    // 电机从运动到静止那一刻时电流下降模式
    // 主要作用是避免电机没有真正停下来时电流变化造成的电机跳动
    // 0 表示瞬间降电流，1~15 表示 每 2^18 个时钟倍数的时间衰减电流
    iholdDelay = 0x00;
    regIHoldIRun |= (long)iholdDelay << 16;

    // 返回
    return regIHoldIRun;
}


/**
 * 获取电机静止标记
 * 注意：待电机停转至少 160ms 后才能获取到真实的电机静止标记值
 * 手册说明：DRV_STATUS.stst 标志表示电机在每个操作模式下保持静止。这发生在最后一步脉冲之后的 2^20 时钟
 *
 * param long  regDrvStatus  源DRV_STATUS寄存器值，-1 时表示自动获取
 * param short getDelay      延时多少秒获取标记
 */
long MOTOR_TMCUART_GetStopFlag( long regDrvStatus, short getDelay )
{
    // 延时
    if( getDelay ) delay_ms(getDelay);
    
    // 自动获取源 DRV_STATUS 寄存器值
    regDrvStatus = regDrvStatus == -1 ? MOTOR_TMCUART_ReadReg(REGADDR_DRV_STATUS) : regDrvStatus;
    regDrvStatus = regDrvStatus == -1 ? 0 : regDrvStatus;

    // 返回
    // & 0x01 主要作用就是当 regDrvStatus 为负数时且数据右移动，那么产生的数据空位填充会自动填充 0xF，使用时容易出问题所以需要将数据空位填充为 0
    return (regDrvStatus >> 31) & 0x01; 
}


/**
 * 获取电机反转标记
 *
 * param long regGConfValue  源GCONF寄存器值，-1 时表示自动获取
 */
long MOTOR_TMCUART_GetReverseFlag( long regGConfValue )
{
    // 自动获取源 GCONF 寄存器值
    regGConfValue = regGConfValue == -1 ? MOTOR_TMCUART_ReadReg(REGADDR_GCONF) : regGConfValue;
    regGConfValue = regGConfValue == -1 ? 0 : regGConfValue;

    // 返回
    // & 0x01 主要作用就是当 regGConfValue 为负数时且数据右移动，那么产生的数据空位填充会自动填充 0xF，使用时容易出问题所以需要将数据空位填充为 0
    return (regGConfValue >> 3) & 0x01; 
}


/**
 * 获取电机运行模式
 *
 * param long regDrvStatus  源DRV_STATUS寄存器值，-1 时表示自动获取
 * return long  0 表示 SpreadCycle模式，1 表示 StealthChop 模式
 */
long MOTOR_TMCUART_GetRunModeFlag( long regDrvStatus )
{
    // 自动获取源 DRV_STATUS 寄存器值
    regDrvStatus = regDrvStatus == -1 ? MOTOR_TMCUART_ReadReg(REGADDR_DRV_STATUS) : regDrvStatus;
    regDrvStatus = regDrvStatus == -1 ? 0 : regDrvStatus;

    // 返回
    // & 0x01 主要作用就是当 regDrvStatus 为负数时且数据右移动，那么产生的数据空位填充会自动填充 0xF，使用时容易出问题所以需要将数据空位填充为 0
    return (regDrvStatus >> 30) & 0x01; 
}


/**
 * 计算并设置 CRC 值
 * 参数 *data 的数组最后位字节是 CRC 预留位，该函数计算出 CRC 值后会赋值到最后位字节，同时返回 CRC 值
 *
 * 对于消息 0x05 0x00 0x00，正确的 CRC 是 0x48，测试代码：MOTOR_TMCUART_CalcCRC([0x05,0x00,0x00,0x00], 0, 4);
 * 对于消息 0x05 0x00 0x06，正确的 CRC 是 0x6F，测试代码：MOTOR_TMCUART_CalcCRC([0x05,0x00,0x06,0x00], 0, 4);
 *
 * @param u8* data        数据数组指针（数组中最后一位必须预留，因为这个数据也参与 CRC 计算，预留值为 0）
 * @param u8  dataStart   参与计算的数据起始（下标从 0 开始）
 * @param u8  dataLength  参与计算的数据长度（包括最后一位预留字节）
 *
 * @return u8  CRC 值
 */
u8 MOTOR_TMCUART_CalcCRC( u8 *data, u8 dataStart, u8 dataLength )
{
    // 初始化
    int i, j;
    u8 *crc = data + (dataStart + dataLength - 1);   // 定位到数据数组中最后一个字节（指针）
    u8 currentByte;
    
    // 初始预留位
    *crc = 0;

    // 遍历全部数组字节
    for( i = 0; i < (dataLength - 1); i++ ){
        currentByte = data[dataStart+i];
      
        for( j = 0; j < 8; j++ ){
            if( (*crc >> 7) ^ (currentByte & 0x01) ){
                *crc = (*crc << 1) ^ 0x07;
            }else{
                *crc = (*crc << 1);
            }
            
            currentByte = currentByte >> 1;
        }
    }
    
    // 返回 CRC 值
    return data[dataStart+dataLength-1];
}



/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */

/**
 * 主逻辑：初始化
 */
void MOTOR_TMCUART_MainInit()
{
    MOTOR_TMCUART_Init();
}


/**
 * 主逻辑：电机运行2秒往返
 * 该函数内部会自动启动电机的运行
 */
void MOTOR_TMCUART_MainRoundTrip()
{
    // 初始化
    static u8 numloop = 0;
    
    // 重置索引
    if( numloop > 10 ) numloop = 1;

    
    /* 采用发送 正负速度值 进行 电机正反转 */
    if( numloop++ % 2 == 0 ){
        MOTOR_TMCUART_WriteReg(REGADDR_VACTUAL, 1000); 
    }else{
        MOTOR_TMCUART_WriteReg(REGADDR_VACTUAL, -1000);
    }


    /* 采用发送 反转寄存值 进行 机正反转 */
    /*
    if( numloop == 0 ){
        MOTOR_TMCUART_WriteReg(REGADDR_VACTUAL, 500);
    }

    if( numloop++ % 2 == 0 ){
        MOTOR_TMCUART_SetReverse();
    }
    */

    
    // 延时
    delay_ms(2000);
}


/**
 * 主逻辑：电机堵转后反转
 * 实时监控 SG_RESULT 值，发现堵转后反转
 *【未解决问题】由于该代码应用在 main.c 的 while 函数内，那么可能存在监控 SG_RESULT 值并非实时（可能间隔 XX 毫秒才执行一次），
 *　　　　　　　那么就有可能电机堵转也持续 XX 毫秒，实践中发现堵转且电流上升到最大后，电机运行就异常（异常现象看上去是CoolStep功能失效了）
 *　　　　　　　# 上述问题仅仅出现在启用了智能电流CoolStep功能的项目中，同时运行电流设置在60%以上
 *
 * param long  stallSGResult  电机反转的负载阈值  
 */
void MOTOR_TMCUART_MainStallReverse( long stallSGResult )
{
    // 未达到反转的堵转值
    if( MOTOR_TMCUART_ReadReg(REGADDR_SG_RESULT) > stallSGResult ) return;
    
    // 电机反转
    MOTOR_TMCUART_SetReverse();

    // 延时，等待电机反转后持续运行 200ms 毫秒后再去检测 SG_RESULT 值
    // 主要作用是担心电机反转瞬间 SG_RESULT 不稳定，导致逻辑判断出问题
    delay_ms(200);  
}


/**
 * 主逻辑：循环执行全部细分值
 * 该函数内部会自动启动电机的运行
 */
void MOTOR_TMCUART_MainMircoStepLoop()
{
    // 初始化
    short i;
    long regval, regvalChopconf;
    static u8 initFlag = 0;
    
    // 初次激活
    if( initFlag++ == 0 ){
        // 激活细分寄存器
        regval = MOTOR_TMCUART_ReadReg(REGADDR_GCONF);
        regval = MOTOR_TMCUART_BuildMircoStepRegSelect(regval, 1);
        MOTOR_TMCUART_WriteReg(REGADDR_GCONF, regval);
        
        // 开始运行电机
        MOTOR_TMCUART_WriteReg(REGADDR_VACTUAL,  1000);
    }

    // 获取 CHOPCONF 寄存器值
    regvalChopconf = MOTOR_TMCUART_ReadReg(REGADDR_CHOPCONF);
    
    // 循环执行细分值  
    for( i = 1; i <= 256; i=i*2 ){
        regvalChopconf = MOTOR_TMCUART_BuildMircoStep(regvalChopconf, i);
        MOTOR_TMCUART_WriteReg(REGADDR_CHOPCONF, regvalChopconf); 

        delay_ms(4000);
    }
}


/**
 * 主逻辑：电机持续运行
 *
 * param short mstep  细分值：1, 2, 4, 8, 16, 32, 64, 128, 256；0 表示不设置细分
 * param long  speed  速度值
 */
void MOTOR_TMCUART_MainRunKeep( short mstep, long speed )
{
    if( mstep ) MOTOR_TMCUART_SetMircoStep(mstep);
    MOTOR_TMCUART_WriteReg(REGADDR_VACTUAL, speed);
}


/**
 * 主逻辑：监视 SG_RESULT 值
 * 电机初始静止时 SG_RESULT 值为 0~10
 * 电机运行后静止 SG_RESULT 值为 0~10（需要静止一小段毫秒时间后）
 *
 * 通过 LCD 显示 SG_RESULT 值，使用前需要：
 * 　　1.初始化 LCD    ：LCD_MainInit()
 * 　　2.开启屏幕的SPI ：LCD_MSP3520_SpiOpenScreen()
 * 　　3.电机运行起来  ：MOTOR_TMCUART_MainRunKeep()
 */
void MOTOR_TMCUART_MainMonitorSGResult()
{
    // 初始化
    long regval, line = 0;
    static long max = 0, min = 0x0FFFFFFF;

    // 显示 SG_RESULT 实时值
    regval = MOTOR_TMCUART_ReadReg(REGADDR_SG_RESULT);
    LCD_MSP3520_ShowInteger(45, 5+15*(line++), regval, 12, LCD_COLOR_BLACK, LCD_COLOR_WHITE, true, 3);

    // 显示 SG_RESULT 最小值
    min = min > regval ? regval : min;
    LCD_MSP3520_ShowString(20, 5+15*line, "MIN:", 12, LCD_COLOR_RED, LCD_COLOR_WHITE, false);
    LCD_MSP3520_ShowInteger(45, 5+15*(line++), min, 12, LCD_COLOR_RED, LCD_COLOR_WHITE, true, 3);
    
    // 显示 SG_RESULT 最大值
    max = regval > max ? regval : max;
    LCD_MSP3520_ShowString(20, 5+15*line, "MAX:", 12, LCD_COLOR_BLUE, LCD_COLOR_WHITE, false);
    LCD_MSP3520_ShowInteger(45, 5+15*(line++), max, 12, LCD_COLOR_BLUE, LCD_COLOR_WHITE, true, 3);
}