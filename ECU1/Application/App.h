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
#define INITIAL_SPEED 			0U
#define INITIAL_GEAR 			PARK
#define MAX_ALLOWED_SPEED 		250U
#define MAXIMUM_ADC			1023U

/* Speed Limit Options */
#define SPEED_LIMIT_OPTION_30 		0U
#define SPEED_LIMIT_OPTION_80 		1U
#define SPEED_LIMIT_OPTION_120 		2U

/* Speed Limit Values */
#define SPEED_LIMIT_30 			30U
#define SPEED_LIMIT_80 			80U
#define SPEED_LIMIT_120 		120U

/* Speed Limit Button Configuration */
#define SPEED_LIMIT_BUTTON_COLUMN  	BUTTON_COLUMN_2
#define SPEED_LIMIT_BUTTON_ROW     	BUTTON_ROW_1

/* Speed Limit Enable Status */
#define SPEED_LIMIT_ENABLE_ON		1U
#define SPEED_LIMIT_ENABLE_OFF 		0U

/* Reverse Gear Speed Limit */
#define REVERSE_SPEED_LIMIT 		30U

/* Debounce Delay in Milliseconds */
#define DEBOUNCE_DELAY_MS 		50U

/* Number of Speed Limit Options */
#define OPTION_COUNT 			3U

/* Gear Enumeration */
typedef enum
{
	PARK,
	REVERSE,
	NEUTRAL,
	DRIVE
} Gear;


/*==================================================================
 * @brief Displays the vehicle's dashboard information on the LCD.
 *
 * This function is responsible for displaying the dashboard information on the 
 * LCD screen, including speed, gear, and speed limit status.
 ==================================================================*/
void DisplayDashboard(void);


/*==================================================================
 * @brief Updates the dashboard gear based on the gear_flag variable.
 *
 * This function updates the vehicle's dashboard gear based on the current
 * gear_flag value.
 ==================================================================*/
void UpdateDashboardGear(void);

/*==================================================================
 * @brief Updates the vehicle's speed based on current conditions and speed limits.
 *
 * This function updates the vehicle's speed based on various conditions, including
 * the speed limit status, current gear, and sensor readings.
 ==================================================================*/
void UpdateSpeed(void);

/*==================================================================
 * @brief Initializes the application and hardware components.
 *
 * This function initializes various hardware components and configurations needed for the application
 * to operate correctly, such as the LCD, keypad, LDR, buzzer, LED, and timers.
 ==================================================================*/
void App_Init(void);

/*==================================================================
 * @brief Manages the main application flow.
 *
 * This function is responsible for managing the main application flow, including
 * displaying the main menu, handling user choices, and executing corresponding actions.
 ==================================================================*/
void MainApp(void);

/*==================================================================
 * @brief Displays team info.
 *
 * This function is responsible for displaying team members names,
 * class and company.
 ==================================================================*/
void App_welcomeScreen(void);

#endif /* APP_H_ */
