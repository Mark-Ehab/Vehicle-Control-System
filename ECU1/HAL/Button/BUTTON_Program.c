/****************************************************************/
/* 		Author   	 : Mark Ehab                    */
/* 		Date     	 : Dec 19, 2021               	*/
/*              SWC              : BUTTON			*/
/*              Description	 : BUTTON Program               */
/* 	        Version          : V1.0                         */
/****************************************************************/

#include "../../Commons/std_types.h"

#include "../../MCAL/DIO/atmega32_DIO.h"

#include <util/delay.h>

#include "BUTTON_Interface.h"
#include "BUTTON_Private.h"
#include "BUTTON_Config.h"

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
uint8 BUTTON_voidInit(uint8 Copy_u8Port , uint8 Copy_u8Pin , uint8 Copy_u8ConnectionType)
{
	/* Local Variables Definitions */
	uint8 Local_u8ErrorStatus = RT_OK;			/* A variable to hold function status */

	/* Check passed port and pin IDs in addition to connection type */
	if((Copy_u8Port >= BUTTON_PORTA_ID && Copy_u8Port <= BUTTON_PORTD_ID) &&
	   (Copy_u8Pin  >= BUTTON_PIN0_ID  && Copy_u8Pin  <= BUTTON_PIN7_ID ) &&
	   (Copy_u8ConnectionType == BUTTON_PULLED_UP || Copy_u8ConnectionType == BUTTON_PULLED_DOWN))
	{
		/* Set the pin direction to which button is connected as input */
		DIO_setupPinDirection(Copy_u8Port,Copy_u8Pin,PIN_INPUT);

		/* Check Passed Button Connection Type */
		switch(Copy_u8ConnectionType)
		{
			case BUTTON_PULLED_UP :

				/* Set the pin to which button is connected to be input pulled up internally */
				DIO_writePin(Copy_u8Port,Copy_u8Pin,LOGIC_HIGH);
			break;
			case BUTTON_PULLED_DOWN:

				/* Set the pin to which button is connected to be input pulled down externally
				 * with a pull down resistor because avr don't support to pull down pins
				 * internally
				 */
			break;
		}
	}
	else
	{
		/* Function is not working as expected */
		Local_u8ErrorStatus = RT_NOK;
	}

	return Local_u8ErrorStatus;
}

/*====================================================================================*/
/* Description     : This function checks if a specific button is pressed or not
/* Input Arguments : uint8 Copy_u8Port, uint8 Copy_u8Pin, uint8 Copy_u8ConnectionType,
/* 		     uint8* Copy_pu8ButtonStatus
/* Return          : uint8
/*====================================================================================*/
uint8 BUTTON_u8IsPressed(uint8 Copy_u8Port , uint8 Copy_u8Pin , uint8 Copy_u8ConnectionType , uint8* Copy_pu8ButtonStatus)
{
	/* Local Variables Definitions */
	uint8 Local_u8ErrorStatus = RT_OK;			/* A variable to hold function status */
	uint8 Local_u8PinStatus;				/* A variable that holds pin status to be updated by hardware */

	/* Check if passed pointer is null or not */
	if(Copy_pu8ButtonStatus != NULL)
	{
		/* Check passed port and pin IDs in addition to connection type */
		if((Copy_u8Port >= BUTTON_PORTA_ID && Copy_u8Port <= BUTTON_PORTD_ID) &&
		   (Copy_u8Pin  >= BUTTON_PIN0_ID  && Copy_u8Pin  <= BUTTON_PIN7_ID ) &&
		   (Copy_u8ConnectionType == BUTTON_PULLED_UP || Copy_u8ConnectionType == BUTTON_PULLED_DOWN))
		{
			/* Initialize Button Status As Not Pressed */
			*Copy_pu8ButtonStatus = BUTTON_NOT_PRESSED;

			/* Get Pin Value */
			Local_u8PinStatus = DIO_readPin(Copy_u8Port,Copy_u8Pin);

			/* Check Passed Button Connection Type */
			switch(Copy_u8ConnectionType)
			{
				case BUTTON_PULLED_UP:

					/* Check if button is pressed */
					if(Local_u8PinStatus == 0)
					{
						/* Set button status as pressed */
						*Copy_pu8ButtonStatus = BUTTON_PRESSED;
					}

					break;

				case BUTTON_PULLED_DOWN:

					/* Check if button is pressed */
					if(Local_u8PinStatus == 1)
					{
						/* Set button status as pressed */
						*Copy_pu8ButtonStatus = BUTTON_PRESSED;
					}

					break;

				default:
					/* Function is not working as expected */
					Local_u8ErrorStatus = RT_NOK;
			}
		}
		else
		{
			/* Function is not working as expected */
			Local_u8ErrorStatus = RT_NOK;
		}
	}
	else
	{
		/* Passed pointer is a NULL pointer */
		Local_u8ErrorStatus = NULL_POINTER;
	}
	return Local_u8ErrorStatus;
}
/*=====================================================================================*/
/* Description     : This function checks if a specific button is pressed or not
/*		     with handling bouncing behavior
/* Input Arguments : uint8 Copy_u8Port, uint8 Copy_u8Pin, uint8 Copy_u8ConnectionType,
/* 		     uint8* Copy_pu8ButtonStatus
/* Return          : uint8
/*=====================================================================================*/
uint8 BUTTON_u8IsPressedDebounced(uint8 Copy_u8Port , uint8 Copy_u8Pin , uint8 Copy_u8ConnectionType , uint8* Copy_pu8ButtonStatus)
{
	/* Local Variables Definitions */
	uint8 Local_u8ErrorStatus = RT_OK;		/* A variable to hold function status */
	uint8 Local_u8PinStatus;			/* A variable that holds pin status to be updated by hardware */

	/* Check if passed pointer is null or not */
	if(Copy_pu8ButtonStatus != NULL)
	{
		/* Check passed port and pin IDs in addition to connection type */
		if((Copy_u8Port >= BUTTON_PORTA_ID && Copy_u8Port <= BUTTON_PORTD_ID) &&
		   (Copy_u8Pin  >= BUTTON_PIN0_ID  && Copy_u8Pin  <= BUTTON_PIN7_ID ) &&
		   (Copy_u8ConnectionType == BUTTON_PULLED_UP || Copy_u8ConnectionType == BUTTON_PULLED_DOWN))
		{
			/* Initialize Button Status As Not Pressed */
			*Copy_pu8ButtonStatus = BUTTON_NOT_PRESSED;

			/* Get Pin Value */
			Local_u8PinStatus = DIO_readPin(Copy_u8Port,Copy_u8Pin);

			/* Check Passed Button Connection Type */
			switch(Copy_u8ConnectionType)
			{
				case BUTTON_PULLED_UP:

					/* Check if button is pressed */
					if(Local_u8PinStatus == 0)
					{
						/* Wait for bounce time */
						_delay_ms(5);

						/* Get Pin Value */
						Local_u8PinStatus = DIO_readPin(Copy_u8Port,Copy_u8Pin);

						/* Check Button Status After Debouncing */
						if(Local_u8PinStatus == 0)
						{
							/* Set button status as pressed */
							*Copy_pu8ButtonStatus = BUTTON_PRESSED;
						}

						/* Check if button is still pressed */
						while(Local_u8PinStatus == 0)
						{
							/* Get Pin Value */
							Local_u8PinStatus = DIO_readPin(Copy_u8Port,Copy_u8Pin);
						}

						/* Wait for bounce time */
						_delay_ms(5);
					}

					break;

				case BUTTON_PULLED_DOWN:

					/* Check if button is pressed */
					if(Local_u8PinStatus == 1)
					{
						/* Wait for bounce time */
						_delay_ms(5);

						/* Get Pin Value */
						Local_u8PinStatus = DIO_readPin(Copy_u8Port,Copy_u8Pin);

						/* Check Button Status After Debouncing */
						if(Local_u8PinStatus == 1)
						{
							/* Set button status as pressed */
							*Copy_pu8ButtonStatus = BUTTON_PRESSED;
						}

						/* Check if button is still pressed */
						while(Local_u8PinStatus == 1)
						{
							/* Get Pin Value */
							Local_u8PinStatus = DIO_readPin(Copy_u8Port,Copy_u8Pin);
						}

						/* Wait for bounce time */
						_delay_ms(5);
					}
					break;

				default:
					/* Function is not working as expected */
					Local_u8ErrorStatus = RT_NOK;
			}
		}
		else
		{
			/* Function is not working as expected */
			Local_u8ErrorStatus = RT_NOK;
		}
	}
	else
	{
		/* Passed pointer is a NULL pointer */
		Local_u8ErrorStatus = NULL_POINTER;
	}

	return Local_u8ErrorStatus;
}
