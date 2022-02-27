/*
 * pixelArray.c
 *
 *  Created on: Nov 13, 2021
 *      Author: Red
 */

#include "pixelArray.h"



// colorExtender is a function only to be used within pixelArray.cpp. It takes a 3 byte input color using RGB and outputs the extended GRB pixel
dmaPixel colorExtender(uint32_t color) {

	// This swaps each nibble, to resolve an issue discovered during testing (probably a MSB/LSB issue I'm not interested in researching further. This fixes the problem.
	uint32_t colorswap = (0xF0F0F0 & color) >> 4;
	color = (0x0F0F0F & color) << 4;
	color = color | colorswap;

	// converts standard color format 0xRRGGBB to extended color format for DMA buffer
	dmaPixel retVal;

	// Initialize each color to the ZERO pattern, 0xc6318c6318
	// Don't try zero pattern, 0x84 0x21 0x08 0x42 0x10
	retVal.green[0] = retVal.red[0] = retVal.blue[0] = 0xc6;
	retVal.green[1] = retVal.red[1] = retVal.blue[1] = 0x31;
	retVal.green[2] = retVal.red[2] = retVal.blue[2] = 0x8c;
	retVal.green[3] = retVal.red[3] = retVal.blue[3] = 0x63;
	retVal.green[4] = retVal.red[4] = retVal.blue[4] = 0x18;

	// Create a comparison variable with same length as color to perform bitwise comparisons
	uint32_t compare = 0;

	// Loop i for each valid bit in color, sectioned off to reorder RGB to GRB
	for (int i = 0; i < 8; ++i) {
		// Shift over the comparison bit i times.
		compare = 1 << i;
		// Only proceed if color has a valid bit at the location shifted to above
		if (color & compare) {
			// This is to match the pattern of 11x00 11x00 11x00...
			// Starts at bit 2 and increments 5 each i
			int index = 5*i + 2;


			// Indexes the color value of the dmaPixel and sets it accordingly.
			retVal.blue[4 - index / 8] |= (1 << (index % 8));

			//int index0 = index-1;
			//retVal.blue[4 - index0 / 8] |= (1 << (index0 % 8));
		}
	}
	for (int i = 8; i < 16; ++i) {
		#warning "you won't remember what all  this does in 2 month. Heavily comment and explain these tricks"
		compare = 1 << i;
		if (color & compare) {
			int index = 5*(i-8) + 2;
			retVal.green[4 - index / 8] |= (1 << (index % 8));

			//int index0 = index-1;
			//retVal.green[4 - index0 / 8] |= (1 << (index0 % 8));
		}

	}
	for (int i = 16; i < 24; ++i) {
		compare = 1 << i;
		if (color & compare) {
			int index = 5*(i-16) + 2;
			retVal.red[4 - index / 8] |= (1 << (index % 8));
			//int index0 = index-1;
			//retVal.red[4 - index0 / 8] |= (1 << (index0 % 8));
		}

	}
	for (int i = 0; i < 15; ++i) {
		if (i < 5) {
			retVal.total[i] = retVal.green[i];
		} else if (i < 10) {
			retVal.total[i] = retVal.red[i-5];
		} else {
			retVal.total[i] = retVal.blue[i-10];
		}
	}
	return retVal;

}



// Take in a RGB hex code and position to color a specific pixel
void pixelArray::setPixel(uint32_t color, int column, int row) {

	// Get the extended color for the input color
	dmaPixel extendedColor = colorExtender(color);

	// Get the index for the first byte of the extended colors memory space
	int index = (row*8 + column) * 15;

	// For each byte of the extended color, write into the buffer
	for (int i = index; i < index+15; ++i) {
		buffer[i] = extendedColor.total[i-index];
	}

	return;
}


// Take in an RGB hex code and color the entire array with the given color
void pixelArray::setStrip(uint32_t color) {

	// Get the extended color for the input color
	dmaPixel extendedColor = colorExtender(color);

	// Loop for each LED in the array
	for (int i = 0; i < length; ++i) {

		// Loop for each byte in the color
		for (int j = 0; j < 15; ++j) {

			// Set each byte of the extended color into the buffer
			buffer[i*15+j] = extendedColor.total[j];
		}
	}

	// Be sure to write over the remaining bytes with 0's for reset code. This may be unnecessary if they are never touched outside of initialization.
	for (int i = length*15; i < buffer_length; ++i) {
		buffer[i] = 0;
	}
	return;
}


// Reset the entire LED array/turn them all off
void pixelArray::reset() {

	// For each R, G, and B in the array, set their color to the 0 code 0xC6318C6318
	for (int i = 0; i < buffer_length; i+=5) {
			buffer[i] = 0xC6;
			buffer[i+1] = 0x31;
			buffer[i+2] = 0x8C;
			buffer[i+3] = 0x63;
			buffer[i+4] = 0x18;
		}

	// Set the reset code
	for (int i = length*15; i < buffer_length; ++i) {
			buffer[i] = 0;
		}
	return;
}


// Given an input length, create a pixel array (hard coded for length of 64 currently)
void pixelArray::initialize(int len) {
	length = len;
	length = 64;
	buffer_length = len*15 + 50;
	//buffer = new uint8_t(buffer_length);
	this->reset();

	// Set two colors for ease of use
	primary = 0x010001;
	secondary = 0x050010;
	return;

}


// For a given switch array, pass through the states to the corresponding LEDs with primary and secondary colors defined in initialization
void pixelArray::passThrough(switchArray* swArray) {

	// Loop for each pixel/switch
	for (int row = 0; row < 8; ++row) {
		for (int column = 0; column < 8; ++column) {

			// If the switch is active at this position, set the primary color. Otherwise, set the secondary color.
			if (swArray->read(column, row)) {
				this->setPixel(primary, column, row);
			} else {
				this->setPixel(secondary, column, row);
			}
		}
	}

	return;
}
