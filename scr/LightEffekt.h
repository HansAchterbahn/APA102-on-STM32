/**
 * @file	LightEffekt.h
 *
 */

#ifndef LIGHTEFFEKT_H_
#define LIGHTEFFEKT_H_


/* includes */
#include "main.h"
#include <stdlib.h>
#include <time.h>


/* variables */
struct _LightEffectColorRGB{
	uint8_t r;		// red
	uint8_t g;		// green
	uint8_t b;		// blue
	uint8_t a;		// alpha
};

union
{
	struct{
		uint8_t r;		// red
		uint8_t g;		// green
		uint8_t b;		// blue
		uint8_t a;		// alpha
	}separate;			// Color separate in red, green, blue and alpha
	uint32_t rgba;		// 0xrrggbbaa
} _LightEffectColor;


/* funktions */
struct _LightEffectColorRGB LightEffect_randomTrueColor();
struct _LightEffectColorRGB LightEffect_randomMixedColor();

void LightEffect_fadeIn(uint8_t r, uint8_t g, uint8_t b, uint8_t Steps, uint8_t DelayTime);
void LightEffect_fadeOut(uint8_t r, uint8_t g, uint8_t b, uint8_t Steps, uint8_t DelayTime);
void LightEffect_fadeInFadeOut(uint8_t r, uint8_t g, uint8_t b, uint8_t Steps, uint8_t DelayTime);

void LightEffect_setColor(uint8_t r, uint8_t g, uint8_t b);
void LightEffect_fadeToColor(uint8_t r, uint8_t g, uint8_t b, uint8_t Steps, uint8_t DelayTime);




#endif /* LIGHTEFFEKT_H_ */
