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

#define MASK_ALPHA 		0xff000000
#define MASK_RED 		0x00ff0000
#define MASK_GREEN 		0x0000ff00
#define MASK_BLUE 		0x000000ff

#define SHIFT_ALPHA		24
#define SHIFT_RED		16
#define SHIFT_GREEN		8
#define SHIFT_BLUE		0

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

void LightEffect_rotatingRight(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_rotatingLeft(uint32_t foreground, uint32_t background, uint8_t DelayTime);

void LightEffect_rotatingFadeRight(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_rotatingFadeLeft(uint32_t foreground, uint32_t background, uint8_t DelayTime);


uint32_t LightEffect_giveColorRgb(uint8_t r, uint8_t g, uint8_t b);
uint32_t LightEffect_giveColorArgb(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
uint8_t LightEffect_giveColorR(uint32_t rgb);
uint8_t LightEffect_giveColorG(uint32_t rgb);
uint8_t LightEffect_giveColorB(uint32_t rgb);
uint8_t LightEffect_giveColorA(uint32_t rgb);





#endif /* LIGHTEFFEKT_H_ */
