/*
 * switches.c
 *
 *  Created on: Nov 13, 2021
 *      Author: Red
 */

#include "switchArray.h"


void switchArray::set(int column, int row) {
	// Create a mask for the column bit, and shift it over.
	char mask = 0x80 >> column;

	// Set only the single bit in the correct row
	state[row] |= mask;
	return;
}

void switchArray::reset(int column, int row) {
	// Create a mask for the column bit, and shift it over.
	char mask = 0x80 >> column;

	// Reset only the single bit in the correct row
	state[row] &= ~(mask);
	return;
}

int switchArray::read(int column, int row) {
	// Create a mask for the column bit, and shift it over.
	char mask = 0x80 >> column;

	// Return the single bit in the correct row
	return (int)(state[row] & mask);
}

void switchArray::initialize() {
	// Set the switches to the default state of a chessboard, that is all on for items in the 1st, 2nd, 7th, and 8th ranks/row, empty elsewhere
	state[0] = state[1] = state[6] = state[7] = 0xFF;
	state[2] = state[3] = state[4] = state[5] = 0x00;
	return;
}
