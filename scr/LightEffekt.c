/**
 * @brief		This library offers advanced functions for light effects on APA102 LED stripes.
 * 				It is based on the library digitalled.h from Hans Achterbahn.
 * @file		LightEffekt.c
 * @date		15.07.2019
 * @version		0.1.0
 * @author		Hans Achterbahn
 * @url			https://github.com/HansAchterbahn/APA102-on-STM32
 * @license		MIT-License
 *
 */

/* includes */
#include "LightEffekt.h"
#include "digitalled.h"

/* functions */


/**
 * @brief	returns a random true color without white
 * @retval	random RGB true color
 */
struct _LightEffectColorRGB LightEffect_randomTrueColor()
{
	struct _LightEffectColorRGB Color;
	uint16_t random = rand () % 382;

	// blue-red block
	if(random >=0 && random < 255)
	{
		random = random - 0;

		Color.r = random;
		Color.g = 0;
		Color.b = 255-random;
		Color.a = 255;
	}

	// red-green block
	if(random >=255 && random < 510)
	{
		random = random - 255;

		Color.r = 255-random;
		Color.g = random;
		Color.b = 0;
		Color.a = 255;
	}

	// green-blue block
	if(random >=510 && random < 775)
	{
		random = random - 510;

		Color.r = 0;
		Color.g = 255-random;
		Color.b = random;
		Color.a = 255;
	}

	// write color values to global variable _LightEffectColor
//	_LightEffectColor.separate.r = Color.r;
//	_LightEffectColor.separate.r = Color.g;
//	_LightEffectColor.separate.r = Color.b;
//	_LightEffectColor.separate.r = Color.a;

	return Color;
}


/**
 * @brief	returns a random color consisting of red, green and blue
 * @retval	random RGB color
 */
struct _LightEffectColorRGB LightEffect_randomMixedColor()
{
	struct _LightEffectColorRGB Color;

	Color.r = rand () % 255;		// red
	Color.g = rand () % 255;		// green
	Color.b = rand () % 255;		// blue
	Color.a = 255;					// alpha

	// write color values to global variable
//	_LightEffectColor.separate.r = Color.r;
//	_LightEffectColor.separate.g = Color.g;
//	_LightEffectColor.separate.b = Color.b;
//	_LightEffectColor.separate.a = Color.a;

	return Color;
}


/**
 * @brief	fades in a color from dark LEDs
 * @param	r			0 ... 255		red led color
 * @param	g			0 ... 255		green led color
 * @param	b			0 ... 255		blue led color
 * @param	Steps		0 ... 255		amount of fading steps
 * @param	DelayTime	0 ... 255 [ms]	delay for every fading step
 */
void LightEffect_fadeIn(uint8_t r, uint8_t g, uint8_t b, uint8_t Steps, uint8_t DelayTime)
{
	/* fade in */
	for(int16_t i = Steps; i >= 0; i--)
	{
		digiLed_setAllColor((r*(Steps-i))/Steps, (g*(Steps-i))/Steps, (b*(Steps-i))/Steps);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}


/**
 * @brief	fades out a color to dark LEDs
 * @param	r			0 ... 255		red led color
 * @param	g			0 ... 255		green led color
 * @param	b			0 ... 255		blue led color
 * @param	Steps		0 ... 255		amount of fading steps
 * @param	DelayTime	0 ... 255 [ms]	delay for every fading step
 */
void LightEffect_fadeOut(uint8_t r, uint8_t g, uint8_t b, uint8_t Steps, uint8_t DelayTime)
{
	/* fade out */
	for(int16_t i = 0; i <= Steps; i++)
	{
		digiLed_setAllColor((r*(Steps-i))/Steps, (g*(Steps-i))/Steps, (b*(Steps-i))/Steps);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}


/**
 * @brief	fades in and out a color from and to dark LEDs
 * @param	r			0 ... 255		red led color
 * @param	g			0 ... 255		green led color
 * @param	b			0 ... 255		blue led color
 * @param	Steps		0 ... 255		amount of fading steps
 * @param	DelayTime	0 ... 255 [ms]	delay for every fading step
 */
void LightEffect_fadeInFadeOut(uint8_t r, uint8_t g, uint8_t b, uint8_t Steps, uint8_t DelayTime)
{
	LightEffect_fadeIn(r, g, b, Steps, DelayTime);
	LightEffect_fadeOut(r, g, b, Steps, DelayTime);
}

/**
 * @brief	set a RGB Color to LEDs
 * @param	r			0 ... 255		red led color
 * @param	g			0 ... 255		green led color
 * @param	b			0 ... 255		blue led color
 */
void LightEffect_setColor(uint8_t r, uint8_t g, uint8_t b)
{
	digiLed_setAllColor(r, g, b);
	digiLed_update(FALSE);
}

/**
 * @brief	fades actual color to chosen color on LEDs
 * @param	r			0 ... 255		red led color
 * @param	g			0 ... 255		green led color
 * @param	b			0 ... 255		blue led color
 * @param	Steps		0 ... 255		amount of fading steps
 * @param	DelayTime	0 ... 255 [ms]	delay for every fading step
 */
void LightEffect_fadeToColor(uint8_t r, uint8_t g, uint8_t b, uint8_t Steps, uint8_t DelayTime)
{

	int16_t step_r = r - _LightEffectColor.separate.r;
	int16_t step_g = g - _LightEffectColor.separate.g;
	int16_t step_b = b - _LightEffectColor.separate.b;

	/* fade to Color */
	for(int16_t i = 0; i <= Steps; i++)
	{
		digiLed_setAllColor((_LightEffectColor.separate.r * Steps + i*step_r)/Steps,
							(_LightEffectColor.separate.g * Steps + i*step_g)/Steps,
							(_LightEffectColor.separate.b * Steps + i*step_b)/Steps);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}

	_LightEffectColor.separate.r = r;
	_LightEffectColor.separate.g = g;
	_LightEffectColor.separate.b = b;

}



void LightEffect_rotatingRight(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);
	digiLed_setRGB(1, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(14, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setAllRGB(background);
	digiLed_setRGB(2, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(13, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setAllRGB(background);
	digiLed_setRGB(3, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(12, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setAllRGB(background);
	digiLed_setRGB(7, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(8, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setAllRGB(background);
	digiLed_setRGB(11, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(4, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setAllRGB(background);
	digiLed_setRGB(15, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(0, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);
}




void LightEffect_rotatingLeft(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);
	digiLed_setRGB(1, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(14, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setAllRGB(background);
	digiLed_setRGB(15, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(0, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setAllRGB(background);
	digiLed_setRGB(11, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(4, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setAllRGB(background);
	digiLed_setRGB(7, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(8, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setAllRGB(background);
	digiLed_setRGB(3, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(12, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setAllRGB(background);
	digiLed_setRGB(2, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(13, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);
}




void LightEffect_rotatingFadeRight(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(1, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(14, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(2, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(13, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(3, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(12, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(7, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(8, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(11, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(4, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(15, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(0, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);
}




void LightEffect_rotatingFadeLeft(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(1, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(14, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(15, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(0, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(11, foreground);
	digiLed_setRGB(10, foreground);
	digiLed_setRGB(5, foreground);
	digiLed_setRGB(4, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(7, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(8, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(3, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(12, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);

	digiLed_setRGB(2, foreground);
	digiLed_setRGB(6, foreground);
	digiLed_setRGB(9, foreground);
	digiLed_setRGB(13, foreground);
	digiLed_update(FALSE);
	HAL_Delay(DelayTime);
}



uint32_t LightEffect_giveColorRgb(uint8_t r, uint8_t g, uint8_t b)
{
	return (uint32_t)(r << SHIFT_RED | g << SHIFT_GREEN | b << SHIFT_BLUE);
}

uint32_t LightEffect_giveColorArgb(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	return (uint32_t)(a << SHIFT_ALPHA | r << SHIFT_RED | g << SHIFT_GREEN | b << SHIFT_BLUE);
}

uint8_t LightEffect_giveColorAlpha(uint32_t rgb)
{
	return (uint8_t)((MASK_ALPHA & rgb) >> SHIFT_ALPHA);
}


uint8_t LightEffect_giveColorRed(uint32_t rgb)
{
	return (uint8_t)((MASK_RED & rgb) >> SHIFT_RED);
}


uint8_t LightEffect_giveColorGreen(uint32_t rgb)
{
	return (uint8_t)((MASK_GREEN & rgb) >> SHIFT_GREEN);
}


uint8_t LightEffect_giveColorBlue(uint32_t rgb)
{
	return (uint8_t)((MASK_BLUE & rgb) >> SHIFT_BLUE);
}
