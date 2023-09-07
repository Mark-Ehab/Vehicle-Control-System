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
						Define the structure for the dashboard
 ============================================================================================*/
typedef struct {
    uint16 speed;
    Gear gear;
    uint8 limitSpeed;
} Dashboard;

/* Initialize the dashboard with default values */
Dashboard dashboard= {INITIAL_SPEED, INITIAL_GEAR, MAX_ALLOWED_SPEED};
	
/*============================================================================================
								Global Variables
 ============================================================================================*/	

/* Initialize the gear_flag to PARK */
volatile Gear gear_flag = PARK;  

/*Speed limit initially off*/
volatile uint8 speedLimitOn = 0;  

/* Forward button pressed flag */
volatile uint8 forwardButtonPressed = 0;

/* Reverse button pressed flag */
volatile uint8 reverseButtonPressed = 0;


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
								Initialization
==============================================================================================*/
void App_Init(void)
{
	/* Initialize LCD */
	RTE_LCD_init();

	/* Set ADC pre-configurations */
	ADC_ConfigType ADC1 = {ADC_PRESCALER_128, ADC_REFERENCE_AVCC};
	
	/* Initialize Communication */
	RTE_COM_masterInit();

	/* Intialize ADC */
	RTE_ADC_Init(&ADC1);

	/* Enable external interrupt 0 with falling edge trigger for forward button */
	Ext_Int0_Enable(INT0_FALLING_EDGE);

	/* Enable external interrupt 1 with falling edge trigger for reverse button */
	Ext_Int1_Enable(INT1_FALLING_EDGE);

	/* Enable global interrupts */
	RTE_GlobalInterrupt_enable();
}


/*============================================================================================
                        Main Application
==============================================================================================*/
void MainApp(void)
{	

	uint8 previousGear = PARK;
	uint8 previousSpeed = INITIAL_SPEED;
	DisplayDashboard();	
	RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_0);

	// /* Display speed on LCD */
	// RTE_LCD_displayString("Speed :");
	// RTE_LCD_integerToString(dashboard.speed);
	
	// /* Display gear on LCD */
	// RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);
	
	// /* Display gear on LCD */
	// RTE_LCD_displayString("MaxSpeed :");
	// RTE_LCD_integerToString(dashboard.maxSpeed);
	
	while (1) {
		/*Check for forward button press*/
		if (forwardButtonPressed) {
			if (gear_flag < DRIVE) {
				_delay_ms(100);
				gear_flag++;
			}
			forwardButtonPressed = 0; 
		}

		/*Check for reverse button press*/
		if (reverseButtonPressed) {
			if (gear_flag > PARK) {
				_delay_ms(100);
				gear_flag--;
			}
			reverseButtonPressed = 0; 
		}
		
/*
		if (RTE_Button_Read(SPEED_LIMIT_BUTTON_COLUMN, SPEED_LIMIT_BUTTON_ROW) == LOGIC_LOW) {
			/ *Toggle speed limit status* /
			speedLimitOn ^=speedLimitOn;
			/ *Delay to debounce the button press* /
			_delay_ms(100);
		}*/
		/*Update dashboard gear based on gear_flag*/
		UpdateDashboardGear();
		UpdateSpeed();
		/*Check if the gear value has changed*/
		if (dashboard.gear != previousGear || dashboard.speed != previousSpeed) {
			DisplayDashboard();
			previousGear = dashboard.gear;
			previousSpeed = dashboard.speed;
		}
		
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

/*============================================================================================
                Display the dashboard information on the LCD
==============================================================================================*/

void DisplayDashboard(void)
{	
	/* Display speed on LCD */
	RTE_LCD_moveCursor(LCD_ROW_0, LCD_COLUMN_0);
	RTE_LCD_displayString("Speed :");
	RTE_LCD_integerToString(dashboard.speed);
	
    /* Display MaxSpeed on LCD */

	RTE_LCD_moveCursor(LCD_ROW_1, LCD_COLUMN_0);
	RTE_LCD_displayString("LimitSpeed :");
	RTE_LCD_integerToString(dashboard.limitSpeed);
	
	/* Display gear on LCD */
	//RTE_LCD_clearLine(LCD_ROW_2);
	RTE_LCD_moveCursor(LCD_ROW_2, LCD_COLUMN_6);
	RTE_LCD_displayString(" ");
	RTE_LCD_moveCursor(LCD_ROW_2, LCD_COLUMN_0);
	RTE_LCD_displayString("Gear :");
	RTE_LCD_displayCharacter(GearToChar(dashboard.gear));
	
}


/*============================================================================================
                Update Speed
==============================================================================================*/
void UpdateSpeed(void)
{	
	/* Local Variables Definitions */
	uint16 speedBuffer;			/* variable to hold ADC digital reading */

	/* Get ADC digital reading */
	RTE_ADC_readChannel_polling(ADC_CHANNEL_0,&speedBuffer);

	/* Function to map ADC digital result to be within required range */
	speedBuffer = (uint32)(speedBuffer * 250U) / 1023U;  

	/*Check if the speed limit is on*/
	if (speedLimitOn == SPEED_LIMIT_ON && dashboard.gear == DRIVE) 
	{
		/* SPI: updating the limitSpeed with the value that is saved in the  *
                 *      slave ECU.                                                   */
		dashboard.limitSpeed = getDataFromSlaveECU();
		
		/*Limit the speed to the maximum allowed speed*/
		if (dashboard.speed < MAX_ALLOWED_SPEED) 
		{
			dashboard.speed = speedBuffer;

		}
	} 
	else if (dashboard.gear == DRIVE && speedLimitOn == SPEED_LIMIT_OFF) 
	{
		dashboard.speed = speedBuffer;
	}
	else if(dashboard.gear == REVERSE && (speedLimitOn == SPEED_LIMIT_ON || speedLimitOn == SPEED_LIMIT_OFF))
	{
		if(speedBuffer <= REVERSE_SPEED_LIMIT)
		{
			dashboard.speed = speedBuffer;
		}
	}
	else
	{
		dashboard.speed = INITIAL_SPEED;
	}
}
/*============================================================================================
            Handle button press to update gear_flag in forward P > R > N > D
==============================================================================================*/
void gearForward_Polling(void)
{
    uint8 forwardButton;
	
    do
    {
        forwardButton = RTE_Button_Read(BUTTON_COLUMN_4, BUTTON_ROW_1);
        if (forwardButton == LOGIC_LOW && gear_flag < DRIVE)
        {
            gear_flag++;
        }

    } while (forwardButton == LOGIC_HIGH);
}

/*============================================================================================
            Handle button press to update gear_flag in reverse D > N > R > D
==============================================================================================*/

void gearReverse_Polling(void)
{
	uint8 reverseButton;

	do {
		reverseButton = RTE_Button_Read(BUTTON_COLUMN_3, BUTTON_ROW_1);
		if (reverseButton == LOGIC_LOW && gear_flag > PARK)
		{
			gear_flag--;
		}

	} while (reverseButton == LOGIC_HIGH);
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
                Convert Gear enum to a character for display
==============================================================================================*/
char GearToChar(Gear gear)
{
    switch (gear)
    {
    case PARK:
        return 'P';
    case REVERSE:
        return 'R';
    case NEUTRAL:
        return 'N';
    case DRIVE:
        return 'D';
    default:
        return ' ';
    }
}
