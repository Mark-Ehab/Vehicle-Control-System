/*
 * Name: Youssef Khaled Ahmed
 * Date: 1/9/2023
 * Description: source file for SPI driver
 */

#include "../../Commons/std_types.h"
#include "../../Commons/Macros.h"

#include "SPI_private.h"
#include "SPI_config.h"
#include "SPI_interface.h"

#include <util/delay.h>


/***********************************************************
 * Service name: SPI_masterInit
 * Parameters in: None
 * Parameters out: None
 * Parameters in/out: None
 * Return: void
 * Description: this function initializes the master node.
 ***********************************************************/
void SPI_masterInit(void){
	/* variable to save the configurations and then accessing the  *
	 * SPCR register once.                                         */
    uint8 SPCR_variable = 0;
    /* setting the configurations as configured in the configurations file */
    SPCR_variable = (SPE << SPCR_SPE)|(DORD << SPCR_DORD) |
    		(MSTR_M << SPCR_MSTR) | (SPI_MODE << SPCR_CPHA) | SPI_CLOCK;
    /* assigning the SPCR with the value of the SPCR_variable that holds *
     * the configurations.                                               */
    SPCR = SPCR_variable;
    /* setting the SPI2X if configured so. */
    SPSR |= SPI2X;
}

/***********************************************************
 * Service name: SPI_slaveInit
 * Parameters in: None
 * Parameters out: None
 * Parameters in/out: None
 * Return: void
 * Description: this function initializes the slave node.
 ***********************************************************/
void SPI_slaveInit(void){
	/* variable to save the configurations and then accessing the  *
	 * SPCR register once.                                         */
    uint8 SPCR_variable = 0;
    /* setting the configurations as configured in the configurations file */
    SPCR_variable = (SPE << SPCR_SPE)|(DORD << SPCR_DORD)|
    		(MSTR_S << SPCR_MSTR)| (SPI_MODE << SPCR_CPHA);
    /* assigning the SPCR with the value of the SPCR_variable that holds *
     * the configurations.                                               */
    SPCR = SPCR_variable;
}

/***********************************************************
 * Service name: SPI_transiver
 * Parameters in: copy_data
 * Parameters out: None
 * Parameters in/out: None
 * Return: uint8
 * Description: this function sends then receives data
 *              since the data register is shift register.
 ***********************************************************/
uint8 SPI_transiver(uint8 copy_data){
	/* loading the passed data in the SPDR register. */
    SPDR = copy_data;
    /* waiting till the flag is set which means the *
     * transmission is completed.                   */
    while(BIT_IS_CLEAR(SPSR , SPSR_SPIF));
    /* returning the value that is loaded in the SPDR register. */
    return SPDR;
}

/***********************************************************
 * Service name: SPI_sendData
 * Parameters in: copy_data
 * Parameters out: None
 * Parameters in/out: None
 * Return: uint8
 * Description: this function sends the data.
 ***********************************************************/
void SPI_sendData(uint8 copy_data){
	/* loading the passed data in the SPDR register. */
    SPDR = copy_data;
    /* waiting till the flag is set which means the *
     * transmission is completed.                   */
    while(BIT_IS_CLEAR(SPSR , SPSR_SPIF));
}

/***********************************************************
 * Service name: SPI_receiveData
 * Parameters in: None
 * Parameters out: None
 * Parameters in/out: None
 * Return: uint8
 * Description: this function receives the data after
 *              sending dummy data.
 ***********************************************************/
uint8 SPI_receiveData(void){
	/* putting dummy data in the SPDR register */
	SPDR = 0xff;
	/* waiting till the flag is set which means the *
	 * transmission is completed.                   */
    while(BIT_IS_CLEAR(SPSR , SPSR_SPIF));
    /* returning the value that is loaded in the SPDR register. */
    return SPDR;
}

