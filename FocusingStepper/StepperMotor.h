
#include "PinsConfig.h"

#define STEP_NUMBER      12
#define STEP_DIVIDER     8    // 1/8 of 1.8 deg step

// Acceleration/decelaration profile:
//
// * Max speed of 48 pulses per second being multiplied by 1/STEP_DIVIDER gives real speed of:
//   E.g. 48*1.8/8  = 6*1.8 deg per s
//
// * Acceleration time is fixed to 1s
// * Max speed is achieved in STEP_NUMBER steps
//    24/8 = in 3*1.8 deg
//


enum MotorState { IDLE, ACC, MARCH, DECC };

class StepMotor
{
private:
	int      iStepsAcc, iStepsDec, iStepsMarch;
	int      iSeq;

	int  	 pin_step, pin_dir, pin_boost, pin_en;

	MotorState mState;

	int      iRotationDir;
	int      iCurPos;

public:

	StepMotor()
	{

		pin_step  = PIN_STEPDRIVER_STEP;
		pin_dir   = PIN_STEPDRIVER_DIR;
		pin_en    = PIN_STEPDRIVER_EN;

		iSeq = 0;
		mState = IDLE;

		iRotationDir = +1;
		iCurPos = 0;
	};

	void MarchNSteps(unsigned int i1_8_StepCount, bool bForward);
	void MarchNStepsSynch(unsigned int i1_8_StepCount, bool bForward);
	void timer_routine(); // timer compare interrupt service routine

private:
	void SendStepPulseToDrive();
	void SetTimerVal(unsigned int val);
};	


