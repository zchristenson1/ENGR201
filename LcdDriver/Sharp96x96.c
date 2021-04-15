
//*****************************************************************************
//
// IMPORTANT NOTE: The following driver file supports using either RAM or
// non-volatile memory(Flash or FRAM) to store the LCD DisplayBuffer.
//
//*****************************************************************************

//*****************************************************************************
//
// Sharp96x96.c
//
//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include "grlib.h"
#include "Sharp96x96.h"
#include <msp430.h>
#ifdef __MSP430FR5969
#include "HAL_MSP_EXP430FR5969_Sharp96x96.h"
#include "driverlib.h"
#elif defined(__MSP430G2553)
#include "HAL_MSP_EXP430G2_Sharp96x96.h"
#elif defined(__MSP430FR4133)
#include "HAL_MSP_EXP430FR4133_Sharp96x96.h"
#endif
#include <stdint.h>

const uint8_t reverse_data[] = {0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE, 0x1,
		0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};
uint8_t VCOMbit= 0x40;
uint8_t flagSendToggleVCOMCommand = 0;


static void Sharp96x96_InitializeDisplayBuffer(void *pvDisplayData, uint8_t ucValue);
static uint8_t Sharp96x96_reverse(uint8_t x);

//*****************************************************************************
//
// If flash is used as non-volatile memory, the DisplayBuffer will have 32 extra
// vertical lines to create a buffer size which is a multiple of 512 Byte
// (this is the default size of flash memory segments in MSP430 device). It is
// required that the display buffer is a multiple of 512 Bytes to be able to
// initialize the buffer using the FLASH segment erase feature of the FLASH
// controller.
//
//*****************************************************************************

#ifdef NON_VOLATILE_MEMORY_BUFFER
#pragma location=NON_VOLATILE_MEMORY_ADDRESS
#endif

#ifndef NON_VOLATILE_MEMORY_BUFFER
uint8_t DisplayBuffer[LCD_VERTICAL_MAX][LCD_HORIZONTAL_MAX/8];
#else
#ifdef __ICC430__
__no_init uint8_t DisplayBuffer[LCD_VERTICAL_MAX +32][LCD_HORIZONTAL_MAX/8];
#else
uint8_t DisplayBuffer[LCD_VERTICAL_MAX +32][LCD_HORIZONTAL_MAX/8];
#endif //__ICC430__
#endif //NON_VOLATILE_MEMORY_BUFFER

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the Sharp96x96 display controller preparing it to
//! display data.
//!
//! \return None.
//
//*****************************************************************************
void Sharp96x96_initDisplay(void)
{
	HAL_LCD_initDisplay();
}

//*****************************************************************************
//
//! Send toggle VCOM command.
//!
//! This function toggles the state of VCOM which prevents a DC bias from being
//! built up within the panel.
//!
//! \return None.
//
//*****************************************************************************
void Sharp96x96_SendToggleVCOMCommand()
{
	VCOMbit ^= SHARP_VCOM_TOGGLE_BIT;

	if(SHARP_SEND_TOGGLE_VCOM_COMMAND == flagSendToggleVCOMCommand)
	{
		//clear screen mode(0X100000b)
		uint8_t command = SHARP_LCD_CMD_CHANGE_VCOM;
		//COM inversion bit
		command = command^VCOMbit;

	    HAL_LCD_setCS();

		HAL_LCD_writeCommandOrData(command);
		HAL_LCD_writeCommandOrData(SHARP_LCD_TRAILER_BYTE);

		// Wait for last byte to be sent, then drop SCS
		HAL_LCD_waitUntilLcdWriteFinish();

		// Ensure a 2us min delay to meet the LCD's thSCS
		__delay_cycles(SYSTEM_CLOCK_SPEED * 0.000002);

		HAL_LCD_clearCS();
	}

	flagSendToggleVCOMCommand = SHARP_SEND_TOGGLE_VCOM_COMMAND;
}


void Sharp96x96_disable(void)
{
	HAL_LCD_disableDisplay();
}

void Sharp96x96_enable(void)
{
	HAL_LCD_enableDisplay();

}

//*******************************************************************************
//
//! Reverses the bit order.- Since the bit reversal function is called
//! frequently by the several driver function this function is implemented
//! to maximize code execution
//
//*******************************************************************************
static uint8_t Sharp96x96_reverse(uint8_t x)
{
  uint8_t b = 0;

  b  = reverse_data[x & 0xF]<<4;
  b |= reverse_data[(x & 0xF0)>>4];
  return b;
}

//*****************************************************************************
//
//! Initialize DisplayBuffer.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//!	\param ucValue is the foreground color of the buffered data.
//!
//! This function initializes the display buffer and discards any cached data.
//!
//! \return None.
//
//*****************************************************************************
static void Sharp96x96_InitializeDisplayBuffer(void *pvDisplayData,
		uint8_t ucValue)
{
#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_prepareMemoryWrite();
#endif

#ifdef USE_FLASH_BUFFER
	// This is a callback function to HAL file since it implements device
	// specific functionality
	HAL_LCD_InitializeDisplayBuffer(pvDisplayData, ucValue);

#else
  uint16_t i=0,j=0;
  uint8_t *pucData = pvDisplayData;
    for(i =0; i< LCD_VERTICAL_MAX; i++)
    for(j =0; j< (LCD_HORIZONTAL_MAX>>3); j++)
            *pucData++ = ucValue;

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_finishMemoryWrite();
#endif

#endif //USE_FLASH_BUFFER
}

//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void Sharp96x96_PixelDraw(void *pvDisplayData, int16_t lX, int16_t lY,
                                   uint16_t ulValue)
{

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_prepareMemoryWrite();
#endif

	if( ClrBlack == ulValue){
		DisplayBuffer[lY][lX>>3] &= ~(0x80 >> (lX & 0x7));
	}else{
		DisplayBuffer[lY][lX>>3] |= (0x80 >> (lX & 0x7));
	}

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_finishMemoryWrite();
#endif

}
//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the first pixel.
//! \param lY is the Y coordinate of the first pixel.
//! \param lX0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param lCount is the number of pixels to draw.
//! \param lBPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param pucData is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pucPalette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void Sharp96x96_DrawMultiple(void *pvDisplayData, int16_t lX,
                                           int16_t lY, int16_t lX0, int16_t lCount,
                                           int16_t lBPP,
                                           const uint8_t *pucData,
                                           const uint32_t *pucPalette)
{  

	uint8_t *pData = &DisplayBuffer[lY][lX>>3];
	uint16_t xj = 0;

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_prepareMemoryWrite();
#endif

	//Write bytes of data to the display buffer
	for(xj=0;xj<(lCount >> 3);xj++){
		*pData++ = *pucData++;
	}

	//Write last data byte to the display buffer
	*pData = (*pData & (0xFF >> (lCount & 0x7))) | *pucData;

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_finishMemoryWrite();
#endif
}
//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void Sharp96x96_LineDrawH(void *pvDisplayData, int16_t lX1, int16_t lX2,
                                   int16_t lY, uint16_t ulValue)
{
	uint16_t xi = 0;
	uint16_t x_index_min = lX1>>3;
	uint16_t x_index_max = lX2>>3;
	uint8_t *pucData, ucfirst_x_byte, uclast_x_byte;

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_prepareMemoryWrite();
#endif

	//calculate first byte
	//mod by 8 and shift this # bits
	ucfirst_x_byte = (0xFF >> (lX1 & 0x7));    
	//calculate last byte
	//mod by 8 and shift this # bits
	uclast_x_byte = (0xFF << (7-(lX2 & 0x7))); 

	//check if more than one data byte
	if(x_index_min != x_index_max){

		//set buffer to correct location
		pucData = &DisplayBuffer[lY][x_index_min];

		//black pixels (clear bits)
		if(ClrBlack == ulValue)
		{
			//write first byte
			*pucData++ &= ~ucfirst_x_byte;

			//write middle bytes
			for(xi = x_index_min; xi < x_index_max-1; xi++)
			{
				*pucData++ = 0x00;
			}

			//write last byte
			*pucData &= ~uclast_x_byte;
		}
		//white pixels (set bits)
		else
		{
			//write first byte
			*pucData++ |= ucfirst_x_byte;

			//write middle bytes
			for(xi = x_index_min; xi < x_index_max-1; xi++)
			{
				*pucData++ = 0xFF;
			}

			//write last byte
			*pucData |= uclast_x_byte;
		}
	}
	//only one data byte
	else
	{
		//calculate value of single byte
		ucfirst_x_byte &= uclast_x_byte;

		//set buffer to correct location
		pucData = &DisplayBuffer[lY][x_index_min];

		//draw black pixels (clear bits)
		if(ClrBlack == ulValue)
		{
			*pucData++ &= ~ucfirst_x_byte;
		}
		//white pixels (set bits)
		else
		{
			*pucData++ |= ucfirst_x_byte;
		}
	}

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_finishMemoryWrite();
#endif
}


//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void Sharp96x96_LineDrawV(void *pvDisplayData, int16_t lX, int16_t lY1,
                                   int16_t lY2, uint16_t ulValue)
{

	uint16_t yi = 0;
	uint16_t x_index = lX>>3;
	uint8_t data_byte;

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_prepareMemoryWrite();
#endif

	//calculate data byte
	//mod by 8 and shift this # bits
	data_byte = (0x80 >> (lX & 0x7));     

	//write data to the display buffer
	for(yi = lY1; yi <= lY2; yi++){

		//black pixels (clear bits)
		if(ClrBlack == ulValue)
		{
			DisplayBuffer[yi][x_index] &= ~data_byte;
		}
		//white pixels (set bits)
		else
		{
			DisplayBuffer[yi][x_index] |= data_byte;
		}
	}

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_finishMemoryWrite();
#endif
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both sXMin and
//! sXMax are drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
static void Sharp96x96_RectFill(void *pvDisplayData, const Graphics_Rectangle *pRect,
                                  uint16_t ulValue)
{
	uint16_t xi = 0;
	uint16_t yi = 0;
	uint16_t x_index_min = pRect->sXMin>>3;
	uint16_t x_index_max = pRect->sXMax>>3;
	uint8_t *pucData, ucfirst_x_byte, uclast_x_byte;

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_prepareMemoryWrite();
#endif

	//calculate first byte
	//mod by 8 and shift this # bits
	ucfirst_x_byte = (0xFF >> (pRect->sXMin & 0x7));  

	//calculate last byte
	//mod by 8 and shift this # bits
	uclast_x_byte = (0xFF << (7-(pRect->sXMax & 0x7)));   

	//check if more than one data byte
	if(x_index_min != x_index_max){

		//write bytes
		for (yi = pRect->sYMin; yi<= pRect->sYMax; yi++)
		{
			//set buffer to correct location
			pucData = &DisplayBuffer[yi][x_index_min];

			//black pixels (clear bits)
			if(ClrBlack == ulValue)
			{
				//write first byte
				*pucData++ &= ~ucfirst_x_byte;

				//write middle bytes
				for(xi = x_index_min; xi < x_index_max-1; xi++)
				{
					*pucData++ = 0x00;
				}

				//write last byte
				*pucData &= ~uclast_x_byte;
			}
			//white pixels (set bits)
			else
			{
				//write first byte
				*pucData++ |= ucfirst_x_byte;

				//write middle bytes
				for(xi = x_index_min; xi < x_index_max-1; xi++)
				{
					*pucData++ = 0xFF;
				}

				//write last byte
				*pucData |= uclast_x_byte;
			}
		}
	}
	//only one data byte
	else
	{
		//calculate value of single byte
		ucfirst_x_byte &= uclast_x_byte;

		//set buffer to correct location
		pucData = &DisplayBuffer[pRect->sYMin][x_index_min];

		//black pixels (clear bits)
		if(ClrBlack == ulValue)
		{
			*pucData++ &= ~ucfirst_x_byte;
		}
		//white pixels (set bits)
		else
		{
			*pucData++ |= ucfirst_x_byte;
		}
	}

#ifdef NON_VOLATILE_MEMORY_BUFFER
	HAL_LCD_finishMemoryWrite();
#endif
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//!   display driver
//! \param ulValue is the 24-bit RGB color.  The least-significant byte is the
//!   blue channel, the next byte is the green channel, and the third byte is
//!   the red channel.
//!
//! This fucntion translates a 24-bit RGB color into a value that can be written
//! into the display's frame buffer in order to reproduce that color, or the
//! closest possible approximation of that color. This particular driver
//! requires the 8-8-8 24 bit RGB color to convert into mono color
//! 1 = White, 0 = Black
//!
//! \return Returns the display-driver specific color
//
//*****************************************************************************
static uint32_t Sharp96x96_ColorTranslate(void *pvDisplayData,
                                        uint32_t ulValue)
{
    //
    // Translate from a 24-bit RGB color to mono color.
    //
    return(((ulValue != 0) ? ulValue = 1 : ulValue ));
}


//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.
//!
//! \return None.
//
//*****************************************************************************
static void Sharp96x96_Flush (void *pvDisplayData)
{
	uint8_t *pucData = &DisplayBuffer[0][0];
	int32_t xi =0;
	int32_t xj = 0;
	//image update mode(1X000000b)
	uint8_t command = SHARP_LCD_CMD_WRITE_LINE;

	//COM inversion bit
	command = command^VCOMbit;

	HAL_LCD_setCS();

	HAL_LCD_writeCommandOrData(command);
	flagSendToggleVCOMCommand = SHARP_SKIP_TOGGLE_VCOM_COMMAND;
#ifdef LANDSCAPE
	for(xj=0; xj<LCD_VERTICAL_MAX; xj++)
		{
		  HAL_LCD_writeCommandOrData(Sharp96x96_reverse(xj + 1));

		  for(xi=0; xi<(LCD_HORIZONTAL_MAX>>3); xi++)
		  {
			HAL_LCD_writeCommandOrData(*(pucData++));
		  }
		  HAL_LCD_writeCommandOrData(SHARP_LCD_TRAILER_BYTE);
		}
#endif
#ifdef LANDSCAPE_FLIP
	pucData = &DisplayBuffer[LCD_VERTICAL_MAX-1][(LCD_HORIZONTAL_MAX>>3)-1];

	for(xj=1; xj<=LCD_VERTICAL_MAX; xj++)
	{
		HAL_LCD_writeCommandOrData(Sharp96x96_reverse(xj));

	  for(xi=0; xi < (LCD_HORIZONTAL_MAX>>3); xi++)
	  {
		HAL_LCD_writeCommandOrData(Sharp96x96_reverse(*pucData--));
	  }
	  HAL_LCD_writeCommandOrData(SHARP_LCD_TRAILER_BYTE);
	}
#endif

	HAL_LCD_writeCommandOrData(SHARP_LCD_TRAILER_BYTE);

	// Wait for last byte to be sent, then drop SCS
	HAL_LCD_waitUntilLcdWriteFinish();

    // Ensure a 2us min delay to meet the LCD's thSCS
    __delay_cycles(SYSTEM_CLOCK_SPEED * 0.000002);

	HAL_LCD_clearCS();

}

//*****************************************************************************
//
//! Send command to clear screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ucValue is the background color of the buffered data.
//!
//! This function sets every pixel to the background color.
//!
//! \return None.
//
//*****************************************************************************
static void Sharp96x96_ClearScreen (void *pvDisplayData, uint16_t ulValue)
{
	//clear screen mode(0X100000b)
	uint8_t command = SHARP_LCD_CMD_CLEAR_SCREEN;
	//COM inversion bit
	command = command^VCOMbit;

	HAL_LCD_setCS();

	HAL_LCD_writeCommandOrData(command);
	flagSendToggleVCOMCommand = SHARP_SKIP_TOGGLE_VCOM_COMMAND;
	HAL_LCD_writeCommandOrData(SHARP_LCD_TRAILER_BYTE);

	// Wait for last byte to be sent, then drop SCS
	HAL_LCD_waitUntilLcdWriteFinish();

	// Ensure a 2us min delay to meet the LCD's thSCS
	__delay_cycles(SYSTEM_CLOCK_SPEED * 0.000002);

	HAL_LCD_clearCS();
	if(ClrBlack == ulValue)
	Sharp96x96_InitializeDisplayBuffer(pvDisplayData, SHARP_BLACK);
	else
	Sharp96x96_InitializeDisplayBuffer(pvDisplayData, SHARP_WHITE);

}




//*****************************************************************************
//
//! The display structure that describes the driver for the 
//! sharpLCD panel 
//
//*****************************************************************************
const Graphics_Display g_sharp96x96LCD =
{
    sizeof(tDisplay),
    DisplayBuffer,
    LCD_HORIZONTAL_MAX,
    LCD_VERTICAL_MAX,
    Sharp96x96_PixelDraw, //PixelDraw,
    Sharp96x96_DrawMultiple,
    Sharp96x96_LineDrawH,
    Sharp96x96_LineDrawV, //LineDrawV,
    Sharp96x96_RectFill, //RectFill,
    Sharp96x96_ColorTranslate,
    Sharp96x96_Flush, //Flush
    Sharp96x96_ClearScreen //Clear screen. Contents of display buffer unmodified
};



//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

