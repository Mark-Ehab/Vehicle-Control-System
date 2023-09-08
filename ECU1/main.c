#include "Application/App.h"

#define TRUE   		1

/* Application Entry Point  */
int main(void)
{
	/* Initialize App */
	App_Init();

	/* Display MetaWare Welcome Screen */
	App_welcomeScreen();

	while(TRUE)
	{
		/* Begin App */
		MainApp();
	}

	return 0;
}

