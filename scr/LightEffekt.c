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


/* light effects */


/**
 * @brief	returns a random true color without white
 * @retval	random RGB true color
 */
uint32_t LightEffect_randomTrueColor()
{
	uint32_t Color;
	uint16_t random = rand () % 382;

	// blue-red block
	if(random >=0 && random < 255)
	{
		random = random - 0;
		Color = LightEffect_getColorArgb(255, random, 0, 255-random);
	}

	// red-green block
	if(random >=255 && random < 510)
	{
		random = random - 255;
		Color = LightEffect_getColorArgb(255, 255-random, random, 0);
	}

	// green-blue block
	if(random >=510 && random < 775)
	{
		random = random - 510;
		Color = LightEffect_getColorArgb(255, 0, 255-random, random);
	}

	return Color;
}


/**
 * @brief	returns a random color consisting of red, green and blue
 * @retval	random RGB color
 */
uint32_t LightEffect_randomMixedColor()
{
	return LightEffect_getColorArgb(255, rand() % 255, rand() % 255, rand() % 255);
}


/**
 * @brief	fades in a color from dark LEDs
 * @param	argb		0xaarrggbb		alpha, red, green, blue value
 * 			a			0x00 ... 0xff	alpha - brightness of led (default -> 0xff)
 * 			r			0x00 ... 0xff	red led color
 * 			g			0x00 ... 0xff	green led color
 * 			b			0x00 ... 0xff	blue led color
 * @param	Steps		0 ... 255		amount of fading steps
 * @param	DelayTime	0 ... 255 [ms]	delay for every fading step
 */
void LightEffect_fadeIn(uint32_t argb, uint8_t Steps, uint8_t DelayTime)
{
	/* split argb to r, g, b */
	uint8_t r = LightEffect_getColorRed(argb);
	uint8_t g = LightEffect_getColorGreen(argb);
	uint8_t b = LightEffect_getColorBlue(argb);

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
 * @param	argb		0xaarrggbb		alpha, red, green, blue value
 * 			a			0x00 ... 0xff	alpha - brightness of led (default -> 0xff)
 * 			r			0x00 ... 0xff	red led color
 * 			g			0x00 ... 0xff	green led color
 * 			b			0x00 ... 0xff	blue led color
 * @param	Steps		0 ... 255		amount of fading steps
 * @param	DelayTime	0 ... 255 [ms]	delay for every fading step
 */
void LightEffect_fadeOut(uint32_t argb, uint8_t Steps, uint8_t DelayTime)
{
	/* split argb to r, g, b */
	uint8_t r = LightEffect_getColorRed(argb);
	uint8_t g = LightEffect_getColorGreen(argb);
	uint8_t b = LightEffect_getColorBlue(argb);

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
 * @param	argb		0xaarrggbb		alpha, red, green, blue value
 * 			a			0x00 ... 0xff	alpha - brightness of led (default -> 0xff)
 * 			r			0x00 ... 0xff	red led color
 * 			g			0x00 ... 0xff	green led color
 * 			b			0x00 ... 0xff	blue led color
 * @param	Steps		0 ... 255		amount of fading steps
 * @param	DelayTime	0 ... 255 [ms]	delay for every fading step
 */
void LightEffect_fadeInFadeOut(uint32_t argb, uint8_t Steps, uint8_t DelayTime)
{
	/* fade in and fade out */
	LightEffect_fadeIn(argb, Steps, DelayTime);
	LightEffect_fadeOut(argb, Steps, DelayTime);
}


/**
 * @brief	set a RGB Color to LEDs
 * @param	r			0 ... 255		red led color
 * @param	g			0 ... 255		green led color
 * @param	b			0 ... 255		blue led color
 */
void LightEffect_setColor(uint32_t argb)
{
	digiLed_setAllRGB(argb);		// set send vector to Color
	digiLed_update(FALSE);			// send vector to LEDs
}

/**
 * @brief	fades actual color to chosen color on LEDs
 * @param	argb		0xaarrggbb		alpha, red, green, blue value
 * 			a			0x00 ... 0xff	alpha - brightness of led (default -> 0xff)
 * 			r			0x00 ... 0xff	red led color
 * 			g			0x00 ... 0xff	green led color
 * 			b			0x00 ... 0xff	blue led color
 * @param	Steps		0 ... 255		amount of fading steps
 * @param	DelayTime	0 ... 255 [ms]	delay for every fading step
 */
void LightEffect_fadeToColor(uint32_t argb, uint8_t Steps, uint8_t DelayTime)
{
	/* split argb to r, g, b */
	uint8_t r = LightEffect_getColorRed(argb);
	uint8_t g = LightEffect_getColorGreen(argb);
	uint8_t b = LightEffect_getColorBlue(argb);

	/* split old dominant color to r, g, b */
	uint8_t r_old = LightEffect_getColorRed(_LightEffectColor);
	uint8_t g_old = LightEffect_getColorGreen(_LightEffectColor);
	uint8_t b_old = LightEffect_getColorBlue(_LightEffectColor);

	/* calculate step size between colors */
	int16_t step_r = r - r_old;
	int16_t step_g = g - g_old;
	int16_t step_b = b - b_old;

	/* fade to Color */
	for(int16_t i = 0; i <= Steps; i++)
	{
		digiLed_setAllColor((r_old * Steps + i*step_r)/Steps,
							(g_old * Steps + i*step_g)/Steps,
							(b_old * Steps + i*step_b)/Steps);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}

	/* set new dominant color */
	_LightEffectColor = LightEffect_getColorArgb(255, r, g, b);

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
	//HAL_Delay(DelayTime);
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
	//HAL_Delay(DelayTime);
}


void LightEffect_scanLeftToRight(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	for(int8_t n=3; n>=0; n--)
	{
		digiLed_setAllRGB(background);
		digiLed_setRGB(0+n, foreground);
		digiLed_setRGB(4+n, foreground);
		digiLed_setRGB(8+n, foreground);
		digiLed_setRGB(12+n, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}

void LightEffect_scanRightToLeft(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	for(uint8_t n=0; n<4; n++)
	{
		digiLed_setAllRGB(background);
		digiLed_setRGB(0+n, foreground);
		digiLed_setRGB(4+n, foreground);
		digiLed_setRGB(8+n, foreground);
		digiLed_setRGB(12+n, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}


void LightEffect_scanUpToDown(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	for(uint8_t n=0; n<4; n++)
	{
		digiLed_setAllRGB(background);
		digiLed_setRGB(0+n*4, foreground);
		digiLed_setRGB(1+n*4, foreground);
		digiLed_setRGB(2+n*4, foreground);
		digiLed_setRGB(3+n*4, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}

void LightEffect_scanDownToUp(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	for(int8_t n=3; n>=0; n--)
	{
		digiLed_setAllRGB(background);
		digiLed_setRGB(0+n*4, foreground);
		digiLed_setRGB(1+n*4, foreground);
		digiLed_setRGB(2+n*4, foreground);
		digiLed_setRGB(3+n*4, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}



void LightEffect_fillLeftToRight(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);

	for(int8_t n=3; n>=0; n--)
	{
		digiLed_setRGB(0+n, foreground);
		digiLed_setRGB(4+n, foreground);
		digiLed_setRGB(8+n, foreground);
		digiLed_setRGB(12+n, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}

void LightEffect_fillRightToLeft(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);

	for(uint8_t n=0; n<4; n++)
	{
		digiLed_setRGB(0+n, foreground);
		digiLed_setRGB(4+n, foreground);
		digiLed_setRGB(8+n, foreground);
		digiLed_setRGB(12+n, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}


void LightEffect_fillUpToDown(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);

	for(uint8_t n=0; n<4; n++)
	{
		digiLed_setRGB(0+n*4, foreground);
		digiLed_setRGB(1+n*4, foreground);
		digiLed_setRGB(2+n*4, foreground);
		digiLed_setRGB(3+n*4, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}

void LightEffect_fillDownToUp(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);

	for(int8_t n=3; n>=0; n--)
	{
		digiLed_setRGB(0+n*4, foreground);
		digiLed_setRGB(1+n*4, foreground);
		digiLed_setRGB(2+n*4, foreground);
		digiLed_setRGB(3+n*4, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}

void LightEffect_fill2Lines(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);

	for(int8_t n=0; n<4; n++)
	{
		digiLed_setRGB(0+n, foreground);
		digiLed_setRGB(4+n, foreground);
		digiLed_setRGB(11-n, foreground);
		digiLed_setRGB(15-n, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}

void LightEffect_fill4Lines(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);

	for(int8_t n=0; n<4; n++)
	{
		digiLed_setRGB(0+n, foreground);
		digiLed_setRGB(7-n, foreground);
		digiLed_setRGB(8+n, foreground);
		digiLed_setRGB(15-n, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}

void LightEffect_fill2Columns(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);

	for(int8_t n=0; n<4; n++)
	{
		digiLed_setRGB(0+n*4, foreground);
		digiLed_setRGB(1+n*4, foreground);
		digiLed_setRGB(14-n*4, foreground);
		digiLed_setRGB(15-n*4, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}

void LightEffect_fill4Columns(uint32_t foreground, uint32_t background, uint8_t DelayTime)
{
	digiLed_setAllRGB(background);

	for(int8_t n=0; n<4; n++)
	{
		digiLed_setRGB(0+n*4, foreground);
		digiLed_setRGB(13-n*4, foreground);
		digiLed_setRGB(2+n*4, foreground);
		digiLed_setRGB(15-n*4, foreground);
		digiLed_update(FALSE);
		HAL_Delay(DelayTime);
	}
}




/* color transformations */

uint32_t LightEffect_getColorRgb(uint8_t r, uint8_t g, uint8_t b)
{
	return (uint32_t)(r << SHIFT_RED | g << SHIFT_GREEN | b << SHIFT_BLUE);
}

uint32_t LightEffect_getColorArgb(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	return (uint32_t)(a << SHIFT_ALPHA | r << SHIFT_RED | g << SHIFT_GREEN | b << SHIFT_BLUE);
}

uint8_t LightEffect_getColorAlpha(uint32_t argb)
{
	return (uint8_t)((MASK_ALPHA & argb) >> SHIFT_ALPHA);
}


uint8_t LightEffect_getColorRed(uint32_t argb)
{
	return (uint8_t)((MASK_RED & argb) >> SHIFT_RED);
}


uint8_t LightEffect_getColorGreen(uint32_t argb)
{
	return (uint8_t)((MASK_GREEN & argb) >> SHIFT_GREEN);
}


uint8_t LightEffect_getColorBlue(uint32_t argb)
{
	return (uint8_t)((MASK_BLUE & argb) >> SHIFT_BLUE);
}


/* light effect combinations */

void LightEffect_comboRotation01(uint8_t DelayTime)
{
	/* get old dominant color and a new random one */
	uint32_t rgb_old = _LightEffectColor;
	uint32_t rgb = LightEffect_randomMixedColor();

	/* rotating light effects */
	LightEffect_rotatingFadeLeft(rgb, rgb_old , DelayTime);
	LightEffect_rotatingFadeRight(rgb_old, rgb, DelayTime);
	LightEffect_rotatingRight(rgb, rgb_old, DelayTime);
	LightEffect_rotatingRight(rgb, rgb_old, DelayTime);
	LightEffect_rotatingFadeLeft(rgb, rgb_old , DelayTime);

	/* set new dominand color */
	_LightEffectColor = rgb;
}

void LightEffect_comboFill01(uint8_t DelayTime)
{
	/* get old dominant color and a new random one */
	uint32_t rgb_old = _LightEffectColor;
	uint32_t rgb = LightEffect_randomMixedColor();

	/* rotating light effects */
	rgb_old = rgb;
	rgb = LightEffect_randomMixedColor();
	LightEffect_fill2Lines(rgb, rgb_old, 250);

	rgb_old = rgb;
	rgb = LightEffect_randomMixedColor();
	LightEffect_fill4Lines(rgb, rgb_old, 250);

	rgb_old = rgb;
	rgb = LightEffect_randomMixedColor();
	LightEffect_fill2Columns(rgb, rgb_old, 250);

	rgb_old = rgb;
	rgb = LightEffect_randomMixedColor();
	LightEffect_fill4Columns(rgb, rgb_old, 250);

	/* set new dominand color */
	_LightEffectColor = rgb;
}
