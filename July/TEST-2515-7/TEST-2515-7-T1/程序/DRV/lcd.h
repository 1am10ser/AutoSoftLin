#ifndef __LCD_H_
#define __LCD_H_



    /* -------------------------------------------------- */
    // ● LCD 常用颜色
    // ● 采用 565 表示颜色：红色占5位，绿色占6位，蓝色占5位
    /* -------------------------------------------------- */

    #define LCD_COLOR_WHITE       0xFFFF  // 白色
    #define LCD_COLOR_BLACK       0x0000  // 黑色

    #define LCD_COLOR_DGRAY       0x62CA  // 深灰色
    #define LCD_COLOR_GRAY        0x8410  // 灰色
    #define LCD_COLOR_LGRAY       0xD69A  // 浅灰色

    #define LCD_COLOR_DRED        0xB800  // 深红色
    #define LCD_COLOR_RED         0xF800  // 红色
    #define LCD_COLOR_LRED        0xFC0E  // 淡红色

    #define LCD_COLOR_DBLUE       0x0019  // 深蓝色
    #define LCD_COLOR_BLUE        0x03FF  // 蓝色
    #define LCD_COLOR_LBLUE       0xAEDC  // 淡蓝色

    #define LCD_COLOR_DGREEN      0x2605  // 深绿色
    #define LCD_COLOR_GREEN       0x07E0  // 绿色
    #define LCD_COLOR_LGREEN      0x67EB  // 淡绿色

    #define LCD_COLOR_DYELLOW     0xE5C0  // 深黄色
    #define LCD_COLOR_YELLOW      0xFFE0  // 黄色
    #define LCD_COLOR_LYELLOW     0xFFF3  // 淡黄色

    #define LCD_COLOR_DMAGENTA    0x9999  // 深紫色
    #define LCD_COLOR_MAGENTA     0xDA7F  // 紫色
    #define LCD_COLOR_LMAGENTA    0xE67C  // 淡紫色

    #define LCD_COLOR_DCYAN       0x0451  // 深青色
    #define LCD_COLOR_CYAN        0x2595  // 青色
    #define LCD_COLOR_LCYAN       0x4E99  // 淡青色

    #define LCD_COLOR_DORANGE     0xD343  // 深橙色
    #define LCD_COLOR_ORANGE      0xFCC0  // 橙色
    #define LCD_COLOR_LORANGE     0xFD8C  // 淡橙色



    /* -------------------------------------------------- */
    // ● LCD 常用图标
    /* -------------------------------------------------- */
    #define LCD_ICON_SUN_ON       1  // 太阳发光
    #define LCD_ICON_SUN_OFF      0  // 太阳无光



    /* -------------------------------------------------- */
    // ● 函数声明（主逻辑）
    /* -------------------------------------------------- */
    void LCD_MainInit();                     // 主逻辑：初始化
    void LCD_MainLogo();                     // 主逻辑：显示 LOGO
    void LCD_MainMotorMonitor();             // 主逻辑：电机运行监视
    void LCD_MainAnimateIcons();             // 主逻辑：显示动态图标
    void LCD_MainColorCircles( bool fill );  // 主逻辑：显示大量颜色的圆 
    
    void LCD_MainLedSceneInit();                           // 主逻辑：LED 场景（初始化）  
    void LCD_MainLedSceneOpen( u8 number );                // 主逻辑：LED 场景（LED 点亮）
    void LCD_MainLedSceneClose( u8 number );               // 主逻辑：LED 场景（LED 关闭）
    bool LCD_MainLedSceneState( u8 number );               // 主逻辑：LED 场景（获取指定 LED 灯状态）
    void LCD_MainLedSceneAction( u8 number, bool state );  // 主逻辑：LED 场景（LED 操作）
    void LCD_MainLedSceneDrawItem( u8 row, u8 column, u8 number, bool state );                          // 主逻辑：LED 场景（绘制 LED 项）
    void LCD_MainLedSceneDrawTitle( u8* titleEnglish, u8* titleChinese, u8* titleRemark, u8* testSn );  // 主逻辑：LED 场景（绘制 LED 标题）
    
    
    
#endif