/*
 * Name: Youssef Khaled Ahmed
 * Date: 1/9/2023
 * Description: configuration header file for SPI driver
 */

#ifndef SPI_CONFIG_H_
#define SPI_CONFIG_H_

/* SPI interrupt enable and disable: */
#define       SPIE_EN       1
#define       SPIE_DS       0

#define       SPIE          SPIE_EN

/* SPI enable and disable: */
#define       SPI_EN        1
#define       SPI_DS        0

#define       SPE           SPI_EN

/* DORD for LSB and MSB */
#define       DORD_LSB      1
#define       DORD_MSB      0

#define       DORD          DORD_LSB

/* MSTR for master and slave */
#define       MSTR_M        1
#define       MSTR_S        0

/* CPOL for rising and falling edges
 * CPHA for sampling and setting up
 * these modes should be put in the
 * CPOL and CPHA bits in the SPCR
 */
#define       SAMPLE_RISING       0
#define       SETUP_RISING        1
#define       SAMPLE_FALLING      2
#define       SETUP_FALLING       3

#define       SPI_MODE            SETUP_RISING

/* values to be put in SPR1 and SPR0 in SPCR */
typedef enum{
    FCPU_4,
    FCPU_16,
    FCPU_64,
    FCPU_128,
    FCPU_2_2x = 0,
    FCPU_8_2x,
    FCPU_32_2x,
    FCPU_64_2x
} SPI_clockFrequency;

#define       SPI_CLOCK           FCPU_16

/* SPI2X  in the SPCR register */
#define       SPI2X               STD_OFF



#endif /* SPI_CONFIG_H_ */
