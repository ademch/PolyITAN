#include "Command.h"
#include "StepperMotor.h"
#include "PinsConfig.h"
#include <wiringPi.h>

#include <stdio.h>

extern StepMotor stepMotor;


MotorCommands commands[CMD_COUNT] =
{
	{CMD_NONE,		         	     "<  SELECT CMD  >"},
	{CMD_STEP_DRIVER_ENABLE,         "1:   DRIVE ENABLE "},
	{CMD_STEP_DRIVER_DISABLE,        "2:   DRIVE DISABLE"},
	{CMD_STEP_DRIVER_5_ROTATE_CW,    "3:   DRIVE 5  x1.8 CW "},
	{CMD_STEP_DRIVER_5_ROTATE_CCW,   "4:   DRIVE 5  x1.8 CCW"},
	{CMD_STEP_DRIVER_10_ROTATE_CW,   "5:   DRIVE 10 x1.8 CW "},
	{CMD_STEP_DRIVER_10_ROTATE_CCW,  "6:   DRIVE 10 x1.8 CCW"},
	{CMD_STEP_DRIVER_20_ROTATE_CW,   "7:   DRIVE 20 x1.8 CW  "},
	{CMD_STEP_DRIVER_20_ROTATE_CCW,  "8:   DRIVE 20 x1.8 CCW "}
};


CommandTypes MenuState = CMD_COUNT; // set to last item in order to be incremented during startup draw

void ShowMenu()
{
	int i;
	
	printf("\n");
	for (i=0; i<CMD_COUNT; i++)
	{
		printf("%s\n", commands[i].Name);
	}
}


void ProcessMenuItemOkClick(unsigned int iVal)
{
	MenuState = (CommandTypes)iVal;

	switch (MenuState)
	{
	case CMD_NONE:
		break;
	//~ case CMD_STEP_DRIVER_SLEEP:
		//~ printf("Sleep...");

		//~ digitalWrite(PIN_STEPDRIVER_SLEEP, LOW);	// negated logic

		//~ printf("done\n");
	//~ break;
	//~ case CMD_STEP_DRIVER_WAKE:
		//~ printf("Wake...");

		//~ digitalWrite(PIN_STEPDRIVER_SLEEP, HIGH);	// negated logic

		//~ printf("done\n");
		//~ break;
	case CMD_STEP_DRIVER_ENABLE:
		printf("Enable...");

		digitalWrite(PIN_STEPDRIVER_EN, LOW);		// negated logic

		printf("done\n");
		break;
	case CMD_STEP_DRIVER_DISABLE:
		printf("Disable...");

		digitalWrite(PIN_STEPDRIVER_EN, HIGH);  	// negated logic

		printf("done\n");
		break;
	case CMD_STEP_DRIVER_5_ROTATE_CW:
		stepMotor.MarchNStepsSynch(5,  rotate_cw);
		break;
	case CMD_STEP_DRIVER_5_ROTATE_CCW:
		stepMotor.MarchNStepsSynch(5,  rotate_ccw);
		break;
	case CMD_STEP_DRIVER_10_ROTATE_CW:
		stepMotor.MarchNStepsSynch(10, rotate_cw);
		break;
	case CMD_STEP_DRIVER_10_ROTATE_CCW:
		stepMotor.MarchNStepsSynch(10, rotate_ccw);
		break;
	case CMD_STEP_DRIVER_20_ROTATE_CW:
		stepMotor.MarchNStepsSynch(20, rotate_cw);
		break;
	case CMD_STEP_DRIVER_20_ROTATE_CCW:
		stepMotor.MarchNStepsSynch(20, rotate_ccw);
		break;
	default:
		break;
	}
}


