/*
 * digitalled.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: Remko welling
 *      		remko@rfsee.nl
 *
 *
 * SPI Settings
 * ------------
 * hspi1.Instance = SPI1;
 * hspi1.Init.Mode = SPI_MODE_MASTER;
 * hspi1.Init.Direction = SPI_DIRECTION_1LINE;
 * hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
 * hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
 * hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
 * hspi1.Init.NSS = SPI_NSS_SOFT;
 * hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
 * hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
 * hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
 * hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
 * hspi1.Init.CRCPolynomial = 7;
 * hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
 * hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
 *
 */

#include "stdint.h"
#include "digitalled.h"

const uint8_t	OUT_OF_RANGE = 1;
const uint8_t 	RANGE_OK = 0;
const uint8_t LED_START_FRAME_SIZE = 4;		// 0x00, 0x00, 0x00, 0x00
const uint8_t LED_END_FRAME_SIZE = 4; 		// 0xFF, 0xFF, 0xFF, 0xFF
uint8_t SpiSendFrame[LED_START_FRAME_SIZE + 4 * LED_FRAME_SIZE + LED_END_FRAME_SIZE];

digitalled::digitalled()
:
		_frameModified(true) // Initial set to true to force update after initialization of frame buffer
{
	// TODO Auto-generated constructor stub

	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		_digitalLedframe[led].fields.INIT = 0x07; // Set MSB first 3 bits to identify start of LED packet
		_digitalLedframe[led].fields.GLOBAL = 0x00; // Switch off LED global
		_digitalLedframe[led].fields.BLUE = 0x00;
		_digitalLedframe[led].fields.GREEN = 0x00;
		_digitalLedframe[led].fields.RED = 0x00;
	}
	update(false); // Update frame buffer using the value of _frameModified as set in initialiser.
}

digitalled::~digitalled()
{
	// TODO Auto-generated destructor stub
}

void digitalled::setColor(const uint8_t led, const uint8_t red, const uint8_t green, const uint8_t blue)
{
	if (TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].fields.INIT = 0x7; // Set MSB first 3 bits to identify start of LED packet
		_digitalLedframe[led].fields.GLOBAL = 0x1F; // Set led at maximum illumination
		_digitalLedframe[led].fields.BLUE = blue;
		_digitalLedframe[led].fields.GREEN = green;
		_digitalLedframe[led].fields.RED = red;
	}
	_frameModified = true;
}

void digitalled::setAllColor(const uint8_t red, const uint8_t green, const uint8_t blue)
{
	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		setColor(led, red, green, blue);
	}
}

void digitalled::setRGB(const uint8_t led, const uint32_t rgb)
{
	_digitalLedframe[led].fields.INIT = 0x7;
	_digitalLedframe[led].fields.GLOBAL = 0x1F;
	_digitalLedframe[led].fields.BLUE = (uint8_t)(rgb);
	_digitalLedframe[led].fields.GREEN = (uint8_t)(rgb >> 8);
	_digitalLedframe[led].fields.RED = (uint8_t)(rgb >> 16);
	_frameModified = true;
		}

void digitalled::setAllRGB(const uint32_t rgb)
{
	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		setRGB(led, rgb);
	}
	_frameModified = true;
}

void digitalled::setLedIllumination(const uint8_t led, uint8_t illumination)
{
	if (TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].fields.GLOBAL = illumination;
	}
	_frameModified = true;
}

void digitalled::setAllIllumination(uint8_t illumination)
{
	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		_digitalLedframe[led].fields.GLOBAL = illumination;
	}
	_frameModified = true;
}

void digitalled::setLedOff(const uint8_t led)
{
	if (TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].fields.GLOBAL = 0x00;
	}
	_frameModified = true;
}

void digitalled::setLedOn(const uint8_t led)
{
	if (TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].fields.GLOBAL = 0x1F;
	}
	_frameModified = true;
}

void digitalled::update(bool forceUpdate)
{
	if(_frameModified | forceUpdate)
	{
		// add start of frame (0x00000000)
		for(int i = 0; i < LED_START_FRAME_SIZE; i++)
		{
			SpiSendFrame[i] = 0x00;
		}
//		HAL_SPI_Transmit(&spi1, &LED_START_FRAME, sizeof(LED_START_FRAME), 10);

		// add all LED packets of the frame
		for (int led = 0; led < LED_FRAME_SIZE; led++)
		{
			SpiSendFrame[LED_START_FRAME_SIZE + led] = _digitalLedframe[led].FieldsOut.CMD;		// Add INIT and GLOBAL to SPI send frame
			SpiSendFrame[LED_START_FRAME_SIZE + led] = _digitalLedframe[led].FieldsOut.BLUE; 	// Add BLUE to SPI send frame
			SpiSendFrame[LED_START_FRAME_SIZE + led] = _digitalLedframe[led].FieldsOut.GREEN;	// Add GREEN to SPI send frame
			SpiSendFrame[LED_START_FRAME_SIZE + led] = _digitalLedframe[led].FieldsOut.RED;		// Add RED to SPI send frame
		}

		// add end of frame (0xffffffff)
		for(int i = 0; i < 4; i++)
		{
			SpiSendFrame[LED_FRAME_SIZE + i] = 0xFF;
		}

		// send spi frame with all led values
		HAL_SPI_Transmit(&spi1, &SpiSendFrame, sizeof(SpiSendFrame), 10);
	}

	_frameModified = false; // reset frame modified identifier.

}

uint8_t digitalled::TestPosition(const uint8_t led)
{
	uint8_t returnValue = OUT_OF_RANGE;
	if (led < LED_FRAME_SIZE)
	{
		returnValue = RANGE_OK;
	}
	return returnValue;
}
