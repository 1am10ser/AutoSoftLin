#include "common.h"



/* -------------------------------------------------- */
// ● 采用 ZDT UART 方式驱动（Emm_V5.0）
// ● 使用前务必先对 UART3 进行初始化：UART3_CInit();
// ● 之所以不使用 UART1，是因为 UART1 需要通过 USB 和电脑通讯，如果电机也采用 UART1 通讯，那么就会因为 CH340G 芯片导致无法收到电机驱动板的反馈信息
//
// ● motor_zdtuart.c 和 motor.c 是完全独立两套逻辑，没有依赖关系，各自使用各自的函数库
// ● 当然 motor_zdtuart.c 和 motor.c 也可以混用，ZDT 支持 UART 和 STEP/DIR 混用，比如利用 UART 设置细分等参数，STEP/DIR 控制电机运行
//
// ● 该 ZDT UART 驱动库基于 0xFA78 版本的驱动板编写，获取驱动板可使用函数：MOTOR_ZDTUART_GetVersion()
// ● 版本号中，高8位表示软件版本：0xF4（Emm42_V5.0.0）、0xF7（Emm42_V5.0.3）、0xFA（Emm_V5.0.6）
// ● 版本号中，低8位表示硬件版本：0x78（ZDT_X42_V1.2）
//
// ● 注意：电机驱动板务必烧录 0xFA（Emm_V5.0.6）程序，否则串口可能出现BUG
// ● 注意：对于一些配置写入到驱动板的API接口，如果不进行延迟等待，那么会导致第二次发送串口命令执行失败且无反馈数据
//         原因是写入配置到驱动板时，驱动板是利用定时器去刷新数据->关闭中断->修改配置值->存储到Flash->然后再初始化，所以需要一定的时间。而这一段时间内发送的新串口命令时无法执行的
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * MOTOR 相关变量
 */
short GV_MotorZdtDir   = 0;   //【系统使用】电机方向
short GV_MotorZdtDiv   = 0;   //【系统使用】电机细分
short GV_MotorZdtSpeed = 15;  //【系统使用】电机速度（RPM,转/每分钟）



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 初始化函数
 *
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_Init()
{
    // 电机启用
    if( MOTOR_ZDTUART_SetEnable() ) return 1;
    
    // 设置细分：256细分
    if( MOTOR_ZDTUART_SetDiv(256,true) ) return 2;
    
    // 设置方向
    if( MOTOR_ZDTUART_SetDir(MOTOR_ZDTUART_DIR_CW) ) return 3;
    
    // 设置速度
    if( MOTOR_ZDTUART_SetSpeed(15) ) return 4;

    // 返回
    return 0;
}


/**
 * 设置电机细分
 *
 * 由于电机驱动板内置补偿细分，所以在 UART 模式下设置不同的细分值实际运行效果相差不大，但是还是建议设置大一些的细分，例如：64、128、256；
 * 因为电机转动角度所需的脉冲总数是根据细分计算而出的，脉冲总数变量是一个整形，过低的细分会导致计算脉冲总数时四舍五入，导致转动角度会有些误差大
 * 
 * 如果采用 UART+STEP/DIR 组合方式去控制电机，那么可以根据实际运行情况设置细分值
 *
 * @param short dir    细分: 0~256
 * @param bool  delay  延时返回（配置完全写入到驱动板需要一定的时间等待，否则接着发送的串口命令将无法执行）
 *
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_SetDiv( short div, bool delay )
{
    // 初始化
    u8 sendData[6] = {0x01, 0x84, 0x8A, 0x00, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0x84, 0x02, 0x6B}, *receiveData;

    // 构建发送命令
    // 对于 STEP/DIR 模式控制的需要将细分值保存到驱动板，所以此处默认保存
    sendData[3] = 0x01;  // 是否存储：细分值是否直接存储到驱动板
    sendData[4] = div >= 256 ? 0 : div;

    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 6); delay_ms(2);

    // 返回执行结果
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }

    // 设置细分值
    GV_MotorZdtDiv = div; 
    
    // 重置接收数据
    UART3_ResetReceiveData(); 

    // 延时等待配置完全写入
    // 为了简化 API 函数的编码工作，所以就将延迟函数就直接写到了这个函数主体内，调用该函数时需要注意该这个函数会阻塞一定时间
    // 原因：写入配置到驱动板时，驱动板是利用定时器去刷新数据->关闭中断->修改配置值->存储到Flash->然后再初始化，所以需要一定的时间。而这一段时间内发送的新串口命令时无法执行的
    // 
    // 经过 Emm_V5.0.6 版本的单函数循环执行反馈测试；延迟 80ms 可以正常数据接收，其他使用场景下如遇异常，可以根据实际情况加大延迟时间
    // 场景1：MOTOR_ZDTUART_SetDiv() + MOTOR_ZDTUART_SetZeroPoint() 偶尔会导致 MOTOR_ZDTUART_SetZeroPoint() 失效，建议延时加大到 150ms
    if( delay == true ) delay_ms(150);
    
    // 返回成功
    return 0;
}


/**
 * 获取电机细分
 *
 * @return  细分值: 0~255, 0 表示 256细分
 */
short MOTOR_ZDTUART_GetDiv()
{
    return GV_MotorZdtDiv;
}


/**
 * 设置电机方向
 * 电机运行方向无法直接配置到电驱板内部，而是随发送运动命令一起发送
 *
 * @param short dir  方向：MOTOR_ZDTUART_DIR_CW、MOTOR_ZDTUART_DIR_CCW
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_SetDir( short dir )
{
    // 设置方向
    GV_MotorZdtDir = dir;
    
    // 返回
    return 0;
}


/**
 * 获取电机方向
 *
 * @return  方向值：MOTOR_ZDTUART_DIR_CW、MOTOR_ZDTUART_DIR_CCW
 */
short MOTOR_ZDTUART_GetDir()
{
    return GV_MotorZdtDir;
}


/**
 * 设置电机速度
 *
 * @param short speed  速度，单位：RPM（转/每分钟）
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_SetSpeed( short speed )
{
    // 设置速度
    GV_MotorZdtSpeed = speed;
    
    // 返回
    return 0;
}


/**
 * 获取电机速度
 *
 * @return  速度值，单位：RPM（转/每分钟）
 */
short MOTOR_ZDTUART_GetSpeed()
{
    return GV_MotorZdtSpeed;
}


/**
 * 获取电机实时角度（阻塞式）
 * 电机驱动板内的角度是一个累加的并且有正负的，
 * 该函数会对这个累加角度值重构，重构方式：基于圆形刻度顺时针增大的绝对角度值（参见PAT工装刻度盘）
 *
 * @return double  角度值，> 0 表示有效的角度值，-1 表示无效值
 */
double MOTOR_ZDTUART_GetDegree360()
{
    // 初始化
    u8 sendData[3] = {0x01, 0x36, 0x6B}, *receiveData;
    u32 degnum = 0; double degree = 0;

    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 3); delay_ms(4);

    // 检查执行结果
    if( UART3_GetReceiveDataCount() < 8 ){ UART3_ResetReceiveData(); return -1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 2; i++ ) if( sendData[i] != receiveData[i] || receiveData[7] != 0x6B ){ UART3_ResetReceiveData(); return -1; }

    // 构建角度
    degnum = degnum | receiveData[3]; degnum = degnum << 8;
    degnum = degnum | receiveData[4]; degnum = degnum << 8;
    degnum = degnum | receiveData[5]; degnum = degnum << 8;
    degnum = degnum | receiveData[6];

    degree = (double)degnum;
    degree = 360*(degree/65535);
    degree = degree*(receiveData[2]==1?-1:1);
    
    while( degree >  360 ) degree -= 360;
    while( degree < -360 ) degree += 360;
        
    // 重置接收数据
    UART3_ResetReceiveData(); 

    // 返回执行结果
    return degree > 0 ? degree : 360-degree;
}


/**
 * 获取就近运行角度
 * 该函数的两个角度参数要求：基于圆形刻度顺时针增大的绝对角度值（参见PAT工装刻度盘）
 *
 * @param double  currDegree  当前角度
 * @param double  distDegree  目标角度
 *
 * @return  > 0 表示顺时需要针运行的角度，< 0 表示逆时针需要运行的角度
 */
double MOTOR_ZDTUART_GetRunDegree360Near( double currDegree, double distDegree )
{
    // 重构参数
    while( currDegree >  360 ) currDegree -= 360;
    while( currDegree < -360 ) currDegree += 360;
    currDegree = currDegree > 0 ? currDegree : 360-currDegree;
    
    while( distDegree >  360 ) distDegree -= 360;
    while( distDegree < -360 ) distDegree += 360;
    distDegree = distDegree > 0 ? distDegree : 360-distDegree;
    
    // 目标角度 > 当前角度
    // distDegree=90    currDegree=40   返回  50（顺时针）
    // distDegree=350   currDegree=40   返回 -50（逆时针）
    if( distDegree > currDegree ){
       if( (distDegree-currDegree) <= (currDegree+360-distDegree) ) return distDegree-currDegree;
       else return -(currDegree+360-distDegree);
    }
     
    // 目标角度 < 当前角度
    // distDegree=10   currDegree=40    返回  -30（逆时针）
    // distDegree=30   currDegree=40    返回  -10（逆时针）
    // distDegree=10   currDegree=200   返回  170（顺时针）
    // distDegree=30   currDegree=200   返回 -170（逆时针）
    if( distDegree < currDegree ){
       if( (currDegree-distDegree) < (distDegree+360-currDegree) ) return -(currDegree-distDegree);
       else return distDegree+360-currDegree;
    }
    
    // 不执行
    return 0;
}


/**
 * 获取驱动板版本
 * 
 * return long -1 表示获取异常
 *             【低16位中】高8位表示软件版本：0xF4（Emm42_V5.0.0）、0xF7（Emm42_V5.0.3）
 *             【低16位中】低8位表示硬件版本：0x78（ZDT_X42_V1.2）
 */
long MOTOR_ZDTUART_GetVersion()
{
    // 初始化
    u8  sendData[3] = {0x01, 0x1F, 0x6B}, *receiveData;
    u16 softVersion, hardwareVersion;
        
    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 3); delay_ms(2);

    // 检查执行结果
    if( UART3_GetReceiveDataCount() < 5 ){ UART3_ResetReceiveData(); return -1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 2; i++ ) if( sendData[i] != receiveData[i] || receiveData[4] != 0x6B ){ UART3_ResetReceiveData(); return -1; }

    // 构建版本号
    softVersion = receiveData[2];
    hardwareVersion = receiveData[3];

    // 返回驱动板版本
    UART3_ResetReceiveData(); 
    return softVersion<<8|hardwareVersion;
}


/**
 * 设置电机使能
 *
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_SetEnable()
{
    // 初始化
    u8 sendData[6] = {0x01, 0xF3, 0xAB, 0x01, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0xF3, 0x02, 0x6B}, *receiveData;
        
    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 6); delay_ms(2);

    // 返回执行结果
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    UART3_ResetReceiveData(); 

    return 0;
}


/**
 * 设置电机禁用
 *
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_SetDisable()
{
    // 初始化
    u8 sendData[6] = {0x01, 0xF3, 0xAB, 0x00, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0xF3, 0x02, 0x6B}, *receiveData;
        
    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 6); delay_ms(2);

    // 返回执行结果
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    UART3_ResetReceiveData(); return 0;
}


/**
 * 获取电机状态集
 * 返回值包括：电机使能状态、电机到位标志、电机堵转标志、电机堵转保护标志
 *
 *  @return 状态集：-1 表示获取异常，正数表示状态集值
 */
short MOTOR_ZDTUART_GetStates()
{
    // 初始化
    u8 sendData[3] = {0x01, 0x3A, 0x6B}, *receiveData;
    short states;
    
    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 3); delay_ms(2);

    // 检查执行结果
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return -1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 2; i++ ) if( sendData[i] != receiveData[i] || receiveData[3] != 0x6B ){ UART3_ResetReceiveData(); return -1; }
    
    // 构建状态集
    states = receiveData[2];

    // 重置接收数据
    UART3_ResetReceiveData(); 
    
    // 返回状态集
    return states;
}


/**
 * 按角度值运行电机
 *
 * @param double degree  角度，单位：度
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_RunDegree( double degree )
{
    // 初始化
    u8 sendData[13] = {0x01, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0xFD, 0x02, 0x6B}, *receiveData;

    // 构建发送的脉冲个数
    u32 sendPulseTotal = (u32)(degree/(MOTOR_ZDTUART_STEP_DEGREE/GV_MotorZdtDiv));

    // 构建发送命令
    sendData[2]  = GV_MotorZdtDir;         // 电机旋转方向
    sendData[3]  = GV_MotorZdtSpeed >> 8;  // 电机速度 RPM（高位）
    sendData[4]  = GV_MotorZdtSpeed;       // 电机速度 RPM（低位）
    sendData[5]  = 0;                      // 电机加速度
    sendData[6]  = sendPulseTotal >> 24;   // 发送脉冲数
    sendData[7]  = sendPulseTotal >> 16;   // 发送脉冲数
    sendData[8]  = sendPulseTotal >> 8;    // 发送脉冲数
    sendData[9]  = sendPulseTotal;         // 发送脉冲数
    sendData[10] = 0;                      // 电机旋转角度模式：0 表示相对角度，1表示绝对角度
    sendData[11] = 0;                      // 电机多机同步模式：0 表示关闭，1表示启用

    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 13); delay_ms(2);
    
    // 返回执行结果
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    UART3_ResetReceiveData(); return 0;
}


/**
 * 电机运行完成状态
 * 电机运行完成的标志是依据电机状态中 "到位标志位" 进行判断
 * 在电机完成运行后，最好延迟个 10ms 后再去执行下一次运行，否则可能出现电机抽风并停止
 * 猜测原因：当电机完成运行后，如果马上去执行下一次的运行，那么很可能由于驱动板来不及反应而从被认定为丢步，那么驱动板强制恢复丢步而出现抽风
 *
 * @param u8 states  电机状态集，通过 MOTOR_ZDTUART_GetStates() 获取
 * @return 运行状态：-1 表示获取异常，0 表示运行未完成，1表示运行完成
 */
short MOTOR_ZDTUART_RunFinish( short states )
{
    return states < 0 ? states : states & 0x02;
}


/**
 * 电机回零
 * 执行失败可能原因：触发了堵转保护、电机没使能、单圈回零的零点位置值无效
 *
 * @param short mode  回零模式：0x00 表示就近回零，0x01 表示单圈方向回零
 *                    单圈方向回零的方向需要在 MOTOR_ZDTUART_SetZeroConfig() 中配置，默认顺时针
 *
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_Zero( short mode )
{
    // 初始化
    u8 sendData[5] = {0x01, 0x9A, 0x00, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0x9A, 0x02, 0x6B}, *receiveData;

    // 构建发送命令
    sendData[2] = mode;   // 回零模式：0x00 表示触发单圈就近回零
                          // 　　　　　0x01 表示触发单圈方向回零
                          // 　　　　　0x02 表示触发多圈无限位碰撞回零
                          // 　　　　　0x03 表示触发多圈有限位开关回零
    
    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 5); delay_ms(2);
    
    // 检查执行结果
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    UART3_ResetReceiveData(); return 0;
}


/**
 * 设置电机零点
 *
 * @param bool delay  延时返回（配置完全写入到驱动板需要一定的时间等待，否则接着发送的串口命令将无法执行）
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_SetZeroPoint( bool delay )
{
    // 初始化
    u8 sendData[5] = {0x01, 0x93, 0x88, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0x93, 0x02, 0x6B}, *receiveData;

    // 构建发送命令
    sendData[3] = 0x00;  // 是否存储：零点位置是否直接存储到驱动板

    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 5); delay_ms(2);
    
    // 检查执行结果
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    
    // 重置接收数据
    UART3_ResetReceiveData(); 
    
    // 延时等待配置完全写入
    // 为了简化 API 函数的编码工作，所以就将延迟函数就直接写到了这个函数主体内，调用该函数时需要注意该这个函数会阻塞一定时间
    // 原因：写入配置到驱动板时，驱动板是利用定时器去刷新数据->关闭中断->修改配置值->存储到Flash->然后再初始化，所以需要一定的时间。而这一段时间内发送的新串口命令时无法执行的
    // 经过 Emm_V5.0.6 版本的单函数循环执行反馈测试；延迟 80ms 可以正常数据接收，其他使用场景下如遇异常，可以根据实际情况加大延迟时间
    if( delay ) delay_ms(80); 
    
    // 返回执行结果
    return 0;
}


/**
 * 获取电机回零配置
 * 目前无法返回配置数据，仅仅返回执行成功或失败，暂时需要通过 断点 查看
 *
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_GetZeroConfig()
{
    // 初始化
    u8 sendData[5] = {0x01, 0x22, 0x6B}, *receiveData;

    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 3); delay_ms(4);
    
    // 检查执行结果
    if( UART3_GetReceiveDataCount() < 18 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 2; i++ ) if( sendData[i] != receiveData[i] || receiveData[17] != 0x6B ){ UART3_ResetReceiveData(); return 2; }
    
    // 重置接收数据
    UART3_ResetReceiveData(); 

    // 返回执行结果
    return 0;
}


/**
 * 设置电机回零配置
 *
 * @param bool delay  延时返回（配置完全写入到驱动板需要一定的时间等待，否则接着发送的串口命令将无法执行）
 * @return 错误码：0 表示执行成功，非0表示执行失败
 */
short MOTOR_ZDTUART_SetZeroConfig( bool delay )
{
    // 初始化
    u8 sendData[20] = {0x01, 0x4C, 0xAE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6B};
    u8 successData[4] = {0x01, 0x4C, 0x02, 0x6B}, *receiveData;

    // 构建发送命令
    sendData[3]  = 0x00;  // 是否存储：回零配置是否直接存储到驱动板
    sendData[4]  = 0x00;  // 回零模式：0x00 表示 单圈就近回零
    sendData[5]  = 0x00;  // 回零方向：0x00 表示 顺时针(CW)

    sendData[6]  = 0x00;  // 回零速度（高位）
    sendData[7]  = 0x1E;  // 回零速度（低位）；0x001E = 30（RPM）

    sendData[8]  = 0x00;  // 回零超时时间（高位）
    sendData[9]  = 0x00;  // 回零超时时间（中位）
    sendData[10] = 0x27;  // 回零超时时间（中位）
    sendData[11] = 0x10;  // 回零超时时间（低位）：0x00002710 = 10000（ms）

    sendData[12] = 0x01;  // 无限位碰撞回零检测转速（高位）
    sendData[13] = 0x2C;  // 无限位碰撞回零检测转速（低位）；0x012C = 300（RPM）

    sendData[14] = 0x03;  // 无限位碰撞回零检测电流（高位）
    sendData[15] = 0x20;  // 无限位碰撞回零检测电流（低位）；0x012C = 800（mA）

    sendData[16] = 0x00;  // 无限位碰撞回零检测时间（高位）
    sendData[17] = 0x3C;  // 无限位碰撞回零检测时间（低位）；0x003C = 60（ms）

    sendData[18] = 0x00;  // 上电自动触发回零：0x00 表示关闭，0x01 表示开启
    
    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 20); delay_ms(4);

    // 返回执行结果
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return 1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 4; i++ ) if( successData[i] != receiveData[i] ){ UART3_ResetReceiveData(); return 2; }
    
    // 重置接收数据
    UART3_ResetReceiveData(); 
        
    // 延时等待配置完全写入
    // 为了简化 API 函数的编码工作，所以就将延迟函数就直接写到了这个函数主体内，调用该函数时需要注意该这个函数会阻塞一定时间
    // 原因：写入配置到驱动板时，驱动板是利用定时器去刷新数据->关闭中断->修改配置值->存储到Flash->然后再初始化，所以需要一定的时间。而这一段时间内发送的新串口命令时无法执行的
    // 经过 Emm_V5.0.6 版本的单函数循环执行反馈测试；延迟 80ms 可以正常数据接收，其他使用场景下如遇异常，可以根据实际情况加大延迟时间
    if( delay ) delay_ms(80); 
    
    // 返回执行结果
    return 0;
}


/**
 * 电机回零完成状态
 *
 * @return  1 表示回零完成，0 表示回零未完成，-1 表示获取异常
 */
short MOTOR_ZDTUART_ZeroFinish()
{
    // 初始化
    u8 sendData[5] = {0x01, 0x3B, 0x6B}, *receiveData;
    short finish = 0;

    // 发送命令并延迟等待执行结果
    // 延迟等待原因：发送命令后电机驱动板反馈数据需要时间
    UART3_SendBytes(sendData, 3); delay_ms(2);

    // 检查执行结果
    if( UART3_GetReceiveDataCount() < 4 ){ UART3_ResetReceiveData(); return -1; }
    receiveData = UART3_GetReceiveData(); for( u8 i=0; i < 2; i++ ) if( sendData[i] != receiveData[i]  || receiveData[3] != 0x6B ){ UART3_ResetReceiveData(); return -1; }

    // 检测回零状态
    if( (receiveData[2]&0x04) == 0x00 ) finish = 1;

    // 重置接收数据
    UART3_ResetReceiveData(); 

    // 返回完成状态
    return finish;
}



/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */


/**
 * 主逻辑：电机运行指定角度并回零
 */
void MOTOR_ZDTUART_MainRunAndZero()
{
    // 电机运行（按角度）
    MOTOR_ZDTUART_RunDegree(170);
    while( MOTOR_ZDTUART_RunFinish(MOTOR_ZDTUART_GetStates()) == 0 ); 

    // 延时 1 秒回零
    delay_ms(1000);

    // 电机回零
    MOTOR_ZDTUART_Zero(MOTOR_ZDTUART_ZERO_NEAR); while( MOTOR_ZDTUART_ZeroFinish() == 0 );

    // 延时 1 秒返回
    delay_ms(1000);
}


/**
 * 主逻辑：电机运行指定角度并回零（结合脉冲模式）
 */
void MOTOR_ZDTUART_MainRunAndZeroWithPulse()
{
    // 电机运行（按角度）
    MOTOR_RunDegree(170);
    while( MOTOR_GetState() != MOTOR_STATE_IDLE );

    // 延时 1 秒回零
    delay_ms(2000);

    // 电机回零
    MOTOR_ZDTUART_Zero(MOTOR_ZDTUART_ZERO_NEAR); while( MOTOR_ZDTUART_ZeroFinish() == 0 );

    // 延时 1 秒返回
    delay_ms(2000);
    
}


/**
 * 主逻辑：电机两点往返运行
 * 
 * @param short degree  往返角度，单位：度
 */
void MOTOR_ZDTUART_MainTwoPointRunByDegree( short degree )
{
    MOTOR_ZDTUART_SetDir(MOTOR_ZDTUART_DIR_CW);
    MOTOR_ZDTUART_RunDegree(degree); 
    while( MOTOR_ZDTUART_RunFinish(MOTOR_ZDTUART_GetStates()) == 0 );
    delay_ms(1000);

    MOTOR_ZDTUART_SetDir(MOTOR_ZDTUART_DIR_CCW);
    MOTOR_ZDTUART_RunDegree(degree); while( MOTOR_ZDTUART_RunFinish(MOTOR_ZDTUART_GetStates()) == 0 );
    delay_ms(1000);
}
