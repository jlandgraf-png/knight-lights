/*
 * chessgpio.cpp
 *
 *  Created on: Nov 14, 2021
 *      Author: Red
 */
#include "chessgpio.h"


// setColumn sets the muxes' select bits to configure for a specific column. Inputs of 0-7 are accepted. Inputs outside this range are not handled.
int setColumn(int column) {

	// Activate the select0 output pin if column contains a 1 at bit 0
	if (column & 1) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	}

	// Activate the select1 output pin if column contains a 1 at bit 1
	if (column & 2) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	}

	// Activate the select2 output pin if column contains a 1 at bit 2
	if (column & 4) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	}

	// Short delay to allow hardware changes to resolve
	HAL_Delay(10);
	return 1;

}



// readColumn returns the vector of row bits for the selected column
// Iteratively goes through each HW input pin from each mux, and changes the corresponding bit in the returnValue
int readColumn() {

	int returnValue = 0;

	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)) {
		returnValue |= 1;
	}

	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)) {
			returnValue |= 2;
		}

	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)) {
			returnValue |= 4;
		}

	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)) {
			returnValue |= 8;
		}

	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)) {
			returnValue |= 16;
		}

	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)) {
			returnValue |= 32;
		}

	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)) {
			returnValue |= 64;
		}

	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)) {
			returnValue |= 128;
		}
	return returnValue;

}

