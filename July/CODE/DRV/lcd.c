#include "common.h"



/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */

/**
 * 主逻辑：初始化
 */
void LCD_MainInit()
{
    LCD_MSP3520_Init(LCD_COLOR_WHITE);
}


/**
 * 主逻辑：显示 LOGO
 */
void LCD_MainLogo()
{
    //static int i = 0; 
    //LCD_MSP3520_ShowImage(0, 0, 160, 80, GV_LcdPicLogoKede, i?false:true);  // 节约单片机 ROM 空间，默认注释掉
    //delay_ms(1000);
}


/**
 * 主逻辑：电机运行监视
 */
void LCD_MainMotorMonitor()
{
    // 初始化变量
    short i, x, y, len;
    short strSpace  = 5;
    short lineSpace = 5;
    short fontSize  = 14;
        
    // 初始化颜色
    u16 color = LCD_COLOR_BLACK;
    u16 background = LCD_COLOR_WHITE;
    
    // 获取圈数和角度
    long   circle = MOTOR_GetCountCircle();
    double degree = MOTOR_GetCountDegree();
    double length = MOTOR_CIRCLE_LEN*circle+MOTOR_CIRCLE_LEN/360.0*degree;

    // 取正角度值
    degree = degree >= 0 ? degree : (360+degree);

    // 开启 SPI
    LCD_MSP3520_SpiOpenScreen();
    
    // 显示圈数
    x = strSpace; y = lineSpace;
    LCD_MSP3520_ShowChinese(x, y, "圈数：", fontSize, color, background, false);
    
    x = strSpace+3*fontSize;
    len = LCD_MSP3520_ShowInteger(x, y, circle, fontSize, color, background, true, 0);
    
    x = strSpace + 3*fontSize + len*(fontSize/2);
    for( i = 0; i < 6-len; i++ ) LCD_MSP3520_ShowChar(x+i*(fontSize/2), y, ' ', fontSize, color, background, true);  // 抹去可能遗留的残影

    
    // 显示角度
    x = strSpace; y = lineSpace*2+fontSize;
    LCD_MSP3520_ShowChinese(x, y, "角度：", fontSize, color, background, false);
    
    x = strSpace + 3*fontSize;
    len = LCD_MSP3520_ShowFloat(x, y, degree, fontSize, color, background, true, 0);
    
    x = strSpace + 3*fontSize + len*(fontSize/2);
    LCD_MSP3520_ShowChinese(x, y, "°", fontSize, color, background, true);  // 显示单位
    
    x = strSpace + 3*fontSize + (len+1)*(fontSize/2);
    for( i = 0; i < 6-len; i++ ) LCD_MSP3520_ShowChar(x+i*(fontSize/2), y, ' ', fontSize, color, background, true);  // 抹去可能遗留的残影
    

    // 显示行进
    x = strSpace; y = lineSpace*3+fontSize*2;
    LCD_MSP3520_ShowChinese(x, y, "行进：", fontSize, color, background, false);
    
    x = strSpace + 3*fontSize;
    len = LCD_MSP3520_ShowFloat(x, y, length, fontSize, color, background, true, 0);
    
    x = strSpace + 3*fontSize + len*(fontSize/2) + 1;
    LCD_MSP3520_ShowString(x, y, "mm", fontSize, color, background, true);  // 显示单位
    
    x = strSpace + 3*fontSize + (len+2)*(fontSize/2) + 1;
    LCD_MSP3520_DrawLine(x+1, y, x+1, y+fontSize, background);
    for( i = 0; i < 6-len; i++ ) LCD_MSP3520_ShowChar(x+i*(fontSize/2), y, ' ', fontSize, color, background, true);  // 抹去可能遗留的残影
            
    // 关闭 SPI
    LCD_MSP3520_SpiClose();
}


/**
 * 主逻辑：显示动态图标
 */
void LCD_MainAnimateIcons()
{
    // 初始化
    static u8 i = 0;

    // 开启 SPI
    LCD_MSP3520_SpiOpenScreen();
    
    // 绘制图标
    if( i ){ 
        LCD_MSP3520_ShowIcon(0,   0, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(0,  17, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(0,  34, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(0,  51, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(0,  68, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(0,  85, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false);
        
        LCD_MSP3520_ShowIcon(20,  0, LCD_ICON_SUN_ON, 24, LCD_COLOR_DBLUE  , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(20, 25, LCD_ICON_SUN_ON, 24, LCD_COLOR_DBLUE  , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(20, 50, LCD_ICON_SUN_ON, 24, LCD_COLOR_DBLUE  , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(20, 75, LCD_ICON_SUN_ON, 24, LCD_COLOR_DBLUE  , LCD_COLOR_WHITE, false); 
        
        LCD_MSP3520_ShowIcon(45,  0, LCD_ICON_SUN_ON, 32, LCD_COLOR_DYELLOW, LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(45, 33, LCD_ICON_SUN_ON, 32, LCD_COLOR_DYELLOW, LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(45, 66, LCD_ICON_SUN_ON, 32, LCD_COLOR_DYELLOW, LCD_COLOR_WHITE, false); 
        
        i = 0;
    }
    else{ 
        LCD_MSP3520_ShowIcon(0,   0, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(0,  17, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(0,  34, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(0,  51, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(0,  68, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(0,  85, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true);
        
        LCD_MSP3520_ShowIcon(20,  0, LCD_ICON_SUN_OFF, 24, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(20, 25, LCD_ICON_SUN_OFF, 24, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(20, 50, LCD_ICON_SUN_OFF, 24, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(20, 75, LCD_ICON_SUN_OFF, 24, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        
        LCD_MSP3520_ShowIcon(45,  0, LCD_ICON_SUN_OFF, 32, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(45, 33, LCD_ICON_SUN_OFF, 32, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(45, 66, LCD_ICON_SUN_OFF, 32, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        
        i = 1;
    }
        
    // 关闭 SPI
    LCD_MSP3520_SpiClose();
}


/**
 * 主逻辑：显示大量颜色的圆 
 *
 * @param bool fill 是否填充
 */
void LCD_MainColorCircles( bool fill )
{
    // 开启 SPI
    LCD_MSP3520_SpiOpenScreen();
    
    // 清屏
    LCD_MSP3520_Fill(0, 0, 200, 200, LCD_COLOR_WHITE);
    
    // 绘制圆圈
    LCD_MSP3520_DrawCircle( 0 , 0, 8, LCD_COLOR_DRED     , fill, 1);
    LCD_MSP3520_DrawCircle( 20, 0, 8, LCD_COLOR_DBLUE    , fill, 2);
    LCD_MSP3520_DrawCircle( 40, 0, 8, LCD_COLOR_DGREEN   , fill, 3);
    LCD_MSP3520_DrawCircle( 60, 0, 8, LCD_COLOR_DMAGENTA , fill, 4);
    LCD_MSP3520_DrawCircle( 80, 0, 8, LCD_COLOR_DYELLOW  , fill, 5);
    LCD_MSP3520_DrawCircle(100, 0, 8, LCD_COLOR_DCYAN    , fill, 6);
    LCD_MSP3520_DrawCircle(120, 0, 8, LCD_COLOR_DORANGE  , fill, 7);
    LCD_MSP3520_DrawCircle(140, 0, 8, LCD_COLOR_DGRAY    , fill, 8);
    
    LCD_MSP3520_DrawCircle(  0, 20, 8, LCD_COLOR_RED     , fill, 8);
    LCD_MSP3520_DrawCircle( 20, 20, 8, LCD_COLOR_BLUE    , fill, 7);
    LCD_MSP3520_DrawCircle( 40, 20, 8, LCD_COLOR_GREEN   , fill, 6);
    LCD_MSP3520_DrawCircle( 60, 20, 8, LCD_COLOR_MAGENTA , fill, 5);
    LCD_MSP3520_DrawCircle( 80, 20, 8, LCD_COLOR_YELLOW  , fill, 4);
    LCD_MSP3520_DrawCircle(100, 20, 8, LCD_COLOR_CYAN    , fill, 3);
    LCD_MSP3520_DrawCircle(120, 20, 8, LCD_COLOR_ORANGE  , fill, 2);
    LCD_MSP3520_DrawCircle(140, 20, 8, LCD_COLOR_GRAY    , fill, 1);
    
    LCD_MSP3520_DrawCircle(  0, 40, 8, LCD_COLOR_LRED    , fill, 1);
    LCD_MSP3520_DrawCircle( 20, 40, 8, LCD_COLOR_LBLUE   , fill, 2);
    LCD_MSP3520_DrawCircle( 40, 40, 8, LCD_COLOR_LGREEN  , fill, 3);
    LCD_MSP3520_DrawCircle( 60, 40, 8, LCD_COLOR_LMAGENTA, fill, 4);
    LCD_MSP3520_DrawCircle( 80, 40, 8, LCD_COLOR_LYELLOW , fill, 5);
    LCD_MSP3520_DrawCircle(100, 40, 8, LCD_COLOR_LCYAN   , fill, 6);
    LCD_MSP3520_DrawCircle(120, 40, 8, LCD_COLOR_LORANGE , fill, 7);
    LCD_MSP3520_DrawCircle(140, 40, 8, LCD_COLOR_LGRAY   , fill, 8);
    
    // 关闭 SPI
    LCD_MSP3520_SpiClose();
}



/* -------------------------------------------------- */
// ● 主逻辑代码：LED 场景
// ● 基于 MSP3520(480*320) 横屏模式下的布局
/* -------------------------------------------------- */

/**
 * 主逻辑：LED 场景（全局变量）
 */
short GV_MainLedSceneLedTotal         = 32;  //【按需设置】LED总数（根据需要自行设置，但最大不能超过 40 个）
short GV_MainLedSceneTitleHeight      = 80;  //【推荐默认】标题高度
short GV_MainLedSceneRowTotal         = 4;   //【推荐默认】LED 行总数       （32个LED以内推荐值：4 ，32个LED以上推荐值：5 ）
short GV_MainLedSceneLedWidth         = 50;  //【推荐默认】LED 宽度         （32个LED以内推荐值：50，32个LED以上推荐值：58）
short GV_MainLedSceneLedHeight        = 50;  //【推荐默认】LED 高度         （32个LED以内推荐值：50，32个LED以上推荐值：46）
short GV_MainLedSceneNumberSize       = 24;  //【推荐默认】LED 序号字体大小
short GV_MainLedSceneLedMarginTop     = 10;   //【推荐默认】LED Margin Top   （32个LED以内推荐值：10，32个LED以上推荐值：2）
short GV_MainLedSceneLedMarginLeft    = 9;   //【推荐默认】LED Margin Left  （32个LED以内推荐值：9 ，32个LED以上推荐值：2）

short GV_MainLedSceneLedStates[40]    = {0};  //【系统使用】LED 灯集合状态
short GV_MainLedSceneColumnTotal      = 0;    //【推荐默认】LED 列数量，为 0 时该值由 LCD_MainLedScene() 进行动态设置，否则按照自定义的固定值
short GV_MainLedSceneLedSetMarginLeft = 0;    //【推荐默认】LED 集合 Margin Left（主要用于 LED 集合居中显示），为 0 时该值由 LCD_MainLedScene() 进行动态设置，否则按照自定义的固定值


/**
 * 主逻辑：LED 场景（初始化）  
 */
void LCD_MainLedSceneInit()
{
    // 初始化
    u8 i, j, num;
    
    // LCD 初始化
    LCD_MSP3520_Init(LCD_COLOR_WHITE);
    
    // LED 列数量
    // 动态初始化：默认采用以显示 4 行 LED 时最合理的列数
    if( GV_MainLedSceneColumnTotal == 0 ) GV_MainLedSceneColumnTotal = GV_MainLedSceneLedTotal/GV_MainLedSceneRowTotal+(GV_MainLedSceneLedTotal%GV_MainLedSceneRowTotal>=1?1:0);

    // LED 居中显示时 LED 集合 Margin Left 的值
    if( GV_MainLedSceneLedSetMarginLeft == 0 ) GV_MainLedSceneLedSetMarginLeft = (LCD_MSP3520_SCREEN_LONG-(GV_MainLedSceneColumnTotal*(GV_MainLedSceneLedMarginLeft+GV_MainLedSceneLedWidth)-GV_MainLedSceneLedMarginLeft))/2;
    if( GV_MainLedSceneLedSetMarginLeft <= 0 ) GV_MainLedSceneLedSetMarginLeft = 0;
    
    // 开启 SPI
    LCD_MSP3520_SpiOpenScreen();

    // 绘制 标题
    LCD_MainLedSceneDrawTitle("KD-281", "通断路测试台", "KD-281-1/2/3/4/5, KD-281-50/60", "TEST-281-T1");
    
    // 绘制 LED 集合
    for( i = 0; i < GV_MainLedSceneRowTotal; i++ ){
        for( j = 0; j < GV_MainLedSceneColumnTotal; j++ ){
            // 构建序号
            num = i*GV_MainLedSceneColumnTotal+j+1;
            if( num > GV_MainLedSceneLedTotal ) break;

            // 绘制 LED 项
            LCD_MainLedSceneDrawItem(i, j, num, false);
        }
    }

    // 关闭 SPI
    LCD_MSP3520_SpiClose();
}


/**
 * 主逻辑：LED 场景（LED 点亮）
 *
 * @param u8 number LED灯编号：编号从1开始
 */
void LCD_MainLedSceneOpen( u8 number )
{
    if( LCD_MainLedSceneState(number) == false ) LCD_MainLedSceneAction(number, 1);
}


/**
 * 主逻辑：LED 场景（LED 关闭）
 *
 * @param u8 number LED灯编号：编号从1开始
 */
void LCD_MainLedSceneClose( u8 number )
{
    if( LCD_MainLedSceneState(number) == true ) LCD_MainLedSceneAction(number, 0);
}


/**
 * 主逻辑：LED 场景（获取指定 LED 灯状态）
 *
 * @param u8 number LED灯编号：编号从1开始
 */
bool LCD_MainLedSceneState( u8 number )
{
    short state = number ? GV_MainLedSceneLedStates[number-1] : 0;
    return state ? true : false;
}


/**
 * 主逻辑：LED 场景（LED 操作）
 *
 * @param u8   number LED灯编号：编号从1开始
 * @param bool state  LED灯状态：true 表示亮，false 表示灭
 */
void LCD_MainLedSceneAction( u8 number, bool state )
{
    // 初始化
    u8 i, j, num;

    // 开启 SPI
    LCD_MSP3520_SpiOpenScreen();

    // 点亮 LED
    for( i = 0; i < GV_MainLedSceneRowTotal; i++ ){
        for( j = 0; j < GV_MainLedSceneColumnTotal; j++ ){
            // 构建序号
            num = i*GV_MainLedSceneColumnTotal+j+1;
            if( num > GV_MainLedSceneLedTotal ) break;
            if( num != number ) continue;

            // 绘制 LED 项
            LCD_MainLedSceneDrawItem(i, j, num, state);

            // 记录 LED 灯状态
            GV_MainLedSceneLedStates[number-1] = state; 
            
            // 结束
            break;
        }
    }
    
    // 关闭 SPI
    LCD_MSP3520_SpiClose();
}


/**
 * 主逻辑：LED 场景（绘制 LED 项）
 */
void LCD_MainLedSceneDrawItem( u8 row, u8 column, u8 number, bool state )
{
    // 绘制 LED
    u16 x = (GV_MainLedSceneLedWidth+GV_MainLedSceneLedMarginLeft)*column + GV_MainLedSceneLedSetMarginLeft;
    u16 y = (GV_MainLedSceneLedHeight+GV_MainLedSceneLedMarginTop)*row + GV_MainLedSceneTitleHeight;
    LCD_MSP3520_Fill(x, y, x+GV_MainLedSceneLedWidth, y+GV_MainLedSceneLedHeight, state?LCD_COLOR_ORANGE:LCD_COLOR_GRAY);  // 采用矩形（显示速度快）

    // 绘制 LED 序号
    u16 numx = x+(GV_MainLedSceneLedWidth/2-GV_MainLedSceneNumberSize*(number>=10?2:1)/4);
    u16 numy = y+(GV_MainLedSceneLedHeight/2-GV_MainLedSceneNumberSize/2);
    LCD_MSP3520_ShowNumber(numx, numy, number, GV_MainLedSceneNumberSize, LCD_COLOR_WHITE, LCD_COLOR_WHITE, false, 0);  // 白色字体
}


/**
 * 主逻辑：LED 场景（绘制 LED 标题）
 *
 * @param u8* titleEnglish  主标题：英文部分
 * @param u8* titleChinese  主标题：中文部分
 * @param u8* testProduct   测试台适用产品
 * @param u8* testSn        测试台型号
 */
void LCD_MainLedSceneDrawTitle( u8* titleEnglish, u8* titleChinese, u8* testProduct, u8* testSn )
{
    // 初始化
    u16 x, y;
    u8 fontSizeTitle = 24, fontSizeProduct = 12, fontSizeSn = 12;

    // 获取标题长度
    u8 titleEnglishLength = 0;
    while( *titleEnglish != 0 ){ titleEnglishLength++; titleEnglish+=1; }
    titleEnglish -= titleEnglishLength;
    
    u8 titleChineseLength = 0;
    while( *titleChinese != 0 ){ titleChineseLength++; titleChinese+=2; }
    titleChinese -= titleChineseLength*2;

    u8 testSnLength = 0;
    while( *testSn != 0 ){ testSnLength++; testSn+=1; }
    testSn -= testSnLength;
    
    // 构建标题坐标
    x = (LCD_MSP3520_SCREEN_LONG-titleEnglishLength*(fontSizeTitle/2)-titleChineseLength*fontSizeTitle)/2;  // 标题居中
    x = 15; // 标题居左
    y = (GV_MainLedSceneTitleHeight-GV_MainLedSceneLedMarginTop-fontSizeTitle-fontSizeProduct-5)/2;

    // 绘制标题：背景
    LCD_MSP3520_Fill(0, 0, LCD_MSP3520_SCREEN_LONG, GV_MainLedSceneTitleHeight-GV_MainLedSceneLedMarginTop, LCD_COLOR_BLACK);
    
    // 绘制标题：标题
    LCD_MSP3520_ShowString(x, y, titleEnglish, fontSizeTitle, LCD_COLOR_WHITE, LCD_COLOR_WHITE, false);
    LCD_MSP3520_ShowChinese(x+titleEnglishLength*(fontSizeTitle/2), y, titleChinese, fontSizeTitle, LCD_COLOR_WHITE, LCD_COLOR_WHITE, false);
    
    // 绘制标题：适用产品
    y = y + fontSizeTitle + 5;
    LCD_MSP3520_ShowString(x, y, testProduct, fontSizeProduct, LCD_COLOR_WHITE, LCD_COLOR_WHITE, false);

    // 绘制标题：测试台型号
    x = LCD_MSP3520_SCREEN_LONG - GV_MainLedSceneLedMarginLeft - testSnLength*(fontSizeSn/2);
    y = GV_MainLedSceneTitleHeight - fontSizeSn - GV_MainLedSceneLedMarginLeft - 5;
    LCD_MSP3520_ShowString(x, y, testSn, fontSizeSn, LCD_COLOR_DGRAY, LCD_COLOR_DGRAY, false);
}
