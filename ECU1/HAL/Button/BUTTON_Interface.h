/****************************************************************/
/* 		Author   	 : Mark Ehab                    */
/* 		Date     	 : Dec 19, 2021               	*/
/*              SWC              : BUTTON			*/
/*              Description	 : BUTTON Interface             */
/* 	        Version          : V1.0                         */
/****************************************************************/

#ifndef HAL_BUTTON_INTERFACE_H_
#define HAL_BUTTON_INTERFACE_H_

/*-----------------------------------------------------------------------------------*/
/*                                                                                   */
/*                           	   INTERFACE MACROS		                     */
/*                                                                                   */
/*-----------------------------------------------------------------------------------*/

/* Types of button connections */
#define BUTTON_PULLED_UP		1U
#define BUTTON_PULLED_DOWN		2U

/* Button status values */
#define BUTTON_NOT_PRESSED	        0U
#define BUTTON_PRESSED		        1U

/* Button available ports */
#define BUTTON_PORTA_ID                 0U
#define BUTTON_PORTB_ID                 1U
#define BUTTON_PORTC_ID                 2U
#define BUTTON_PORTD_ID                 3U

/* Button available ports */
#define BUTTON_PIN0_ID                  0U
#define BUTTON_PIN1_ID                  1U
#define BUTTON_PIN2_ID                  2U
#define BUTTON_PIN3_ID                  3U
#define BUTTON_PIN4_ID                  4U
#define BUTTON_PIN5_ID                  5U
#define BUTTON_PIN6_ID                  6U
#define BUTTON_PIN7_ID                  7U

/*-----------------------------------------------------------------------------------*/
/*                                                                                   */
/*                           	 FUNCTIONS PROTOTYPES		                     */
/*                                                                                   */
/*-----------------------------------------------------------------------------------*/

/*====================================================================================*/
/* Description     : This function initializes any button based on its port, pin and
/* 		     connection type (Pulled_up / Pulled_down) values passed
/* Input Arguments : uint8 Copy_u8Port , uint8 Copy_u8Pin , uint8 Copy_u8ConnectionType
/* Return          : uint8
/*====================================================================================*/
uint8 BUTTON_voidInit(uint8 Copy_u8Port , uint8 Copy_u8Pin , uint8 Copy_u8ConnectionType);
/*====================================================================================*/
/* Description     : This function checks if a specific button is pressed or not
/* Input Arguments : uint8 Copy_u8Port, uint8 Copy_u8Pin, uint8 Copy_u8ConnectionType,
/* 		     uint8* Copy_pu8ButtonStatus
/* Return          : uint8
/*====================================================================================*/
uint8 BUTTON_u8IsPressed(uint8 Copy_u8Port , uint8 Copy_u8Pin , uint8 Copy_u8ConnectionType , uint8* Copy_pu8ButtonStatus);
/*=====================================================================================*/
/* Description     : This function checks if a specific button is pressed or not
/*		     with handling bouncing behavior
/* Input Arguments : uint8 Copy_u8Port, uint8 Copy_u8Pin, uint8 Copy_u8ConnectionType,
/* 		     uint8* Copy_pu8ButtonStatus
/* Return          : uint8
/*=====================================================================================*/
uint8 BUTTON_u8IsPressedDebounced(uint8 Copy_u8Port , uint8 Copy_u8Pin , uint8 Copy_u8ConnectionType , uint8* Copy_pu8ButtonStatus);

#endif /* HAL_BUTTON_INTERFACE_H_ */
