#include "common.h"



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * LIN 基础变量
 */
u32 GV_LinMode = LIN_MODE_MASTER;  // LIN 模式：LIN_MODE_MASTER 或 LIN_MODE_SLAVE



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * LIN 初始化
 * 初始化 LIN 相关的数据，芯片的 LIN 串口的初始化参考 uart3.c 中的函数
 */
void LIN_Init()
{
    // LIN 主机相关初始化
    if( GV_LinMode == LIN_MODE_MASTER ){
        LIN_MASTER_InitAuxSignal();  // 初始化下发的辅信号
    }
    
    // LIN 从机相关初始化
    if( GV_LinMode == LIN_MODE_SLAVE ){
        // TODO...
    }
}


/**
 * LIN 计算校验码
 *
 * @param data    数据指针
 * @param length  数据数组长度
 * @param start   数据计算的起始索引
 *
 * @return u8     数据字节
 */
u8 LIN_CalcChecknum( u8 *bytes, short length, short start )
{
    // 初始化
    short checknum = 0;

    // 计算校验码并返回
    for( short i = start; i < length; i++ ) checknum += bytes[i];
    return (u8)(checknum%0xFF)^0xFF;
}