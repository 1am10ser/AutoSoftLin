#include "common.h"



/* -------------------------------------------------- */
// ● 外部变量
/* -------------------------------------------------- */

/**
 * LIN 外部变量
 * 以下变量定义在 lin.c, lin.m.c, lin.s.c 文件中
 */
extern u32 GV_LinMode;
extern u32 GV_LinSlaveBaudRate;
extern u32 GV_LinMasterBaudRate;



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */
u8 GV_Uart3Lin = false;             // 是否启用 LIN
u8 GV_Uart3ReceiveData[40];         // 数据接收容器
u8 GV_Uart3ReceiveDataCount  = 0;   // 数据接收实际长度
u8 GV_Uart3ReceiveDataLength = 40;  // 数据接收容器长度



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * UART3 初始化
 */
void UART3_CInit()
{
    if( GV_Uart3Lin == false ) UART3_CInitUart();
    if( GV_Uart3Lin == true && GV_LinMode == LIN_MODE_SLAVE  ) UART3_CInitLinSlave();
    if( GV_Uart3Lin == true && GV_LinMode == LIN_MODE_MASTER ) UART3_CInitLinMaster();
}


/**
 * UART3 串口初始化
 */
void UART3_CInitUart()
{
    // 配置串口参数为：波特率115200，8位数据位，1位停止位，无校验，允许接收发送
    UART3_Init((u32)115200, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);

    // 启用 UART3 中断
    // UART3_IT_RXNE_OR 表示 接收数据寄存器非空或者溢出错误时 触发中断
    UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
    
    // 启用 UART3
    UART3_Cmd(ENABLE);
}


/**
 * UART3 LIN 配置成从节点
 */
void UART3_CInitLinSlave()
{
    // 初始 UART3
    UART3_DeInit();
    UART3_Init((u32)GV_LinSlaveBaudRate, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);
    
    // 配置 UART3 LIN
    // UART3_LIN_AUTOSYNC_ENABLE 表示每次在 Synch Field 之后会进行波特率调整
    UART3_LINConfig(UART3_LIN_MODE_SLAVE, UART3_LIN_AUTOSYNC_ENABLE, UART3_LIN_DIVUP_NEXTRXNE);
    UART3_LINBreakDetectionConfig(UART3_LINBREAKDETECTIONLENGTH_11BITS);

    // 启用 UART3 读中断，启用 Lin Break 中断
    UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
    UART3_ITConfig(UART3_IT_LBDF, ENABLE);
    
    // 启用 UART3 LIN
    UART3_LINCmd(ENABLE);
}


/**
 * UART3 LIN 配置成主节点
 */
void UART3_CInitLinMaster()
{
    // 初始 UART3
    UART3_DeInit();
    UART3_Init((u32)GV_LinMasterBaudRate, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);
    
    // 配置 UART3 LIN
    // UART3_LIN_AUTOSYNC_ENABLE 表示每次在 Synch Field 之后会进行波特率调整
    UART3_LINConfig(UART3_LIN_MODE_MASTER, UART3_LIN_AUTOSYNC_DISABLE,  UART3_LIN_DIVUP_LBRR1);
    UART3_LINBreakDetectionConfig(UART3_LINBREAKDETECTIONLENGTH_11BITS);

    // 启用 UART3 读中断，关闭 Lin Break 中断
    UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
    UART3_ITConfig(UART3_IT_LBDF, DISABLE);
    
    // 启用 UART3 LIN
    UART3_LINCmd(ENABLE);
}


/**
 * UART3 发送字节
 *
 * @param byte  8位字节
 */
void UART3_SendByte( u8 byte )
{
    // 发送字节
    UART3_SendData8(byte);
    
    // 等待发送完成
    // RESET = 0; SET = 1; 
    // UART3_FLAG_TXE 表示 发送数据寄存器空标志（即表示UART3没有发送），
    // 如果其等于 1，那么表示 URAR1 发送完成或者闲置中；如果其等于 0，那么表示 UART3 正在发送
    while( UART3_GetFlagStatus(UART3_FLAG_TXE) == RESET );
}


/**
 * UART3 发送字节集
 *
 * @param bytes   8位字节集
 * @param length  字节集长度
 */
void UART3_SendBytes( u8 *bytes, short length )
{
    for( short i = 0; i < length; i++ ) UART3_SendByte(bytes[i]);
}


/**
 * 获取接收到的数据
 *
 * return u8*  数组指针
 */
u8* UART3_GetReceiveData()
{
    return GV_Uart3ReceiveData;
}


/**
 * 获取接收到的数据长度
 *
 * return u8
 */
u8 UART3_GetReceiveDataCount()
{
    return GV_Uart3ReceiveDataCount;
}


/**
 * 复制接收到的数据
 *
 * @param u8 *data  数组，该数组字节数量必须 >= GV_Uart3ReceiveDataLength
 * return u8 拷贝的数据个数
 */
u8 UART3_CopyReceiveData( u8 *data )
{
    // 复制数据
    for( u8 i = 0; i < GV_Uart3ReceiveDataCount; i++ ) data[i] = GV_Uart3ReceiveData[i];

    // 返回
    return GV_Uart3ReceiveDataCount;
}


/**
 * 重置接收到的数据
 */
void UART3_ResetReceiveData()
{
    // 重置变量
    GV_Uart3ReceiveDataCount = 0;
    for( u8 i = 0; i < GV_Uart3ReceiveDataLength; i++ ) GV_Uart3ReceiveData[i] = 0;
}



/* -------------------------------------------------- */
// ● 中断回调函数
// ● 主调用入库参见 stm8s.it.c
/* -------------------------------------------------- */

/**
 * UART3 接收中断回调处理
 */
void UART3_InterruptHandlerRX()
{
    if( GV_Uart3Lin == false ) UART3_InterruptHandlerRXUart();
    if( GV_Uart3Lin == true && GV_LinMode == LIN_MODE_SLAVE  ) UART3_InterruptHandlerRXTriggerLinSlave();
    if( GV_Uart3Lin == true && GV_LinMode == LIN_MODE_MASTER ) UART3_InterruptHandlerRXTriggerLinMaster();
}


/**
 * UART3 读中断回调处理 - 串口数据接收
 */
void UART3_InterruptHandlerRXUart()
{
    // 防止 OR 过载错误导致不停的进入读中断（OR 源自 UART3_IT_RXNE_OR）
    // STM8 内部机制：先读取 UART_SR，再读取 UART_DR 可把 OR 位中断状态清零
    // 当通过PC端的串口软件向 STM8 一次性发送多字节数据时（例如：01 02 03），那么 STM8 就会一直不停的进入读中断，陷入死循环
    if( UART3_GetITStatus(UART3_IT_RXNE) == RESET ) return;
    
    // 读取数据
    u8 byte = UART3_ReceiveData8();
    
    // 保存数据（如果过长则丢弃）
    if( GV_Uart3ReceiveDataCount < GV_Uart3ReceiveDataLength ) GV_Uart3ReceiveData[GV_Uart3ReceiveDataCount++] = byte;
}


/**
 * UART3 读中断回调处理 - LIN 从机接收到主机的请求数据
 */
void UART3_InterruptHandlerRXTriggerLinSlave()
{
    // LIN DATA 中断
    // LIN Break Detection Interrupt
    if( UART3_GetITStatus(UART3_IT_LBDF) != RESET ){
        // 启动业务
        LIN_SLAVE_ReceiveStart();
        LIN_SLAVE_ReceiveHandle(0x00);

        // 清除中断
        // 如果不清除该标记位那么系统就会一直进入 LIN DATA 中断逻辑
        UART3_ClearITPendingBit(UART3_IT_LBDF);
    }

    // LIN 同步段标志
    // 默认情况下 STM8S 的 UART3 RX 中断中无法读取到 LIN 的同步段 0x55 值
    // 但是如果清除了同步段标志，那么 UART3 RX 就可以正常读取到 LIN 的同步段 0x55 值
    // 
    // 例如：郭雄茂夹具（281-3）默认下发 80,80 FC,02 和 42
    // 　　　在清除同步段标志的情况下下发数据为：55,80,80 FC,02 和 55,42
    // 　　　不清除同步段标志的情况下下发数据为：80,80 FC,02 和 42
    // 总结：以上仅仅测试郭雄茂的夹具，具体还需要自己在实车上进行采集测试
    // 　　　在清除同步段标志的情况下，同步段 0x55 值能接收到（实车测试清除同步段标志后能收到0x55数据）
    if( UART3_GetFlagStatus(UART3_FLAG_LSF) != RESET ){
        UART3_ClearFlag(UART3_FLAG_LSF);
    }

    // LIN DATA 数据读取
    if( UART3_GetITStatus(UART3_IT_RXNE) != RESET ){
        LIN_SLAVE_ReceiveHandle(UART3_ReceiveData8());
        UART3_ClearITPendingBit(UART3_IT_RXNE);
    }
}


/**
 * UART3 读中断回调处理 - LIN 主机接收到从机的响应数据
 */
void UART3_InterruptHandlerRXTriggerLinMaster()
{
    // 初始化
    u8 byte = UART3_ReceiveData8();
    
    // 接收反馈的信号
    LIN_MASTER_InterruptHandlerRxFindReceive(byte);
    LIN_MASTER_InterruptHandlerRxPrimaryReceive(byte);
}


/**
 * UART3 发送中断回调处理
 */
void UART3_InterruptHandlerTX()
{
}
