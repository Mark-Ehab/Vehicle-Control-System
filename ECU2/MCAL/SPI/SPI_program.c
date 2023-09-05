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
//#include <avr/interrupt.h>


//uint8 * g_sendBuffer;
//
//ISR(SPI_STC_vect){
//	SPDR = *g_sendBuffer;
//}


void SPI_masterInit(void){
    uint8 SPCR_variable = 0;
    SPCR_variable = (SPE << SPCR_SPE)|(DORD << SPCR_DORD) |
    		(MSTR_M << SPCR_MSTR) | (SPI_MODE << SPCR_CPHA) | SPI_CLOCK;
    SPCR = SPCR_variable;
    SPSR |= SPI2X;
}

void SPI_slaveInit(void){
    uint8 SPCR_variable = 0;
    SPCR_variable = (SPE << SPCR_SPE)|(DORD << SPCR_DORD)|
    		(MSTR_S << SPCR_MSTR)| (SPI_MODE << SPCR_CPHA);
    SPCR = SPCR_variable;
}

uint8 SPI_transiver(uint8 copy_data){
    SPDR = copy_data;
    while(BIT_IS_CLEAR(SPSR , SPSR_SPIF));
    return SPDR;
}

void SPI_sendData(uint8 copy_data){
    SPDR = copy_data;
    while(BIT_IS_CLEAR(SPSR , SPSR_SPIF));
}

uint8 SPI_receiveData(void){
	SPDR = 0xff;
    while(BIT_IS_CLEAR(SPSR , SPSR_SPIF));
    return SPDR;
}

//void SPI_enableInterrupt(uint8 * sendBuffer){
////	g_sendBuffer = sendBuffer;
//	SET_BIT(SPCR , SPCR_SPIE);
//}
