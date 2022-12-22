/**
 * @file digitalled.h
 */

#ifndef DIGITALLED_H_
#define DIGITALLED_H_

/* includes */
#include <stdint.h>
#include "main.h"
#include "stm32f4xx_hal_spi.h"


/* defines */
#define 	LED_FRAME_SIZE   		1		///< \define number of LEDs in the chain

#define 	FALSE 					0		// false value
#define 	TRUE 					1		// true value
#define 	OUT_OF_RANGE			1		// chosen LED does not exist
#define 	RANGE_OK 				0		// chosen LED exist
#define 	LED_START_FRAME_SIZE 	4		// 0x00, 0x00, 0x00, 0x00
#define 	LED_END_FRAME_SIZE 		4 		// 0xFF, 0xFF, 0xFF, 0xFF




/// \class digitalled digitalled.h <digitalled.h>
/// \brief Manipulate a string of RGB LEds using SPI.
///
/// Supported are:
///	- WS2801
/// - APA102 (tested)
/// - APA102c
///
/// The class communicates with a object of the class type myspi that will interact with the API interface.



/* variables */
union
{
	struct
	{					// LSB
		uint8_t RED    : 8;	///< RED led intensity from 0 (off) to 255 (max)
		uint8_t GREEN  : 8; 	///< GREEN led intensity from 0 (off) to 255 (max)
		uint8_t BLUE   : 8; 	///< BLUE led intensity from 0 (off) to 255 (max)
		uint8_t GLOBAL : 5; 	///< Global intensity for all LEDs from 0 (off) to 32 (max)
		uint8_t INIT   : 3; 	///< Initialize, the led configuration starts with '111'
						// MSB
	} FieldsIn;					///< All fields in a LED packet
	struct
	{					// LSB
		uint8_t RED    : 8;		///< RED led intensity from 0 (off) to 255 (max)
		uint8_t GREEN  : 8; 	///< GREEN led intensity from 0 (off) to 255 (max)
		uint8_t BLUE   : 8; 	///< BLUE led intensity from 0 (off) to 255 (max)
		uint8_t CMD	   : 8; 	///< Global intensity for all LEDs && frame start with
						// MSB
			} FieldsOut;
	uint32_t data; 				///< RAW LED packet data
} _digitalLedframe[LED_FRAME_SIZE];	///< Frame of LED packets



/* functions */
void DigiLed_init(SPI_HandleTypeDef *hspi);
void DigiLed_setColor(uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
void DigiLed_setAllColor(uint8_t red, uint8_t green, uint8_t blue);
void DigiLed_setRGB(uint8_t led, uint32_t rgb);
void DigiLed_setAllRGB(uint32_t rgb);
void DigiLed_setLedIllumination(uint8_t led, uint8_t illumination);
void DigiLed_setAllIllumination(uint8_t illumination);
void DigiLed_setLedOff(uint8_t led);
void DigiLed_setLedOn(uint8_t led);
void DigiLed_update(uint8_t forceUpdate);
uint8_t DigiLed_getFrameSize(void);
uint8_t DigiLed_TestPosition(uint8_t led);






#endif /* DIGITALLED_H_ */
