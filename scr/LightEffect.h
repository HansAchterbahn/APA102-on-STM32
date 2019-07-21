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
uint32_t _LightEffectColor; 		// saves the actual dominant LED light color


/* funktions */

/* light effects */
uint32_t LightEffect_randomTrueColor();
uint32_t LightEffect_randomMixedColor();

void LightEffect_fadeIn(uint32_t argb, uint8_t Steps, uint8_t DelayTime);
void LightEffect_fadeOut(uint32_t argb, uint8_t Steps, uint8_t DelayTime);
void LightEffect_fadeInFadeOut(uint32_t argb, uint8_t Steps, uint8_t DelayTime);

void LightEffect_setColor(uint32_t argb);
void LightEffect_fadeToColor(uint32_t argb, uint8_t Steps, uint8_t DelayTime);

void LightEffect_rotatingRight(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_rotatingLeft(uint32_t foreground, uint32_t background, uint8_t DelayTime);

void LightEffect_rotatingFadeRight(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_rotatingFadeLeft(uint32_t foreground, uint32_t background, uint8_t DelayTime);

void LightEffect_scanLeftToRight(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_scanRightToLeft(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_scanUpToDown(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_scanDownToUp(uint32_t foreground, uint32_t background, uint8_t DelayTime);

void LightEffect_fillLeftToRight(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_fillRightToLeft(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_fillUpToDown(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_fillDownToUp(uint32_t foreground, uint32_t background, uint8_t DelayTime);

void LightEffect_fill2Lines(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_fill4Lines(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_fill2Columns(uint32_t foreground, uint32_t background, uint8_t DelayTime);
void LightEffect_fill4Columns(uint32_t foreground, uint32_t background, uint8_t DelayTime);



/* color transformations */
uint32_t LightEffect_getColorRgb(uint8_t r, uint8_t g, uint8_t b);
uint32_t LightEffect_getColorArgb(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
uint8_t LightEffect_getColorRed(uint32_t argb);
uint8_t LightEffect_getColorGreen(uint32_t argb);
uint8_t LightEffect_getColorBlue(uint32_t argb);
uint8_t LightEffect_getColorAlpha(uint32_t argb);

/* light effect combinations */
void LightEffect_comboRotation01(uint8_t DelayTime);
void LightEffect_comboFill01(uint8_t DelayTime);






#endif /* LIGHTEFFEKT_H_ */
