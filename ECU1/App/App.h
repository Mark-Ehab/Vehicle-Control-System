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
 Description: This header file contains the declarations and
 function prototypes for the main application logic and functions
 responsible for controlling alarms and user interactions.
 ================================================================
 FILE NAME: App.h
 ================================================================*/

#ifndef APP_H_
#define APP_H_

#include <util/delay.h>
#include "App_Cfg.h"
#include "../RTE/rte.h"

/* Define initial values for the dashboard */
#define INITIAL_SPEED 0
#define INITIAL_GEAR PARK
#define MAX_ALLOWED_SPEED 120

#define SPEED_LIMIT_BUTTON_COLUMN  1
#define SPEED_LIMIT_BUTTON_ROW     1 
#define SPEED_LIMIT_ON 1
#define SPEED_LIMIT_OFF 0
#define REVERSE_SPEED_LIMIT 40
typedef enum {
	PARK,
	REVERSE,
	NEUTRAL,
	DRIVE
} Gear;

/*==================================================================
							Function prototypes
 ==================================================================*/
void DisplayDashboard(void);
void gearForward_Polling(void);
void gearReverse_Polling(void);
void UpdateDashboardGear(void);
char GearToChar(Gear gear);
void UpdateSpeed(void);

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
