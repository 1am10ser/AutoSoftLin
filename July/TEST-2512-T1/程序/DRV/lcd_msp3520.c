#include "common.h"



/* -------------------------------------------------- */
// ● LCD 部分针脚采用 IOEXT 控制
// ● 使用前务必先对 IOEXT 进行初始化
/* -------------------------------------------------- */
// ● 为了提高 LCD 显示屏速度，底层函数将不再直接控制 SPI
// ● 所以除了初始化函数，其他所有配置或者绘图都需要调用 LCD_MSP3520_SpiOpenScreen() 和 LCD_MSP3520_SpiClose();
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * LCD 外部变量
 * 以下变量定义在 lcd.c 文件中
 */
extern const u8 GV_LcdAscii0612[][12];
extern const u8 GV_LcdAscii0714[][14];
extern const u8 GV_LcdAscii0816[][16];
extern const u8 GV_LcdAscii1224[][48];
extern const u8 GV_LcdAscii1632[][64];

extern const u8 GV_LcdIcon16[][32];
extern const u8 GV_LcdIcon24[][72];
extern const u8 GV_LcdIcon32[][128];

extern const short GV_LcdChinese12Total;
extern const short GV_LcdChinese14Total;
extern const short GV_LcdChinese16Total;
extern const short GV_LcdChinese24Total;
//extern const short GV_LcdChinese32Total;  // 节约单片机 ROM 空间，默认注释掉

extern const struct STRUCT_LCD_CHINESE12 GV_LcdChinese12[];
extern const struct STRUCT_LCD_CHINESE14 GV_LcdChinese14[];
extern const struct STRUCT_LCD_CHINESE16 GV_LcdChinese16[];
extern const struct STRUCT_LCD_CHINESE24 GV_LcdChinese24[];
//extern const struct STRUCT_LCD_CHINESE32 GV_LcdChinese32[];  // 节约单片机 ROM 空间，默认注释掉



/* -------------------------------------------------- */
// ● LCD MSP3520 相关函数
/* -------------------------------------------------- */


/**
 * 初始化函数
 *
 * @param u16  background  背景色
 */
void LCD_MSP3520_Init( u16 background )
{
    // 初始化 LCD MSP3520 基础 IO
    LCD_MSP3520_InitIO();

    // 液晶屏复位
    // 否则第一次上电时可能出现无法点亮背光灯
    IOEXT_LcdMsp3520Reset();
    
    // 开启 SPI 屏幕
    LCD_MSP3520_SpiOpenScreen();

    
    //--------------------- Sleep OUT ---------------------//
    // 睡眠退出
    // 参考数据手册第 166 页
    // 手册上规定在 Sleep Out 指令发出后须等 5ms 才能再发出下一个指令，这是给电压和时钟电路的稳定提供足够时间。同时在这 5ms 中，显示模块加载所有的出厂默认值到寄存器
    LCD_MSP3520_SendCmd(0x11);
    delay_ms(50);
        

    //--------------------- Memory Access Control ---------------------//
    // 内存数据访问控制
    // 参考数据手册第 192 页
    //
    // This command defines read/write scanning direction of frame memory
    // 数据位：MY(Mirror Y-axis),    MX(Mirror X-axis),                  MV,                     ML,           RGB,                       MH, -, -
    // 数据位：Row Address Order, Column Address Order, Row/Column Exchange, Vertical Refresh Order, RGB-BGR ORDER, Horizontal Refresh Order, -, -
    //
    // MV=0 表示竖屏, MV=1 表示横屏
    // MY 和 MX 表示坐标镜像，通过合理设置可以将坐标 0,0 绑定到屏幕的左上/左下/右上/右下
    //
    // ● 竖屏时，程序规定 短边 是列像素(x)
    // ● 竖屏时，程序规定 长边 是行像素(y)
    // 
    // ● 横屏时，程序规定 短边 是行像素(y)
    // ● 横屏时，程序规定 长边 是列像素(x)
    LCD_MSP3520_SendCmd(0x36);
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_SHU ) LCD_MSP3520_SendData8(0x48);  // 1100 1000
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_HEN ) LCD_MSP3520_SendData8(0x28);  // 1010 1000
    
    
    //--------------------- Interface Pixel Format ---------------------//
    // 接口像素格式
    // 参考数据手册第 200 页
    //
    // 设置一个颜色像素占用的位数，虽然该液晶屏是 RGB16 的颜色，但是配置时必须设置为 RGB18，同时发送颜色时 R、G、B 要各自按照1个字节发送 
    // 以上规则在数据手册第 119 页有介绍，3线的 SPI 颜色深度需要 18-bit
    LCD_MSP3520_SendCmd(0x3A);
    LCD_MSP3520_SendData8(0x06);  // 18-bit
    
    
    //--------------------- VCOM Control ---------------------//
    // 参考数据手册第 243 页
    // 目前测试过来无明显作用，就是让灰度背景的每个像素之间更加和谐细腻；直接使用现成的优化过的数据
    LCD_MSP3520_SendCmd(0xC5);
    LCD_MSP3520_SendData8(0x00);
    LCD_MSP3520_SendData8(0x00);
    LCD_MSP3520_SendData8(0x80);
    

    //--------------------- PGAMCTRL (Positive Gamma Control)  ---------------------//
    // 设置灰度电压以调整 TFT 面板的 Gamma 特性
    // 参考数据手册第 265 页
    // 主要作用就是让灰度背景的每个像素之间更加和谐细腻；除非非常了解底层，否则直接使用现成的优化过的数据
    LCD_MSP3520_SendCmd(0xE0);
    LCD_MSP3520_SendData8(0x0B);   
    LCD_MSP3520_SendData8(0x17);   
    LCD_MSP3520_SendData8(0x0A);   
    LCD_MSP3520_SendData8(0x0D);   
    LCD_MSP3520_SendData8(0x1A);   
    LCD_MSP3520_SendData8(0x19);   
    LCD_MSP3520_SendData8(0x16);   
    LCD_MSP3520_SendData8(0x1D);   
    LCD_MSP3520_SendData8(0x21);   
    LCD_MSP3520_SendData8(0x26);   
    LCD_MSP3520_SendData8(0x37);   
    LCD_MSP3520_SendData8(0x3c);   
    LCD_MSP3520_SendData8(0x00);   
    LCD_MSP3520_SendData8(0x09);   
    LCD_MSP3520_SendData8(0x05);   
    LCD_MSP3520_SendData8(0x10);   


    //--------------------- NGAMCTRL (Negative Gamma Control)  ---------------------//
    // 设置灰度电压以调整 TFT 面板的 Gamma 特性
    // 参考数据手册第 266 页
    // 主要作用就是让灰度背景的每个像素之间更加和谐细腻；除非非常了解底层，否则直接使用现成的优化过的数据
    LCD_MSP3520_SendCmd(0xE1);
    LCD_MSP3520_SendData8(0x0C);   
    LCD_MSP3520_SendData8(0x19);   
    LCD_MSP3520_SendData8(0x09);   
    LCD_MSP3520_SendData8(0x0D);   
    LCD_MSP3520_SendData8(0x1b);   
    LCD_MSP3520_SendData8(0x19);   
    LCD_MSP3520_SendData8(0x15);   
    LCD_MSP3520_SendData8(0x1d);   
    LCD_MSP3520_SendData8(0x21);   
    LCD_MSP3520_SendData8(0x26);   
    LCD_MSP3520_SendData8(0x39);   
    LCD_MSP3520_SendData8(0x3E);   
    LCD_MSP3520_SendData8(0x00);   
    LCD_MSP3520_SendData8(0x09);   
    LCD_MSP3520_SendData8(0x05);   
    LCD_MSP3520_SendData8(0x10);   
            
    
    //--------------------- 初始化背景 ---------------------//
    // 在开启显示模式之前初始化液晶屏背景颜色
    // 由于 MSP3520 液晶屏采用 SPI 通讯，清屏时需要通过 SPI 发送每个像素的颜色，但是 SPI 速度较慢，MSP3520屏幕又太大，
    // 导致清屏时能明显感觉到屏幕的清屏扫描过程，所以在液晶屏正式显示内容之前，我们预先给内存设置数据，以便消除液晶屏扫描过程的画面
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_HEN ) LCD_MSP3520_Fill(0, 0, LCD_MSP3520_SCREEN_LONG, LCD_MSP3520_SCREEN_SHORT, background);
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_SHU ) LCD_MSP3520_Fill(0, 0, LCD_MSP3520_SCREEN_SHORT, LCD_MSP3520_SCREEN_LONG, background);
    
    LCD_360ADCoordinate(LCD_COLOR565_LBLUE);
    //--------------------- Display On ---------------------//
    // 开启显示模式
    // 参考数据手册第 174 页
    // 将 MSP3520 中的内存数据实时显示到液晶屏
    LCD_MSP3520_SendCmd(0x29);


    // 默认 SPI 关闭
    LCD_MSP3520_SpiClose();
}


/**
 * 初始化 LCD MSP3520 基础 IO
 */
void LCD_MSP3520_InitIO()
{
    // 默认启用
    IOEXT_LcdMsp3520Enable(true);

    // 默认发送数据
    IOEXT_LcdMsp3520SendModeSetData(true);

    // 默认 SPI 关闭
    IOEXT_LcdMsp3520SpiClose(true);

    // SPI IO初始
    GPIO_Init(LCD_MSP3520_SCK_PORT, LCD_MSP3520_SCK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(LCD_MSP3520_SDI_PORT, LCD_MSP3520_SDI_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  
    // SPI 反初始
    SPI_DeInit();
    
    // SPI 初始化
    //     参数1：帧格式，SPI_FIRSTBIT_MSB 表示优先发送高位
    //     参数2：波特率分频值
    //     参数3：主从模式，SPI_MODE_MASTER 表示主模式
    //     参数4：时钟极性，SPI_CLOCKPOLARITY_LOW 表示 SCK 引脚在空闲状态时的电平
    //     　　 　具体参考《【众拳STM8 56 讲】SPI工作总线原理》中的 “6. 时钟信号的相位和极性”
    //     参数5：时钟相位，SPI_CLOCKPHASE_1EDGE 表示数据采样从第一个时钟边沿开始
    //     　　 　具体参考《【众拳STM8 56 讲】SPI工作总线原理》中的 “6. 时钟信号的相位和极性”
    //     参数6：数据方向，SPI_DATADIRECTION_1LINE_TX 表示 只发送模式
    //     参数7：从机管理，SPI_NSS_SOFT 表示禁止软件从设备管理
    //     参数8：CRC校验，该参数网上很多设置了 0x00，但是我们用的这个无法直接设置 0x00，如果设置了，那么程序就全部死掉无法继续执行
    //     　　 　程序执行死机的代码段：SPI_Init()                              ---->  assert_param(IS_SPI_CRC_POLYNOMIAL_OK(CRCPolynomial));
    //     　　 　　　　　　　　　　　　IS_SPI_CRC_POLYNOMIAL_OK(CRCPolynomial) ---->  #define IS_SPI_CRC_POLYNOMIAL_OK(POLYNOMIAL) ((POLYNOMIAL) > (uint8_t)0x00)
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_1LINE_TX, SPI_NSS_SOFT, 0x01);
    
    // SPI 启用
    SPI_Cmd(ENABLE);
}


/**
 * LCD 打开触摸 SPI
 */
void LCD_MSP3520_SpiOpenTouch()
{
    IOEXT_LcdMsp3520SpiOpen(LCD_MSP3520_SPI_DEVICE_TOUCH, true);
}


/**
 * LCD 打开屏幕 SPI
 */
void LCD_MSP3520_SpiOpenScreen()
{
    IOEXT_LcdMsp3520SpiOpen(LCD_MSP3520_SPI_DEVICE_SCREEN, true);
}


/**
 * LCD 打开存储 SPI
 */
void LCD_MSP3520_SpiOpenStorage()
{
    IOEXT_LcdMsp3520SpiOpen(LCD_MSP3520_SPI_DEVICE_STORAGE, true);
}


/**
 * LCD 关闭 SPI
 */
void LCD_MSP3520_SpiClose()
{
    IOEXT_LcdMsp3520SpiClose(true);
}

/**
 * 向 LCD MSP3520 发送命令
 * 命令发送完成后马上切回发送数据模式，也就是说整个 LCD MSP3520 驱动函数默认模式下都是发送数据
 *
 * @param u8 data  数据字节
 */
void LCD_MSP3520_SendCmd( u8 data )
{
    // 启用发送命令模式
    IOEXT_LcdMsp3520SendModeSetCmd(true);

    // 发送命令
    LCD_MSP3520_SendByte(data);
    
    // 启用发送数据模式（默认）
    IOEXT_LcdMsp3520SendModeSetData(true);
}


/**
 * 向 LCD MSP3520 发送数据
 * 基于 LCD_MSP3520_SendByte() 发送函数（确保数据安全的完全发送，但速度有些损失）
 *
 * @param u8 data  数据字节
 */
void LCD_MSP3520_SendData8( u8 data )
{
    LCD_MSP3520_SendByte(data);
}
void LCD_MSP3520_SendData16( u16 data )
{
    LCD_MSP3520_SendByte(data>>8);
    LCD_MSP3520_SendByte(data);
}
void LCD_MSP3520_SendColor( u16 color )
{
    LCD_MSP3520_SendByte((color>>8)&0xF8);  // R，取前 5 位
    LCD_MSP3520_SendByte((color>>3)&0xFC);  // G，取中 6 位
    LCD_MSP3520_SendByte((color<<3));       // B，取后 5 位
}


/**
 * 向 LCD MSP3520 发送颜色数据
 * 基于 SPI_SendData() 底层发送函数（不关心数据是否成功发送，提升速度）
 * 为了获取更加极致的速度，你可以不通过调用 LCD_MSP3520_SendColorSpeed()，而是直接拷贝 LCD_MSP3520_SendColorSpeed() 里面的代码到业务逻辑中
 *
 * @param u8 data  数据字节
 */
void LCD_MSP3520_SendColorSpeed( u16 color )
{
    SPI_SendData((color>>8)&0xF8);  // R，取前 5 位
    SPI_SendData((color>>3)&0xFC);  // G，取中 6 位
    SPI_SendData((color<<3));       // B，取后 5 位
}


/**
 * 向 LCD MSP3520 发送数据
 *
 * @param u8 byte  数据字节
 */
void LCD_MSP3520_SendByte( u8 byte )
{
    // 等待 SPI 空闲
    while(SPI_GetFlagStatus(SPI_FLAG_BSY));

    // 发送数据并等待发送完成
    SPI_SendData(byte);
    while( !SPI_GetFlagStatus(SPI_FLAG_TXE) );
}


/**
 * 填充的区域
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16 xBeg   起始地址 X 轴值（横向坐标值）
 * @param u16 yBeg   起始地址 Y 轴值（纵向坐标值）
 * @param u16 xEnd   结束地址 X 轴值（横向坐标值）
 * @param u16 yEnd   结束地址 Y 轴值（纵向坐标值）
 * @param u16 color  颜色
 */
void LCD_MSP3520_Fill( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd, u16 color )
{      
    // 初始化
    u16 i,j; 
    
    // 设置即将填充的区域
    LCD_MSP3520_FillSetArea(xBeg, yBeg, xEnd, yEnd);

    // 循环发送数据
    // 使用 LCD_MSP3520_SendColor() 函数可以确保数据安全的完全发送，但速度有些损失
    // 所以这里程序自动根据需要发送的数据量选择使用 LCD_MSP3520_SendColor() 或者直接调用 SPI 发送函数
    if( (u32)(xEnd-xBeg)*(u32)(yEnd-yBeg) < 100 ){
        for( i = yBeg; i <= yEnd; i++ ){
            for( j = xBeg; j <= xEnd; j++ ){
                LCD_MSP3520_SendColor(color); 
            }
        }
    }else{
        for( i = yBeg; i <= yEnd; i++ ){
            for( j = xBeg; j <= xEnd; j++ ){
                SPI_SendData((color>>8)&0xF8);  // R，取前 5 位
                SPI_SendData((color>>3)&0xFC);  // G，取中 6 位
                SPI_SendData((color<<3));       // B，取后 5 位
            }
        }
    }
}


/**
 * 设置即将填充的区域
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * 设置完区域后，该函数会再写入一个 Memory Write 命令，因为 LCD_MSP3520_FillSetArea() 一般都是要传颜色数据
 * 
 * @param u16 xBeg  起始地址 X 轴值（横向坐标值）
 * @param u16 yBeg  起始地址 Y 轴值（纵向坐标值）
 * @param u16 xEnd  结束地址 X 轴值（横向坐标值）
 * @param u16 yEnd  结束地址 Y 轴值（纵向坐标值）
 */
void LCD_MSP3520_FillSetArea( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd )
{
    // 竖屏显示
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_SHU ){
        // 列地址设置
        LCD_MSP3520_SendCmd(0x2A); 
        LCD_MSP3520_SendData16(xBeg+LCD_MSP3520_SCREEN_SHU_XOFFSET);
        LCD_MSP3520_SendData16(xEnd+LCD_MSP3520_SCREEN_SHU_XOFFSET);
        
        // 行地址设置
        LCD_MSP3520_SendCmd(0x2B); 
        LCD_MSP3520_SendData16(yBeg+LCD_MSP3520_SCREEN_SHU_YOFFSET);
        LCD_MSP3520_SendData16(yEnd+LCD_MSP3520_SCREEN_SHU_YOFFSET);
    }
    
    // 横屏显示
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_HEN ){
        // 列地址设置
        LCD_MSP3520_SendCmd(0x2A); 
        LCD_MSP3520_SendData16(xBeg+LCD_MSP3520_SCREEN_HEN_XOFFSET);
        LCD_MSP3520_SendData16(xEnd+LCD_MSP3520_SCREEN_HEN_XOFFSET);
        
        // 行地址设置
        LCD_MSP3520_SendCmd(0x2B); 
        LCD_MSP3520_SendData16(yBeg+LCD_MSP3520_SCREEN_HEN_YOFFSET);
        LCD_MSP3520_SendData16(yEnd+LCD_MSP3520_SCREEN_HEN_YOFFSET);
    }
    
    // Memory Write
    // 参考数据手册第 179 页
    // 内存写的顺序依据 0x36 命令的 D5 位值不同而不同
    LCD_MSP3520_SendCmd(0x2C); 
}



/**
 * 画点
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16 x      起始地址 X 轴值（横向坐标值）
 * @param u16 y      起始地址 Y 轴值（纵向坐标值）
 * @param u16 color  颜色
 */
void LCD_MSP3520_DrawPoint( u16 x, u16 y, u16 color )
{
    LCD_MSP3520_FillSetArea(x, y, x, y);
    LCD_MSP3520_SendColor(color);
} 


/**
 * 画线
 * 使用 Bresenham 算法画任意斜率的直线
 * 参考：https://blog.csdn.net/weixin_44350205/article/details/105642536
 *
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16 x1     起点 X 轴值（横向坐标值）
 * @param u16 y1     起点 Y 轴值（纵向坐标值）
 * @param u16 x2     终点 X 轴值（横向坐标值）
 * @param u16 y2     终点 Y 轴值（纵向坐标值）
 * @param u16 color  颜色
 */
void LCD_MSP3520_DrawLine( u16 x1, u16 y1, u16 x2, u16 y2, u16 color )
{
    // 初始化
    int x = x1;
    int y = y1;
    
    int dx = x2-x1; dx = dx > 0 ? dx : -dx;
    int dy = y2-y1; dy = dy > 0 ? dy : -dy;
    
    int s1 = x2 > x1 ? 1 : -1;
    int s2 = y2 > y1 ? 1 : -1;
 
    // 默认不互换 dx、dy
    // 当斜率大于 1 时，dx、dy 互换
    bool interchange = false;    
    if( dy > dx ){
        int temp = dx;
        dx = dy; dy = temp;
        interchange = true;
    }
 
    int p = 2*dy - dx;
    
    for( int i = 0; i < dx; i++ ){
        LCD_MSP3520_DrawPoint(x, y, color);
        
        if( p >= 0 ){
            if( !interchange ) y += s2;  // 当斜率 < 1 时，选取上下象素点
            else x += s1;  // 当斜率 > 1 时，选取左右象素点
            p -= 2 * dx;
        }
        
        if( !interchange ) x += s1;  // 当斜率 < 1 时，选取 x 为步长
        else y += s2;  // 当斜率 > 1 时，选取 y 为步长
        p += 2 * dy;
    }
    
    // 补全最有一个终点
    // 因为上面的 Bresenham 算法包括起始点，但不包括终止点
    LCD_MSP3520_DrawPoint(x2, y2, color);
}


/**
 * 画圆
 * 使用 Bresenham 算法画圆
 * 参考：https://blog.csdn.net/weixin_44350205/article/details/105665525
 *
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16  x       起点 X 轴值（横向坐标值）
 * @param u16  y       起点 Y 轴值（纵向坐标值）
 * @param u8   r       半径
 * @param u16  color   颜色
 * @param bool fill    填充：空心圆 和 实心圆
 * @param u8   border  边框：空心圆时必须大于等于 1，实心圆时无效。采用 内边框 方式，不影响外形总尺寸
 */
void LCD_MSP3520_DrawCircle( u16 x, u16 y, u8 r, u16 color, bool fill, u8 border )
{
    // 初始化
    int tx = 0, ty = r, d = 3 - 2 * r;
    
    // 重构起始坐标
    x = x + r; y = y + r;
    
    // 重构 border 和 fill 参数
    border = border < 1 ? 1 : (border>r?r:border);
    if( border == r ) fill = true;
    
    // Bresenham 核心算法
    while( tx <= ty ){
        // 利用圆的八分对称性画点或话线
        if( fill == false ){
            LCD_MSP3520_DrawPoint(x-ty, y+tx, color);  // 第1部分列（从左到右）
            LCD_MSP3520_DrawPoint(x-ty, y-tx, color);
            LCD_MSP3520_DrawPoint(x-tx, y+ty, color);  // 第2部分列（从左到右）
            LCD_MSP3520_DrawPoint(x-tx, y-ty, color);
            LCD_MSP3520_DrawPoint(x+tx, y+ty, color);  // 第3部分列（从左到右）
            LCD_MSP3520_DrawPoint(x+tx, y-ty, color);
            LCD_MSP3520_DrawPoint(x+ty, y+tx, color);  // 第4部分列（从左到右）
            LCD_MSP3520_DrawPoint(x+ty, y-tx, color);
            
            // 采用递归 LCD_MSP3520_DrawCircle 的方式画 Border
            // 每次递归 “起点坐标+1，半径-1” 画空心圆，但是由于 Bresenham 算法的原因，部分 border 直接可能会有一个点的空隙，这里进行补偿
            if( border > 1 ){
                LCD_MSP3520_DrawPoint(x-ty+1, y+tx, color);  // 第1部分列（从左到右）
                LCD_MSP3520_DrawPoint(x-ty+1, y-tx, color);
                LCD_MSP3520_DrawPoint(x-tx+1, y+ty, color);  // 第2部分列（从左到右）
                LCD_MSP3520_DrawPoint(x-tx+1, y-ty, color);
                LCD_MSP3520_DrawPoint(x+tx-1, y+ty, color);  // 第3部分列（从左到右）
                LCD_MSP3520_DrawPoint(x+tx-1, y-ty, color);
                LCD_MSP3520_DrawPoint(x+ty-1, y+tx, color);  // 第4部分列（从左到右）
                LCD_MSP3520_DrawPoint(x+ty-1, y-tx, color);
            }
        }else{
            LCD_MSP3520_DrawLine(x-ty, y-tx, x-ty, y+tx, color);  // 第1部分列（从左到右）
            LCD_MSP3520_DrawLine(x-tx, y-ty, x-tx, y+ty, color);  // 第2部分列（从左到右）
            LCD_MSP3520_DrawLine(x+tx, y-ty, x+tx, y+ty, color);  // 第3部分列（从左到右）
            LCD_MSP3520_DrawLine(x+ty, y-tx, x+ty, y+tx, color);  // 第4部分列（从左到右）
        }

        // 取上面的点
        if( d < 0 ) d += 4*tx + 6;
        
        // 取下面的点
        else d += 4*(tx-ty)+10, ty--;
 
        tx++;
    }
    
    // 递归画 Border
    if( fill == false && border > 1 ) LCD_MSP3520_DrawCircle(x+1-r, y+1-r, r-1, color, fill, border-1);
}


/**
 * 画矩形
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16  x       起点 X 轴值（横向坐标值）
 * @param u16  y       起点 Y 轴值（纵向坐标值）
 * @param u16  xlen    矩形宽度（横向值）
 * @param u16  ylen    矩形高度（横向值）
 * @param u16  color   颜色
 * @param bool fill    填充：空心矩形 和 实心矩形
 * @param u8   border  边框：空心矩形时必须大于等于 1，实心矩形时无效。采用 内边框 方式，不影响外形总尺寸
 */
void LCD_MSP3520_DrawRectangle( u16 x, u16 y, u16 xlen, u16 ylen, u16 color, bool fill, u8 border )
{
    // 初始化
    short i;

    // 重构 xlen 和 ylen
    // 矩形边框也算在高度和宽度范围以内
    xlen = xlen-1; ylen = ylen-1;
    
    // 重构 border 和 fill 参数
    if( border < 1 ) border = 1;
    if( border*2 > xlen || border*2 > ylen ) fill = true;

    // 画实心矩形
    if( fill ){
        for( i = 0; i <= xlen; i++ ) LCD_MSP3520_DrawLine(i+x, y, i+x, y+ylen, color);
    }

    // 画空心矩形
    else{
        // 画矩形
        LCD_MSP3520_DrawLine(x, y, x+xlen, y, color);
        LCD_MSP3520_DrawLine(x, y, x, y+ylen, color);
        LCD_MSP3520_DrawLine(x, y+ylen, x+xlen, y+ylen, color);
        LCD_MSP3520_DrawLine(x+xlen, y, x+xlen, y+ylen, color);
        
        // 递归画 Border
        if( border > 1 ) LCD_MSP3520_DrawRectangle(x+1, y+1, xlen-1, ylen-1, color, fill, border-1);
    }
}


/**
 * 显示字符
 * 字模需要采用逐行方式生成，同时点阵数据字节数：(sizex/8+((sizex%8)?1:0))*size
 *
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16  x           起点 X 轴值（横向坐标值）
 * @param u16  y           起点 Y 轴值（纵向坐标值）
 * @param u8   ascii       ASCII 值
 * @param u8   size        字体大小（以 Y 轴值为基准），支持：12, 14, 16, 24, 32
 * @param u16  color       前景色
 * @param u16  background  背景色
 * @param bool backfill    背景覆盖
 */
void LCD_MSP3520_ShowChar( u16 x, u16 y, u8 ascii, u8 size, u16 color, u16 background, bool backfill )
{
    // 无效参数
    if( ascii > 32+94 ) return;
    if( !(size==12||size==14||size==16||size==24||size==32) ) return;
    
    // 初始化
    u8 i, bit, m = 0, sizex = size/2;
    u16 xorigin = x;

    // ' ' 空格真正的 ASCII 值是 32
    // 此处空格我们从 0 开始索引，所以直接减去 ' '
    ascii = ascii - ' ';

    // 一个字符所占总字节数
    // 即显示一个字符时需要传送给LCD的数据字节数
    u16 lcdCharByte;
    u16 lcdCharBytes = (sizex/8+((sizex%8)?1:0))*size;  

    // 设置即将填充的区域
    LCD_MSP3520_FillSetArea(x, y, x+sizex-1, y+size-1);  
    
    // 循环字符的 LCD 字节集
    // 例如：循环一个 24*12 像素的字符，根据 GV_LcdAscii1224 中的字模规则，需要共48个字节
    for( i = 0; i < lcdCharBytes; i++ ){
        // 循环获取字符的LCD字节
        if( size == 12 ) lcdCharByte = GV_LcdAscii0612[ascii][i];  // 调用 6x12  字体
        if( size == 14 ) lcdCharByte = GV_LcdAscii0714[ascii][i];  // 调用 6x12  字体
        if( size == 16 ) lcdCharByte = GV_LcdAscii0816[ascii][i];  // 调用 8x16  字体
        if( size == 24 ) lcdCharByte = GV_LcdAscii1224[ascii][i];  // 调用 12x24 字体
        if( size == 32 ) lcdCharByte = GV_LcdAscii1632[ascii][i];  // 调用 16x32 字体

        // 循环处理每个 LCD 字节位
        // lcdCharByte&(0x01<<bit) 表示取出一个字节里第 bit 位的值
        for( bit = 0; bit < 8; bit++ ){
            // 背景填充
            if( backfill ){
                LCD_MSP3520_SendColor(lcdCharByte&(0x01<<bit)?color:background);
                if( ++m%sizex == 0 ){ m = 0; break; }
            }
            
            // 背景忽略
            else{
                // 画点
                if( lcdCharByte&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);
                
                // Y 自增，开始处理下一行
                // 由于字模采用逐行方式生成，所以显示字符的代码也要按照逐行处理
                if( (++x-xorigin) == sizex ){ x = xorigin; y++; break; }
            }
        }
    }              
}


/**
 * 显示图标
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16  x           起点 X 轴值（横向坐标值）
 * @param u16  y           起点 Y 轴值（纵向坐标值）
 * @param u8   index       图标索引
 * @param u8   size        图标大小（以 Y 轴值为基准），支持：16, 24, 32
 * @param u16  color       前景色
 * @param u16  background  背景色
 * @param bool backfill    背景覆盖
 */
void LCD_MSP3520_ShowIcon( u16 x, u16 y, u8 index, u8 size, u16 color, u16 background, bool backfill )
{
    // 无效参数
    if( !(size==16||size==24||size==32) ) return;
    
    // 初始化
    u8 i, bit, m = 0;
    u16 lcdIconByte, xorigin = x;

    // 设置即将填充的区域
    LCD_MSP3520_FillSetArea(x, y, x+size-1, y+size-1);  

    // 循环图标的 LCD 字节集
    // 例如：循环一个 16*16 像素的图标，根据 GV_LcdIcon16 中的字模规则，需要共32个字节
    for( i = 0; i < size*size/8; i++ ){
        // 循环获取字符的LCD字节
        if( size == 16 ) lcdIconByte = GV_LcdIcon16[index][i];
        if( size == 24 ) lcdIconByte = GV_LcdIcon24[index][i];
        if( size == 32 ) lcdIconByte = GV_LcdIcon32[index][i];

        // 循环处理每个 LCD 字节位
        // lcdIconByte&(0x01<<bit) 表示取出一个字节里第 bit 位的值
        for( bit = 0; bit < 8; bit++ ){
            // 背景填充
            if( backfill ){
                LCD_MSP3520_SendColor(lcdIconByte&(0x01<<bit)?color:background);
                if( ++m%size == 0 ){ m = 0; break; }
            }
            
            // 背景忽略
            else{
                // 画点
                if( lcdIconByte&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);
                
                // Y 自增，开始处理下一行
                // 由于字模采用逐行方式生成，所以显示字符的代码也要按照逐行处理
                if( (++x-xorigin) == size ){ x = xorigin; y++; break; }
            }
        }
    }   
}


/**
 * 显示字符串
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16  x           起点 X 轴值（横向坐标值）
 * @param u16  y           起点 Y 轴值（纵向坐标值）
 * @param u8   *fonts      字集
 * @param u8   size        字体大小（以 Y 轴值为基准），支持：12, 14, 16, 24, 32
 * @param u16  color       前景色
 * @param u16  background  背景色
 * @param bool backfill    背景覆盖
 */
void LCD_MSP3520_ShowString( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, bool backfill )
{      
    // 无效参数
    if( !(size==12||size==14||size==16||size==24||size==32) ) return;
    
    // 画字符串
    while( *fonts != '\0' ){       
        LCD_MSP3520_ShowChar(x, y, *fonts, size, color, background, backfill);
        x += size/2; fonts++;
    }  
}


/**
 * 显示汉字
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16  x           起点 X 轴值（横向坐标值）
 * @param u16  y           起点 Y 轴值（纵向坐标值）
 * @param u8   *fonts      字集
 * @param u8   size        字体大小（以 Y 轴值为基准），支持：12, 14, 16, 24, 32
 * @param u16  color       前景色
 * @param u16  background  背景色
 * @param bool backfill    背景覆盖
 */
void LCD_MSP3520_ShowChinese( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, bool backfill )
{
    // 无效参数
    if( !(size==12||size==14||size==16||size==24||size==32) ) return;
    
    // 画汉字
    while( *fonts != 0 ){
        LCD_MSP3520_ShowChineseOne(x, y, fonts, size, color, background, backfill);
        fonts += 2; x += size;
    }
}


/**
 * 显示单汉字
 * 字模需要采用逐行方式生成，同时点阵数据字节数：(size/8+((size%8)?1:0))*size
 *
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16  x           起点 X 轴值（横向坐标值）
 * @param u16  y           起点 Y 轴值（纵向坐标值）
 * @param u8   *font       单汉字
 * @param u16  color       前景色
 * @param u16  background  背景色
 * @param bool backfill    背景覆盖
 */
void LCD_MSP3520_ShowChineseOne( u16 x, u16 y, u8 *font, u8 size, u16 color, u16 background, bool backfill )
{
    // 初始化
    u16 xorigin = x;
    u8 i, k, bit, find;
    short m = 0, total = 0;

    // 初始汉字集总数
    if( size == 12 ) total = GV_LcdChinese12Total;
    if( size == 14 ) total = GV_LcdChinese14Total;
    if( size == 16 ) total = GV_LcdChinese16Total;
    if( size == 24 ) total = GV_LcdChinese24Total;
    //if( size == 32 ) total = GV_LcdChinese32Total;

    // 一个汉字所占总字节数
    // 即显示一个字符时需要传送给LCD的数据字节数
    u16 lcdCharBytes = (size/8+((size%8)?1:0))*size;

    // 循环汉字集，找到要显示的对应汉字
    for( k = 0; k < total; k++ ){
        // 查找对应的单汉字
        find = false;
        if( size == 12 && (GV_LcdChinese12[k].Index[0]==*(font)) && (GV_LcdChinese12[k].Index[1]==*(font+1)) ) find = true;
        if( size == 14 && (GV_LcdChinese14[k].Index[0]==*(font)) && (GV_LcdChinese14[k].Index[1]==*(font+1)) ) find = true;
        if( size == 16 && (GV_LcdChinese16[k].Index[0]==*(font)) && (GV_LcdChinese16[k].Index[1]==*(font+1)) ) find = true;
        if( size == 24 && (GV_LcdChinese24[k].Index[0]==*(font)) && (GV_LcdChinese24[k].Index[1]==*(font+1)) ) find = true;
        //if( size == 32 && (GV_LcdChinese32[k].Index[0]==*(font)) && (GV_LcdChinese32[k].Index[1]==*(font+1)) ) find = true;  // 节约单片机 ROM 空间，默认注释掉
        if( find == false ) continue;
        
        // 设置即将填充的区域
        LCD_MSP3520_FillSetArea(x, y, x+size-1, y+size-1);

        // 循环字符的 LCD 字节集
        for( i = 0; i < lcdCharBytes; i++ ){
            // 循环处理每个 LCD 字节位
            // GV_LcdChineseXX[k].Masks[i]&(0x01<<bit) 表示取出一个字节里第 bit 位的值
            for( bit=0; bit<8; bit++ ){    
                // 背景填充
                if( backfill ){
                    if( size == 12 ) LCD_MSP3520_SendColor(GV_LcdChinese12[k].Masks[i]&(0x01<<bit)?color:background);
                    if( size == 14 ) LCD_MSP3520_SendColor(GV_LcdChinese14[k].Masks[i]&(0x01<<bit)?color:background);
                    if( size == 16 ) LCD_MSP3520_SendColor(GV_LcdChinese16[k].Masks[i]&(0x01<<bit)?color:background);
                    if( size == 24 ) LCD_MSP3520_SendColor(GV_LcdChinese24[k].Masks[i]&(0x01<<bit)?color:background);
                    //if( size == 32 ) LCD_MSP3520_SendColor(GV_LcdChinese32[k].Masks[i]&(0x01<<bit)?color:background);  // 节约单片机 ROM 空间，默认注释掉
                    if( ++m%size == 0 ){ m = 0; break; }
                }

                // 背景忽略
                else{
                    // 画点
                    if( size == 12 && GV_LcdChinese12[k].Masks[i]&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);
                    if( size == 14 && GV_LcdChinese14[k].Masks[i]&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);
                    if( size == 16 && GV_LcdChinese16[k].Masks[i]&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);
                    if( size == 24 && GV_LcdChinese24[k].Masks[i]&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);  // 节约单片机 ROM 空间，默认注释掉
                    //if( size == 32 && GV_LcdChinese32[k].Masks[i]&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);  // 节约单片机 ROM 空间，默认注释掉

                    // Y 自增，开始处理下一行
                    // 由于字模采用逐行方式生成，所以显示字符的代码也要按照逐行处理
                    if( (++x-xorigin) == size ){ x = xorigin; y++; break; }
                }
            }
        }
        
        // 查找到对应点阵字库立即退出
        // 防止多个汉字重复取模带来影响
        break;  
    }
}


/**
 * 显示图片
 * 使用 Image2Lcd 生成的数据，生成时候需要选择：水平扫描，16位真彩色（颜色：565）
 *
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16  x           起点 X 轴值（横向坐标值）
 * @param u16  y           起点 Y 轴值（纵向坐标值）
 * @param u16  xlen        图片宽度（横向值）
 * @param u16  ylen        图片高度（纵向值）
 * @param u8*  image       图片数据指针
 * @param bool reverse     颜色是否取反
 */
void LCD_MSP3520_ShowImage( u16 x, u16 y, u16 xlen, u16 ylen, const u8 image[], bool reverse )
{
    // 初始化
    u16 i, j;
    u32 datai = 0;
    
    // 设置即将填充的区域
    LCD_MSP3520_FillSetArea(x, y, x+xlen-1, y+ylen-1);
    
    // 按顺序发送颜色数据
    for( i = 0; i < xlen; i++ ){
        for( j = 0; j < ylen; j++ ){
            LCD_MSP3520_SendData8(reverse?~image[datai*2]:image[datai*2]);
            LCD_MSP3520_SendData8(reverse?~image[datai*2+1]:image[datai*2]);
            datai++;
        }
    }            
}


/**
 * 显示数字
 * 自动选择浮点数或者整形
 * 数字从低位开始显示，如果显示的长度小于数字长度，那么数字高位不显示
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16   x           起点 X 轴值（横向坐标值）
 * @param u16   y           起点 Y 轴值（纵向坐标值）
 * @param float number      浮点数
 * @param u8    size        字体大小（以 Y 轴值为基准），支持：12, 14, 16, 24, 32
 * @param u16   color       前景色
 * @param u16   background  背景色
 * @param u8    backfill    背景覆盖
 * @param short showlen     要显示的长度（包括负号和小数点），数字长度不足时空格补全，0 表示自动识别长度，正数表示左对齐，负数表示右对齐
 *
 * @param short 返回实际显示的数字个数（包括小数点和负号）
 */
short LCD_MSP3520_ShowNumber( u16 x, u16 y, double number, u8 size, u16 color, u16 background, bool backfill, short showlen )
{
    long intnum = ((long)(number*100));
    if( intnum%100 == 0 ) return LCD_MSP3520_ShowInteger(x, y, (long)number, size, color, background, backfill, showlen);
    else return LCD_MSP3520_ShowFloat(x, y, number, size, color, background, backfill, showlen );
}
void LCD_MSP3520_ShowSplitNumber( u16 x, u16 y, short *splitnums, short splitindex, u8 size, u16 color, u16 background, bool backfill, short showlen )
{
    // 初始化
    short i, l;
    short sizex = size/2;
    
    // 全部显示
    if( showlen == 0 ){
        for( i = 0; i < splitindex; i++ ){
            if( splitnums[i] >=  0 ) LCD_MSP3520_ShowChar(x+i*sizex, y, splitnums[i]+48, size, color, background, backfill);
            if( splitnums[i] == -1 ) LCD_MSP3520_ShowChar(x+i*sizex, y, '.', size, color, background, backfill);
            if( splitnums[i] == -2 ) LCD_MSP3520_ShowChar(x+i*sizex, y, '-', size, color, background, backfill);
        }
    }
    
    // 显示个数限制（左对齐）
    if( showlen > 0 ){
        for( l = 0; l < showlen; l++ ){
            i = l;
            if( i < splitindex ){
                if( splitnums[i] >=  0 ) LCD_MSP3520_ShowChar(x+l*sizex, y, splitnums[i]+48, size, color, background, backfill);
                if( splitnums[i] == -1 ) LCD_MSP3520_ShowChar(x+l*sizex, y, '.', size, color, background, backfill);
                if( splitnums[i] == -2 ) LCD_MSP3520_ShowChar(x+l*sizex, y, '-', size, color, background, backfill);
            }else{
                LCD_MSP3520_ShowChar(x+l*sizex, y, ' ', size, color, background, backfill);
            }
        }
    }
    
    // 显示个数限制（右对齐）
    if( showlen < 0 ){
        showlen = -showlen;
        for( l = showlen; l > 0; l-- ){
            i = splitindex - (showlen-l) - 1;
            if( i >= 0 && i < splitindex ){
                if( splitnums[i] >= 0  ) LCD_MSP3520_ShowChar(x+(l-1)*sizex, y, splitnums[i]+48, size, color, background, backfill);
                if( splitnums[i] == -1 ) LCD_MSP3520_ShowChar(x+(l-1)*sizex, y, '.', size, color, background, backfill);
                if( splitnums[i] == -2 ) LCD_MSP3520_ShowChar(x+(l-1)*sizex, y, '-', size, color, background, backfill);
            }else{
                LCD_MSP3520_ShowChar(x+(l-1)*sizex, y, ' ', size, color, background, backfill);
            }
        }
    }
}

/**
 * 显示浮点数
 * 默认显示两位小数点
 * 数字从低位开始显示，如果显示的长度小于数字长度，那么数字高位不显示
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16   x           起点 X 轴值（横向坐标值）
 * @param u16   y           起点 Y 轴值（纵向坐标值）
 * @param float number      浮点数
 * @param u8    size        字体大小（以 Y 轴值为基准），支持：12, 14, 16, 24, 32
 * @param u16   color       前景色
 * @param u16   background  背景色
 * @param bool  backfill    背景覆盖
 * @param short showlen     要显示的长度（包括负号和小数点），数字长度不足时空格补全，0 表示自动识别长度，正数表示左对齐，负数表示右对齐
 *
 * @param short 返回实际显示的数字个数（包括小数点和负号）
 */
short LCD_MSP3520_ShowFloat( u16 x, u16 y, double number, u8 size, u16 color, u16 background, bool backfill, short showlen )
{
    // 初始化
    short i, onenum;

    long  intnum = (long)(number*100);
    long  absintnum = intnum >= 0 ? intnum : -intnum;
    long  absintnumsub = intnum >= 0 ? intnum : -intnum;

    short splitnums[15] = {-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9};  // 0~9 表示数字，-1 表示 “.”，-2 表示 “-”，-9 表示初始值
    short splitindex = 0;

    // 分解数字
    while( absintnumsub > 0 ){
        // 提取数字
        onenum = absintnumsub%10;
        absintnumsub = absintnumsub/10;
        
        // 数组内容后移一位，腾出索引为 0 的数组空间并赋值当前分解出的单个数字
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = onenum;
        
        // 小数点处理
        if( (splitindex==1&&absintnum<10) || splitindex == 2 ){
            for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
            splitnums[0] = -1;
        }
    }
    
    // 如果数字小于 1，那么赋值前置 “0”
    // 例如参数 number = 0.12，经过上面的 “分解数字” 代码段分解后，会变成 .12，所以下面的代码就在其最前面赋值一个0，变成 0.12
    if( absintnum > 0 && absintnum < 100 ){
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = 0;
    }

    // 如果数字等于 0，那么赋值 0
    // 上面的 “分解数字” 代码段并不会处理 number = 0 时情况，所以需要单独处理
    if( intnum == 0 ){
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1]; splitnums[0] = 0;
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1]; splitnums[0] = 0;
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1]; splitnums[0] = -1;
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1]; splitnums[0] = 0;
    }
    
    // 如果数字小于 0，那么赋值前置 “负” 号
    if( intnum < 0 ){
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = -2;
    }
    
    // 显示输出
    LCD_MSP3520_ShowSplitNumber(x, y, splitnums, splitindex, size, color, background, backfill, showlen);
        
    // 返回实际显示的数字个数（包括小数点和负号）
    showlen = showlen > 0 ? showlen : -showlen;
    return showlen == 0 ? splitindex : (showlen>splitindex?splitindex:showlen);
}


/**
 * 显示整型
 * 数字从低位开始显示，如果显示的长度小于数字长度，那么数字高位不显示
 * X/Y 轴是一个抽象的坐标：人正对着竖屏或者横屏，X 轴指横向坐标，Y轴指纵向坐标
 *
 * @param u16   x           起点 X 轴值（横向坐标值）
 * @param u16   y           起点 Y 轴值（纵向坐标值）
 * @param long  number      整型数字
 * @param u8    size        字体大小（以 Y 轴值为基准），支持：12, 14, 16, 24, 32
 * @param u16   color       前景色
 * @param u16   background  背景色
 * @param bool  backfill    背景覆盖
 * @param short showlen     要显示的长度（包括负号），数字长度不足时空格补全，0 表示自动识别长度，正数表示左对齐，负数表示右对齐
 *
 * @param short 返回实际显示的数字个数（包括小数点和负号）
 */
short LCD_MSP3520_ShowInteger( u16 x, u16 y, long number, u8 size, u16 color, u16 background, bool backfill, short showlen )
{
    // 初始化
    short i, onenum;
    long  absnumsub = number < 0 ? -number : number;

    short splitnums[15] = {-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9};  // 0~9 表示数字，-1 表示 “.”，-2 表示 “-”，-9 表示初始值
    short splitindex = 0;

    // 分解数字
    while( absnumsub > 0 ){
        // 提取数字
        onenum = absnumsub%10;
        absnumsub = absnumsub/10;
        
        // 数组内容后移一位，腾出索引为 0 的数组空间并赋值当前分解出的单个数字
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = onenum;
    }
    
    // 如果数字等于 0，那么赋值 0
    // 上面的 “分解数字” 代码段并不会处理 number = 0 时情况，所以需要单独处理
    if( number == 0 ){
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = 0;
    }

    // 如果数字小于 0，那么赋值前置 “负” 号
    if( number < 0 ){
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = -2;
    }

    // 显示输出
    LCD_MSP3520_ShowSplitNumber(x, y, splitnums, splitindex, size, color, background, backfill, showlen);
    
    // 返回实际显示的数字个数（包括小数点和负号）
    showlen = showlen > 0 ? showlen : -showlen;
    return showlen == 0 ? splitindex : (showlen>splitindex?splitindex:showlen);
} 

