/*
 * com.h
 *
 *  Created on: Sep 5, 2023
 *      Author: youss
 */

#ifndef HAL_COM_COM_H_
#define HAL_COM_COM_H_

/* dummy data to send when we want to receive data */
#define    DUMMY_DATA    'n'

/* save and get #defines */
#define    SAVE          1
#define    GET           2

/* functions prototypes: */
/* initialization of the pins and the SPI of the slave node. */
void COM_slaveInit(void);
/* initialization of the pins and the SPI of the master node. */
void COM_masterInit(void);
/* sending data through SPI. */
void COM_send(uint8 copy_data);
/* receiving data vis SPI. */
uint8 COM_receive(void);

#endif /* HAL_COM_COM_H_ */
