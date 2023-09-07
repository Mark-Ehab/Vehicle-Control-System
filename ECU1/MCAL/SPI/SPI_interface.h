/*
 * Name: Youssef Khaled Ahmed
 * Date: 1/9/2023
 * Description: interface header file for SPI driver
 */

#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

/* functions prototypes: */
void SPI_masterInit(void);
void SPI_slaveInit(void);
uint8 SPI_transiver(uint8 copy_data);
void SPI_sendData(uint8 copy_data);
uint8 SPI_receiveData(void);

#endif /* SPI_INTERFACE_H_ */
