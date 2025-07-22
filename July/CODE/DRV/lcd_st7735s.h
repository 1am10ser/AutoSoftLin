#ifndef __LCD_ST7735S_H_
#define __LCD_ST7735S_H_



    /* -------------------------------------------------- */
    // ● LCD ST7735S 驱动的屏幕介绍
    /* -------------------------------------------------- */
    // ● 基于中景园 0.96寸 80*160 的屏幕
    // ● 该屏幕基于 LCD ST7735S 芯片驱动
    //
    // ● 竖屏时，对 ST7735S 来说 80  是列像素(x)，需要 +23 进行偏移
    // ● 竖屏时，对 ST7735S 来说 160 是行像素(y)，需要 +2  进行偏移
    // 
    // ● 横屏时，对 ST7735S 来说 80  是行像素(y)，需要 +23 进行偏移
    // ● 横屏时，对 ST7735S 来说 160 是列像素(x)，需要 +2  进行偏移
    //
    // ● 竖屏或横屏由 LCD_ST7735S_Init() 中的 “Memory Data Access Control” 段代码定义
    // ● 淘宝：https://item.taobao.com/item.htm?spm=a1z09.2.0.0.1f592e8dLHeauk&id=611057315553&_u=l9v5jidd90

    #define LCD_ST7735S_SCREEN              2

    #define LCD_ST7735S_SCREEN_SHU          1
    #define LCD_ST7735S_SCREEN_SHU_XOFFSET  24  // 竖屏时，横坐标偏移；由于每个品牌屏幕的工艺不一样，真正的原点可能有偏移，需设置修正（需要确定好原点再调偏移）
    #define LCD_ST7735S_SCREEN_SHU_YOFFSET  0   // 竖屏时，纵坐标偏移；由于每个品牌屏幕的工艺不一样，真正的原点可能有偏移，需设置修正（需要确定好原点再调偏移）

    #define LCD_ST7735S_SCREEN_HEN          2
    #define LCD_ST7735S_SCREEN_HEN_XOFFSET  0   // 横屏时，横坐标偏移；由于每个品牌屏幕的工艺不一样，真正的原点可能有偏移，需设置修正（需要确定好原点再调偏移）
    #define LCD_ST7735S_SCREEN_HEN_YOFFSET  24  // 横屏时，纵坐标偏移；由于每个品牌屏幕的工艺不一样，真正的原点可能有偏移，需设置修正（需要确定好原点再调偏移）



    /* -------------------------------------------------- */
    // ● LCD ST7735S 驱动的针脚定义
    /* -------------------------------------------------- */

    // Pin6 - 重置引脚
    // 低电平时 LCD 不工作，高电平/悬空工作
    #define LCD_ST7735S_RESET_PORT  GPIOC
    #define LCD_ST7735S_RESET_PIN   GPIO_PIN_7

    // Pin4 - SPI 时钟脚
    // 连接到单片机的 SPI_SCK 脚
    #define LCD_ST7735S_SCL_PORT    GPIOC
    #define LCD_ST7735S_SCL_PIN     GPIO_PIN_5  

    // Pin3 - SPI 输出脚
    // 连接到单片机的 SPI_MOSI 脚
    #define LCD_ST7735S_SDA_PORT    GPIOC
    #define LCD_ST7735S_SDA_PIN     GPIO_PIN_6

    // Pin5 - RS 引脚
    // 发送 命令（低电平） 或者 数据（高电平） 的控制引脚
    #define LCD_ST7735S_RS_PORT     GPIOE
    #define LCD_ST7735S_RS_PIN      GPIO_PIN_5

    // Pin7 - SPI 片选引脚
    // 低电平时 SPI 主机将于 LCD 通讯
    #define LCD_ST7735S_CS_PORT     GPIOE
    #define LCD_ST7735S_CS_PIN      GPIO_PIN_7

    // 其他 LCD 引脚单片机不直接控制
    // 重置引脚 悬空
    // 背光灯脚 Pin12(LEDA) 接 3.3V，Pin11(LEDK) 接 GND；因为无背光灯时看不出LCD显示的内容，所有默认直接点亮



    /* -------------------------------------------------- */
    // ● 函数定义
    /* -------------------------------------------------- */
    void LCD_ST7735S_Init();
    void LCD_ST7735S_InitIO();
    void LCD_ST7735S_SendCmd( u8 data );
    void LCD_ST7735S_SendByte( u8 byte );
    void LCD_ST7735S_SendData8( u8 data );
    void LCD_ST7735S_SendData16( u16 data );

    void LCD_ST7735S_Fill( u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color );
    void LCD_ST7735S_FillSetArea( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd );
    
    void LCD_ST7735S_DrawLine( u16 x1, u16 y1, u16 x2, u16 y2, u16 color );
    void LCD_ST7735S_DrawPoint( u16 x, u16 y, u16 color );
    void LCD_ST7735S_DrawCircle( u16 x, u16 y, u8 r, u16 color, bool fill, u8 border );
    void LCD_ST7735S_DrawRectangle( u16 x, u16 y, u16 xlen, u16 ylen, u16 color, bool fill, u8 border );

    void LCD_ST7735S_ShowIcon( u16 x, u16 y, u8 index, u8 size, u16 color, u16 background, u8 backfill );
    void LCD_ST7735S_ShowChar( u16 x, u16 y, u8 ascii, u8 size, u16 color, u16 background, u8 backfill );
    void LCD_ST7735S_ShowImage( u16 x, u16 y, u16 xlen, u16 ylen, const u8 image[], bool reverse );
    void LCD_ST7735S_ShowString( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, u8 backfill );
    void LCD_ST7735S_ShowChinese( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, u8 backfill );
    void LCD_ST7735S_ShowChineseOne( u16 x, u16 y, u8 *font, u8 size, u16 color, u16 background, u8 backfill );
    
    short LCD_ST7735S_ShowFloat( u16 x, u16 y, double number, u8 size, u16 color, u16 background, u8 backfill, short showlen );
    short LCD_ST7735S_ShowNumber( u16 x, u16 y, double number, u8 size, u16 color, u16 background, u8 backfill, short showlen );
    short LCD_ST7735S_ShowInteger( u16 x, u16 y, long number, u8 size, u16 color, u16 background, u8 backfill, short showlen );

    
    
#endif

