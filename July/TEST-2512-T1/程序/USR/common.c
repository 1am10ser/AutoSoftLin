#include "common.h"




/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 毫秒级的延迟函数
 *
 * @param integer millisec  毫秒数
 */
void delay_ms( short millisec )
{
    if( millisec <= 0 ) return;
    while( millisec-- ) delay_us(1000);
}

/**
 * 微秒级的延迟函数
 * 该函数是基于 STM8 16M(1分频) 晶振时实验得出的大概循环次数编写而成
 * 该函数并不是非常精准的计数，容易受定时器、中断等影响，可用于要求不高的延时逻辑
 *
 * 注意：如果 microsec 大于 1000，那么需要直接使用 delay_ms()，否则计数将更加不准
 * 原因：多个嵌套的 while 循环可能需要启动时间
 *       就像 FTP 传送 “1 个文件（2G）” 的时间小于 “1000个文件（共2G）” 一样（因为1000个文件需要1000次的握手连接时间）
 *
 * @param integer microsec  微秒数
 */
void delay_us( short microsec )
{
    if( microsec <= 0 ) return;
    microsec *= 3; while(--microsec);
}