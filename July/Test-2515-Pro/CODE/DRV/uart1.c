#include "common.h"



/* -------------------------------------------------- */
// ● 外部变量
/* -------------------------------------------------- */

/**
 * LIN 外部变量
 * 以下变量定义在 lin.m.c 文件中
 */
//extern u8 GV_LinMasterMainARecResponseToUsbDnHead;
//extern u8 GV_LinMasterMainARecResponseToUsbDnFoot;



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */
u8 GV_Uart1ReceiveData[40];         // 数据接收容器
u8 GV_Uart1ReceiveDataCount  = 0;   // 数据接收实际长度
u8 GV_Uart1ReceiveDataLength = 40;  // 数据接收容器长度



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


/**
 * 获取接收到的数据
 *
 * return u8*  数组指针
 */
u8* UART1_GetReceiveData()
{
    return GV_Uart1ReceiveData;
}


/**
 * 获取接收到的数据长度
 *
 * return u8
 */
u8 UART1_GetReceiveDataCount()
{
    return GV_Uart1ReceiveDataCount;
}


/**
 * 复制接收到的数据
 *
 * @param u8 *data  数组，该数组字节数量必须 >= GV_Uart1ReceiveDataLength
 * return u8 拷贝的数据个数
 */
u8 UART1_CopyReceiveData( u8 *data )
{
    // 复制数据
    for( u8 i = 0; i < GV_Uart1ReceiveDataCount; i++ ) data[i] = GV_Uart1ReceiveData[i];

    // 返回
    return GV_Uart1ReceiveDataCount;
}


/**
 * 重置接收到的数据
 */
void UART1_ResetReceiveData()
{
    // 重置变量
    GV_Uart1ReceiveDataCount = 0;
    for( u8 i = 0; i < GV_Uart1ReceiveDataLength; i++ ) GV_Uart1ReceiveData[i] = 0;
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
    // 防止 OR 过载错误导致不停的进入读中断（OR 源自 UART1_IT_RXNE_OR）
    // STM8 内部机制：先读取 UART_SR，再读取 UART_DR 可把 OR 位中断状态清零
    // 当通过PC端的串口软件向 STM8 一次性发送多字节数据时（例如：01 02 03），那么 STM8 就会一直不停的进入读中断，陷入死循环
    if( UART1_GetITStatus(UART1_IT_RXNE) == RESET ) return;
    
    // 读取数据
    u8 byte = UART1_ReceiveData8();
    
    // 保存数据（如果过长则丢弃）
    if( GV_Uart1ReceiveDataCount < GV_Uart1ReceiveDataLength ) GV_Uart1ReceiveData[GV_Uart1ReceiveDataCount++] = byte;
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
 * 主逻辑：接收数据并发送给 LIN
 * 　　　　接收数据后根据实际需要可以在该函数对数据进行采集/重构，然后再发送给 LIN
 *
 * 注意：由于该函数会重置 UART1 的接收计数，所以注意和其他 UART1_MainXXX 冲突
 */
/*
void UART1_MainReceiveAndSendToLin()
{
    // 未读取到数据
    if( GV_Uart1ReceiveDataCount == 0 ) return;

    // 初始化
    u8 i, bytes[20];
    
    // 延时等待接收后面的字节
    delay_ms(20);

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
*/


/**
 * 主逻辑：接收数据并直接转发送给 USB（即发送给 UART1 自身）
 * 注意：由于该函数会重置 UART1 的接收计数，所以注意和其他 UART1_MainXXX 冲突
 */
void UART1_MainReceiveAndSendToUsbByDirect()
{
    // 未读取到数据
    if( GV_Uart1ReceiveDataCount == 0 ) return;

    // 初始化
    u8 i, sendLength, sendBytes[40];  // 发送的数据长度和数据数组（数组字节长度和 GV_Uart1ReceiveDataLength 一致） 
    
    // 延时等待接收后面的字节
    delay_ms(20);


    // 构建发回的数据：复制接收到的数据
    sendLength = GV_Uart1ReceiveDataCount;
    for( i = 0; i < sendLength; i++ ) sendBytes[i] = GV_Uart1ReceiveData[i];

    
    // 重置接收计数
    GV_Uart1ReceiveDataCount = 0;

    // UART1 发回
    UART1_SendBytes(sendBytes, sendLength);
}


/**
 * 主逻辑：接收数据并重构数据发送给 USB（即发送给 UART1 自身）
 * 　　　　接收数据后根据实际需要可以在该函数对数据进行采集/重构，然后再发送给 USB
 *
 * 注意：由于该函数会重置 UART1 的接收计数，所以注意和其他 UART1_MainXXX 冲突
 * 
 * 注意：UART1 接收的数据格式要求（即上位机发给测试板的数据格式要求）：AE [DatLen] [Dat1] [Dat2] [Dat3] ... AF
 * 　　　UART1 发回的数据格式标准（即测试板发给上位机的数据格式标准）：AE [DatLen] [Dat1] [Dat2] [Dat3] ... AF
 */
void UART1_MainReceiveAndSendToUsbByHeadFoot()
{
    // 未读取到数据
    if( GV_Uart1ReceiveDataCount == 0 ) return;

    // 初始化
    u8 i, sendLength, sendBytes[40];  // 发送的数据长度和数据数组（数组字节长度和 GV_Uart1ReceiveDataLength 一致） 
    
    // 延时等待接收后面的字节
    delay_ms(20);

    // 初始化
    u8 recHead = GV_Uart1ReceiveData[0];
    u8 recDatLength = GV_Uart1ReceiveData[1];

    // 数据完整性检测
    if( recHead != 0xAE ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( recDatLength > (GV_Uart1ReceiveDataLength-3) ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( GV_Uart1ReceiveData[recDatLength+2] != 0xAF ){ GV_Uart1ReceiveDataCount = 0; return; }

    // 构建发回的数据：复制接收到的数据
    for( i = 0; i < recDatLength; i++ ) sendBytes[2+i] = GV_Uart1ReceiveData[2+i];

    // 补全发回的数据格式标准
    sendLength = recDatLength + 3;
    sendBytes[0] = 0xAE; sendBytes[1] = recDatLength; sendBytes[2+recDatLength] = 0xAF;

    // 重置接收计数
    GV_Uart1ReceiveDataCount = 0;

    // UART1 发回
    UART1_SendBytes(sendBytes, sendLength);
}


/**
 * 主逻辑：KedeAngleSenser.exe 上位机中的 ProductAutoTester 测试台专用
 *
 * 注意：由于该函数会重置 UART1 的接收计数，所以注意和其他 UART1_MainXXX 冲突

 * 注意：UART1 接收的数据格式要求（即上位机发给测试板的数据格式要求）：AE AF [DatLen] [Dat1] [Dat2] [Dat3] ... BE BF
 * 　　　UART1 发回的数据格式标准（即测试板发给上位机的数据格式标准）：CE CF [DatLen] [Dat1] [Dat2] [Dat3] ... CE CF
 */
void UART1_MainProductAutoTester()
{
        // 未读取到数据
    if( GV_Uart1ReceiveDataCount == 0 ) return;

    // 初始化
    u8 dataLength, sendLength, sendBytes[40];  // 发送的数据长度和数据数组（数组字节长度和 GV_Uart1ReceiveDataLength 一致） 
    
    // 延时等待接收后面的字节
    delay_ms(20);

    // 初始化
    u8 recHead1 = GV_Uart1ReceiveData[0];
    u8 recHead2 = GV_Uart1ReceiveData[1];
    u8 recDatLength = GV_Uart1ReceiveData[2];

    // 数据完整性检测
    if( recHead1 != 0xAE || recHead2 != 0xAF ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( recDatLength > (GV_Uart1ReceiveDataLength-5) ){ GV_Uart1ReceiveDataCount = 0; return; }
    if( GV_Uart1ReceiveData[recDatLength+3] != 0xBE || GV_Uart1ReceiveData[recDatLength+4] != 0xBF ){ GV_Uart1ReceiveDataCount = 0; return; }

    // 提取功能码和功能辅助码
    u8 funcCodeX1    = GV_Uart1ReceiveData[3];
    u8 funcCodeAuxY1 = GV_Uart1ReceiveData[4];
    u8 funcCodeAuxY2 = GV_Uart1ReceiveData[5];

    // 初始返回码
    sendBytes[0] = 0xCE;
    sendBytes[1] = 0xCF;
    sendBytes[2] = 0xFF;  // 初始数据长度：不含自身，不含开始/结束码
    sendBytes[3] = funcCodeX1;
    
    // 执行功能码
    dataLength = UART1_MainProductAutoTesterFuncExe(funcCodeX1, funcCodeAuxY1, funcCodeAuxY2, sendBytes);

    // 补全返回码
    dataLength = dataLength+1;       // 构建数据长度：+1 表示加上功能码
    sendBytes[2] = dataLength;       // 设置数据长度：不含自身，不含开始/结束码
    
    sendBytes[dataLength+3] = 0xDE;  // 设置结束码
    sendBytes[dataLength+4] = 0xDF;  // 设置结束码
    
    sendLength = dataLength+5;       // 构建发送长度
    
    
    // 重置接收计数
    GV_Uart1ReceiveDataCount = 0;

    // UART1 发回
    UART1_SendBytes(sendBytes, sendLength);
}
short UART1_MainProductAutoTesterFuncExe( u8 funcCodeX1, u8 funcCodeAuxY1, u8 funcCodeAuxY2, u8 *sendBytes )
{
    // 初始化
    u8  dataLength = 0;
    u8  sendBytesStart = 4;  // 发送字节集可用起始位，注意前4位已经被使用，参加 UART1_MainProductAutoTester()
    u16 funcCodeAux;
    
    // 初始化
    short error = 0;
    
    // 功能码：设置电机可用
    if( funcCodeX1 == 0x01 ){
        error = MOTOR_ZDTUART_SetEnable();

        dataLength = 1;
        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }
    
    // 功能码：设置电机不可用
    if( funcCodeX1 == 0x02 ){
        error = MOTOR_ZDTUART_SetDisable();

        dataLength = 1;
        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }
    
    // 功能码：设置电机零点
    if( funcCodeX1 == 0x03 ){
        MOTOR_ZDTUART_SetEnable();
        error = MOTOR_ZDTUART_SetZeroPoint(false);
        
        dataLength = 1;
        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }
    
    // 功能码：电机归零
    if( funcCodeX1 == 0x04 ){
        MOTOR_ZDTUART_SetEnable();
        error = MOTOR_ZDTUART_Zero(funcCodeAuxY1);
        
        dataLength = 1;
        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }
    
    // 功能码：电机顺时针运行角度
    if( funcCodeX1 == 0x05 ){
        MOTOR_ZDTUART_SetDir(MOTOR_ZDTUART_DIR_CW);
        MOTOR_ZDTUART_SetEnable();

        funcCodeAux = (funcCodeAuxY1<<8) | funcCodeAuxY2;
        error = MOTOR_ZDTUART_RunDegree(funcCodeAux);
        
        dataLength = 1;
        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }
    
    // 功能码：电机逆时针运行角度
    if( funcCodeX1 == 0x06 ){
        dataLength = 1;
        MOTOR_ZDTUART_SetDir(MOTOR_ZDTUART_DIR_CCW);
        MOTOR_ZDTUART_SetEnable();

        funcCodeAux = (funcCodeAuxY1<<8) | funcCodeAuxY2;
        error = MOTOR_ZDTUART_RunDegree(funcCodeAux);

        sendBytes[sendBytesStart] = error ? 0x01 : 0x00;
    }

    // 返回数据长度
    return dataLength;
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