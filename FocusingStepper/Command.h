

void ShowMenu();
void ProcessMenuItemOkClick(unsigned int);



enum CommandTypes {
					CMD_NONE,
					CMD_STEP_DRIVER_ENABLE,         CMD_STEP_DRIVER_DISABLE,
					CMD_STEP_DRIVER_5_ROTATE_CW,    CMD_STEP_DRIVER_5_ROTATE_CCW,
					CMD_STEP_DRIVER_10_ROTATE_CW,   CMD_STEP_DRIVER_10_ROTATE_CCW,
					CMD_STEP_DRIVER_20_ROTATE_CW,   CMD_STEP_DRIVER_20_ROTATE_CCW,
					CMD_COUNT
				  };

struct MotorCommands
{
	CommandTypes command;
	const char* Name;
};


#define rotate_cw  true
#define rotate_ccw false

