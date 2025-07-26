#ifndef __LCD_H_
#define __LCD_H_



    /* -------------------------------------------------- */
    // ● LCD 常用颜色
    // ● 565 表示红色占用5位，绿色占用6位，蓝色占用5位
    /* -------------------------------------------------- */

    #define LCD_COLOR565_WHITE       0xFFFF  // 白色
    #define LCD_COLOR565_BLACK       0x0000  // 黑色

    #define LCD_COLOR565_DGRAY       0x8430  // 深灰色
    #define LCD_COLOR565_GRAY        0xAD55  // 灰色
    #define LCD_COLOR565_LGRAY       0xC618  // 浅灰色

    #define LCD_COLOR565_DRED        0xB800  // 深红色
    #define LCD_COLOR565_RED         0xF800  // 红色
    #define LCD_COLOR565_LRED        0xFC0E  // 淡红色

    #define LCD_COLOR565_DBLUE       0x0019  // 深蓝色
    #define LCD_COLOR565_BLUE        0x001F  // 蓝色
    #define LCD_COLOR565_LBLUE       0x03FF	 // 淡蓝色

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
    // ● 结构定义
    /* -------------------------------------------------- */

    /**
     * LCD 汉字字模结构体
     */
    struct STRUCT_LCD_CHINESE12{ unsigned char Index[2]; unsigned char Masks[24];  }; 
    struct STRUCT_LCD_CHINESE14{ unsigned char Index[2]; unsigned char Masks[28];  }; 
    struct STRUCT_LCD_CHINESE16{ unsigned char Index[2]; unsigned char Masks[32];  }; 
    struct STRUCT_LCD_CHINESE24{ unsigned char Index[2]; unsigned char Masks[72];  }; 
    struct STRUCT_LCD_CHINESE32{ unsigned char Index[2]; unsigned char Masks[128]; }; 



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
    

    
#endif