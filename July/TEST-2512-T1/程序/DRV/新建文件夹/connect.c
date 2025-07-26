#include "common.h"



/* -------------------------------------------------- */
// ● CONNECT 采用 74HC164 和 74HC165 控制
// ● 使用前务必先对 74HC164 和 74HC165 进行初始化
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// ● 外部变量
/* -------------------------------------------------- */

/**
 * 74HC165 外部变量
 * 以下变量定义在 74hc165.c 文件中
 */
//extern short GV_HC165DataValues[];



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * CONNECT 基础变量
 */

//【按需设置】Connect 编号集，只有将 Connect 编号加入到这个变量后才能实现对应 Connect 的相关功能
//short GV_ConnectNumbers[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};   
short GV_ConnectNumbers[] = {1,2,3,4,5,6,7,8,9,10};   

//【系统使用】Connect 数据状态集，该数组长度固定，将 74HC165 读到的数据存入，数组数据按索引顺序存储以下数据脚的值：U10:D7~D0，U11:D7~D0，U12:D7~D0，U13:D7~D0
//【系统固值】Connect 编号映射数据集的索引，由于 Connect 编号和数据脚没有直接的对应关系，所以通过该变量进行关联。“数组索引+1”即为 Connect 编号
short GV_ConnectDataStates[] = {0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0};  
const short GV_ConnectNumMapDataStateIndexs[] = {0,1,2,3,7,6,5,4,  11,10,9,8,15,14,13,12,  19,18,17,16,23,22,21,20,  27,26,25,24,31,30,29,28};  

//【系统使用】Connect 控制状态集，该数组长度固定，将按照倒序发送给 74HC164 级联，最终数组数据按顺序映射到：U6:Q0~Q7，U7:Q0~Q7，U8:Q0~Q7，U9:Q0~Q7
//【系统固值】Connect 编号映射状态集的索引，由于 Connect 编号和控制脚没有直接的对应关系，所以通过该变量进行关联。“数组索引+1”即为 Connect 编号
short GV_ConnectControlStates[] = {0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0};  
const short GV_ConnectNumMapControlStateIndexs[] = {0,1,2,3,7,6,5,4,  8,9,10,11,15,14,13,12,  16,17,18,19,23,22,21,20,  24,25,26,27,28,29,30,31};  

// 【系统使用】Connect 控制状态集缓存
short GV_ConnectControlStatesCache[] = {-1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1,  -1,-1,-1,-1,-1,-1,-1,-1};  



///* -------------------------------------------------- */
//// ● 函数主体
///* -------------------------------------------------- */
//
///**
// * Connect 控制连通
// * 注意：使用该函数打开控制开关后记得调用 CONNECT_CtlRender() 进行渲染
// *
// * @param short number  PCB上的编号
// * @param bool  render  是否渲染
// */
//void CONNECT_CtlOpen( short number, bool render )
//{
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ) if( GV_ConnectNumbers[i] == number ) GV_ConnectControlStates[GV_ConnectNumMapControlStateIndexs[GV_ConnectNumbers[i]-1]] = 1;
//    if( render ) CONNECT_CtlRender();
//}
//
//
///**
// * Connect 控制全连通
// * 注意：使用该函数打开控制开关后记得调用 CONNECT_CtlRender() 进行渲染
// *
// * @param bool  render  是否渲染
// */
//void CONNECT_CtlOpenAll( bool render )
//{
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ) CONNECT_CtlOpen(GV_ConnectNumbers[i], false);
//    if( render ) CONNECT_CtlRender();
//}
//
//
///**
// * Connect 控制断开
// * 注意：使用该函数打开控制开关后记得调用 CONNECT_CtlRender() 进行渲染
// *
// * @param short number  PCB上的编号
// * @param bool  render  是否渲染
// */
//void CONNECT_CtlClose( short number, bool render )
//{
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ) if( GV_ConnectNumbers[i] == number ) GV_ConnectControlStates[GV_ConnectNumMapControlStateIndexs[GV_ConnectNumbers[i]-1]] = 0;
//    if( render ) CONNECT_CtlRender();
//}
//
//
///**
// * Connect 控制全断开
// * 注意：使用该函数打开控制开关后记得调用 CONNECT_CtlRender() 进行渲染
// */
//void CONNECT_CtlCloseAll( bool render )
//{
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ) CONNECT_CtlClose(GV_ConnectNumbers[i], false);
//    if( render ) CONNECT_CtlRender();
//}
//
//
///**
// * Connect 控制渲染
// * 由于 Connect 控制 采用 74HC164 控制，所以通过全局变量 GV_ConnectControlStates 记录各控制点的状态，然后一次性发送数据打开或关闭 Connect 的控制开关
// */
//void CONNECT_CtlRender()
//{
//    // 初始化
//    int i, ii;
//    u8 byte, same = 1;
// 
//    // 防止重复发送
//    for( i = 0; i < ARR_LENGTH(GV_ConnectControlStates); i++ ) same &= GV_ConnectControlStates[i] == GV_ConnectControlStatesCache[i];
//    if( same ) return;
//
//    // 构建数据并发送数据给 74HC164
//    for( i=(ARR_LENGTH(GV_ConnectControlStates)-1)/8; i >= 0; i-- ){
//        byte = 0;
//        
//        for( ii = 0; ii < 8; ii++ ){
//            byte <<= 1;
//            byte |= GV_ConnectControlStates[i*8+ii];
//        }
//        
//        // 控制 Connect 的 74HC164 编号为 6，可以参考原理图
//        HC164_SendByte(6, byte);
//    }
//        
//    // 缓存
//    for( i = 0; i < ARR_LENGTH(GV_ConnectControlStates); i++ ) GV_ConnectControlStatesCache[i] = GV_ConnectControlStates[i];
//}
//
//
///**
// * Connect 数据装载
// * 由于采用 74HC165 获取数据，所以在使用 Connect 数据相关功能时，需要使用该函数进行数据装载
// */
//void CONNECT_DatLoad()
//{
//    HC165_ReadDataValues(10); 
//    for( int i=0; i < ARR_LENGTH(GV_ConnectDataStates); i++ ) GV_ConnectDataStates[i] = GV_HC165DataValues[i];
//}
//
//
///**
// * Connect 数据是否连通
// * 注意：使用时记得通过函数 CONNECT_CtlOpen 打开或关闭 Connect 编号对应的控制开关
// * 注意：使用该函数判断 Connect 是否连通前记得调用 CONNECT_DatLoad() 进行数据装载
// *
// * @param number    PCB上的编号
// * @param autoload  自动装载数据
// *
// * @return  CONNECT_STATE_YES 表示连通中，CONNECT_STATE_NO 表示断开
// */
//short CONNECT_DatIsConnect( short number, bool autoload )
//{
//    // 自动装载数据
//    if( autoload ) CONNECT_DatLoad();
//    
//    // 返回指定 Connect 的数据连通状态
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ) if( GV_ConnectNumbers[i] == number ) return GV_ConnectDataStates[GV_ConnectNumMapDataStateIndexs[GV_ConnectNumbers[i]-1]] ? CONNECT_STATE_NO : CONNECT_STATE_YES;
//
//    // 默认返回
//    return CONNECT_STATE_NO;
//}
//
//
///**
// * Connect 数据是否连通（仅单路连通）
// * 指定路连通，其他路全部断开，那么返回 CONNECT_STATE_YES；否则返回 CONNECT_STATE_NO；
// *
// * 注意：该函数不会去判断 Connect 控制开关是否使能，直接对 Connect 数据进行判定，使用前自行按需调用 Connect 控制开关
// * 注意：使用该函数判断 Connect 是否连通前记得调用 CONNECT_DatLoad() 进行数据装载
// *
// * @param number    PCB上的编号
// * @param autoload  自动装载数据
// *
// * @return  CONNECT_STATE_YES 表示连通中，CONNECT_STATE_NO 表示断开
// */
//short CONNECT_DatIsConnectOnly( short number, bool autoload )
//{
//    // 初始化
//    short connectState;
//    short connectStateSelf   = 0;
//    short connectStateOthers = 0;
//    
//    // 自动装载数据
//    if( autoload ) CONNECT_DatLoad();
//
//    // 遍历并检测状态数据
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ){
//        connectState = CONNECT_DatIsConnect(GV_ConnectNumbers[i], false);
//        if( connectState == CONNECT_STATE_INVALID ) return CONNECT_STATE_INVALID;
//        
//        if( number == GV_ConnectNumbers[i] ) connectStateSelf = connectState == CONNECT_STATE_YES ? 1 : 0;
//        else connectStateOthers |= connectState == CONNECT_STATE_YES ? 1 : 0;
//    }
//    
//    // 返回
//    return connectStateSelf == 1 && connectStateOthers == 0 ? CONNECT_STATE_YES : CONNECT_STATE_NO;
//}
//
//
///**
// * Connect 数据是否连通（任何一路）
// * 任何某一路连通，那么返回 CONNECT_STATE_YES；否则返回 CONNECT_STATE_NO；
// *
// * 注意：该函数不会去判断 Connect 控制开关是否使能，而是直接对 Connect 数据进行判定，使用前自行按需调用 Connect 控制开关
// * 注意：使用该函数判断 Connect 是否连通前记得调用 CONNECT_DatLoad() 进行数据装载
// *
// * @param autoload  自动装载数据
// * @return  CONNECT_STATE_YES 表示连通中，CONNECT_STATE_NO 表示断开
// */
//short CONNECT_DatIsConnectAny( bool autoload )
//{
//    // 自动装载数据
//    if( autoload ) CONNECT_DatLoad();
//    
//    // 遍历检测
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ){
//        if( CONNECT_DatIsConnect(GV_ConnectNumbers[i],false) == CONNECT_STATE_YES ) return CONNECT_STATE_YES;
//    }
//    
//    // 返回未连通
//    return CONNECT_STATE_NO;
//}
//
//
//
///* -------------------------------------------------- */
//// ● 主逻辑代码
///* -------------------------------------------------- */
//
///**
// * Connect 主逻辑：连通点亮灯
// * Connect 编号的连通后那么点亮相同编号 LED 灯
// *
// * 该函数依赖于液晶屏以及液晶屏LED场景，使用前需要对液晶屏和场景进行初始化：LCD_MainInit(); LCD_MainLedScene();
// */
//void CONNECT_MainConnectMapLed()
//{
//    // 数据装载
//    CONNECT_DatLoad();
//    
//    // 开启全部控制
//    CONNECT_CtlOpenAll(true);
//
//    // 遍历连通，同时按需点亮对应编号的灯
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ){
//        if( CONNECT_DatIsConnect(GV_ConnectNumbers[i],false) == CONNECT_STATE_YES ){
//            LCD_MainLedSceneOpen(GV_ConnectNumbers[i]);
//        }else{
//            LCD_MainLedSceneClose(GV_ConnectNumbers[i]);
//        }
//    }
//}
//
//
///**
// * Connect 主逻辑：连通点亮灯并处理短路
// * Connect 编号的连通后那么点亮相同编号 LED 灯，如果短路那么不点亮
// *
// * 注意：调整好全局变量 GV_ConnectNumbers 的值，未启用的 Connect 编号需要删掉，否则检测达不到预期效果
// * 该函数依赖于液晶屏以及液晶屏LED场景，使用前需要对液晶屏和场景进行初始化：LCD_MainInit(); LCD_MainLedScene();
// */
//void CONNECT_MainConnectMapLedFilterShort()
//{
//    // 遍历连通，同时按需点亮对应编号的灯
//    for( int i=0; i < ARR_LENGTH(GV_ConnectNumbers); i++ ){  
//        // 关闭全部控制
//        CONNECT_CtlCloseAll(false);
//
//        // 开启
//        CONNECT_CtlOpen(GV_ConnectNumbers[i], true);
//
//        // Connect 单路检测并亮灯
//        if( CONNECT_DatIsConnectOnly(GV_ConnectNumbers[i],true) == CONNECT_STATE_YES ){
//            LCD_MainLedSceneOpen(GV_ConnectNumbers[i]);
//        }else{
//            LCD_MainLedSceneClose(GV_ConnectNumbers[i]);
//        }
//    }
//}
//
//
///**
// * Connect 演示：任一路连通那么启动蜂鸣器
// * 注意：调整好全局变量 GV_ConnectNumbers 的值，未启用的 Connect 编号需要删掉，否则检测达不到预期效果
// */
//void CONNECT_MainConnectAnyMapBuzzer()
//{
//    // 关闭全部控制
//    CONNECT_CtlOpenAll(true);
//
//    // 判断任一连通
////    if( CONNECT_DatIsConnectAny(true) == CONNECT_STATE_YES ){
////        BUZZER_Open(1);
////    }else{
////        BUZZER_Close(1);
////    }
//}


