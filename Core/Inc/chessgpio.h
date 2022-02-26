/*
 * chessgpio.h
 *
 *  Created on: Nov 14, 2021
 *      Author: Red
 *
 *
 *      This file interacts with the MCU hardware GPIO to interpret the state of magnetic switches connected through 8-to-1 muxes to the MCU.
 *      Eight muxes have been arranged with one on each row. Their 8 inputs correspond to the eight column choices for their row
 */

#ifndef INC_CHESSGPIO_H_
#define INC_CHESSGPIO_H_
#include "stm32l0xx_hal.h"



// setColumn() sets the multiplexers to the given column.
int setColumn(int column);

/*  Once the muxes are set to a specific column, readColumn() returns a vector of bits for each of the rows in that column.
*   The following table describes which bit corresponds to which row
*		Row 1	| Bit 0
*		Row 2 | Bit 1
*		Row 3 | Bit 2
*		Row 4 | Bit 3
*		Row 5 | Bit 4
*		Row 6 | Bit 5
*		Row 7 | Bit 6
*		Row 8 | Bit 7
*
*		The bits can be indexed out with the following operation
*		columnVector & (1 << (row-1))
*/
int readColumn();



#endif /* INC_CHESSGPIO_H_ */
