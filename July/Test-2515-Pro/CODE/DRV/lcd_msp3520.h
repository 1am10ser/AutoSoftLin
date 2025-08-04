#ifndef __LCD_MSP3520_H_
#define __LCD_MSP3520_H_



    /* -------------------------------------------------- */
    // ● LCD 部分针脚采用 IOEXT 控制
    // ● 使用前务必先对 IOEXT 进行初始化
    /* -------------------------------------------------- */
    // ● 所有配置或者绘图都需要调用 LCD_MSP3520_SpiOpenScreen() 和 LCD_MSP3520_SpiClose();
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // ● LCD MSP3520 驱动的屏幕介绍
    /* -------------------------------------------------- */
    // ● 基于 MSP3520 3.5寸 480*320 的屏幕
    // ● 该屏幕基于 LCD ILI9488 芯片驱动
    //
    // ● 竖屏时，对 MSP3520 来说 320 是列像素(x)，需要 +.. 进行偏移
    // ● 竖屏时，对 MSP3520 来说 480 是行像素(y)，需要 +.. 进行偏移
    // 
    // ● 横屏时，对 MSP3520 来说 320 是行像素(y)，需要 +.. 进行偏移
    // ● 横屏时，对 MSP3520 来说 480 是列像素(x)，需要 +.. 进行偏移
    //
    // ● 竖屏或横屏由 LCD_MSP3520_Init() 中的 “Memory Data Access Control” 段代码定义
    // ● 淘宝：http://www.lcdwiki.com/zh/3.5inch_SPI_Module_ILI9488_SKU:MSP3520

    #define LCD_MSP3520_SCREEN_SHU          1
    #define LCD_MSP3520_SCREEN_SHU_XOFFSET  0   // 竖屏时，横坐标偏移；由于每个品牌屏幕的工艺不一样，真正的原点可能有偏移，需设置修正（需要确定好原点再调偏移）
    #define LCD_MSP3520_SCREEN_SHU_YOFFSET  0   // 竖屏时，纵坐标偏移；由于每个品牌屏幕的工艺不一样，真正的原点可能有偏移，需设置修正（需要确定好原点再调偏移）

    #define LCD_MSP3520_SCREEN_HEN          2
    #define LCD_MSP3520_SCREEN_HEN_XOFFSET  0   // 横屏时，横坐标偏移；由于每个品牌屏幕的工艺不一样，真正的原点可能有偏移，需设置修正（需要确定好原点再调偏移）
    #define LCD_MSP3520_SCREEN_HEN_YOFFSET  0   // 横屏时，纵坐标偏移；由于每个品牌屏幕的工艺不一样，真正的原点可能有偏移，需设置修正（需要确定好原点再调偏移）
    
    #define LCD_MSP3520_SCREEN_LONG         480  // 液晶屏长边像素
    #define LCD_MSP3520_SCREEN_SHORT        320  // 液晶屏短边像素

    #define LCD_MSP3520_SCREEN              LCD_MSP3520_SCREEN_HEN



    /* -------------------------------------------------- */
    // ● LCD MSP3520 驱动的针脚定义
    /* -------------------------------------------------- */

    // Pin7 - SPI 时钟脚
    // 连接到单片机的 SPI_SCK 脚
    #define LCD_MSP3520_SCK_PORT    GPIOC
    #define LCD_MSP3520_SCK_PIN     GPIO_PIN_5  

    // Pin6 - SPI 输出脚
    // 连接到单片机的 SPI_MOSI 脚
    #define LCD_MSP3520_SDI_PORT    GPIOC
    #define LCD_MSP3520_SDI_PIN     GPIO_PIN_6



    /* -------------------------------------------------- */
    // ● LCD MSP3520 SPI 相关设备
    /* -------------------------------------------------- */
    #define LCD_MSP3520_SPI_DEVICE_TOUCH    1
    #define LCD_MSP3520_SPI_DEVICE_SCREEN   2
    #define LCD_MSP3520_SPI_DEVICE_STORAGE  3



    /* -------------------------------------------------- */
    // ● 函数定义（API）
    /* -------------------------------------------------- */
    void LCD_MSP3520_Init( u16 background );       // 初始化函数
    void LCD_MSP3520_InitIO();                     // 初始化 LCD MSP3520 基础 IO

    void LCD_MSP3520_SpiClose();                   // LCD 关闭 SPI
    void LCD_MSP3520_SpiOpenTouch();               // LCD 打开触摸 SPI
    void LCD_MSP3520_SpiOpenScreen();              // LCD 打开屏幕 SPI
    void LCD_MSP3520_SpiOpenStorage();             // LCD 打开存储 SPI

    void LCD_MSP3520_SendCmd( u8 data );           // 向 LCD MSP3520 发送命令
    void LCD_MSP3520_SendByte( u8 byte );          // 向 LCD MSP3520 发送数据（基础）
    void LCD_MSP3520_SendData8( u8 data );         // 向 LCD MSP3520 发送数据（封装）
    void LCD_MSP3520_SendData16( u16 data );       // 向 LCD MSP3520 发送数据（封装）
    void LCD_MSP3520_SendColor( u16 color );       // 向 LCD MSP3520 发送颜色数据（正常模式）
    void LCD_MSP3520_SendColorSpeed( u16 color );  // 向 LCD MSP3520 发送颜色数据（极速模式）
    
    void LCD_MSP3520_Fill( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd, u16 color );  // 填充的区域
    void LCD_MSP3520_FillSetArea( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd );      // 设置即将填充的区域
    
    void LCD_MSP3520_DrawLine( u16 x1, u16 y1, u16 x2, u16 y2, u16 color );                               // 画线
    void LCD_MSP3520_DrawPoint( u16 x, u16 y, u16 color );                                                // 画点
    void LCD_MSP3520_DrawCircle( u16 x, u16 y, u8 r, u16 color, bool fill, u8 border );                   // 画圆          
    void LCD_MSP3520_DrawRectangle( u16 x, u16 y, u16 xlen, u16 ylen, u16 color, bool fill, u8 border );  // 画矩形

    void LCD_MSP3520_ShowIcon( u16 x, u16 y, u8 index, u8 size, u16 color, u16 background, short backfill );        // 显示图标
    void LCD_MSP3520_ShowChar( u16 x, u16 y, u8 ascii, u8 size, u16 color, u16 background, short backfill );        // 显示字符
    void LCD_MSP3520_ShowImage( u16 x, u16 y, u16 xlen, u16 ylen, const u8 image[], bool reverse );                 // 显示图片
    void LCD_MSP3520_ShowString( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, short backfill );     // 显示字符串
    void LCD_MSP3520_ShowChinese( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, short backfill );    // 显示汉字
    void LCD_MSP3520_ShowChineseOne( u16 x, u16 y, u8 *font, u8 size, u16 color, u16 background, short backfill );  // 显示单汉字
    
    short LCD_MSP3520_ShowHex( u16 x, u16 y, u8 number, u8 size, u16 color, u16 background, short backfill, bool showfix );          // 显示十六进制数
    short LCD_MSP3520_ShowFloat( u16 x, u16 y, double number, u8 size, u16 color, u16 background, short backfill, short showlen );   // 显示浮点数
    short LCD_MSP3520_ShowNumber( u16 x, u16 y, double number, u8 size, u16 color, u16 background, short backfill, short showlen );  // 显示数字
    short LCD_MSP3520_ShowInteger( u16 x, u16 y, long number, u8 size, u16 color, u16 background, short backfill, short showlen );   // 显示整型

    
    
#endif