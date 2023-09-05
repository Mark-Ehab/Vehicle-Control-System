/*
 * com_program.c
 *
 *  Created on: Sep 5, 2023
 *      Author: youss
 */

#include "../../Commons/std_types.h"
#include "../../HAL/EEPROM/EEPROM_Interface.h"
#include "../../MCAL/DIO/atmega32_DIO.h"

#include "../../MCAL/SPI/SPI_interface.h"
#include "../../MCAL/SPI/SPI_config.h"
#include "../../MCAL/SPI/SPI_private.h"

#include "com.h"

#include <util/delay.h>

/***********************************************************
 * Service name: COM_slaveInit
 * Parameters in: None
 * Parameters out: None
 * Parameters in/out: None
 * Return: void
 * Description: this function enables the slave node.
 ***********************************************************/
void COM_slaveInit(void){
/* initializations: */
	/* initializing pins direction */
	DIO_setupPinDirection(PORTB_ID , PIN4_ID , PIN_INPUT);  /* Slave select */
	DIO_setupPinDirection(PORTB_ID , PIN5_ID , PIN_INPUT);  /* MOSI  */
	DIO_setupPinDirection(PORTB_ID , PIN6_ID , PIN_OUTPUT); /* MISO  */
	DIO_setupPinDirection(PORTB_ID , PIN7_ID , PIN_INPUT);  /* Clock */
	/* SPI initialization*/
	SPI_slaveInit();

	/* Initialize EEPROM */
	EEPROM_vidInit();
}


/***********************************************************
 * Service name: COM_masterInit
 * Parameters in: None
 * Parameters out: None
 * Parameters in/out: None
 * Return: void
 * Description: this function enables the master node.
 ***********************************************************/
void COM_masterInit(void){
/* initializations: */
	/* initializing pins direction */
	DIO_setupPinDirection(PORTB_ID , PIN4_ID , PIN_OUTPUT); /* Slave select */
	DIO_setupPinDirection(PORTB_ID , PIN5_ID , PIN_OUTPUT); /* MOSI  */
	DIO_setupPinDirection(PORTB_ID , PIN6_ID , PIN_INPUT);  /* MISO  */
	DIO_setupPinDirection(PORTB_ID , PIN7_ID , PIN_OUTPUT); /* Clock */
	/* SPI initialization*/
	SPI_masterInit();
}


/***********************************************************
 * Service name: COM_send
 * Parameters in: copy_data
 * Parameters out: None
 * Parameters in/out: None
 * Return: void
 * Description: this function sends data through SPI
 *              protocol.
 ***********************************************************/
void COM_send(uint8 copy_data){
	/* sends the passed data through SPI protocol */
	SPI_transiver(copy_data);
}


/***********************************************************
 * Service name: COM_receive
 * Parameters in: None
 * Parameters out: None
 * Parameters in/out: None
 * Return: uint8
 * Description: this function receives data via SPI
 *              protocol.
 ***********************************************************/
uint8 COM_receive(void){
	/* variable to receive the data */
	uint8 receiveBuffer;
	/* send dummy data first to receive the data from SPI */
	receiveBuffer = SPI_transiver(DUMMY_DATA);
	/* return the received data. */
	return receiveBuffer;
}
