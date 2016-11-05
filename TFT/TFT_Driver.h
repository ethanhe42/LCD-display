#ifndef __TFT_DRIVER_H
#define __TFT_DRIVER_H

#define H_VIEW
//#define USB_HARDWARE_SPI

#ifdef H_VIEW
#define X_MAX_PIXEL         320
#define Y_MAX_PIXEL         240
#else
#define X_MAX_PIXEL         240
#define Y_MAX_PIXEL         320
#endif

#define RED     0xf800
#define GREEN   0x07e0
#define BLUE    0x001f
#define WHITE   0xffff
#define BLACK   0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410//灰色1      00000 000000 00000
#define GRAY2   0x4208//灰色2      11111 111110 11111
//红5位(0-31) 绿6位(0-63) 蓝5位(0-31)

//#define TFT_CTRL        P1OUT    //定义TFT数据端口为PB组
#define TFT_CS          BIT3     //对应接液晶屏(或者模块)TFT --CS
#define TFT_RST         BIT2     //对应接液晶屏(或者模块)TFT --RST
#define TFT_RS          BIT4     //对应接液晶屏(或者模块)TFT --RS/DC
#define TFT_SDI         BIT7     //MOSI 对应接液晶屏(或者模块)TFT --SDI 
#define TFT_SCK         BIT5     //对应接液晶屏(或者模块)TFT --SCK
#define TFT_SDO         BIT6     //MISO 对应接液晶屏(或者模块)TFT --SDO
//#define TFT_LED         //对应接液晶屏(或者模块)TFT --PIN_LED背光正极

//液晶控制口置1操作语句宏定义
#define TFT_CS_SET      P1OUT |= TFT_CS
#define TFT_RS_SET      P1OUT |= TFT_RS
#define TFT_SDI_SET     P1OUT |= TFT_SDI
#define TFT_SCK_SET     P1OUT |= TFT_SCK
#define TFT_RST_SET     P1OUT |= TFT_RST
//#define TFT_LED_SET     P1OUT |= TFT_LED

//液晶控制口置0操作语句宏定义
#define TFT_CS_CLR      P1OUT &= ~TFT_CS
#define TFT_RS_CLR      P1OUT &= ~TFT_RS
#define TFT_SDI_CLR     P1OUT &= ~TFT_SDI
#define TFT_SCK_CLR     P1OUT &= ~TFT_SCK
#define TFT_RST_CLR     P1OUT &= ~TFT_RST
//#define TFT_LED_CLR     P1OUT &= ~TFT_LED

typedef unsigned int u16;
typedef unsigned char u8;

void TFT_Init(void);
void TFT_Clear(u16 Color);
void TFT_SetXY(u16 x, u16 y);
void TFT_DrawPoint(u16 x, u16 y, u16 Data);
u16  TFT_ReadPoint(u16 x, u16 y);

void TFT_Circle(u16 X, u16 Y, u16 R, u16 fc);
void TFT_DrawLine(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);
void TFT_Box(u16 x, u16 y, u16 w, u16 h, u16 bc);
void TFT_Box2(u16 x, u16 y, u16 w, u16 h, u8 mode);
void DisplayButtonDown(u16 x1, u16 y1, u16 x2, u16 y2);
void DisplayButtonUp(u16 x1, u16 y1, u16 x2, u16 y2);
void TFT_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
void TFT_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
void TFT_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num) ;

void Color_Test(void);

#endif
