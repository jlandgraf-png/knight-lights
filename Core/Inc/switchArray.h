/*
 * switches.h
 *
 *  Created on: Nov 13, 2021
 *      Author: Red
 *
 *
 *      The switchArray class provides a data space for saving the current status of the magnetic switches on the board, either as detecting a magnet (1) or not detecting a magnet (0)
 */

#ifndef INC_SWITCHARRAY_H_
#define INC_SWITCHARRAY_H_
#include "stm32l0xx_hal.h"

struct switchArray {
	// Member structures

	// state contains the full status of the switches on the chess board. Each char represents a row, with the bits within the char representing columns
	char state[8];

	// Member functions

	// Set a given position in the array
	void set(int column, int row);

	// Reset a given position in the array
	void reset(int column, int row);

	// Return the status of a given position in the array
	int read(int column, int row);

	// Initialize the array
	void initialize();
};






#endif /* INC_SWITCHARRAY_H_ */
