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
// ● 函数主体
/* -------------------------------------------------- */

/**
 * UART3 初始化
 */
void UART3_CInit()
{
    // 配置 LIN 模式
    if( GV_LinMode == LIN_MODE_SLAVE  ) UART3_CInitLinSlave();
    if( GV_LinMode == LIN_MODE_MASTER ) UART3_CInitLinMaster();
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



/* -------------------------------------------------- */
// ● 中断回调函数
//    主调用入库参见 stm8s.it.c
/* -------------------------------------------------- */

/**
 * UART3 接收中断回调处理
 */
void UART3_InterruptHandlerRX()
{
    if( GV_LinMode == LIN_MODE_SLAVE  ) UART3_InterruptHandlerRXTriggerLinSlave();
    if( GV_LinMode == LIN_MODE_MASTER ) UART3_InterruptHandlerRXTriggerLinMaster();
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
    LIN_MASTER_ReceiveMainARecResponse(byte);
    LIN_MASTER_ReceiveFindARecResponse(byte);
}


/**
 * UART3 发送中断回调处理
 */
void UART3_InterruptHandlerTX()
{
}
