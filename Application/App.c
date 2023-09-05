/*==================================================================
               __  __      _     __          __
              |  \/  |    | |    \ \        / /
              | \  / | ___| |_ __ \ \  /\  / /_ _ _ __ ___
              | |\/| |/ _ \ __/ _` \ \/  \/ / _` | '__/ _ \
              | |  | |  __/ || (_| |\  /\  / (_| | | |  __/
              |_|  |_|\___|\__\__,_| \/  \/ \__,_|_|  \___|
==================================================================
    SWC : App.c
    Date: 8/28/2023
    Authors: MetaWare LLC
            Mahmoud Sayed Mahmoud Helmy (1285)
            Mohamed Mahmoud Masoud (200)
            Mark Ehab Tawfik (201)
            Hazzem Mohamed Ezzeldin (1297)
            Yousef Khaled Ahmed (558)
================================================================
     Description: This file contains the main application logic and
     functions for controlling alarms and user interactions.
================================================================
                        FILE NAME: App.c
================================================================*/

#include "App.h"       /* Include the header file for this application */
#include <stdlib.h>    /* Include the standard library for various functions */

/*============================================================================================
 Global Variables
 ============================================================================================*/

dashboard_t dashboard= {INTIAL_SPEED, INTIAL_GEAR, INTIAL_MAX_SPEED};

uint8 gear_flag;


//uint8 gearReverse_FLAG;

/*==================================================================
 Initialization
 ==================================================================*/
void App_Init(void)
{
        /* Initialize LCD */
        RTE_LCD_init();
        /* Initialize Keypad */
        RTE_KEYPAD_init();


}



void MainApp()
{
    while(TRUE)
    {
        /*    */


       /* Clearing the LCD to start writing*/
       RTE_LCD_clearScreen();

       /* Moving the LCD cursor */
       RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_0);
       RTE_LCD_displayString("Speed :     ");

       /* Moving the LCD cursor */
       RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_8);
       RTE_LCD_integerToString(dashboard.speed);

       /* Moving the LCD cursor */
       RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);
       RTE_LCD_displayString("Gear  :      ");

       /* Moving the LCD cursor */
       RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_8);
       RTE_LCD_displayCharacter(dashboard.gear);

       /*Moving the LCD cursor */
       RTE_LCD_moveCursor(LCD_ROW_2, LCD_COLUMN_0);
       RTE_LCD_displayString("Speed Limit :      ");

       /* Moving the cursor to the first row and first column*/
       RTE_LCD_moveCursor(LCD_ROW_2, LCD_COLUMN_14);
       RTE_LCD_integerToString(dashboard.maxSpeed);


       _delay_ms(1000);


    }


}




