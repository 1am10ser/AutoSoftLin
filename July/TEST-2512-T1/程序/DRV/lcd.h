#ifndef __LCD_H_
#define __LCD_H_



    /* -------------------------------------------------- */
    // ● LCD 常用颜色
    // ● 565 表示红色占用5位，绿色占用6位，蓝色占用5位
    /* -------------------------------------------------- */

    #define LCD_COLOR565_WHITE       0xFFFF  // 白色
    #define LCD_COLOR565_BLACK       0x0000  // 黑色

    #define LCD_COLOR565_DGRAY       0x62CA  // 深灰色
    #define LCD_COLOR565_GRAY        0x8410  // 灰色
    #define LCD_COLOR565_LGRAY       0xD69A  // 浅灰色

    #define LCD_COLOR565_DRED        0xB800  // 深红色
    #define LCD_COLOR565_RED         0xF800  // 红色
    #define LCD_COLOR565_LRED        0xFC0E  // 淡红色

    #define LCD_COLOR565_DBLUE       0x0019  // 深蓝色
    #define LCD_COLOR565_BLUE        0x03FF  // 蓝色
    #define LCD_COLOR565_LBLUE       0xAEDC  // 淡蓝色

    #define LCD_COLOR565_DGREEN      0x2605  // 深绿色
    #define LCD_COLOR565_GREEN       0x07E0  // 绿色
    #define LCD_COLOR565_LGREEN      0x67EB  // 淡绿色

    #define LCD_COLOR565_DYELLOW     0xE5C0  // 深黄色
    #define LCD_COLOR565_YELLOW      0xFFE0  // 黄色
    #define LCD_COLOR565_LYELLOW     0xFFF3  // 淡黄色

    #define LCD_COLOR565_DMAGENTA    0x9999  // 深紫色
    #define LCD_COLOR565_MAGENTA     0xDA7F  // 紫色
    #define LCD_COLOR565_LMAGENTA    0xE67C  // 淡紫色

    #define LCD_COLOR565_DCYAN       0x0451  // 深青色
    #define LCD_COLOR565_CYAN        0x2595  // 青色
    #define LCD_COLOR565_LCYAN       0x4E99  // 淡青色

    #define LCD_COLOR565_DORANGE     0xD343  // 深橙色
    #define LCD_COLOR565_ORANGE      0xFCC0  // 橙色
    #define LCD_COLOR565_LORANGE     0xFD8C  // 淡橙色



    /* -------------------------------------------------- */
    // ● LCD 常用图标
    /* -------------------------------------------------- */
    #define LCD_ICON_SUN_ON       1  // 太阳发光
    #define LCD_ICON_SUN_OFF      0  // 太阳无光



    /* -------------------------------------------------- */
    // ● 函数定义
    /* -------------------------------------------------- */
    u16 LCD_Color565( u8 red, u8 green, u8 blue );



    /* -------------------------------------------------- */
    // ● 主逻辑函数定义
    /* -------------------------------------------------- */
    void LCD_MainInit();
    void LCD_MainLogo();
    void LCD_MainMotorMonitor();
    void LCD_MainAnimateIcons();
    void LCD_MainColorCircles( bool fill );
    
    void LCD_MainLedScene();
    void LCD_MainLedSceneOpen( u8 number );
    void LCD_MainLedSceneClose( u8 number );
    bool LCD_MainLedSceneState( u8 number );
    void LCD_MainLedSceneAction( u8 number, bool state );
    void LCD_MainLedSceneDrawItem( u8 row, u8 column, u8 number, bool state );
    void LCD_MainLedSceneDrawTitle( u8* titleEnglish, u8* titleChinese, u8* titleRemark, u8* testSn );

    void LCD_MainMagneticScene();
    void LCD_MainMagneticSceneSampling();
    void LCD_MainMagneticSceneDrawTitle( u8* titleEnglish, u8* titleChinese, u8* titleSn );
    void LCD_MainMagneticSceneDrawMagnetic( char magnetic, u16 gauss );
    
    void LCD_360ADCoordinate(u16 color);
    
#endif