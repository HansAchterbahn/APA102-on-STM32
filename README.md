[ ![](https://cdn.sparkfun.com/assets/parts/1/1/8/0/9/14016-action.jpg) ](https://www.sparkfun.com/products/14016)


# APA102 driver for STM32 for HAL library

* developed by HansAchterbahn, since July 2019
* forked from [Remko Welling](https://github.com/pe1mew/APA102-on-STM32), May 2016


## Genaral

This library implements the APA102 digital LED using the famous STM32 HAL library.

_Have fun with this library, improve and share it!_


## Repository Content

| folder/file               | explanation                                                                                       |
|---------------------------|---------------------------------------------------------------------------------------------------|
| __/src__                  | folder contains C source and header files                                                         |
| __/scr/Colors.h__         | file with predefined colors                                                                       |
| __/scr/DigiLed.h/.c__     | a pair of C files which define a basic interface to the APA102 leds by using the HAL SPI library  |
| __/scr/LightEffect.h/.c__ | a pair of C files which allows advanced light effects based on the digitalled library             |
| __LICENSE.md__            | license file: [CC-BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/)                      |
| __README.md__             | info file to this repository                                                                      |


## Usage

To get this library working do the following steps.

![STM32CubeMx (Picture: STM32)](https://www.st.com/content/ccc/fragment/product_related/rpn_information/board_photo/74/79/d3/ee/5c/37/4e/7e/stm32cubemx.jpg/files/stm32cubemx.jpg/_jcr_content/translations/en.stm32cubemx.jpg)


#### STM32CubeMx

* use [STM32CubeMx](https://www.st.com/en/development-tools/stm32cubemx.html) to create a new STM32 project based on your board or microcontroller
* add an SPI interface with following settings:
	* __Frame Format:__ Motorola
	* __Data Size:__ 8 Bits
	* __First Bit:__ MSB First
	* __Prescaler:__ 32
	* __Clock Polarity (CPOL):__ Low
	* __Clock Phase (CPHA):__ 1 Edge
	* __CRC Calculation:__ Disabled
	* __NSSP Mode:__ Disabled
	* __NSS Signal Type:__ Software
* create project and open it in your IDE (I use STM32CubeIDE)


#### STM32CubeIDE

* open your CubeMx generated Project in [STM32CubeIDE](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-ides/stm32cubeide.html) (or your favorite IDE)
* add the file
	* __DigiLed.h__ to __Inc__ folder
	* __DigiLed.c__ to __Scr__ folder
* open the __main.c__ file and add the line `#include "DigiLed.h"` in the __USER CODE Includes__ section
	* it should looks like this

```c
...
/* USER CODE BEGIN Includes */
#include "DigiLed.h"
/* USER CODE END Includes */
...
```

* initial the DigiLed library by using the function `DigiLed_init(*hspi)`
* it expects at __*hspi__ a pointer to the __SPI handler__ which is responsible for the SPI handling of the APA102 LEDs
* to the end add the LED frame size by changing the `LED_FRAME_SIZE` value in the DigiLed.h file to the amount of LEDs you are working with
* ___now you are ready to go !___
	* try changing the color of all LEDs by using 'void DigiLed_setAllColor(red, green, blue)' or
	* try to change the color of a single LED by using 'DigiLed_setColor(led, red, green, blue)'
* the files __LightEffect.h/.c__ contain a library for simple light effects on a 4×4 APA102 matrix (`LED_FRAME_SIZE = 16`)
	* you can easily include them in the same way in your project sources and modify the effects to have quick success with your project


## Datasheet

[Datasheet APA102](https://cdn-shop.adafruit.com/datasheets/APA102.pdf)


## License

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/80x15.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.


<!--
## Pictures

[ ![APA102 LED (Picture: www.adafruit.com](https://cdn-shop.adafruit.com/970x728/2343-03.jpg) ](https://www.adafruit.com/product/2343#description-anchor)
-->