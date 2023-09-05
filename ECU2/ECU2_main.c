/*
 * metaWare
 * @return
 */

#include "RTE/rte.h"
#include <util/delay.h>

int main(){
	/* variable to receive the data that should be saved in the    *
	 * memory.                                                     */
	uint8 receiveBuffer = '5';
	/* variable to receive whether to save data or get it from the memory. */
	uint8 saveOrGet;
	/* initialize the communication module */
	RTE_COM_slaveInit();

	while(TRUE){
		/* receiving whether to save the data or get it from the memory. */
		saveOrGet = RTE_COM_receive();
	/* if save: */
		if(saveOrGet == SAVE){
			/* receiving the data to be saved in the memory*/
			receiveBuffer = RTE_COM_receive();
			/* Write Data to EEPROM */
			RTE_EEPROM_vidSendDataByte(0x0000,receiveBuffer);
			/* wait for 10 ms till the EEPROM finishes writing */
			_delay_ms(10);
	/* if get: */
		} else if(saveOrGet == GET){
			/* Read Data from EEPROM that is at the address 0x0000 */
			RTE_EEPROM_vidReadDataByte(0x0000,&receiveBuffer);
			/* waiting till the EEPROM finishes reading */
			_delay_ms(10);
			/* sending the data that is read from the EEPROM through *
			 * SPI to the master ECU.                                */
			RTE_COM_send(receiveBuffer);
		}
	}
}

