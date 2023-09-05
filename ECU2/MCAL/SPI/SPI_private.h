/*
 * Name: Youssef Khaled Ahmed
 * Date: 1/9/2023
 * Description: private header file for SPI driver
 */

#ifndef SPI_PRIVATE_H_
#define SPI_PRIVATE_H_

/* registers addresses: */
#define       SPCR       (*((volatile uint8 *)(0x2D)))/* control register */
#define       SPSR       (*((volatile uint8 *)(0x2E)))/* status register */
#define       SPDR       (*((volatile uint8 *)(0x2F)))/* data register */

/* control registers bits: */
typedef enum{
    SPCR_SPR0,
    SPCR_SPR1,
    SPCR_CPHA,
    SPCR_CPOL,
    SPCR_MSTR,
    SPCR_DORD,
    SPCR_SPE,
    SPCR_SPIE
} SPCR_REG;

/* status registers bits: */
typedef enum{
    SPSR_SPI2X,
    SPSR_WCOL = 6,
    SPSR_SPIF
} SPSR_REG;

#endif /* SPI_PRIVATE_H_ */
