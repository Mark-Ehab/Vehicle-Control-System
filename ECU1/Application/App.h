/*==================================================================
 *     __  __      _     __          __
 *    |  \/  |    | |    \ \        / /
 *    | \  / | ___| |_ __ \ \  /\  / /_ _ _ __ ___
 *    | |\/| |/ _ \ __/ _` \ \/  \/ / _` | '__/ _ \
 *    | |  | |  __/ || (_| |\  /\  / (_| | | |  __/
 *    |_|  |_|\___|\__\__,_| \/  \/ \__,_|_|  \___|
 *==================================================================
 SWC: App.h
 Date: 8/28/2023
 Authors: MetaWare LLC
 Mahmoud Sayed Mahmoud Helmy (1285)
 Mohamed Mahmoud Masoud (200)
 Mark Ehab Tawfik (201)
 Hazzem Mohamed Ezzeldin (1297)
 Yousef Khaled Ahmed (558)
 ================================================================

 ================================================================
 FILE NAME: App.h
 ================================================================*/

#ifndef APP_H_
#define APP_H_

#include <util/delay.h>
#include "App_Cfg.h"
#include "../RTE/rte.h"


#define PARK_GEAR            'P'


#define NEUTRAL_GEAR         'N'


#define DRIVE_GEAR           'D'


#define REVERSE_GEAR         'R'

/* Return value indicating continuation */
#define CONTINUE 0

/* Return value indicating cancelled operation*/
#define CANCELLED 0

/* Return value indicating exit operation*/
#define EXIT       2

/* Return value indicating confirmed operation*/
#define CONFIRMED 1

/* Return value indicating error in operation*/
#define ERR_STATE   8

/* Return value indicating return to main menu operation*/
#define RETURN_TO_MAIN_MENU 0

#define INTIAL_SPEED            0

#define INTIAL_MAX_SPEED        100

#define INTIAL_GEAR             'P'

/* Structure for alarm configuration */
typedef struct
{
    uint8 speed;
    uint8 gear;
    uint8 maxSpeed;
} dashboard_t;

/*==================================================================*/
/**
 * @brief Initializes the application by initializing various drivers
 *  and components.
 *
 * This function initializes the LCD, keypad, LDR, buzzer, LED, and timers.
 */
void App_Init(void);

/*==================================================================*/
/**
 * @brief Manages the main application flow.
 *
 * This function displays the main menu, handles user choices, and executes
 * corresponding actions.
 */
void MainApp(void);


#endif /* APP_H_ */
