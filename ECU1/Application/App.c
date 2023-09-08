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
                        FILE NAME: App.c
================================================================*/

#include "App.h"       /* Include the header file for this application */

/*============================================================================================
                        Define the Structure for the Dashboard
==============================================================================================*/

/**
 * @struct Dashboard
 * @brief Structure to represent the dashboard information.
 *
 * This structure stores information related to the vehicle's dashboard, including speed, gear,
 * and maximum speed.
 */
typedef struct
{
	uint8 speed;   		/**< Current speed of the vehicle. */
	uint8 speedLimitEnable; 	/**< Maximum allowed speed for the vehicle. */
	uint8 speedLimit; 		/**< Speed limit allowed for the vehicle. */
	Gear gear;     		/**< Current gear of the vehicle. */
} Dashboard;

/*============================================================================================
                                Global Variables
==============================================================================================*/
/**
 * @brief Global variable representing the dashboard.
 *
 * This variable holds the values of current speed, speed limit, speed limit enable state and gear state of the vehicle
 */
Dashboard dashboard = {INITIAL_SPEED,SPEED_LIMIT_ENABLE_OFF,SPEED_LIMIT_80,INITIAL_GEAR};	/* Initialize the dashboard with default values */

/**
 * @brief Global variable representing the selected gear.
 *
 * This variable holds the current gear state of the vehicle, which can be one of the values
 * defined in the Gear enum.
 */
volatile Gear gear_flag = PARK;  /* Initialize the gear_flag to PARK */

/**
 * @brief Global variable indicating whether the forward button is pressed.
 *
 * This variable is used to detect the state of the forward button. When set to 1, it indicates
 * that the forward button has been pressed.
 */
volatile uint8 forwardButtonPressed = BUTTON_NOT_PRESSED; /* forwardButtonPressed is initially not pressed */

/**
 * @brief Global variable indicating whether the reverse button is pressed.
 *
 * This variable is used to detect the state of the reverse button. When set to 1, it indicates
 * that the reverse button has been pressed.
 */
volatile uint8 reverseButtonPressed = BUTTON_NOT_PRESSED; 	/* reverseButtonPressed is initially not pressed */

/**
 * @brief Global variable that holds the digital output of ADC .
 */
volatile uint16 SpeedBuffer;


/* Functions added to access the COM module */

/*
 * Service description: this is the service that is responsible of
 * sending the data to the slave ECU that will save it in the EEPROM
 */
void saveDataInSlaveECU(uint8 data){
	/* sending an indication that it's required to save some data */
	RTE_COM_send(SAVE);
	/* waiting to assure that the slave ECU is ready to receive the  *
	 * data that will be saved in the EEPROM                         */
	_delay_ms(100);
	/* sending the data that should be saved in the EEPROM */
	RTE_COM_send(data);
	/* waiting till the data is saved in the EEPROM */
	_delay_ms(100);
}

/*
 * Service description: this is the service that is responsible of
 * receiving the data from the slave ECU that is saved in the EEPROM
 */
uint8 getDataFromSlaveECU(void){
	/* local variable that will hold the received data */
	uint8 receiveBuffer = NULL;
	/* sending a value to the slave ECU which indicates that we  *
	 * want to get the data that is saved in the EEPROM          */
	RTE_COM_send(GET);
	/* waiting till the slave ECU prepares the data */
	_delay_ms(100);
	/* receiving the data from the slave ECU */
	receiveBuffer = RTE_COM_receive();
	/* returning this data */
	return receiveBuffer;
}



/*============================================================================================
					Initialization Function
==============================================================================================*/

/**
 * @brief Initialize the application and hardware components.
 *
 * This function initializes various hardware components and configurations needed for the application
 * to operate correctly. It performs the following tasks:
 *
 * 1. Enables global interrupts to allow for interrupt-driven operations.
 * 2. Configures external interrupt 0 for the forward button with falling edge trigger.
 * 3. Configures external interrupt 1 for the reverse button with falling edge trigger.
 * 4. Sets up ADC pre-configurations for analog input reading.
 * 5. Initializes the ADC
 * 6. Initializes the button interface
 * 7. Initializes LED components for status indication.
 * 8. Initializes the LCD display for dashboard information.
 */
void App_Init(void)
{
	/* Initialize this node as SPI master */
	RTE_COM_masterInit();

	/* Initialize LED components for status indication */
	RTE_LED_Init_All();

	/* Initialize the LCD display for dashboard information */
	RTE_LCD_init();

	/* Initialize Gear Forward Button */
	RTE_BUTTON_voidInit(BUTTON_PORTD_ID,BUTTON_PIN3_ID,BUTTON_PULLED_UP);

	/* Initialize Gear Reverse Button */
	RTE_BUTTON_voidInit(BUTTON_PORTD_ID,BUTTON_PIN2_ID,BUTTON_PULLED_UP);

	/* Initialize SpeedLimitEnable Button */
	RTE_BUTTON_voidInit(BUTTON_PORTD_ID,BUTTON_PIN6_ID,BUTTON_PULLED_UP);

	/* Initialize SetSpeedLimit  Button */
	RTE_BUTTON_voidInit(BUTTON_PORTD_ID,BUTTON_PIN7_ID,BUTTON_PULLED_UP);

	/* Set ADC pre-configurations */
	ADC_ConfigType ADC1 = {ADC_PRESCALER_128, ADC_REFERENCE_AVCC};

	/* Initialize ADC */
	RTE_ADC_Init(&ADC1);

	/* Enable external interrupt 0 with falling edge trigger for the forward button */
	Ext_Int0_Enable(INT0_FALLING_EDGE);

	/* Enable external interrupt 1 with falling edge trigger for the reverse button */
	Ext_Int1_Enable(INT1_FALLING_EDGE);

	/* Enable global interrupts */
	RTE_GlobalInterrupt_enable();

}


/*============================================================================================
                        Main Application Function
==============================================================================================*/

/**
 * @brief The main application loop responsible for vehicle control and dashboard display.
 *
 * This function is the main entry point of the application. It continuously monitors user inputs,
 * updates vehicle status, and displays dashboard information on an LCD screen.
 *
 * 1. Initializing variables to keep track of the previous gear and speed.
 * 2. Displaying the initial dashboard on the LCD screen.
 * 3. Entering an infinite loop to continuously monitor and respond to user inputs and vehicle status.
 * 4. Checking for the forward button press, incrementing the gear if applicable, and applying a debounce delay.
 * 5. Checking for the reverse button press, decrementing the gear if applicable, and applying a debounce delay.
 * 6. Toggling the speed limit status if D6 button is pressed.
 * 7. Updating the dashboard gear based on the `gear_flag` variable.
 * 8. Updating the vehicle's speed based on speed limits and gear settings.
 * 9. Checking if the gear value or speed has changed and updating the dashboard display accordingly.
 *
 * @note This function operates in an infinite loop, continuously monitoring and updating the vehicle's
 *       status and dashboard display.
 */
void MainApp(void)
{
	uint8 speedLimitEnablePressed = BUTTON_NOT_PRESSED;		/* SpeedLimitEnable button pressed flag */
	uint8 setSpeedLimitPressed = BUTTON_NOT_PRESSED;		/* SetSpeedLimit button pressed flag */


	/* Display the initial dashboard on the LCD screen */
	DisplayDashboard();

	while (1)
	{
		/* Get SpeedLimitEnable button status */
		BUTTON_u8IsPressedDebounced(BUTTON_PORTD_ID,BUTTON_PIN6_ID,BUTTON_PULLED_UP,&speedLimitEnablePressed);

		/* Get SetSpeedLimit button status */
		BUTTON_u8IsPressedDebounced(BUTTON_PORTD_ID,BUTTON_PIN7_ID,BUTTON_PULLED_UP,&setSpeedLimitPressed);

		/* Check for forward button press */
		if (forwardButtonPressed)
		{
			/* Delay to denounce the button press */
			_delay_ms(100);

			/* Increment the gear if it's below DRIVE */
			if (gear_flag < DRIVE)
			{
				gear_flag++;

				/* Update the dashboard gear based on the gear_flag */
				UpdateDashboardGear();

				/* Update gear state on LCD */
				LCD_moveCursor(LCD_ROW_0,LCD_COLUMN_18);

				switch (dashboard.gear)
				{
				case PARK:

					/* Display 'P' on LCD */
					RTE_LCD_displayString("P");

					/* Change speed limit state */
					dashboard.speedLimitEnable = SPEED_LIMIT_ENABLE_OFF;

					/* Display speed limit enable state as "OFF" */
					RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);
					RTE_LCD_displayString("OFF] ");

					/* Indicate the speed limit is OFF by turning on the red LED and turning off the green LED */
					RTE_LED_on(LED_RED);
					RTE_LED_Off(LED_GREEN);

					break;
				case REVERSE:

					/* Display 'R' on LCD */
					RTE_LCD_displayString("R");

					break;
				case NEUTRAL:

					/* Display 'N' on LCD */
					RTE_LCD_displayString("N");

					/* Change speed limit state */
					dashboard.speedLimitEnable = SPEED_LIMIT_ENABLE_OFF;

					/* Display speed limit enable state as "OFF" */
					RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);
					RTE_LCD_displayString("OFF] ");

					/* Indicate the speed limit is OFF by turning on the red LED and turning off the green LED */
					RTE_LED_on(LED_RED);
					RTE_LED_Off(LED_GREEN);

					break;
				case DRIVE:

					/* Display 'D' on LCD */
					RTE_LCD_displayString("D");

					break;
				default:
					break;
				}
			}

			/* Clear the button press flag */
			forwardButtonPressed = 0;
		}

		/* Check for reverse button press */
		if (reverseButtonPressed)
		{
			/* Delay to denounce the button press */
			_delay_ms(100);

			/* Decrement the gear if it's above PARK */
			if (gear_flag > PARK)
			{
				gear_flag--;

				/* Update the dashboard gear based on the gear_flag */
				UpdateDashboardGear();

				/* Update gear state on LCD */
				LCD_moveCursor(LCD_ROW_0,LCD_COLUMN_18);

				switch (dashboard.gear)
				{
				case PARK:

					/* Display 'P' on LCD */
					RTE_LCD_displayString("P");

					/* Change speed limit state */
					dashboard.speedLimitEnable = SPEED_LIMIT_ENABLE_OFF;

					/* Display speed limit enable state as "OFF" */
					RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);
					RTE_LCD_displayString("OFF] ");

					/* Indicate the speed limit is OFF by turning on the red LED and turning off the green LED */
					RTE_LED_on(LED_RED);
					RTE_LED_Off(LED_GREEN);

					break;
				case REVERSE:

					/* Display 'R' on LCD */
					RTE_LCD_displayString("R");

					break;
				case NEUTRAL:

					/* Display 'N' on LCD */
					RTE_LCD_displayString("N");

					/* Change speed limit state */
					dashboard.speedLimitEnable = SPEED_LIMIT_ENABLE_OFF;

					/* Display speed limit enable state as "OFF" */
					RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);
					RTE_LCD_displayString("OFF] ");

					/* Indicate the speed limit is OFF by turning on the red LED and turning off the green LED */
					RTE_LED_on(LED_RED);
					RTE_LED_Off(LED_GREEN);

					break;
				case DRIVE:

					/* Display 'D' on LCD */
					RTE_LCD_displayString("D");

					break;
				default:
					break;
				}
			}

			/* Clear the button press flag */
			reverseButtonPressed = 0;
		}

		/* Check if SpeedLimitEnable button is pressed */
		if(speedLimitEnablePressed)
		{
			/* Check current gear state */
			if(dashboard.gear == DRIVE || dashboard.gear == REVERSE)
			{
				/* Check current speed limit enable state */
				switch(dashboard.speedLimitEnable)
				{
				case SPEED_LIMIT_ENABLE_ON:

					/* Change speed limit state */
					dashboard.speedLimitEnable = SPEED_LIMIT_ENABLE_OFF;

					/* Display speed limit enable state as "ON" */
					RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);
					RTE_LCD_displayString("OFF] ");

					/* Indicate the speed limit is OFF by turning on the red LED and turning off the green LED */
					RTE_LED_on(LED_RED);
					RTE_LED_Off(LED_GREEN);

					break;
				case SPEED_LIMIT_ENABLE_OFF:

					/* Change speed limit state */
					dashboard.speedLimitEnable = SPEED_LIMIT_ENABLE_ON;

					/* Display speed limit enable state as "OFF" */
					RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);
					RTE_LCD_displayString("ON] =");

					/* Indicate the speed limit is ON by turning on the green LED and turning off the red LED */
					RTE_LED_on(LED_GREEN);
					RTE_LED_Off(LED_RED);

					/* Read speed limit from slave node */
					dashboard.speedLimit = getDataFromSlaveECU();

					/* Update LCD with the speed limit read from slave node */
					RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_16);
					RTE_LCD_displayString("   ");
					RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_16);
					RTE_LCD_integerToString(dashboard.speedLimit);

					break;
				}
			}
			else
			{
				/* Speed limit enable is OFF in neutral and park gears */
				dashboard.speedLimitEnable = SPEED_LIMIT_ENABLE_OFF;

				/* Change speed limit state */
				dashboard.speedLimitEnable = SPEED_LIMIT_ENABLE_ON;

				/* Display speed limit enable state as "OFF" */
				RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);
				RTE_LCD_displayString("OFF] ");

				/* Indicate the speed limit is OFF by turning on the red LED and turning off the green LED */
				RTE_LED_on(LED_RED);
				RTE_LED_Off(LED_GREEN);
			}

		}

		/* Check if SetSpeedLimit button is pressed */
		if(setSpeedLimitPressed)
		{
			/* Read the current speed input into SpeedBuffer */
			RTE_ADC_readChannel_polling(ADC_CHANNEL_0, &SpeedBuffer);

			/* Scale the ADC reading to a maximum value of 250 */
			SpeedBuffer = (uint16)((uint32)SpeedBuffer * MAX_ALLOWED_SPEED / MAXIMUM_ADC);

			/* Check if the speed limit is ON and the vehicle is in DRIVE gear */
			if (dashboard.speedLimitEnable == SPEED_LIMIT_ENABLE_OFF && dashboard.gear == DRIVE)
			{
				/* Set speed limit as current speed */
				dashboard.speedLimit = SpeedBuffer;

				/* Update speed limit on LCD */
				RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_16);
				RTE_LCD_displayString("   ");
				RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_16);
				RTE_LCD_integerToString(dashboard.speedLimit);

				/* Send new speed limit to slave node */
				saveDataInSlaveECU(dashboard.speedLimit);

			}
		}

		/* Update the vehicle's speed based on gear and speed limits */
		UpdateSpeed();

	}
}


/*============================================================================================
                Display the Dashboard Information on the LCD
==============================================================================================*/

/**
 * @brief Display the vehicle's dashboard information on an LCD screen.
 *
 * This function is responsible for displaying the dashboard information on the 
 * LCD screen. The dashboard includes details such as the current speed, gear, and speed limit status.
 *
 * LCD Display Layout:
 *
 * |====================|
 * |SPEED[   ] | GEAR[P]|
 * |LIMIT[OFF] ==> [   ]|
 * |LIMIT[ON] ===> [   ]|
 * |			|
 * |			|
 * |====================|
 *
 * @note This function updates the LCD display with the current dashboard information.
 */
void DisplayDashboard(void)
{
	/* Set the cursor position to the right arrow indicator */
	RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_11);
	RTE_LCD_displayString("|");

	/* Display "1)Speed limit ON/OFF" option on the third line of LCD */
	RTE_LCD_moveCursor(LCD_ROW_2, LCD_COLUMN_0);
	RTE_LCD_displayString("1)Speed limit ON/OFF");

	/* Display "2)Set speed limit" option on the forth line of LCD */
	RTE_LCD_moveCursor(LCD_ROW_3, LCD_COLUMN_0);
	RTE_LCD_displayString("2)Set speed limit");

	/* Display the current speed on the LCD */
	RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_0);
	RTE_LCD_displayString("SPEED[");

	/* Clear any previous speed display */
	RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_6);
	RTE_LCD_displayString("   ");
	/* Display the updated speed value */
	RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_6);
	RTE_LCD_integerToString(dashboard.speed);
	/* Display the unit "mph" */
	RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_9);
	RTE_LCD_displayString("]");

	/*===============================================*/

	/* Display the current gear on the LCD */
	RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_13);
	RTE_LCD_displayString("GEAR[");

	/* Clear any previous gear display */
	RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_18);
	RTE_LCD_displayString(" ");

	/* Display the updated gear value within parentheses */
	RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_18);
	switch (dashboard.gear)
	{
	case PARK:
		RTE_LCD_displayString("P]");
		break;
	case REVERSE:
		RTE_LCD_displayString("R]");
		break;
	case NEUTRAL:
		RTE_LCD_displayString("N]");
		break;
	case DRIVE:
		RTE_LCD_displayString("D]");
		break;
	default:
		break;
	}

	/*===============================================*/

	/* Display the speed limit status on the LCD */
	RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);
	RTE_LCD_displayString("LIMIT[");

	/* Clear any previous speed limit status display */
	RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);
	RTE_LCD_displayString("   ");

	/* Display "ON" or "OFF" based on the speed limit status */
	RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);

	if (dashboard.speedLimitEnable == SPEED_LIMIT_ENABLE_ON)
	{
		RTE_LCD_displayString("ON] =");

		/* Indicate the speed limit is ON by turning on the green LED and turning off the red LED */
		RTE_LED_on(LED_GREEN);
		RTE_LED_Off(LED_RED);
	}
	else
	{
		RTE_LCD_displayString("OFF] ");

		/* Indicate the speed limit is OFF by turning on the red LED and turning off the green LED */
		RTE_LED_on(LED_RED);
		RTE_LED_Off(LED_GREEN);
		RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_15);
		RTE_LCD_displayString("     ");
	}

	/* Display "==>" after limit in the second line */
	RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_11);
	RTE_LCD_displayString("==>");

	/* Display speed limit value */
	RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_15);
	RTE_LCD_displayString("[");
	RTE_LCD_displayString("   ");
	RTE_LCD_displayString("]");
	RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_16);
	RTE_LCD_integerToString(dashboard.speedLimit);


}


/*============================================================================================
                Update Speed Function
==============================================================================================*/

/**
 * @brief Update the vehicle's speed based on current conditions and speed limits.
 *
 * This function is responsible for updating the vehicle's speed based on various conditions, including
 * the speed limit status, current gear, and sensor readings. The key tasks performed by this function
 * include:
 *
 * 1. Reading the current speed sensor input and storing it in the `SpeedBuffer`.
 * 2. Checking if the speed limit is turned on and the vehicle is in the DRIVE gear:
 *    - If so, limit the speed to the maximum allowed speed (MAX_ALLOWED_SPEED) if it exceeds that limit.
 * 3. If the speed limit is off and the vehicle is in DRIVE gear:
 *    - Set the vehicle's speed to the current sensor reading.
 * 4. If the vehicle is in REVERSE gear:
 *    - Check if the current speed reading exceeds the reverse speed limit (REVERSE_SPEED_LIMIT).
 *      - If so, activate the speed limit and set the speed to the reverse speed limit.
 *      - If not, set the speed to the current sensor reading.
 * 5. For any other gear or condition:
 *    - Set the vehicle's speed to the initial speed value (INITIAL_SPEED).
 *    - Turn off the speed limit.
 *
 */
void UpdateSpeed(void)
{
	/* Read the current speed input into SpeedBuffer */
	RTE_ADC_readChannel_polling(ADC_CHANNEL_0, &SpeedBuffer);

	/* Scale the ADC reading to a maximum value of 250 */
	SpeedBuffer = (uint16)((uint32)SpeedBuffer * MAX_ALLOWED_SPEED / MAXIMUM_ADC);

	/* Check if the speed limit is ON and the vehicle is in DRIVE gear */
	if (dashboard.speedLimitEnable == SPEED_LIMIT_ENABLE_ON && dashboard.gear == DRIVE)
	{
		/* Limit the speed to the maximum allowed speed if it exceeds that limit */
		if (SpeedBuffer <= dashboard.speedLimit)
		{
			dashboard.speed = SpeedBuffer;
		}
		else
		{
			dashboard.speed = dashboard.speedLimit;
		}
	}
	/* Check if the speed limit is OFF and the vehicle is in DRIVE gear */
	else if (dashboard.speedLimitEnable == SPEED_LIMIT_ENABLE_OFF && dashboard.gear == DRIVE)
	{
		/* Set the vehicle's speed to the current sensor reading */
		dashboard.speed = SpeedBuffer;
	}
	/* Check if the vehicle is in REVERSE gear */
	else if (dashboard.gear == REVERSE)
	{
		/* Check if the current speed reading exceeds the reverse speed limit */
		if (SpeedBuffer < REVERSE_SPEED_LIMIT)
		{
			/* Deactivate the speed limit enable */
			dashboard.speedLimitEnable = SPEED_LIMIT_ENABLE_OFF;

			/* Display speed limit enable state as "OFF" */
			RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);
			RTE_LCD_displayString("OFF] ");

			/* Indicate the speed limit is OFF by turning on the red LED and turning off the green LED */
			RTE_LED_on(LED_RED);
			RTE_LED_Off(LED_GREEN);

			/* Set the speed to the reverse speed limit */
			dashboard.speed = REVERSE_SPEED_LIMIT;

			/* Set the speed to the current sensor reading */
			dashboard.speed = SpeedBuffer;
		}
		else
		{
			/* Activate the speed limit enable */
			dashboard.speedLimitEnable = SPEED_LIMIT_ENABLE_ON;

			/* Display speed limit enable state as "OFF" */
			RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_6);
			RTE_LCD_displayString("ON] =");

			/* Indicate the speed limit is ON by turning on the green LED and turning off the red LED */
			RTE_LED_on(LED_GREEN);
			RTE_LED_Off(LED_RED);

			/* Set the speed to the reverse speed limit */
			dashboard.speed = REVERSE_SPEED_LIMIT;
		}
	}
	/* Handle any other gear or condition */
	else
	{
		/* Set the vehicle's speed to the initial speed value */
		dashboard.speed = INITIAL_SPEED;
	}

	/* Display current speed on LCD */
	LCD_moveCursor(LCD_ROW_0,LCD_COLUMN_6);

	if(dashboard.speed < 10)
	{
		LCD_integerToString(dashboard.speed);
		LCD_displayString("  ");
	}
	else if(dashboard.speed < 100)
	{
		LCD_integerToString(dashboard.speed);
		LCD_displayString(" ");
	}
	else if(dashboard.speed < 1000)
	{
		LCD_integerToString(dashboard.speed);
	}
}


/*============================================================================================
                Update the dashboard gear based on gear_flag
==============================================================================================*/
void UpdateDashboardGear(void)
{
	switch (gear_flag)
	{
	case PARK:
		dashboard.gear = PARK;
		break;
	case REVERSE:
		dashboard.gear = REVERSE;
		break;
	case NEUTRAL:
		dashboard.gear = NEUTRAL;
		break;
	case DRIVE:
		dashboard.gear = DRIVE;
		break;
	}
}


/*============================================================================================
                Interrupt Service Routine for INT 1
==============================================================================================*/
ISR(VECTOR_2)
{	
	forwardButtonPressed = 1;
}

/*============================================================================================
                Interrupt Service Routine for INT 0
==============================================================================================*/
ISR(VECTOR_1)
{
	reverseButtonPressed = 1;
}

/**===============================================================
 Welcome Screen Display
 ==================================================================
 ==================================================================
 * Function: App_welcomeScreen
 *
 * Description:
 *   This function displays a welcome screen on an LCD.
 *   It shows team information, names, class, company info
 *   and a welcome message.
 *
 * Inputs:
 *   None.
 *
 * Returns:
 *   None.
 *
 *===============================================================*/
void App_welcomeScreen(void)
{
    /* Initialize LCD */
    LCD_clearScreen();

    /* Display Team Information */
    LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);

    LCD_displayString(TEAM);
    _delay_ms(50); /* Delay for 50 milli seconds */
    LCD_clearScreen();
    LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);
    LCD_displayString(TEAM_REVERSE);
    _delay_ms(50); /* Delay for 50 milli seconds */
    LCD_clearScreen();
    LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);
    LCD_displayString(TEAM);
    _delay_ms(2000); /* Delay for 2 seconds */
    LCD_clearScreen();
    LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_0);
    LCD_displayString(NAME_1);
    LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);
    LCD_displayString(NAME_2);
    LCD_moveCursor(LCD_ROW_2, LCD_COLUMN_0);
    LCD_displayString(NAME_3);
    LCD_moveCursor(LCD_ROW_3, LCD_COLUMN_0);
    LCD_displayString(NAME_4);
    _delay_ms(700); /* Delay for 1 second */
    LCD_clearScreen();
    LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);
    LCD_displayString(NAME_5);
    _delay_ms(700); /* Delay for 3 seconds */

    /* Display class and company info */
    LCD_clearScreen();
    LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_0);
    LCD_displayString(COMPANY);
    LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);
    LCD_displayString(CLASS);
    _delay_ms(700); /* Delay for 3 seconds */

    /* Display Welcome Message */
    LCD_clearScreen();
    LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_3);
    LCD_displayString(WELCOME);
    _delay_ms(700); /* Delay for 2 seconds */

    /* Display Start Engine Message */
    LCD_clearScreen();
    LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);
    LCD_displayString(START_PROJ);
    _delay_ms(750); /* Delay for 2 seconds */
    LCD_clearScreen();
}
