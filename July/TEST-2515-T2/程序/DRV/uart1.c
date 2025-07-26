#include "common.h"



/* -------------------------------------------------- */
// ● 外部变量
/* -------------------------------------------------- */

/**
 * LIN 外部变量
 * 以下变量定义在 lin.m.c 文件中
 */
extern u8 GV_LinMasterMainARecResponseToUsbDnHead;
extern u8 GV_LinMasterMainARecResponseToUsbDnFoot;



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */
u8 GV_Uart1ReceiveData[20];         // 数据接收容器
u8 GV_Uart1ReceiveDataCount  = 0;   // 数据接收实际长度
u8 GV_Uart1ReceiveDataLength = 20;  // 数据接收容器长度



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * UART1 初始化
 */
void UART1_CInit()
{
    // 配置串口参数为：波特率115200，8位数据位，1位停止位，无校验，禁止同步传输，允许接收发送
    UART1_Init((u32)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    
    // 启用 UART1 中断
    // UART1_IT_RXNE_OR 表示 接收数据寄存器非空或者溢出错误时 触发中断
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
    
    // 启用 UART1
    UART1_Cmd(ENABLE);
}


/**
 * UART1 发送字节
 *
 * @param byte  8位字节
 */
void UART1_SendByte( u8 byte )
{
    // 发送字节
    UART1_SendData8(byte);
    
    // 等待发送完成
    // RESET = 0; SET = !RESET; 
    // UART1_FLAG_TXE 表示 发送数据寄存器空标志（即表示UART1没有发送），
    // 如果其等于 1，那么表示 URAR1 发送完成或者闲置中；如果其等于 0，那么表示 UART1 正在发送
    while( UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET );
}


/**
 * UART1 发送字节集
 *
 * @param bytes   8位字节集
 * @param length  字节集长度
 */
void UART1_SendBytes( u8 *bytes, short length )
{
    for( short i = 0; i < length; i++ ) UART1_SendByte(bytes[i]);
}



/* -------------------------------------------------- */
// ● 中断回调函数
// ● 主调用入库参见 stm8s.it.c
/* -------------------------------------------------- */

/**
 * UART1 接收中断回调处理
 */
void UART1_InterruptHandlerRX()
{
    // 防止 OR 过载错误导致不停的进入读中断
    // STM8 内部机制：先读取 UART_SR，再读取 UART_DR 可把 OR 位中断状态清零
    // 当通过PC端的串口软件向 STM8 一次性发送多字节数据时（例如：01 02 03），那么 STM8 就会一直不停的进入读中断，陷入死循环
    if( UART1_GetITStatus(UART1_IT_RXNE) == RESET ) return;
    
    // 读取数据
    u8 byte = UART1_ReceiveData8();
    
    // 数据丢弃
    if( GV_Uart1ReceiveDataCount > GV_Uart1ReceiveDataLength ) return;
    
    // 接收数据并存储到全局变量
    GV_Uart1ReceiveData[GV_Uart1ReceiveDataCount++] = byte;
}


/**
 * UART1 发送中断回调处理
 */
void UART1_InterruptHandlerTX()
{
}



/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */

/**
 * UART1 主逻辑：接收数据并转发给 LIN
 */
void UART1_MainReceiveSendToLin()
{
    // 未读取到数据
    if( GV_Uart1ReceiveDataCount == 0 ) return;

    // 初始化
    u8 i, bytes[20];
    
    // 延时等待接收后面的字节
    delay_ms(10);

    // 初始化
    u8 recHead   = GV_Uart1ReceiveData[0];
    u8 recLength = GV_Uart1ReceiveData[1];

    // 数据完整性检测
    // LIN USB 交互的数据结构：AE [DatLen] [Dat1] [Dat2] [Dat3] ... AF
    // 首尾字节通常是 AE 和 AF，由变量 GV_LinMasterMainARecResponseToUsbDnHead 和 GV_LinMasterMainARecResponseToUsbDnFoot 决定
    if( recHead != GV_LinMasterMainARecResponseToUsbDnHead ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( recLength > (GV_Uart1ReceiveDataLength-3) ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( GV_Uart1ReceiveData[recLength+2] != GV_LinMasterMainARecResponseToUsbDnFoot ){ GV_Uart1ReceiveDataCount = 0; return; }

    // 提取有效的数据段
    // 去除了首字节 AE，尾字节 AF，以及第2个长度字节
    for( i = 0; i < recLength; i++ ) bytes[i] = GV_Uart1ReceiveData[2+i];
    
    // 重置接收计数
    GV_Uart1ReceiveDataCount = 0;
    
    // 设置并发送 LIN 辅信号
    LIN_MASTER_SetDownASignalBytes(bytes, recLength);
    LIN_MASTER_SendDownASignal();
}



/* -------------------------------------------------- */
// ● 系统内置函数重构
/* -------------------------------------------------- */

/**
 * 输出函数 printf 重载
 * 从 UART1 串口输出打印内容
 */

#ifdef __GNUC__
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
/*
// 发送一个字符协议
PUTCHAR_PROTOTYPE 
{
    UART1_SendByte((unsigned char) ch);
    return (ch);
}
*/