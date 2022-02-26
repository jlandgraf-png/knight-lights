/*
 * pixelArray.h
 *
 *  Created on: Nov 13, 2021
 *      Author: Red
 *
 *      pixelArray implements the SW functionality required to run neopixels on the STML081 MCU.
 *      The neopixel output is DMA driven - the bitstream of colors must be formatted for the neopixel protocol and saved in memory
 *      Adafruit has a similar library for DMA-driven  NeoPixels located here - https://learn.adafruit.com/dma-driven-neopixels/overview
 *
 *      SK6821 datasheet calls out the following timings, but I have aligned closer to and seen better results with Adafruits timings for the WS2812 timings in their uberguide:
 *
 *      								 	Adafruit Uberguide 	|  	SK6812 Datasheet
 *      Time 0 High		|	 	0.4us  +/- 0.15us		|		0.3us +/- 0.15us
 *      Time 1 High		|	 	0.8us  +/- 0.15us		| 	0.6us +/- 0.15us
 *      Time 0 Low		|  	0.85us +/- 0.15us		|		0.9us +/- 0.15us
 *      Time 1 Low		|  	0.45us +/- 0.15us		|		0.6us +/- 0.15us
 *      Reset Code		| 	At least 50us low		|		At least 80us low
 *
 *      Time high + time low for each bit is 1.25us +/- 0.3us
 *
 *      For the rest of documentation, we will use the Adafruit WS2812 Uberguide timings. This can be found here - https://cdn-learn.adafruit.com/downloads/pdf/adafruit-neopixel-uberguide.pdf
 *
 *      The MCU will be set to have a SPI clock of 4MHz. This translates to bits with timings of 250ns.
 *      Using 5 bits in code, we can accurately send NeoPixel data. To send a one, send code "11100" over SPI. To send a 0, send code "11000" over SPI.
 *      You can double check the timings yourself, with 250ns for each bit.
 *      Each bit we wish to send will need to be padded in this manner with two 1's before and two 0's following.
 *
 *
 *  		Since each bit is expanded into 5 "SPI/DMA bits", a regular hex code color of 3 bytes will be expanded into 15 bytes
 */

#ifndef INC_PIXELARRAY_H_
#define INC_PIXELARRAY_H_
#include "stm32l0xx_hal.h"
#include "switchArray.h"


// Some default color codes. In the NeoPixel space, colors are GRB instead of RGB
#define WHITE 0xFFFFFF
#define GREEN 0xFF0000
#define RED 0x00FF00
#define BLUE 0x0000FF
#define OFF 0x000000


// dmaPixel/expanded Pixel. Each color byte now takes 5 bytes, with 15 total. For use only within the pixelArray functions.
struct dmaPixel {
	uint8_t green[5];
	uint8_t red[5];
	uint8_t blue[5];
	uint8_t total[15];
};


// We'll save all of our information about the array of colors we want to send out in a single structure
struct pixelArray {
	// Member structures
		// This buffer is the memory space that will be sent out through SPI MOSI at 4MHz. Initialize it to have 64 LEDs * 15 bytes/LED + 50 bytes for reset code
		uint8_t buffer[64*15+50];

		// length is the length of the array in general LEDs
		int length;

		// buffer_length is the total size of our buffer in bytes
		int buffer_length;

		// primary and secondary colors for ease of use
		uint32_t primary;
		uint32_t secondary;

	// Member functions
		// setPixel() sets an individual pixel within the array to a specified color
		void setPixel(uint32_t color, int column, int row);

		// setStrip() sets the entire buffer to a single color
		void setStrip(uint32_t color);

		// reset() sets the entire buffer to "0" or off
		void reset();

		// Initialize the pixel array with the given length. In actuality this parameter is not used, and 64 LEDs is hard coded.
		void initialize(int len);

		// This function takes an input switchArray, denoting which switches are active, and lights up the corresponding LEDs on the board
		void passThrough(switchArray* swArray);
};


#endif /* INC_PIXELARRAY_H_ */
