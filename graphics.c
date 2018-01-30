#include "graphics.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawBG() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	
	REG_DMA3SAD = (u32)background_space;
	REG_DMA3DAD = (u32)videoBuffer;
	REG_DMA3CNT = (160*240) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
}

//Non-DMA draw rectangle
void drawRectangleNonDMA(int row, int col, int height, int width, u16 color) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			setPixel(row + i, col + j, color);
		}
	}
}

//DMA draw rectangle
void drawRectangleDMA(int row, int col, int height, int width, u16 color) {
	for (int i = 0; i < height; i++) {
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)&videoBuffer[OFFSET(row + i, col, 240)];
		REG_DMA3CNT = width | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
	}
}

void drawImage3(int row, int col, int height, int width, const u16 *image) {
	for (int i = 0; i < height; i++) {
		REG_DMA3SAD = (u32)&image[OFFSET(row + i, col, 240)];
		REG_DMA3DAD = (u32)&videoBuffer[OFFSET(row + i, col, 240)];
		REG_DMA3CNT = width | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
	}
}

void drawChar(int row, int col, char ch, u16 color)
{
	for(int r=0; r< 8; r++)
	{
		for(int c=0; c<6; c++)
		{
			if (fontdata_6x8[OFFSET(r, c, 6) + ch*48] == 1)
			{
				setPixel(row+r, col+c, color);
			}
		}
	}
}

void drawString(int row, int col, char str[], u16 color){
	while(*str)
	{
		drawChar(row, col, *str++, color);
		col += 6;
		
	}
}
