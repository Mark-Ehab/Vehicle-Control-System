/*================================================================
   _____ _______ ______   _           __     ________ _____
  |  __ |__   __|  ____| | |        /\\ \   / |  ____|  __ \
  | |__) | | |  | |__    | |       /  \\ \_/ /| |__  | |__) |
  |  _  /  | |  |  __|   | |      / /\ \\   / |  __| |  _  /
  | | \ \  | |  | |____  | |____ / ____ \| |  | |____| | \ \
  |_|  \_\ |_|  |______| |______/_/    \_|_|  |______|_|  \_\

================================================================
              : RTE LAYER
      Date    : 8/26/2023
      Authors : MetaWare LLC
                Mahmoud Sayed Mahmoud Helmy (1285)
                Mohamed Mahmoud Masoud (200)
                Mark Ehab Tawfik (201)
                Hazzem Mohamed Ezzeldin (1297)
                Yousef Khaled Ahmed (558)
================================================================*/

#ifndef RTE_H_
#define RTE_H_

#include "../Commons/std_types.h"
#include "../HAL/LCD/AVR_LCD.h"
#include "../HAL/Keypad/keypad.h"
#include "../HAL/Button/BUTTON_Interface.h"
#include "../HAL/LED/LED.h"
#include "../HAL/COM/com.h"
#include "../MCAL/Interrupt/atmega32_Interrupts.h"
#include "../MCAL/ADC/Adc.h"


/* Keypad Module RTE Services */
#define RTE_KEYPAD_init				        KeyPad_Init
#define RTE_KEYPAD_getPressedKey 			GetPresseKey

/* ADC Module RTE Services */
#define RTE_ADC_Init 					ADC_Init
#define RTE_ADC_readChannel_polling 			ADC_readChannel_polling
#define RTE_ADC_readChannel_interrupt			ADC_readChannel_interrupt
#define RTE_ADC_DeInit					ADC_DeInit

/* Character LCD Module RTE Services */
#define RTE_LCD_init 				        LCD_init
#define RTE_LCD_displayCharacter 			LCD_displayCharacter
#define RTE_LCD_displayString 				LCD_displayString
#define RTE_LCD_integerToString 			LCD_integerToString
#define RTE_LCD_clearScreen 				LCD_clearScreen
#define RTE_LCD_moveCursor			        LCD_moveCursor
#define RTE_LCD_displayIntegerRowColumn     		LCD_displayIntegerRowColumn
#define RTE_LCD_clearLine				ClearLCDRow

/* Button Module RTE Services */
#define RTE_BUTTON_voidInit                             BUTTON_voidInit
#define RTE_BUTTON_u8IsPressed                          BUTTON_u8IsPressed
#define RTE_BUTTON_u8IsPressedDebounced                 BUTTON_u8IsPressedDebounced

/* LED Module RTE Services */
#define RTE_LED_Init                                    LED_Init
#define RTE_LED_Init_All                                LED_Init_All
#define RTE_LED_on                                      LED_on
#define RTE_LED_Off                                     LED_Off
#define RTE_LED_Toggle                                  LED_Toggle
#define RTE_LED_Check                                   LED_Check
#define RTE_LED_All_Off                                 LED_All_Off

/* Interrupt RTE Services */
#define RTE_GlobalInterrupt_enable                      GlobalInterrupt_enable

/* COM Module RTE Services */
#define RTE_COM_slaveInit                               COM_slaveInit
#define RTE_COM_masterInit                              COM_masterInit
#define RTE_COM_send                                    COM_send
#define RTE_COM_receive                                 COM_receive

#endif /* RTE_H_ */
