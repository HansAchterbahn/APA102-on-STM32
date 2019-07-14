/**
 * @brief		This library enables the use of the popular APA102 addressable
 * 				RGB LEDs with the familiar STM32 HAL library.
 * @file		digitalled.c
 * @date		13.07.2019
 * @version		0.1.0
 * @author		Hans Achterbahn
 * @url			https://github.com/HansAchterbahn/APA102-on-STM32
 * @license		CC-BY-SA 4.0
 *
 *
 * @forkAuthor		Remko Welling
 * @forkE-Mail		remko@rfsee.nl
 * @forkUrl			https://github.com/pe1mew/APA102-on-STM32
 * @forkLicense		CC-BY-SA 4.0
 *
 *
 * @settings
 *
 * 		SPI Settings
 * 		------------
 * 		hspi1.Instance = SPI1;
 * 		hspi1.Init.Mode = SPI_MODE_MASTER;
 * 		hspi1.Init.Direction = SPI_DIRECTION_1LINE;
 * 		hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
 * 		hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
 * 		hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
 * 		hspi1.Init.NSS = SPI_NSS_SOFT;
 * 		hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
 * 		hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
 * 		hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
 * 		hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
 *	 	hspi1.Init.CRCPolynomial = 7;
 * 		hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
 * 		hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
 *
 */

/* includes */
#include "digitalled.h"
#include "stdint.h"


/* variables */
uint8_t SpiSendFrame[LED_START_FRAME_SIZE + 4 * LED_FRAME_SIZE + LED_END_FRAME_SIZE];
uint8_t _frameModified; 		// when frame is changed the stimuli is set high
SPI_HandleTypeDef *_spiHandler;

/* functions */

/**
 *  @brief Initialize digital led class
 */
void digiLed_init(SPI_HandleTypeDef *hspi)
{
	_frameModified = TRUE; 		// Initial set to true to force update after initialization of frame buffer

	_spiHandler = hspi;			// SPI handler is given to library

	// TODO Auto-generated constructor stub

	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		_digitalLedframe[led].FieldsIn.INIT = 0x07; // Set MSB first 3 bits to identify start of LED packet
		_digitalLedframe[led].FieldsIn.GLOBAL = 0x00; // Switch off LED global
		_digitalLedframe[led].FieldsIn.BLUE = 0x00;
		_digitalLedframe[led].FieldsIn.GREEN = 0x00;
		_digitalLedframe[led].FieldsIn.RED = 0x00;
	}
	digiLed_update(FALSE); // Update frame buffer using the value of _frameModified as set in initialiser.
}


/**
 * @brief set color of a single led
 * Set the colors of a single led ad position 'led' using single colors
 * @param led position of the led in the string
 * @param blue intensity of the blue color from 0 to 255
 * @param green intensity of the green color from 0 to 255
 * @param red intensity of the red color from 0 to 255
 */
void digiLed_setColor(uint8_t led, uint8_t red, uint8_t green, uint8_t blue)
{
	if (digiLed_TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].FieldsIn.INIT = 0x7; // Set MSB first 3 bits to identify start of LED packet
		_digitalLedframe[led].FieldsIn.GLOBAL = 0x1F; // Set led at maximum illumination
		_digitalLedframe[led].FieldsIn.BLUE = blue;
		_digitalLedframe[led].FieldsIn.GREEN = green;
		_digitalLedframe[led].FieldsIn.RED = red;
	}
	_frameModified = TRUE;
}


/**
 * @brief set color of all LEDs in a string
 * @param blue intensity of the blue color from 0 to 255
 * @param green intensity of the green color from 0 to 255
 * @param red intensity of the red color from 0 to 255
 */
void digiLed_setAllColor(uint8_t red, uint8_t green, uint8_t blue)
{
	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		digiLed_setColor(led, red, green, blue);
	}
}


/**
 * @brief set color of a single led
 * Set the colors of a single led ad position 'led' using RGB color scheme
 * RGB colors are 24 bits of a 32 bit word where the intensity of the colors red, green en blue are
 * expressed as hex values from 0 to 255 (0 - FF).
 * Colors can be set using defines from "APA102_colors.h"
 * @param led position of the led in the string
 * @param rgb color of led in RGB color scheme
 */
void digiLed_setRGB(uint8_t led, uint32_t rgb)
{
	_digitalLedframe[led].FieldsIn.INIT = 0x7;
	_digitalLedframe[led].FieldsIn.GLOBAL = 0x1F;
	_digitalLedframe[led].FieldsIn.BLUE = (uint8_t)(rgb);
	_digitalLedframe[led].FieldsIn.GREEN = (uint8_t)(rgb >> 8);
	_digitalLedframe[led].FieldsIn.RED = (uint8_t)(rgb >> 16);
	_frameModified = TRUE;
}


/**
 * @brief set color of a single led
 * Set the colors of a single led ad position 'led' using RGB color scheme
 * RGB colors are 24 bits of a 32 bit word where the intensity of the colors red, green and blue are
 * expressed as hex values from 0 to 255 (0 - FF).
 * Colors can be set using defines from "APA102_colors.h"
 * @param rgb color of led in RGB color scheme
 */
void digiLed_setAllRGB(uint32_t rgb)
{
	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		digiLed_setRGB(led, rgb);
	}
	_frameModified = TRUE;
}


/**
 * @brief set illumination of a single LED
 * Illumination is a value from 0 to 31. 0 means no light, and 31 maximum illumination.
 * setting illumination can interfere with individual RGB settings
 * @param led position of the led in the string
 * @param intensity of illumination
 */
void digiLed_setLedIllumination(uint8_t led, uint8_t illumination)
{
	if (digiLed_TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].FieldsIn.GLOBAL = illumination;
	}
	_frameModified = TRUE;
}


/**
 * @brief set illumination of a all LEDs in the frame
 * Illumination is a value from 0 to 31. 0 means no light, and 31 maximum illumination.
 * setting illumination can interfere with individual RGB settings
 * @param intensity of illumination
 */
void digiLed_setAllIllumination(uint8_t illumination)
{
	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		_digitalLedframe[led].FieldsIn.GLOBAL = illumination;
	}
	_frameModified = TRUE;
}


/**
 * @brief switch a single led off
 * @param led position of the led in the string to be switched off
 */
void digiLed_setLedOff(uint8_t led)
{
	if (digiLed_TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].FieldsIn.GLOBAL = 0x00;
	}
	_frameModified = TRUE;
}


/**
 * @brief switch a single led on
 * Using this function will preserve the active color settings for the led
 * @param led position of the led in the string to be switched on
 */
void digiLed_setLedOn(uint8_t led)
{
	if (digiLed_TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].FieldsIn.GLOBAL = 0x1F;
	}
	_frameModified = TRUE;
}


/**
 * @brief update led string
 * @param set true to force update leds and false to update only when frame is modified
 */
void digiLed_update(uint8_t forceUpdate)
{
	if(_frameModified | forceUpdate)
	{
		// add start of frame (0x00000000)
		for(int i = 0; i < LED_START_FRAME_SIZE; i++)
		{
			SpiSendFrame[i] = 0x00;
		}

		// add all LED packets of the frame
		uint32_t SpiDataPacket = 0;
		for (uint32_t led = 0; led < LED_FRAME_SIZE; led++)
		{
			SpiSendFrame[LED_START_FRAME_SIZE + SpiDataPacket + 0] = _digitalLedframe[led].FieldsOut.CMD;		// Add INIT and GLOBAL to SPI send frame
			SpiSendFrame[LED_START_FRAME_SIZE + SpiDataPacket + 1] = _digitalLedframe[led].FieldsOut.BLUE; 	// Add BLUE to SPI send frame
			SpiSendFrame[LED_START_FRAME_SIZE + SpiDataPacket + 2] = _digitalLedframe[led].FieldsOut.GREEN;	// Add GREEN to SPI send frame
			SpiSendFrame[LED_START_FRAME_SIZE + SpiDataPacket + 3] = _digitalLedframe[led].FieldsOut.RED;		// Add RED to SPI send frame

			SpiDataPacket = SpiDataPacket + 4;
		}

		// add end of frame (0xffffffff)
		for(int i = 0; i < 4; i++)
		{
			SpiSendFrame[LED_START_FRAME_SIZE + 4*LED_FRAME_SIZE + i] = 0xFF;
		}

		// send spi frame with all led values
		HAL_SPI_Transmit(_spiHandler, SpiSendFrame, sizeof(SpiSendFrame), 10);
	}

	_frameModified = FALSE; // reset frame modified identifier.

}


/**
 * @brief get LED frame size
 * @return LED frame size
 */
uint8_t digiLed_getFrameSize(void)
{
	return LED_FRAME_SIZE;
}


/**
 * @brief Test led position is within range.
 * @param led led position
 * @return result of evaluation ad define.
 */
uint8_t digiLed_TestPosition(uint8_t led)
{
	uint8_t returnValue = OUT_OF_RANGE;
	if (led < LED_FRAME_SIZE)
	{
		returnValue = RANGE_OK;
	}
	return returnValue;
}
