/*
 * com.h
 *
 *  Created on: Sep 5, 2023
 *      Author: youss
 */

#ifndef HAL_COM_COM_H_
#define HAL_COM_COM_H_

#define    DUMMY_DATA    'n'

#define    SAVE          1
#define    GET           2

void COM_slaveInit(void);
void COM_masterInit(void);
void COM_send(uint8 copy_data);
uint8 COM_receive(void);

#endif /* HAL_COM_COM_H_ */
