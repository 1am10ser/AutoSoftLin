#include "common.h"



/* -------------------------------------------------- */
// ● KEY 采用 74HC165 控制
// ● 使用前务必先对 74HC165 进行初始化
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// ● 外部变量
/* -------------------------------------------------- */

/**
 * 74HC165 外部变量
 * 以下变量定义在 74hc165.c 文件中
 */
extern short GV_HC165DataValues[];



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * Key 基础变量
 */

//【系统使用】KEY 数据状态集，该数组长度固定，将 74HC165 读到的数据存入，数组数据按索引顺序存储以下数据脚的值：U16:D7~D0
short GV_KeyDataStates[] = {1, 1, 1, 1, 1, 1, 1, 1};  // 0 表示按下，1 表示松开；依次对应的按键编号：J51, J50, J48, J47, J46, J45, J44, J43



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * Key 数据装载
 * 由于采用 74HC165 获取数据，所以在使用 Key 数据相关功能时，需要使用该函数进行数据装载
 */
void KEY_DatLoad()
{
    HC165_ReadDataValues(16); 
    for( int i=0; i < ARR_LENGTH(GV_KeyDataStates); i++ ) GV_KeyDataStates[i] = GV_HC165DataValues[i];
}


/**
 * Key 是否按下
 *
 * @param number    PCB上的编号
 * @param autoload  自动装载数据
 *
 * @return  KEY_STATE_YES 表示连通中，KEY_STATE_NO 表示断开
 */
short KEY_IsPress( short number, bool autoload )
{
    // 自动装载数据
    if( autoload ) KEY_DatLoad();

    // 返回指定 Key 的数据连通状态
    if( number == 43 ) return GV_KeyDataStates[7] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 44 ) return GV_KeyDataStates[6] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 45 ) return GV_KeyDataStates[5] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 46 ) return GV_KeyDataStates[4] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 47 ) return GV_KeyDataStates[3] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 48 ) return GV_KeyDataStates[2] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 50 ) return GV_KeyDataStates[1] ? KEY_STATE_NO : KEY_STATE_YES;
    if( number == 51 ) return GV_KeyDataStates[0] ? KEY_STATE_NO : KEY_STATE_YES;

    // 默认返回
    return KEY_STATE_NO;
}



/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */

/**
 * 主逻辑：按下点亮灯
 *
 * 该函数依赖于液晶屏以及液晶屏LED场景，使用前需要对液晶屏LED场景进行初始化：LCD_MainLedSceneInit();
 */
void KEY_MainKeyMapLed()
{
    // 数据装载
    KEY_DatLoad();

    // 按键对应 LED 灯点亮或熄灭
    if( KEY_IsPress(43,false) == KEY_STATE_YES ){ LCD_MainLedSceneOpen(1); }else{ LCD_MainLedSceneClose(1); }
    if( KEY_IsPress(44,false) == KEY_STATE_YES ){ LCD_MainLedSceneOpen(2); }else{ LCD_MainLedSceneClose(2); }
    if( KEY_IsPress(45,false) == KEY_STATE_YES ){ LCD_MainLedSceneOpen(3); }else{ LCD_MainLedSceneClose(3); }
    if( KEY_IsPress(46,false) == KEY_STATE_YES ){ LCD_MainLedSceneOpen(4); }else{ LCD_MainLedSceneClose(4); }
    if( KEY_IsPress(47,false) == KEY_STATE_YES ){ LCD_MainLedSceneOpen(5); }else{ LCD_MainLedSceneClose(5); }
    if( KEY_IsPress(48,false) == KEY_STATE_YES ){ LCD_MainLedSceneOpen(6); }else{ LCD_MainLedSceneClose(6); }
    if( KEY_IsPress(50,false) == KEY_STATE_YES ){ LCD_MainLedSceneOpen(7); }else{ LCD_MainLedSceneClose(7); }
    if( KEY_IsPress(51,false) == KEY_STATE_YES ){ LCD_MainLedSceneOpen(8); }else{ LCD_MainLedSceneClose(8); }
}


/**
 * 主逻辑：按下蜂鸣器响
 */
void KEY_MainKeyMapBuzzer()
{
    // 初始化
    u8 press = 0;
  
    // 数据装载
    KEY_DatLoad();

    // 按键对应 LED 灯点亮或熄灭
    if( KEY_IsPress(43,false) == KEY_STATE_YES ) press = 1;
    if( KEY_IsPress(44,false) == KEY_STATE_YES ) press = 1;
    if( KEY_IsPress(45,false) == KEY_STATE_YES ) press = 1;
    if( KEY_IsPress(46,false) == KEY_STATE_YES ) press = 1;
    if( KEY_IsPress(47,false) == KEY_STATE_YES ) press = 1;
    if( KEY_IsPress(48,false) == KEY_STATE_YES ) press = 1;
    if( KEY_IsPress(50,false) == KEY_STATE_YES ) press = 1;
    if( KEY_IsPress(51,false) == KEY_STATE_YES ) press = 1;
    
    // 蜂鸣器响
    press ? BUZZER_Open(1) : BUZZER_Close(1);
}