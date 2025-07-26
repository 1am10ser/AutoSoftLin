#include "common.h"


/* -------------------------------------------------- */
// ● LCD 相关函数
/* -------------------------------------------------- */
u16 LCD_Color565( u8 red, u8 green, u8 blue )
{
    return ( ((red &0xF8)<<0x08) | ((green&0xFC)<<0x03) | (blue>>0x03) );
}

u16 GV_MainLedSceneLedTotal      = 32;  //【按需设置】LED总数（根据需要自行设置，但最大不能超过 32 个）
u16 GV_MainLedSceneTitleHeight   = 80;  //【推荐默认】标题高度
u16 GV_MainLedSceneRowTotal      = 4;   //【推荐默认】LED 行总数
u16 GV_MainLedSceneLedRadius     = 25;  //【推荐默认】LED 圆圈半径
u16 GV_MainLedSceneNumberSize    = 24;  //【推荐默认】LED 序号字体大小
u16 GV_MainLedSceneLedMarginTop  = 10;  //【推荐默认】LED Margin Top
u16 GV_MainLedSceneLedMarginLeft = 9;   //【推荐默认】LED Margin Left

u32 GV_MainLedSceneLedStates        = 0x00000000; //【系统使用】LED 灯集合状态，最高位表示 1 号LED灯的状态，最低位表示第 32 号LED灯的状态
u16 GV_MainLedSceneColumnTotal      = 0;          //【推荐默认】LED 列数量，为 0 时该值由 LCD_MainLedScene() 进行动态设置，否则按照自定义的固定值
u16 GV_MainLedSceneLedSetMarginLeft = 0;          //【推荐默认】LED 集合 Margin Left（主要用于 LED 集合居中显示），为 0 时该值由 LCD_MainLedScene() 进行动态设置，否则按照自定义的固定值


/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */


/**
 * 主逻辑：初始化
 */
void LCD_MainInit()
{
    LCD_MSP3520_Init(LCD_COLOR565_WHITE);
}

/**
 * 主逻辑：LED 场景（初始化）
 */
void LCD_MainLedScene()
{
    // 初始化

    // LED 列数量
    // 动态初始化：默认采用以显示 4 行 LED 时最合理的列数
    if( GV_MainLedSceneColumnTotal == 0 ) GV_MainLedSceneColumnTotal = GV_MainLedSceneLedTotal/GV_MainLedSceneRowTotal+(GV_MainLedSceneLedTotal%GV_MainLedSceneRowTotal>=1?1:0);

    // LED 居中显示时 LED 集合 Margin Left 的值
    if( GV_MainLedSceneLedSetMarginLeft == 0 ) GV_MainLedSceneLedSetMarginLeft = (LCD_MSP3520_SCREEN_LONG-(GV_MainLedSceneColumnTotal*(GV_MainLedSceneLedMarginLeft+GV_MainLedSceneLedRadius*2)-GV_MainLedSceneLedMarginLeft))/2;  
    
    // 开启 SPI
    LCD_MSP3520_SpiOpenScreen();

    // 绘制 标题
    LCD_MainLedSceneDrawTitle("KD-2512", "成品测试台", "KD-2512", "TEST-2512-T1");
    
    
    // 关闭 SPI
    LCD_MSP3520_SpiClose();
}


void LCD_MainLedSceneDrawTitle( u8* titleEnglish, u8* titleChinese, u8* testProduct, u8* testSn )
{
    // 初始化
    u16 x, y;
    u8 fontSize = 24, fontSizeProduct = 12, fontSizeSn = 12;

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
    x = (LCD_MSP3520_SCREEN_LONG-titleEnglishLength*(fontSize/2)-titleChineseLength*fontSize)/2;  // 标题居中
    x = 15; // 标题居左
    y = (GV_MainLedSceneTitleHeight-GV_MainLedSceneLedMarginTop-fontSize-fontSizeProduct-5)/2;

    // 绘制标题：背景
    LCD_MSP3520_Fill(0, 0, LCD_MSP3520_SCREEN_LONG, GV_MainLedSceneTitleHeight-GV_MainLedSceneLedMarginTop, LCD_COLOR565_BLACK);
    
    // 绘制标题：标题
    LCD_MSP3520_ShowString(x, y, titleEnglish, fontSize, LCD_COLOR565_WHITE, LCD_COLOR565_WHITE, false);
    LCD_MSP3520_ShowChinese(x+titleEnglishLength*(fontSize/2), y, titleChinese, fontSize, LCD_COLOR565_WHITE, LCD_COLOR565_WHITE, false);
    
    // 绘制标题：适用产品
    y = y + fontSize + 5;
    LCD_MSP3520_ShowString(x, y, testProduct, fontSizeProduct, LCD_COLOR565_WHITE, LCD_COLOR565_WHITE, false);

    // 绘制标题：测试台型号
    x = LCD_MSP3520_SCREEN_LONG - GV_MainLedSceneLedMarginLeft - testSnLength*(fontSizeSn/2);
    y = GV_MainLedSceneTitleHeight - fontSizeSn - GV_MainLedSceneLedMarginLeft - 5;
    LCD_MSP3520_ShowString(x, y, testSn, fontSizeSn, LCD_COLOR565_DGRAY, LCD_COLOR565_DGRAY, false);
}

void LCD_360ADCoordinate(u16 color)
{
    unsigned int CZ,CK,i;
    i = 0;
    CZ = GV_DisplayX;
    CK = GV_DisplayY;
    LCD_MSP3520_SpiOpenScreen();    
    
//    LCD_MSP3520_Fill(GV_InitialX + 10,GV_InitialY - 190,GV_InitialX + 30,GV_InitialY - 170,LCD_COLOR565_BLUE);
//    LCD_MSP3520_Fill(GV_InitialX + 40,GV_InitialY - 190,GV_InitialX + 60,GV_InitialY - 170,LCD_COLOR565_BLUE);
    
    for(i = 0;i < 40;i++)
    {
        /*if(i < CONNECT_LEDQuantity)*/LCD_MSP3520_Fill(GV_InitialX + CZ,GV_InitialY - CK,GV_InitialX + GV_ADDXY + CZ,GV_InitialY - CK + GV_ADDXY,LCD_COLOR565_GRAY);
//        else if(i > CONNECT_LEDQuantity - 1)LCD_MSP3520_DrawCircle(GV_InitialX + CZ,GV_InitialY - CK,20,LCD_COLOR565_GRAY,1,1);
        LCD_MSP3520_ShowChar(GV_InitialX + CZ + 6,GV_InitialY - CK + 8, '0' + ((i+1) / 10), 24, LCD_COLOR565_WHITE, LCD_COLOR565_GRAY, 0);
        LCD_MSP3520_ShowChar(GV_InitialX + CZ + 18,GV_InitialY - CK + 8, '0' + ((i+1) % 10), 24, LCD_COLOR565_WHITE, LCD_COLOR565_GRAY, 0);
        CZ = CZ + GV_ADDX;
        if(CZ == GV_ADDX * 8 + GV_DisplayX)
        {
            CZ = GV_DisplayX;
            CK = CK - GV_ADDY;
        }
    }
//    LCD_MSP3520_Fill(GV_InitialX + 10,GV_InitialY - 190,GV_InitialX + 30,GV_InitialY - 170,LCD_COLOR565_BLUE);
    
//    LCD_MSP3520_DrawLine(GV_InitialX - 10,GV_InitialY,GV_InitialX + 410,GV_InitialY,color);
//    LCD_MSP3520_DrawLine(GV_InitialX,GV_InitialY + 10,GV_InitialX,GV_InitialY - 210,color);
//    
////    LCD_MSP3520_ShowChar(GV_InitialX - 20,GV_InitialY + 5,'2',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX - 14,GV_InitialY + 5,'0',12,color,0,0);
////    LCD_MSP3520_ShowChar(GV_InitialX - 11,GV_InitialY + 5,'5',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX - 5,GV_InitialY - 40,GV_InitialX,GV_InitialY - 40,color);
//    LCD_MSP3520_ShowChar(GV_InitialX - 25,GV_InitialY - 43,'2',12,color,0,0);
////    LCD_MSP3520_ShowChar(GV_InitialX - 19,GV_InitialY - 43,'.',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX - 16,GV_InitialY - 43,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX - 5,GV_InitialY - 80,GV_InitialX,GV_InitialY - 80,color);
//    LCD_MSP3520_ShowChar(GV_InitialX - 25,GV_InitialY - 83,'4',12,color,0,0);
////    LCD_MSP3520_ShowChar(GV_InitialX - 19,GV_InitialY - 83,'.',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX - 16,GV_InitialY - 83,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX - 5,GV_InitialY - 120,GV_InitialX,GV_InitialY - 120,color);
//    LCD_MSP3520_ShowChar(GV_InitialX - 25,GV_InitialY - 123,'6',12,color,0,0);
////    LCD_MSP3520_ShowChar(GV_InitialX - 19,GV_InitialY - 123,'.',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX - 16,GV_InitialY - 123,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX - 5,GV_InitialY - 160,GV_InitialX,GV_InitialY - 160,color);
//    LCD_MSP3520_ShowChar(GV_InitialX - 25,GV_InitialY - 163,'8',12,color,0,0);
////    LCD_MSP3520_ShowChar(GV_InitialX - 19,GV_InitialY - 163,'.',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX - 16,GV_InitialY - 163,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX - 5,GV_InitialY - 200,GV_InitialX,GV_InitialY - 200,color);
//    LCD_MSP3520_ShowChar(GV_InitialX - 25,GV_InitialY - 203,'1',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX - 19,GV_InitialY - 203,'0',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX - 13,GV_InitialY - 203,'0',12,color,0,0);
//    
//    LCD_MSP3520_ShowChar(GV_InitialX + 5,GV_InitialY - 215,'D',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 11,GV_InitialY - 215,'(',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 17,GV_InitialY - 215,'%',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 23,GV_InitialY - 215,')',12,color,0,0);
//    
////    LCD_MSP3520_ShowChar(GV_InitialX + 390,GV_InitialY + 10,' ',12,color,0,0);
//    LCD_MSP3520_ShowChineseOne(GV_InitialX + 396,GV_InitialY + 10," ",12,color,0,0);
////    LCD_MSP3520_ShowChar(GV_InitialX + 402,GV_InitialY + 10,' ',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 30,GV_InitialY,GV_InitialX + 30,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 24,GV_InitialY + 10,'3',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 30,GV_InitialY + 10,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 60,GV_InitialY,GV_InitialX + 60,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 54,GV_InitialY + 10,'6',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 60,GV_InitialY + 10,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 90,GV_InitialY,GV_InitialX + 90,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 84,GV_InitialY + 10,'9',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 90,GV_InitialY + 10,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 120,GV_InitialY,GV_InitialX + 120,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 111,GV_InitialY + 10,'1',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 117,GV_InitialY + 10,'2',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 123,GV_InitialY + 10,'0',12,color,0,0);
//
//    LCD_MSP3520_DrawLine(GV_InitialX + 150,GV_InitialY,GV_InitialX + 150,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 141,GV_InitialY + 10,'1',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 147,GV_InitialY + 10,'5',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 153,GV_InitialY + 10,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 180,GV_InitialY,GV_InitialX + 180,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 171,GV_InitialY + 10,'1',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 177,GV_InitialY + 10,'8',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 183,GV_InitialY + 10,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 210,GV_InitialY,GV_InitialX + 210,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 201,GV_InitialY + 10,'2',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 207,GV_InitialY + 10,'1',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 213,GV_InitialY + 10,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 240,GV_InitialY,GV_InitialX + 240,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 231,GV_InitialY + 10,'2',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 237,GV_InitialY + 10,'4',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 243,GV_InitialY + 10,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 270,GV_InitialY,GV_InitialX + 270,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 261,GV_InitialY + 10,'2',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 267,GV_InitialY + 10,'7',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 273,GV_InitialY + 10,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 300,GV_InitialY,GV_InitialX + 300,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 291,GV_InitialY + 10,'3',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 297,GV_InitialY + 10,'0',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 303,GV_InitialY + 10,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 330,GV_InitialY,GV_InitialX + 330,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 321,GV_InitialY + 10,'3',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 327,GV_InitialY + 10,'3',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 333,GV_InitialY + 10,'0',12,color,0,0);
//    
//    LCD_MSP3520_DrawLine(GV_InitialX + 360,GV_InitialY,GV_InitialX + 360,GV_InitialY + 5,color);
//    LCD_MSP3520_ShowChar(GV_InitialX + 351,GV_InitialY + 10,'3',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 357,GV_InitialY + 10,'6',12,color,0,0);
//    LCD_MSP3520_ShowChar(GV_InitialX + 363,GV_InitialY + 10,'0',12,color,0,0);
    
//    LCD_MSP3520_SpiClose();

}
