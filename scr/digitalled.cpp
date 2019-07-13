/*
 * digitalled.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: Remko welling
 *      		remko@rfsee.nl
 *
 */

#include "stdint.h"
#include "..\libraries\digitalled.h"
#include"..\Libraries\myspi.h"

const uint8_t	OUT_OF_RANGE = 1;
const uint8_t 	RANGE_OK = 0;

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
	// Send start of frame (0x00000000)
	_spiInterface.sendRaw(0x00);
	_spiInterface.sendRaw(0x00);
	_spiInterface.sendRaw(0x00);
	_spiInterface.sendRaw(0x00);

	// Send all LED packets of the frame
	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		_spiInterface.sendRaw((uint8_t)(_digitalLedframe[led].data >> 24)); // Send INIT and GLOBAL
		_spiInterface.sendRaw((uint8_t)(_digitalLedframe[led].data >> 16)); // Send BLUE
		_spiInterface.sendRaw((uint8_t)(_digitalLedframe[led].data >> 8));  // Send GREEN
		_spiInterface.sendRaw((uint8_t)(_digitalLedframe[led].data));		// Send RED
	}

	// Send end of frame (0xffffffff)
	_spiInterface.sendRaw(0xff);
	_spiInterface.sendRaw(0xff);
	_spiInterface.sendRaw(0xff);
	_spiInterface.sendRaw(0xff);

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
