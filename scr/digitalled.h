/*
 * \file digitalled.h
 *
 *  Created on: Apr 8, 2016
 *  \author Remko Welling
 *          remko@rfsee.nl
 *
 *
 *	\version 0.8	Initial release for the HACOX project team for code review
 *	\version 0.9	Added: Feature to decrease the number of frame updates transmitted over SPI.
 *	                       Every change of the frame buffer will set _frameModified to true to
 *	                       allow update of the LEDs.
 *	                       To allow force of update the update() function can now be forced to
 *	                       update when set to true.
 *
 */

#ifndef DIGITALLED_H_
#define DIGITALLED_H_

#define LED_FRAME_SIZE   60	///< \define number of LEDs in the chain

#include"..\Libraries\myspi.h"
#include <stdint.h>


#include "touchdisplay.h"

extern const uint8_t OUT_OF_RANGE;	///< OUT_OF_RANGE value
extern const uint8_t RANGE_OK;		///< RANGE OK value

/// \class digitalled digitalled.h <digitalled.h>
/// \brief Manipulate a string of RGB LEds using SPI.
///
/// Supported are:
///	- WS2801
/// - APA102 (tested)
/// - APA102c
///
/// The class communicates with a object of the class type myspi that will interact with the API interface.
class digitalled
{
public:

	/// \brief Initialize digital led class
	digitalled();
	virtual ~digitalled();

	/// \brief set color of a single led
	/// Set the colors of a single led ad position 'led' using single colors
	/// \param led position of the led in the string
	/// \param blue intensity of the blue color from 0 to 255
	/// \param green intensity of the green color from 0 to 255
	/// \param red intensity of the red color from 0 to 255
	void setColor(const uint8_t led, const uint8_t red, const uint8_t green, const uint8_t blue);

	/// \brief set color of all LEDs in a string
	/// \param blue intensity of the blue color from 0 to 255
	/// \param green intensity of the green color from 0 to 255
	/// \param red intensity of the red color from 0 to 255
	void setAllColor(const uint8_t red, const uint8_t green, const uint8_t blue);

	/// \brief set color of a single led
	/// Set the colors of a single led ad position 'led' using RGB color scheme
	/// RGB colors are 24 bits of a 32 bit word where the intensity of the colors red, green en blue are
	/// expressed as hex values from 0 to 255 (0 - FF).
	/// Colors can be set using defines from "APA102_colors.h"
	/// \param led position of the led in the string
	/// \param rgb color of led in RGB color scheme
	void setRGB(const uint8_t led, const uint32_t rgb);

	/// \brief set color of a single led
	/// Set the colors of a single led ad position 'led' using RGB color scheme
	/// RGB colors are 24 bits of a 32 bit word where the intensity of the colors red, green and blue are
	/// expressed as hex values from 0 to 255 (0 - FF).
	/// Colors can be set using defines from "APA102_colors.h"
	/// \param rgb color of led in RGB color scheme
	void setAllRGB(const uint32_t rgb);

	/// \brief set illumination of a single LED
	/// Illumination is a value from 0 to 31. 0 means no light, and 31 maximum illumination.
	/// setting illumination can interfere with individual RGB settings
	/// \param led position of the led in the string
	/// \param intensity of illumination
	void setLedIllumination(const uint8_t led, uint8_t illumination);

	/// \brief set illumination of a all LEDs in the frame
	/// Illumination is a value from 0 to 31. 0 means no light, and 31 maximum illumination.
	/// setting illumination can interfere with individual RGB settings
	/// \param intensity of illumination
	void setAllIllumination(uint8_t illumination);

	/// \brief switch a single led off
	/// \param led position of the led in the string to be switched off
	void setLedOff(const uint8_t led);

	/// \brief switch a single led on
	/// Using this function will preserve the active color settings for the led
	/// \param led position of the led in the string to be switched on
	void setLedOn(const uint8_t led);

	/// \brief update led string
	/// \param set true to force update leds and false to update only when frame is modified
	void update(bool forceUpdate);

	/// \brief get LED frame size
	/// \return LED frame size
	uint8_t getFrameSize(void){return LED_FRAME_SIZE;};

private:

	myspi _spiInterface; ///< SPI interface object to send LED data to.

	/// \todo add documentation
	union
	{
		struct
		{					// LSB
			uint32_t RED    : 8;	///< RED led intensity from 0 (off) to 255 (max)
			uint32_t GREEN  : 8; 	///< GREEN led intensity from 0 (off) to 255 (max)
			uint32_t BLUE   : 8; 	///< BLUE led intensity from 0 (off) to 255 (max)
			uint32_t GLOBAL : 5; 	///< Global intensity for all LEDs from 0 (off) to 32 (max)
			uint32_t INIT   : 3; 	///< Initialize, the led configuration starts with '111'
							// MSB
		} fields;					///< All fields in a LED packet
		uint32_t data; 				///< RAW LED packet data
	} _digitalLedframe[LED_FRAME_SIZE];	///< Frame of LED packets

	bool _frameModified; ///< Identifier to indicate that frame modified.

	/// \brief Test led position is within range.
	/// \param led led position
	/// \return result of evaluation ad define.
	uint8_t TestPosition(const uint8_t led);

};

#endif /* DIGITALLED_H_ */
