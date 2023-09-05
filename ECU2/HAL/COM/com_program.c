/*
 * com_program.c
 *
 *  Created on: Sep 5, 2023
 *      Author: youss
 */

#include "../../Commons/std_types.h"
#include "../../HAL/EEPROM/EEPROM_Interface.h"
#include "../../MCAL/DIO/atmega32_DIO.h"
#include <util/delay.h>

#include "../../MCAL/SPI/SPI_interface.h"
#include "../../MCAL/SPI/SPI_config.h"
#include "../../MCAL/SPI/SPI_private.h"

#include "com.h"


void COM_slaveInit(void){
/* initializations: */
	/* initializing pins direction */
	DIO_setupPinDirection(PORTB_ID , PIN4_ID , PIN_INPUT);
	DIO_setupPinDirection(PORTB_ID , PIN5_ID , PIN_INPUT);
	DIO_setupPinDirection(PORTB_ID , PIN6_ID , PIN_OUTPUT);
	DIO_setupPinDirection(PORTB_ID , PIN7_ID , PIN_INPUT);
	/* SPI initialization*/
	SPI_slaveInit();

	/* Initialize EEPROM */
	EEPROM_vidInit();
}

void COM_masterInit(void){
/* initializations: */
	/* initializing pins direction */
	DIO_setupPinDirection(PORTB_ID , PIN4_ID , PIN_OUTPUT);
	DIO_setupPinDirection(PORTB_ID , PIN5_ID , PIN_OUTPUT);
	DIO_setupPinDirection(PORTB_ID , PIN6_ID , PIN_INPUT);
	DIO_setupPinDirection(PORTB_ID , PIN7_ID , PIN_OUTPUT);
	/* SPI initialization*/
	SPI_masterInit();
}


void COM_send(uint8 copy_data){
	SPI_transiver(copy_data);
}

uint8 COM_receive(void){
	uint8 receiveBuffer;
	receiveBuffer = SPI_transiver(DUMMY_DATA);
	return receiveBuffer;
}
