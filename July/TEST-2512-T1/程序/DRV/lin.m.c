#include "common.h"



/* -------------------------------------------------- */
// ● LIN 主机模式
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// ● 全局变量
// ● 以下信号数据基于道奇汽车的ECU
/* -------------------------------------------------- */

/**
 * LIN 基础变量
 */
u32 GV_LinMasterBaudRate     = 9600;  // 波特率
u32 GV_LinMasterSendInterval = 50;    // 信号下发到设备的间隔时间（毫秒）

/**
 * LIN 调试相关变量
 */
short GV_LinMasterDebugSendOutUart1            = 0;  // Debug 发送的数据输出到 UART1
short GV_LinMasterDebugSendOutPrintf           = 0;  // Debug 发送的数据输出到 printf（注意：该系统中可能重载 printf 函数输出到 UART1）
short GV_LinMasterDebugReceiveOutUart1         = 0;  // Debug 接收的数据输出到 UART1
short GV_LinMasterDebugReceiveOutPrintf        = 0;  // Debug 接收的数据输出到 printf（注意：该系统中可能重载 printf 函数输出到 UART1）
short GV_LinMasterDebugRecResponseChkOutUart1  = 0;  // Debug 反馈数据检测时输出到 UART1
short GV_LinMasterDebugRecResponseChkOutPrintf = 0;  // Debug 反馈数据检测时输出到 printf（注意：该系统中可能重载 printf 函数输出到 UART1）

/**
 * LIN 主信号相关变量
 * 主信号是指会收到从机反馈的信号
 */
u8 GV_LinMasterMainASignal[]               = {0x42, 0x00, 0x00, 0x00, 0xF2, 0xCA};  // 汽车 ON 档默认的主信号（ID，DATA，CheckNum）
                                                                                    // 该变量包含了主机发送的ID，从机默认响应的数据 和 校验码
                                                                                    // 系统自动根据这些数据发送ID，以及匹对从机反馈的信号是否准确等功能
                                                                                    // 【自产件】KD-281-D1 ：{0x42, 0x40, 0x00, 0x20, 0x04, 0x59};  // 线路板上未放置铜片，锁窗键处于触发状态
                                                                                    // 【自产件】KD-281-D3 ：{0x42, 0x00, 0x00, 0x20, 0x04, 0x99};  // 线路板上未放置铜片，锁窗键处于触发状态
                                                                                    // 【原厂件】KD-281-D27：{0x42, 0x00, 0x00, 0x00, 0xF2, 0xCA};
u8 GV_LinMasterMainASignalChecknumStart    = 0;                                     // 【按需设置】主信号校验码起始索引（基于 GV_LinMasterMainASignal 变量）  

u8 GV_LinMasterMainARecResponse[]          = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // 【系统使用】主信号发送后收到的反馈数据容器，包含 ID，DATA，CheckNum；数组长度需要和 GV_LinMasterMainASignal 一样
u8 GV_LinMasterMainARecResponseCnt         = 0;                                     // 【系统使用】用于 GV_LinMasterMainARecResponse 数组变量保存数据的个数
u8 GV_LinMasterMainARecResponseWait        = 0;                                     // 【系统使用】主信号等待反馈的标志，系统自动设置该标志后中断才正式将反馈信号记录到 GV_LinMasterMainARecResponse
u8 GV_LinMasterMainARecResponseChks[]      = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // 【系统使用】反馈信号检测的临时储存容器，包含 ID，DATA，CheckNum；数组长度需要和 GV_LinMasterMainASignal 一样
u8 GV_LinMasterMainARecResponsePrev[]      = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // 【系统使用】上一个信号临时储存容器，包含 ID，DATA，CheckNum；数组长度需要和 GV_LinMasterMainASignal 一样

u8 GV_LinMasterMainARecResponseToUsbMode   = LIN_TXUSB_MODE_DOWNUPONE;              // 【按需设置】主信号发送后收到的反馈数据输出模式：LIN_TXUSB_MODE_DEFAULT, LIN_TXUSB_MODE_DOWNONE（从机按下发送一次）, LIN_TXUSB_MODE_DOWNUPONE（从机按下和松开各发送一次）
u8 GV_LinMasterMainARecResponseToUsbDnHead = 0xAE;                                  // 【按需设置】从机按下发送一次信号的数据头
u8 GV_LinMasterMainARecResponseToUsbDnFoot = 0xAF;                                  // 【按需设置】从机按下发送一次信号的数据尾
u8 GV_LinMasterMainARecResponseToUsbUpHead = 0xBE;                                  // 【按需设置】从机松开发送一次信号的数据头
u8 GV_LinMasterMainARecResponseToUsbUpFoot = 0xBF;                                  // 【按需设置】从机松开发送一次信号的数据尾


/**
 * LIN 辅信号相关变量
 * 辅信号仅仅用于主机通知从机
 */
u8 GV_LinMasterDownASignal[]               = {0x80, 0x80, 0xFC};  // 汽车 ON 档默认下发的辅信号（ID 和 DATA，不含 CheckNum），一般用于汽车 ECU 通知开关背光灯亮或左右使能灯亮等情况
u8 GV_LinMasterDownASignalBox[]            = {0x00, 0x00, 0x00};  // 【系统使用】下发的辅信号容器，数组长度和 GV_LinMasterDownASignal 一致；主要用于构建并存放即将发送辅信号
u8 GV_LinMasterDownASignalChecknumStart    = 0;                   // 下发的辅信号校验码起始索引（基于 GV_LinMasterDownASignal 变量）


/**
 * LIN 查找信号相关变量
 */
u8 GV_LinMasterFindARecResponseCnt         = 0;  // 【系统使用】用于 GV_LinMasterFindARecResponse 数组变量保存数据的个数
u8 GV_LinMasterFindARecResponseWait        = 0;  // 【系统使用】查信号等待反馈的标志，系统自动设置该标志后中断才正式将反馈信号记录到 GV_LinMasterFindARecResponse
u8 GV_LinMasterFindARecResponse[20];             // 【系统使用】查信号发送后收到的反馈数据容器，包含 ID，DATA，CheckNum



/* -------------------------------------------------- */
// ● 函数主体（主信号）
/* -------------------------------------------------- */

/**
 * 发送下发的主信号
 * 主信号是指会收到从机反馈的信号
 *
 * 发送主信号 ID 后，从机会反馈数据，在 STM8 的 "INTERRUPT_HANDLER(UART3_RX_IRQHandler,21)" 中断里
 * 调用了 UART3_InterruptHandlerRX()->LIN_MASTER_ReceiveMainARecResponse() 函数记录反馈的数据到全局变量 GV_LinMasterMainARecResponse
 */
void LIN_MASTER_SendMainASignal()
{
    // 准备数据接收
    GV_LinMasterMainARecResponseCnt  = 0;
    GV_LinMasterMainARecResponseWait = 1;
    
    // 发送间隔段，同步段，ID
    UART3_SendBreak();
    UART3_SendByte(0x55);
    UART3_SendByte(GV_LinMasterMainASignal[0]);
    
    // 时间间隔
    delay_ms(GV_LinMasterSendInterval);
}


/**
 * 发送反馈的信号到 USB
 */
void LIN_MASTER_SendMainARecResponseToUsb()
{
    // 初始化
    u8 i, match = 1;
    u8 mode = GV_LinMasterMainARecResponseToUsbMode;
    u8 length = ARR_LENGTH(GV_LinMasterMainARecResponse);

    // 和默认信号比较
    match = 1; 
    for( i = 0; i < length; i++ ) match &= GV_LinMasterMainASignal[i] == GV_LinMasterMainARecResponse[i];

    // 和默认信号相同并且模式为："按下发一次" 或 "按下和松开发一次"
    // 信号相同则重置上一个信号变量且不发送接受到的反馈信号
    // 反馈信号和默认信号相同说明了线路板上处于默认状态，没有按键按下或其他默认情况
    if( match == 1 && (mode==LIN_TXUSB_MODE_DOWNONE||mode==LIN_TXUSB_MODE_DOWNUPONE) ){
        // 上一个信号是否全部是 0，那么说明线路板上一个状态也是默认状态
        // 说明没有按键按下，那么直接退出
        match = 1;
        for( i = 0; i < length; i++ ) match &= GV_LinMasterMainARecResponsePrev[i] == 0x00;
        if( match == 1 ) return;
        
        // 上一个信号非 0，那么上一个状态按键按下，那么此时的程序逻辑就是由按键松开触发
        if( mode == LIN_TXUSB_MODE_DOWNUPONE ){
            UART1_SendByte(GV_LinMasterMainARecResponseToUsbUpHead); UART1_SendByte(length);
            for( i = 0; i < length; i++ ) UART1_SendByte(GV_LinMasterMainARecResponsePrev[i]);
            UART1_SendByte(GV_LinMasterMainARecResponseToUsbUpHead);
        }
        
        // 重置上一个信号
        // 按键松开了，那么重置上一个信号全部为默认状态
        for( i = 0; i < length; i++ ) GV_LinMasterMainARecResponsePrev[i] = 0x00;
        
        // 返回
        return;
    }
    
    // 和上一个信号比较
    // 反馈信号和上一个信号相同则不发送反馈信号
    if( mode == LIN_TXUSB_MODE_DOWNONE || mode == LIN_TXUSB_MODE_DOWNUPONE ){
        match = 1; for( i = 0; i < length; i++ ) match &= GV_LinMasterMainARecResponsePrev[i] == GV_LinMasterMainARecResponse[i];
        if( match == 1 ) return;
    }

    
    // 发送开头数据
    UART1_SendByte(GV_LinMasterMainARecResponseToUsbDnHead);
    UART1_SendByte(length);
    
    // 发送反馈数据到 USB
    for( i = 0; i < length; i++ ) UART1_SendByte(GV_LinMasterMainARecResponse[i]);

    // 保存当前数据到上一个信号的变量中
    for( i = 0; i < length; i++ ) GV_LinMasterMainARecResponsePrev[i] = GV_LinMasterMainARecResponse[i];

    // 发送收尾数据
    UART1_SendByte(GV_LinMasterMainARecResponseToUsbDnFoot);
}


/**
 * 反馈的信号包含检测
 * 该函数根据全局变量 GV_LinMasterMainARecResponse 值为基础
 * 该函数采用位码标记法，例如：1H40(表示检查 LIN 的数据段第1个数据字节的高位4是否等于0)
 * 　　　　　　　　　　　　　　2L11(表示检查 LIN 的数据段第2个数据字节的低位1是否等于1)
 *
 * @param dataNum           数据编号：从 1 开始
 * @param dataHightLow      数据高低位：H、L
 * @param data8421Bit       数据位，以 8421 为基准
 * @param data8421BitValue  数据位值：0、1
 *
 * @return 0 表示不包含，1 表示包含
 */
short LIN_MASTER_MainARecResponseContain( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue )
{
    // 重构数据位
    if( dataHighLow == 'h' ) data8421Bit = data8421Bit << 4;
    if( dataHighLow == 'H' ) data8421Bit = data8421Bit << 4;

    if( data8421BitValue == 0 && (GV_LinMasterMainARecResponse[dataNum]&data8421Bit) == 0 ) return 1;
    if( data8421BitValue == 1 && (GV_LinMasterMainARecResponse[dataNum]&data8421Bit) == data8421Bit ) return 1;
    
    return 0;
}


/**
 * 接收反馈的信号
 * 该函数在 UART 的中断中被调用，它能自动根据相关的配置决定是否记录数据
 *
 * @param byte  8位字节
 */
void LIN_MASTER_ReceiveMainARecResponse( u8 byte )
{
    // 无效参数
    if( GV_LinMasterMainARecResponseWait == 0 ) return;
    if( GV_LinMasterMainARecResponseCnt >= ARR_LENGTH(GV_LinMasterMainARecResponse) ) return;

    // 接收间隔段和同步段并过滤
    if( GV_LinMasterMainARecResponseWait == 1 && byte == 0x00 ){ GV_LinMasterMainARecResponseWait = 2; return; }
    if( GV_LinMasterMainARecResponseWait == 2 && byte == 0x55 ){ GV_LinMasterMainARecResponseWait = 3; return; }
    if( GV_LinMasterMainARecResponseWait != 3 ) return;
    
    //【DEBUG】 输出接收到的反馈信号
    if( GV_LinMasterDebugReceiveOutUart1  == 1 ) UART1_SendByte(byte);
    if( GV_LinMasterDebugReceiveOutPrintf == 1 ) printf("%02X ", byte);

    // 数据赋值
    GV_LinMasterMainARecResponse[GV_LinMasterMainARecResponseCnt++] = byte;
}

/**
 * 重置接收反馈信号
 */
void LIN_MASTER_ReceiveMainARecResponseReset()
{
    GV_LinMasterMainARecResponseCnt  = 0;
    GV_LinMasterMainARecResponseWait = 0;
}



/* -------------------------------------------------- */
// ● 函数主体（辅信号）
/* -------------------------------------------------- */

/**
 * 初始化下发的辅信号
 * 该函数直接初始化全局变量 GV_LinMasterDownASignalBox
 */
void LIN_MASTER_InitDownASignal()
{
    for( int i=0; i < ARR_LENGTH(GV_LinMasterDownASignal); i++ ) GV_LinMasterDownASignalBox[i] = GV_LinMasterDownASignal[i];
}


/**
 * 设置下发的辅信号
 * 该函数直接设置全局变量 GV_LinMasterDownASignalBox
 * 该函数采用位码标记法，例如：1H40(表示将LIN的数据段第1个数据字节的高位4设置为0)
 * 　　　　　　　　　　　　　　2L11(表示将LIN的数据段第2个数据字节的低位1设置为1)
 *
 * @param dataNum           数据编号：从 1 开始
 * @param dataHightLow      数据高低位：H、L
 * @param data8421Bit       数据位，以 8421 为基准
 * @param data8421BitValue  数据位值：0、1
 */
void LIN_MASTER_SetDownASignal( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue )
{
    // 重构数据位
    if( dataHighLow == 'h' ) data8421Bit = data8421Bit << 4;
    if( dataHighLow == 'H' ) data8421Bit = data8421Bit << 4;

    // 设置下发的辅信号
    if( data8421BitValue == 1 ) GV_LinMasterDownASignalBox[dataNum] |= data8421Bit;
    if( data8421BitValue == 0 ) GV_LinMasterDownASignalBox[dataNum] &= data8421Bit^0xFF;
}


/**
 * 发送下发的辅信号
 * 该函数直接发送全局变量 GV_LinMasterDownASignalBox 里的数据
 */
void LIN_MASTER_SendDownASignal()
{
    // 初始化
    u8 start  = GV_LinMasterDownASignalChecknumStart;
    u8 length = ARR_LENGTH(GV_LinMasterDownASignal);

    // 发送 LIN 标志
    UART3_SendBreak();
    UART3_SendByte(0x55);

    // 发送下发的辅信号
    for( int i = 0; i < length; i++ ) UART3_SendByte(GV_LinMasterDownASignalBox[i]);
    UART3_SendByte(LIN_CalcChecknum(GV_LinMasterDownASignalBox,length,start));

    // 时间间隔
    delay_ms(GV_LinMasterSendInterval);
    
    //【DEBUG】 通过 UART1 串口输出下发的辅信号
    if( GV_LinMasterDebugSendOutUart1 == 1 ){
        for( int i = 0; i < length; i++ ) UART1_SendByte(GV_LinMasterDownASignalBox[i]);
        UART1_SendByte(LIN_CalcChecknum(GV_LinMasterDownASignalBox,length,start));
    }
    
    //【DEBUG】 通过 Printf 串口输出下发的辅信号
    if( GV_LinMasterDebugSendOutPrintf == 1 ){
        for( int i = 0; i < ARR_LENGTH(GV_LinMasterDownASignal); i++ ) printf("%02X ", GV_LinMasterDownASignalBox[i]);
        printf("%02X\n", LIN_CalcChecknum(GV_LinMasterDownASignalBox,length,start));
    }
}


/**
 * 设置下发的辅信号
 * 该函数直接设置全局变量 GV_LinMasterDownASignalBox
 * 直接将数据字节集赋值给 GV_LinMasterDownASignalBox，但赋值前会检查 ID 是否一致，同时过滤 bytes 中的 Checknum
 *
 * @param datBytes   数据字节集
 * @param datLength  数据字节长度
 */
void LIN_MASTER_SetDownASignalBytes( u8 datBytes[], u8 datLength )
{
    // 初始化
    u8 i;
    u8 length = ARR_LENGTH(GV_LinMasterDownASignal);

    // 无效 ID
    if( datBytes[0] != GV_LinMasterDownASignal[0] ) return;

    // 赋值
    length = length > datLength ? datLength : length;
    for( i = 0; i < length; i++ ) GV_LinMasterDownASignalBox[i] = datBytes[i];
}


/**
 * 设置下发的辅信号
 * 预设各个功能对应的数据信号
 */
void LIN_MASTER_SetDownASignalLLedOn(){ LIN_MASTER_SetDownASignal(1, 'L', 4, 1); }  // 信号：左使能灯亮
void LIN_MASTER_SetDownASignalRLedOn(){ LIN_MASTER_SetDownASignal(1, 'L', 1, 1); }  // 信号：右使能灯亮



/* -------------------------------------------------- */
// ● 函数主体（LIN ID 查找相关）
/* -------------------------------------------------- */

/**
 * 发送下发的主信号
 * 主信号是指会收到从机反馈的信号
 *
 * @param id  LIN ID 字节
 */
void LIN_MASTER_SendFindASignal( u8 id )
{
    // 准备数据接收
    GV_LinMasterFindARecResponseCnt  = 0;
    GV_LinMasterFindARecResponseWait = 1;
    
    // 发送间隔段，同步段，ID
    UART3_SendBreak();
    UART3_SendByte(0x55);
    UART3_SendByte(id);

    // 时间间隔
    delay_ms(GV_LinMasterSendInterval);
}


/**
 * 接收反馈的信号
 * 该函数在 UART 的中断中被调用，它能自动根据相关的配置决定是否记录数据
 *
 * @param byte  8位字节
 */
void LIN_MASTER_ReceiveFindARecResponse( u8 byte )
{
    // 无效参数
    if( GV_LinMasterFindARecResponseWait == 0 ) return;
    if( GV_LinMasterFindARecResponseCnt >= ARR_LENGTH(GV_LinMasterFindARecResponse) ) return;

    // 接收间隔段和同步段并过滤
    if( GV_LinMasterFindARecResponseWait == 1 && byte == 0x00 ){ GV_LinMasterFindARecResponseWait = 2; return; }
    if( GV_LinMasterFindARecResponseWait == 2 && byte == 0x55 ){ GV_LinMasterFindARecResponseWait = 3; return; }
    if( GV_LinMasterFindARecResponseWait != 3 ) return;

    //【DEBUG】 输出接收到的反馈信号
    if( GV_LinMasterDebugReceiveOutUart1  == 1 ) UART1_SendByte(byte);
    if( GV_LinMasterDebugReceiveOutPrintf == 1 ) printf("%02X ", byte);

    // 数据赋值
    GV_LinMasterFindARecResponse[GV_LinMasterFindARecResponseCnt++] = byte;
}


/**
 * 发送反馈的信号到 USB
 */
void LIN_MASTER_SendFindARecResponseToUsb()
{
    // 发送反馈数据到 USB
    for( int i = 0; i < GV_LinMasterFindARecResponseCnt; i++ ) UART1_SendByte(GV_LinMasterFindARecResponse[i]);
}


/**
 * 重置接收反馈信号
 */
void LIN_MASTER_ReceiveFindARecResponseReset()
{
    GV_LinMasterFindARecResponseCnt  = 0;
    GV_LinMasterFindARecResponseWait = 0;
}



/* -------------------------------------------------- */
// ● 函数主体（主信号检测匹配相关）
/* -------------------------------------------------- */

/**
 * 初始反馈的信号（检测专用）
 */
void LIN_MASTER_MainARecResponseChkInit()
{
    for( int i=0; i < ARR_LENGTH(GV_LinMasterMainASignal); i++ ) GV_LinMasterMainARecResponseChks[i] = GV_LinMasterMainASignal[i];
}

/**
 * 设置反馈的信号（检测专用）
 * 该函数直接设置全局变量 GV_LinMasterMainARecResponseChks
 * 该函数采用位码标记法，例如：1H40(表示将 LIN 的数据段第1个数据字节的高位4设置为0)
 * 　　　　　　　　　　　　　　2L11(表示将 LIN 的数据段第2个数据字节的低位1设置为1)
 *
 * @param dataNum           数据编号：从 1 开始
 * @param dataHightLow      数据高低位：H、L
 * @param data8421Bit       数据位，以 8421 为基准
 * @param data8421BitValue  数据位值：0、1
 */
void LIN_MASTER_MainARecResponseChkSet( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue )
{
    // 重构数据位
    if( dataHighLow == 'h' ) data8421Bit = data8421Bit << 4;
    if( dataHighLow == 'H' ) data8421Bit = data8421Bit << 4;

    // 设置反馈的信号（检测专用）
    if( data8421BitValue == 1 ) GV_LinMasterMainARecResponseChks[dataNum] |= data8421Bit;
    if( data8421BitValue == 0 ) GV_LinMasterMainARecResponseChks[dataNum] &= data8421Bit^0xFF;
}

// 后视镜向上
short LIN_MASTER_MainARecResponseChkHasMirrUp()     { return LIN_MASTER_MainARecResponseContain(3,'L',1,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }  
void  LIN_MASTER_MainARecResponseChkSetMirrUp()     {         LIN_MASTER_MainARecResponseChkSet(3,'L',1,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }
 
// 后视镜向左
short LIN_MASTER_MainARecResponseChkHasMirrLeft()   { return LIN_MASTER_MainARecResponseContain(3,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }  
void  LIN_MASTER_MainARecResponseChkSetMirrLeft()   {         LIN_MASTER_MainARecResponseChkSet(3,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 后视镜向下
short LIN_MASTER_MainARecResponseChkHasMirrDown()   { return LIN_MASTER_MainARecResponseContain(3,'L',2,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetMirrDown()   {         LIN_MASTER_MainARecResponseChkSet(3,'L',2,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 后视镜向右
short LIN_MASTER_MainARecResponseChkHasMirrRight()  { return LIN_MASTER_MainARecResponseContain(3,'L',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetMirrRight()  {         LIN_MASTER_MainARecResponseChkSet(3,'L',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 后视镜辅助按键（包括方向，收放，左右使能）
short LIN_MASTER_MainARecResponseChkHasMirrAssist() { return LIN_MASTER_MainARecResponseContain(4,'L',8,1); }
void  LIN_MASTER_MainARecResponseChkSetMirrAssist() {         LIN_MASTER_MainARecResponseChkSet(4,'L',8,1); }

// 后视镜辅助按键（包括方向，收放，左右使能）
short LIN_MASTER_MainARecResponseChkHasMirrAssist2(){ return LIN_MASTER_MainARecResponseContain(2,'H',4,1); }
void  LIN_MASTER_MainARecResponseChkSetMirrAssist2(){         LIN_MASTER_MainARecResponseChkSet(2,'H',4,1); }

// 后视镜收放
short LIN_MASTER_MainARecResponseChkHasMirrOC()     { return LIN_MASTER_MainARecResponseContain(3,'H',1,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetMirrOC()     {         LIN_MASTER_MainARecResponseChkSet(3,'H',1,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 左后视镜使能
short LIN_MASTER_MainARecResponseChkHasMirrSWL()    { return LIN_MASTER_MainARecResponseContain(2,'H',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetMirrSWL()    {         LIN_MASTER_MainARecResponseChkSet(2,'H',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 右后视镜使能
short LIN_MASTER_MainARecResponseChkHasMirrSWR()    { return LIN_MASTER_MainARecResponseContain(2,'H',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetMirrSWR()    {         LIN_MASTER_MainARecResponseChkSet(2,'H',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 左前升
short LIN_MASTER_MainARecResponseChkHasLFUp()       { return LIN_MASTER_MainARecResponseContain(1,'L',2,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetLFUp()       {         LIN_MASTER_MainARecResponseChkSet(1,'L',2,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 左前升一键
short LIN_MASTER_MainARecResponseChkHasLFUpA()      { return LIN_MASTER_MainARecResponseContain(1,'L',7,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetLFUpA()      {         LIN_MASTER_MainARecResponseChkSet(1,'L',7,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 左前一键升
short LIN_MASTER_MainARecResponseChkHasLFUpAuto()   { return LIN_MASTER_MainARecResponseContain(1,'L',6,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetLFUpAuto()   {         LIN_MASTER_MainARecResponseChkSet(1,'L',6,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 左前降
short LIN_MASTER_MainARecResponseChkHasLFDown()     { return LIN_MASTER_MainARecResponseContain(1,'L',1,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetLFDown()     {         LIN_MASTER_MainARecResponseChkSet(1,'L',1,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 左前降一键
short LIN_MASTER_MainARecResponseChkHasLFDownA()    { return LIN_MASTER_MainARecResponseContain(1,'L',7,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetLFDownA()    {         LIN_MASTER_MainARecResponseChkSet(1,'L',7,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 左前一键降
short LIN_MASTER_MainARecResponseChkHasLFDownAuto() { return LIN_MASTER_MainARecResponseContain(1,'L',5,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }  
void  LIN_MASTER_MainARecResponseChkSetLFDownAuto() {         LIN_MASTER_MainARecResponseChkSet(1,'L',5,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 右前升
short LIN_MASTER_MainARecResponseChkHasRFUp()       { return LIN_MASTER_MainARecResponseContain(1,'H',1,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFUp()       {         LIN_MASTER_MainARecResponseChkSet(1,'H',1,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 右前升一键
short LIN_MASTER_MainARecResponseChkHasRFUpA()      { return LIN_MASTER_MainARecResponseContain(1,'H',3,1) && LIN_MASTER_MainARecResponseContain(1,'L',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFUpA()      {         LIN_MASTER_MainARecResponseChkSet(1,'H',3,1);    LIN_MASTER_MainARecResponseChkSet(1,'L',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 右前一键升
short LIN_MASTER_MainARecResponseChkHasRFUpAuto()   { return LIN_MASTER_MainARecResponseContain(1,'H',3,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFUpAuto()   {         LIN_MASTER_MainARecResponseChkSet(1,'H',3,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 右前降
short LIN_MASTER_MainARecResponseChkHasRFDown()     { return LIN_MASTER_MainARecResponseContain(1,'L',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFDown()     {         LIN_MASTER_MainARecResponseChkSet(1,'L',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 右前降一键
short LIN_MASTER_MainARecResponseChkHasRFDownA()    { return LIN_MASTER_MainARecResponseContain(1,'H',3,1) && LIN_MASTER_MainARecResponseContain(1,'L',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFDownA()    {         LIN_MASTER_MainARecResponseChkSet(1,'H',3,1);    LIN_MASTER_MainARecResponseChkSet(1,'L',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 右前一键降
short LIN_MASTER_MainARecResponseChkHasRFDownAuto() { return LIN_MASTER_MainARecResponseContain(1,'H',2,1) && LIN_MASTER_MainARecResponseContain(1,'L',8,1) && LIN_MASTER_MainARecResponseContain(4,'L',4,1) && LIN_MASTER_MainARecResponseContain(4,'L',2,0); }
void  LIN_MASTER_MainARecResponseChkSetRFDownAuto() {         LIN_MASTER_MainARecResponseChkSet(1,'H',2,1);    LIN_MASTER_MainARecResponseChkSet(1,'L',8,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',4,1);    LIN_MASTER_MainARecResponseChkSet(4,'L',2,0); }

// 车窗加锁
short LIN_MASTER_MainARecResponseChkHasWinLock()    { return LIN_MASTER_MainARecResponseContain(3,'H',4,1); }
void  LIN_MASTER_MainARecResponseChkSetWinLock()    {         LIN_MASTER_MainARecResponseChkSet(3,'H',4,1); }  

// 车窗解锁
short LIN_MASTER_MainARecResponseChkHasWinUnLock()  { return LIN_MASTER_MainARecResponseContain(3,'H',8,1); }
void  LIN_MASTER_MainARecResponseChkSetWinUnLock()  {         LIN_MASTER_MainARecResponseChkSet(3,'H',8,1); }  



/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */

/**
 * LIN 主逻辑：查找从机的 ID 标识
 *
 * @param process  显示进度
 */
void LIN_MASTER_MainFindASlaveId( bool process )
{
    // 初始化
    int id;
    int finded = 0;
    
    // LIN ID 遍历
    for( id = 0; id <= 0xFF; id++ ){
        // 发送请求主信号
        LIN_MASTER_SendFindASignal((u8)id);

        // 缓存状态
        finded = GV_LinMasterFindARecResponseCnt > 1 ? 1 : 0;

        // 主信号发送后对反馈信号处理
        if( finded ){
            // 延迟，使上位机软件读取数据时能自动换行
            if( process ) delay_ms(500); 
            
            // 发送 LIN 信号到 USB
            LIN_MASTER_SendFindARecResponseToUsb();
            
            // 延迟，使上位机软件读取数据时能自动换行
            delay_ms(500); 
        }
        else if( process ){
            UART1_SendByte((short)id);
        }

        // 重置反馈信号以便等下次主信号请求
        LIN_MASTER_ReceiveFindARecResponseReset();
    }
}


/**
 * LIN 主逻辑：发送主信号（MainA）并接收从机反馈信号，然后输出到 USB
 */
void LIN_MASTER_MainMainAReceiveSlaveToUsb()
{
    // 发送请求主信号
    LIN_MASTER_SendMainASignal();

    // 主信号发送后对反馈信号处理
    LIN_MASTER_SendMainARecResponseToUsb();

    // 重置反馈信号以便等下次主信号请求
    LIN_MASTER_ReceiveMainARecResponseReset();
}


/**
 * LIN 主逻辑：发送主信号（MainA）并接收从机反馈信号，然后进行检测
 * 该函数一般用于单独的线路板检测
 */
void LIN_MASTER_MainMainAReceiveSlaveAndCheck()
{
    // 初始化
    u8 signalMatch;
    u8 signalLength = ARR_LENGTH(GV_LinMasterMainASignal);
    u8 signalContain = 0;
    
    // 检验是否与默认信号匹配
    // 反馈的信号与默认信号一样说明了线路板未按下按键
    signalMatch = 1; for( int i = 0; i < signalLength; i++ ) signalMatch &= GV_LinMasterMainASignal[i] == GV_LinMasterMainARecResponse[i];
    
    //【DEBUG】输出接收到的反馈信号
    if( GV_LinMasterDebugRecResponseChkOutUart1  == 1 ){ for( u8 i = 0; i < signalLength; i++ ) UART1_SendByte(GV_LinMasterMainARecResponse[i]); delay_ms(600); }
    if( GV_LinMasterDebugRecResponseChkOutPrintf == 1 ){ for( u8 i = 0; i < signalLength; i++ ) printf("==="); printf("\n"); for( u8 i = 0; i < signalLength; i++ ) printf("%02X ", GV_LinMasterMainARecResponse[i]); printf("\n"); }
    
    // 与默认信号匹配
    //if( signalMatch == 1 ){ LED_Close(2); return; }

    
    // ↓与默认信号不匹配↓
    
    // 初始反馈的信号（检测专用）
    LIN_MASTER_MainARecResponseChkInit();
    
    // 检测是否包含正确的按键信号
    // 同时根据按键按下构建理论上的反馈信号（用于检测对比）
    if( LIN_MASTER_MainARecResponseChkHasMirrUp()      == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrUp();      signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrLeft()    == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrLeft();    signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrDown()    == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrDown();    signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrRight()   == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrRight();   signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrAssist()  == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrAssist();  signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrAssist2() == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrAssist2(); signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrOC()      == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrOC();      signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrSWL()     == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrSWL();     signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasMirrSWR()     == 1 ){ LIN_MASTER_MainARecResponseChkSetMirrSWR();     signalContain |= 1; }

    if( LIN_MASTER_MainARecResponseChkHasLFUp()        == 1 ){ LIN_MASTER_MainARecResponseChkSetLFUp();        signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasLFUpA()       == 1 ){ LIN_MASTER_MainARecResponseChkSetLFUpA();       signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasLFUpAuto()    == 1 ){ LIN_MASTER_MainARecResponseChkSetLFUpAuto();    signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasLFDown()      == 1 ){ LIN_MASTER_MainARecResponseChkSetLFDown();      signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasLFDownA()     == 1 ){ LIN_MASTER_MainARecResponseChkSetLFDownA();     signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasLFDownAuto()  == 1 ){ LIN_MASTER_MainARecResponseChkSetLFDownAuto();  signalContain |= 1; }
    
    if( LIN_MASTER_MainARecResponseChkHasRFUp()        == 1 ){ LIN_MASTER_MainARecResponseChkSetRFUp();        signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasRFUpA()       == 1 ){ LIN_MASTER_MainARecResponseChkSetRFUpA();       signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasRFUpAuto()    == 1 ){ LIN_MASTER_MainARecResponseChkSetRFUpAuto();    signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasRFDown()      == 1 ){ LIN_MASTER_MainARecResponseChkSetRFDown();      signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasRFDownA()     == 1 ){ LIN_MASTER_MainARecResponseChkSetRFDownA();     signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasRFDownAuto()  == 1 ){ LIN_MASTER_MainARecResponseChkSetRFDownAuto();  signalContain |= 1; }
    
    if( LIN_MASTER_MainARecResponseChkHasWinLock()     == 1 ){ LIN_MASTER_MainARecResponseChkSetWinLock();     signalContain |= 1; }
    if( LIN_MASTER_MainARecResponseChkHasWinUnLock()   == 1 ){ LIN_MASTER_MainARecResponseChkSetWinUnLock();   signalContain |= 1; }

    // 构建反馈信号校验码（检测专用）
    GV_LinMasterMainARecResponseChks[signalLength-1] = LIN_CalcChecknum(GV_LinMasterMainARecResponseChks, signalLength-GV_LinMasterMainASignalChecknumStart-1, GV_LinMasterMainASignalChecknumStart);
      
    //【DEBUG】输出根据按键按下构建理论
    if( GV_LinMasterDebugRecResponseChkOutUart1  == 1 ){ UART1_SendBytes(GV_LinMasterMainARecResponseChks, signalLength); delay_ms(500); }
    if( GV_LinMasterDebugRecResponseChkOutPrintf == 1 ){ for( u8 i = 0; i < signalLength; i++ ) printf("%02X ", GV_LinMasterMainARecResponseChks[i]); printf("\n"); }
    
    // 比较接收到的反馈信号和按理论构建的反馈信号是否一直
    signalMatch = 1; for( int i = 0; i < signalLength; i++ ) signalMatch &= GV_LinMasterMainARecResponse[i] == GV_LinMasterMainARecResponseChks[i];
    
    // 和按照理论值反馈信号不一致，说明接收到的信号有问题
    //if( signalMatch == 0 ){ LED_Open(2); return; }
  
    // 按键按下
    //if( signalContain ){ BUZZER_Open(1); delay_ms(5); BUZZER_Close(1); }
}
