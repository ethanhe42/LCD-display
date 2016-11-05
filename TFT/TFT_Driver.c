#include <msp430g2553.h>
#include "TFT_Driver.h"
#include "font.h"

//SPI å†™ä¸€ä¸ªå­—èŠ‚
u8 SPI_WriteByte(u8 Byte)
{
	TFT_CS_CLR;
	UCB0TXBUF = Byte;   //å�‘é€�ä¸€ä¸ªbyte
	while ((UCB0STAT & UCBUSY));   //ç­‰å¾…æŽ¥æ”¶å®Œä¸€ä¸ªbyte
	TFT_CS_SET;
	return UCB0TXBUF;
}

//ç¡¬ä»¶SPIé…�ç½®
void TFT_IO_Init(void)
{
	//UCB0CLK:P1.5; UCB0SOMI:P1.6; UCB0SIMO:P1.7.
	P1SEL |= TFT_SCK + TFT_SDO + TFT_SDI;
	P1SEL2 |= TFT_SCK + TFT_SDO + TFT_SDI;
	P1DIR |= TFT_SCK;
	P1DIR |= TFT_SDI;
	P1DIR &= ~TFT_SDO;

	//CS:P1.3; RS:P1.4; RST:P1.2.
	P1SEL &= ~(TFT_CS + TFT_RS + TFT_RST);
	P1DIR |= TFT_CS + TFT_RS + TFT_RST;
	P1OUT |= TFT_CS + TFT_RS;
}

void TFT_SPI_Init(void)
{
	//UCB0CLK:P1.5; UCB0SIMO:P1.6; UCB0SOMI:P1.7.
	//    UCB0CTL1 |= UCSWRST;
	//    UCB0CTL0 |= UCCKPL + UCMSB+ UCMST+ UCMODE_0 + UCSYNC;  // 3-pin, 8-bit SPI mstr, MSb 1st
	//    UCB0CTL1 |= UCSSEL_2;  // SMCLK
	//    UCB0CTL1 &= ~UCSWRST;
	//    IFG2 &= ~(UCA0TXIFG);
	//    IE2 |= UCA0TXIE;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL  = CALDCO_16MHZ;
	UCB0CTL1 |= UCSWRST;
	UCB0CTL1 = (UCB0CTL1 & 0x3F) | UCSSEL_2;
	UCB0CTL0 |= UCCKPL + UCMSB + UCMST + UCMODE_0 + UCSYNC;
	UCB0BR0 = 0x01;
	UCB0BR1 = 0;
	UCB0CTL1 &= ~UCSWRST;
}

void TFT_WriteIndex(u8 Index)
{
	TFT_RS_CLR;
	SPI_WriteByte(Index);
}

void TFT_WriteData(u8 Data)
{
	TFT_RS_SET;
	SPI_WriteByte(Data);
}

void TFT_WriteData16Bit(u16 Data)
{
	TFT_WriteData(Data >> 8);
	TFT_WriteData(Data);
}

void TFT_WriteIndex16Bit(u16 Index)
{
	TFT_WriteIndex(Index >> 8);
	TFT_WriteIndex(Index);
}

void TFT_Reset(void)
{
	TFT_RST_CLR;
	__delay_cycles(10000);
	TFT_RST_SET;
	__delay_cycles(5000);
}

void TFT_Init(void)
{
	TFT_IO_Init();
	TFT_SPI_Init();
	TFT_Reset();

	TFT_WriteIndex(0xCB);
	TFT_WriteData(0x39);
	TFT_WriteData(0x2C);
	TFT_WriteData(0x00);
	TFT_WriteData(0x34);
	TFT_WriteData(0x02);

	TFT_WriteIndex(0xCF);
	TFT_WriteData(0x00);
	TFT_WriteData(0XC1);
	TFT_WriteData(0X30);

	TFT_WriteIndex(0xE8);
	TFT_WriteData(0x85);
	TFT_WriteData(0x00);
	TFT_WriteData(0x78);

	TFT_WriteIndex(0xEA);
	TFT_WriteData(0x00);
	TFT_WriteData(0x00);

	TFT_WriteIndex(0xED);
	TFT_WriteData(0x64);
	TFT_WriteData(0x03);
	TFT_WriteData(0X12);
	TFT_WriteData(0X81);

	TFT_WriteIndex(0xF7);
	TFT_WriteData(0x20);

	TFT_WriteIndex(0xC0);    //Power control
	TFT_WriteData(0x23);   //VRH[5:0]

	TFT_WriteIndex(0xC1);    //Power control
	TFT_WriteData(0x10);   //SAP[2:0];BT[3:0]

	TFT_WriteIndex(0xC5);    //VCM control
	TFT_WriteData(0x3e); //å¯¹æ¯”åº¦è°ƒèŠ‚
	TFT_WriteData(0x28);

	TFT_WriteIndex(0xC7);    //VCM control2
	TFT_WriteData(0x86);  //--
	TFT_WriteIndex(0x36);    // Memory Access Control
#ifdef H_VIEW
	TFT_WriteData(0xE8); //C8      //48 68ç«–å±�//28 E8 æ¨ªå±�
#else
	TFT_WriteData(0x48);
#endif
	TFT_WriteIndex(0x3A);
	TFT_WriteData(0x55);

	TFT_WriteIndex(0xB1);
	TFT_WriteData(0x00);
	TFT_WriteData(0x18);

	TFT_WriteIndex(0xB6);    // Display Function Control
	TFT_WriteData(0x08);
	TFT_WriteData(0x82);
	TFT_WriteData(0x27);

	TFT_WriteIndex(0xF2);    // 3Gamma Function Disable
	TFT_WriteData(0x00);

	TFT_WriteIndex(0x26);    //Gamma curve selected
	TFT_WriteData(0x01);

	TFT_WriteIndex(0xE0);    //Set Gamma
	TFT_WriteData(0x0F);
	TFT_WriteData(0x31);
	TFT_WriteData(0x2B);
	TFT_WriteData(0x0C);
	TFT_WriteData(0x0E);
	TFT_WriteData(0x08);
	TFT_WriteData(0x4E);
	TFT_WriteData(0xF1);
	TFT_WriteData(0x37);
	TFT_WriteData(0x07);
	TFT_WriteData(0x10);
	TFT_WriteData(0x03);
	TFT_WriteData(0x0E);
	TFT_WriteData(0x09);
	TFT_WriteData(0x00);

	TFT_WriteIndex(0XE1);    //Set Gamma
	TFT_WriteData(0x00);
	TFT_WriteData(0x0E);
	TFT_WriteData(0x14);
	TFT_WriteData(0x03);
	TFT_WriteData(0x11);
	TFT_WriteData(0x07);
	TFT_WriteData(0x31);
	TFT_WriteData(0xC1);
	TFT_WriteData(0x48);
	TFT_WriteData(0x08);
	TFT_WriteData(0x0F);
	TFT_WriteData(0x0C);
	TFT_WriteData(0x31);
	TFT_WriteData(0x36);
	TFT_WriteData(0x0F);

	TFT_WriteIndex(0x11);    //Exit Sleep
	__delay_cycles(12000);

	TFT_WriteIndex(0x29);    //Display on
	TFT_WriteIndex(0x2c);
}


/*************************************************
å‡½æ•°å��ï¼šTFT_Set_Region
åŠŸèƒ½ï¼šè®¾ç½®lcdæ˜¾ç¤ºåŒºåŸŸï¼Œåœ¨æ­¤åŒºåŸŸå†™ç‚¹æ•°æ�®è‡ªåŠ¨æ�¢è¡Œ
å…¥å�£å�‚æ•°ï¼šxyèµ·ç‚¹å’Œç»ˆç‚¹,Y_IncModeè¡¨ç¤ºå…ˆè‡ªå¢žyå†�è‡ªå¢žx
è¿”å›žå€¼ï¼šæ— 
*************************************************/
void TFT_SetRegion(u16 x_start, u16 y_start, u16 x_end, u16 y_end)
{
	TFT_WriteIndex(0x2a);
	TFT_WriteData16Bit(x_start);
	TFT_WriteData16Bit(x_end);

	TFT_WriteIndex(0x2b);
	TFT_WriteData16Bit(y_start);
	TFT_WriteData16Bit(y_end);

	TFT_WriteIndex(0x2c);

}

/*************************************************
å‡½æ•°å��ï¼šTFT_Set_XY
åŠŸèƒ½ï¼šè®¾ç½®lcdæ˜¾ç¤ºèµ·å§‹ç‚¹
å…¥å�£å�‚æ•°ï¼šxyå��æ ‡
è¿”å›žå€¼ï¼šæ— 
*************************************************/
void TFT_SetXY(u16 x, u16 y)
{
	TFT_WriteIndex(0x2a);
	TFT_WriteData16Bit(x);

	TFT_WriteIndex(0x2b);
	TFT_WriteData16Bit(y);

	TFT_WriteIndex(0x2c);
}


/*************************************************
å‡½æ•°å��ï¼šTFT_DrawPoint
åŠŸèƒ½ï¼šç”»ä¸€ä¸ªç‚¹
å…¥å�£å�‚æ•°ï¼šæ— 
è¿”å›žå€¼ï¼šæ— 
*************************************************/
void TFT_DrawPoint(u16 x, u16 y, u16 Data)
{
	TFT_SetXY(x, y);
	TFT_WriteData16Bit(Data);
}

/*****************************************
 å‡½æ•°åŠŸèƒ½ï¼šè¯»TFTæŸ�ä¸€ç‚¹çš„é¢œè‰²
 å‡ºå�£å�‚æ•°ï¼šcolor  ç‚¹é¢œè‰²å€¼
******************************************/
u16 TFT_ReadPoint(u16 x, u16 y)
{
	u8 r, g, b;
	u16 R, G, B, Data;
	TFT_SetXY(x, y);
	TFT_CS_CLR;
	TFT_WriteIndex(0X2E); //ç¬¬ä¸€æ¬¡å†™å…¥æŽ§åˆ¶å‘½ä»¤
	TFT_RS_SET;
	TFT_RS_CLR; //äº§ç”Ÿè„‰å†²
	SPI_WriteByte(0xff); //ç¬¬äºŒæ¬¡ç©ºè¯»å†™DUMMY CLOCK
	r = SPI_WriteByte(0xff); //é«˜å…­ä½�æœ‰æ•ˆ
	g = SPI_WriteByte(0xff); //é«˜å…­ä½�æœ‰æ•ˆ
	b = SPI_WriteByte(0xff); //é«˜å…­ä½�æœ‰æ•ˆ
	TFT_CS_SET;
	R = (r << 1) & 0x00FF;
	G = g & 0x00FF;
	B = (b << 1) & 0x00FF;
	Data =  (R << 8) | (G << 5) | (B >> 3);
	return Data;
}
/*
        r=LCD->LCD_RAM;
        delay_us(2);
        b=LCD->LCD_RAM;
        g=r&0XFF;//9341è¦�åˆ†ä¸¤æ¬¡è¯»å‡º
        g<<=8;
        return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
*/

/*************************************************
å‡½æ•°å��ï¼šTFT_Clear
åŠŸèƒ½ï¼šå…¨å±�æ¸…å±�å‡½æ•°
å…¥å�£å�‚æ•°ï¼šå¡«å……é¢œè‰²COLOR
è¿”å›žå€¼ï¼šæ— 
*************************************************/
void TFT_Clear(u16 Color)
{
	unsigned int i, j;
	TFT_SetRegion(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
	TFT_CS_CLR;
	TFT_RS_SET;

	for (i = Y_MAX_PIXEL; i > 0; i--) {
		for (j = X_MAX_PIXEL; j > 0; j--) {
			TFT_WriteData16Bit(Color);
		}
	}

	TFT_CS_SET;
}


void TFT_Circle(u16 X, u16 Y, u16 R, u16 fc)
{
	//Bresenhamç®—æ³•
	unsigned short  a, b;
	int c;
	a = 0;
	b = R;
	c = 3 - 2 * R;
	while (a < b) {
		TFT_DrawPoint(X + a, Y + b, fc); //        7
		TFT_DrawPoint(X - a, Y + b, fc); //        6
		TFT_DrawPoint(X + a, Y - b, fc); //        2
		TFT_DrawPoint(X - a, Y - b, fc); //        3
		TFT_DrawPoint(X + b, Y + a, fc); //        8
		TFT_DrawPoint(X - b, Y + a, fc); //        5
		TFT_DrawPoint(X + b, Y - a, fc); //        1
		TFT_DrawPoint(X - b, Y - a, fc); //        4

		if (c < 0) c = c + 4 * a + 6;
		else {
			c = c + 4 * (a - b) + 10;
			b -= 1;
		}
		a += 1;
	}
	if (a == b) {
		TFT_DrawPoint(X + a, Y + b, fc);
		TFT_DrawPoint(X + a, Y + b, fc);
		TFT_DrawPoint(X + a, Y - b, fc);
		TFT_DrawPoint(X - a, Y - b, fc);
		TFT_DrawPoint(X + b, Y + a, fc);
		TFT_DrawPoint(X - b, Y + a, fc);
		TFT_DrawPoint(X + b, Y - a, fc);
		TFT_DrawPoint(X - b, Y - a, fc);
	}

}
//ç”»çº¿å‡½æ•°ï¼Œä½¿ç”¨Bresenham ç”»çº¿ç®—æ³•
void TFT_DrawLine(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color)
{
	int dx,             // difference in x's
	    dy,             // difference in y's
	    dx2,            // dx,dy * 2
	    dy2,
	    x_inc,          // amount in pixel space to move during drawing
	    y_inc,          // amount in pixel space to move during drawing
	    error,          // the discriminant i.e. error i.e. decision variable
	    index;          // used for looping

	TFT_SetXY(x0, y0);
	dx = x1 - x0; //è®¡ç®—xè·�ç¦»
	dy = y1 - y0; //è®¡ç®—yè·�ç¦»

	if (dx >= 0) {
		x_inc = 1;
	} else {
		x_inc = -1;
		dx    = -dx;
	}

	if (dy >= 0) {
		y_inc = 1;
	} else {
		y_inc = -1;
		dy    = -dy;
	}

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy) { //xè·�ç¦»å¤§äºŽyè·�ç¦»ï¼Œé‚£ä¹ˆæ¯�ä¸ªxè½´ä¸Šå�ªæœ‰ä¸€ä¸ªç‚¹ï¼Œæ¯�ä¸ªyè½´ä¸Šæœ‰è‹¥å¹²ä¸ªç‚¹
		//ä¸”çº¿çš„ç‚¹æ•°ç­‰äºŽxè·�ç¦»ï¼Œä»¥xè½´é€’å¢žç”»ç‚¹
		// initialize error term
		error = dy2 - dx;

		// draw the line
		for (index = 0; index <= dx; index++) { //è¦�ç”»çš„ç‚¹æ•°ä¸�ä¼šè¶…è¿‡xè·�ç¦»
			//ç”»ç‚¹
			TFT_DrawPoint(x0, y0, Color);

			// test if error has overflowed
			if (error >= 0) { //æ˜¯å�¦éœ€è¦�å¢žåŠ yå��æ ‡å€¼
				error -= dx2;

				// move to next line
				y0 += y_inc; //å¢žåŠ yå��æ ‡å€¼
			} // end if error overflowed

			// adjust the error term
			error += dy2;

			// move to the next pixel
			x0 += x_inc; //xå��æ ‡å€¼æ¯�æ¬¡ç”»ç‚¹å�Žéƒ½é€’å¢ž1
		} // end for
	} // end if |slope| <= 1
	else { //yè½´å¤§äºŽxè½´ï¼Œåˆ™æ¯�ä¸ªyè½´ä¸Šå�ªæœ‰ä¸€ä¸ªç‚¹ï¼Œxè½´è‹¥å¹²ä¸ªç‚¹
		//ä»¥yè½´ä¸ºé€’å¢žç”»ç‚¹
		// initialize error term
		error = dx2 - dy;

		// draw the line
		for (index = 0; index <= dy; index++) {
			// set the pixel
			TFT_DrawPoint(x0, y0, Color);

			// test if error overflowed
			if (error >= 0) {
				error -= dy2;

				// move to next line
				x0 += x_inc;
			} // end if error overflowed

			// adjust the error term
			error += dx2;

			// move to the next pixel
			y0 += y_inc;
		} // end for
	} // end else |slope| > 1
}

void TFT_Box(u16 x, u16 y, u16 w, u16 h, u16 bc)
{
	TFT_DrawLine(x, y, x + w, y, 0xEF7D);
	TFT_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0x2965);
	TFT_DrawLine(x, y + h, x + w, y + h, 0x2965);
	TFT_DrawLine(x, y, x, y + h, 0xEF7D);
	TFT_DrawLine(x + 1, y + 1, x + 1 + w - 2, y + 1 + h - 2, bc);
}

void TFT_Box2(u16 x, u16 y, u16 w, u16 h, u8 mode)
{
	if (mode == 0)    {
		TFT_DrawLine(x, y, x + w, y, 0xEF7D);
		TFT_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0x2965);
		TFT_DrawLine(x, y + h, x + w, y + h, 0x2965);
		TFT_DrawLine(x, y, x, y + h, 0xEF7D);
	}
	if (mode == 1)    {
		TFT_DrawLine(x, y, x + w, y, 0x2965);
		TFT_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0xEF7D);
		TFT_DrawLine(x, y + h, x + w, y + h, 0xEF7D);
		TFT_DrawLine(x, y, x, y + h, 0x2965);
	}
	if (mode == 2)    {
		TFT_DrawLine(x, y, x + w, y, 0xffff);
		TFT_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0xffff);
		TFT_DrawLine(x, y + h, x + w, y + h, 0xffff);
		TFT_DrawLine(x, y, x, y + h, 0xffff);
	}
}


/**************************************************************************************
åŠŸèƒ½æ��è¿°: åœ¨å±�å¹•æ˜¾ç¤ºä¸€å‡¸èµ·çš„æŒ‰é’®æ¡†
è¾“    å…¥: u16 x1,y1,x2,y2 æŒ‰é’®æ¡†å·¦ä¸Šè§’å’Œå�³ä¸‹è§’å��æ ‡
è¾“    å‡º: æ— 
**************************************************************************************/
void DisplayButtonUp(u16 x1, u16 y1, u16 x2, u16 y2)
{
	TFT_DrawLine(x1,  y1,  x2, y1, WHITE); //H
	TFT_DrawLine(x1,  y1,  x1, y2, WHITE); //V

	TFT_DrawLine(x1 + 1, y2 - 1, x2, y2 - 1, GRAY1); //H
	TFT_DrawLine(x1,  y2,  x2, y2, GRAY2); //H
	TFT_DrawLine(x2 - 1, y1 + 1, x2 - 1, y2, GRAY1); //V
	TFT_DrawLine(x2  , y1  , x2, y2, GRAY2); //V
}

/**************************************************************************************
åŠŸèƒ½æ��è¿°: åœ¨å±�å¹•æ˜¾ç¤ºä¸€å‡¹ä¸‹çš„æŒ‰é’®æ¡†
è¾“    å…¥: u16 x1,y1,x2,y2 æŒ‰é’®æ¡†å·¦ä¸Šè§’å’Œå�³ä¸‹è§’å��æ ‡
è¾“    å‡º: æ— 
**************************************************************************************/
void DisplayButtonDown(u16 x1, u16 y1, u16 x2, u16 y2)
{
	TFT_DrawLine(x1,  y1,  x2, y1, GRAY2); //H
	TFT_DrawLine(x1 + 1, y1 + 1, x2, y1 + 1, GRAY1); //H
	TFT_DrawLine(x1,  y1,  x1, y2, GRAY2); //V
	TFT_DrawLine(x1 + 1, y1 + 1, x1 + 1, y2, GRAY1); //V
	TFT_DrawLine(x1,  y2,  x2, y2, WHITE); //H
	TFT_DrawLine(x2,  y1,  x2, y2, WHITE); //V
}

void TFT_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i, j;
	unsigned short k, x0;
	x0 = x;

	while (*s) {
		if ((*s) < 128) {
			k = *s;
			if (k == 13) {
				x = x0;
				y += 16;
			} else {
				if (k > 32) k -= 32; else k = 0;

				for (i = 0; i < 16; i++)
					for (j = 0; j < 8; j++) {
						if (asc16[k * 16 + i] & (0x80 >> j)) TFT_DrawPoint(x + j, y + i, fc);
						else {
							if (fc != bc) TFT_DrawPoint(x + j, y + i, bc);
						}
					}
				x += 8;
			}
			s++;
		}

		else {


			for (k = 0; k < hz16_num; k++) {
				if ((hz16[k].Index[0] == *(s)) && (hz16[k].Index[1] == *(s + 1))) {
					for (i = 0; i < 16; i++) {
						for (j = 0; j < 8; j++) {
							if (hz16[k].Msk[i * 2] & (0x80 >> j))  TFT_DrawPoint(x + j, y + i, fc);
							else {
								if (fc != bc) TFT_DrawPoint(x + j, y + i, bc);
							}
						}
						for (j = 0; j < 8; j++) {
							if (hz16[k].Msk[i * 2 + 1] & (0x80 >> j))    TFT_DrawPoint(x + j + 8, y + i, fc);
							else {
								if (fc != bc) TFT_DrawPoint(x + j + 8, y + i, bc);
							}
						}
					}
				}
			}
			s += 2; x += 16;
		}

	}
}

void TFT_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i, j;
	unsigned short k;

	while (*s) {
		if ( *s < 0x80 ) {
			k = *s;
			if (k > 32) k -= 32; else k = 0;

			for (i = 0; i < 16; i++)
				for (j = 0; j < 8; j++) {
					if (asc16[k * 16 + i] & (0x80 >> j))
						TFT_DrawPoint(x + j, y + i, fc);
					else {
						if (fc != bc) TFT_DrawPoint(x + j, y + i, bc);
					}
				}
			s++; x += 8;
		} else {

			for (k = 0; k < hz24_num; k++) {
				if ((hz24[k].Index[0] == *(s)) && (hz24[k].Index[1] == *(s + 1))) {
					for (i = 0; i < 24; i++) {
						for (j = 0; j < 8; j++) {
							if (hz24[k].Msk[i * 3] & (0x80 >> j))
								TFT_DrawPoint(x + j, y + i, fc);
							else {
								if (fc != bc) TFT_DrawPoint(x + j, y + i, bc);
							}
						}
						for (j = 0; j < 8; j++) {
							if (hz24[k].Msk[i * 3 + 1] & (0x80 >> j))    TFT_DrawPoint(x + j + 8, y + i, fc);
							else {
								if (fc != bc) TFT_DrawPoint(x + j + 8, y + i, bc);
							}
						}
						for (j = 0; j < 8; j++) {
							if (hz24[k].Msk[i * 3 + 2] & (0x80 >> j))
								TFT_DrawPoint(x + j + 16, y + i, fc);
							else {
								if (fc != bc) TFT_DrawPoint(x + j + 16, y + i, bc);
							}
						}
					}
				}
			}
			s += 2; x += 24;
		}
	}
}
void TFT_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num)
{
	unsigned char i, j, k, c;
	//TFT_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
	//  w=w/8;

	for (i = 0; i < 32; i++) {
		for (j = 0; j < 4; j++) {
			c = *(sz32 + num * 32 * 4 + i * 4 + j);
			for (k = 0; k < 8; k++) {

				if (c & (0x80 >> k)) TFT_DrawPoint(x + j * 8 + k, y + i, fc);
				else {
					if (fc != bc) TFT_DrawPoint(x + j * 8 + k, y + i, bc);
				}
			}
		}
	}
}

void Color_Test(void)
{
	u8 i = 2;

	TFT_Clear(BLACK);
	TFT_Clear(GRAY2);
	TFT_Clear(GRAY1);
	TFT_Clear(GRAY0);
	TFT_Clear(WHITE);

	TFT_DrawFont_GBK16(20, 10, BLUE, WHITE, "Color Test Start.");
	TFT_DrawFont_GBK16(20, 10, BLUE, WHITE, "Color Test Start.");
	TFT_DrawFont_GBK16(20, 10, BLUE, WHITE, "Color Test Start.");
	TFT_DrawLine(140,100,180,140,BLUE);
	TFT_Circle(160,120,20,BLUE);
	TFT_DrawPoint(160,120,RED);
	__delay_cycles(10000000);

	TFT_WriteIndex(0x2c);
	TFT_WriteData16Bit(GREEN);

	while (i--) {
		TFT_Clear(YELLOW);
		TFT_Clear(BLACK);
		TFT_Clear(RED);
		TFT_Clear(GREEN);
		TFT_Clear(BLUE);
		TFT_Clear(WHITE);
	}

	TFT_DrawFont_GBK16(20, 10, BLUE, WHITE, "Color Test Finished.");
	TFT_DrawFont_GBK16(20, 10, BLUE, WHITE, "Color Test Finished.");
	TFT_DrawFont_GBK16(20, 10, BLUE, WHITE, "Color Test Finished.");
	TFT_Box(160,110,100,20,BLUE);
	TFT_Box2(160,130,100,20,1);
}
