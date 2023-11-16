
#include "StepperMotor.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

#include <wiringPi.h>

extern void TIMER3_COMPA_vect(void);

// usec 6*8pulses final vel, 0.5s accel time
static const unsigned int aSpeedProfile[STEP_NUMBER] =
{
	189437,52865,38820,32249,28390,25868,24127,22289,22021,21421,21045,20863
};


void StepMotor::MarchNStepsSynch(unsigned int i1_8_StepCount, bool bForward)
{
	delay(10); // compulsory delay after the previous step

	MarchNSteps(i1_8_StepCount, bForward);
	while (mState != IDLE)
	{
		delay(10);
	}
}

void StepMotor::MarchNSteps(unsigned int i1_8_StepCount, bool bCW)
{
unsigned int iStepCount = i1_8_StepCount*STEP_DIVIDER;

	if (iStepCount == 0) return;
	if (mState != IDLE) return;

	if (iStepCount < STEP_NUMBER * 2) {
		iStepsAcc = iStepCount / 2;
		iStepsDec = iStepCount - iStepsAcc;
		iStepsMarch = 0;
	}
	else {
		iStepsAcc = STEP_NUMBER;
		iStepsDec = STEP_NUMBER;
		iStepsMarch = iStepCount - STEP_NUMBER * 2;
	}

	printf("Rotating ");
	printf("%i", i1_8_StepCount);
	printf("...");

	if (bCW) {
		digitalWrite(pin_dir, HIGH);		// clockwise
		iRotationDir = -1;
	}
	else {
		digitalWrite(pin_dir, LOW);			// counterclockwise
		iRotationDir = 1;
	}
	usleep(20);								// signal sent at 50 kHz having 200kHz described as max allowed 

	mState = ACC;
	iSeq = 0;
	
	SetTimerVal(10000);						// start timer
}


void StepMotor::SendStepPulseToDrive()
{
	digitalWrite(pin_step, HIGH);
	
		usleep(20);		   					// signal sent at 50 kHz having 200kHz described as max allowed
		
	digitalWrite(pin_step, LOW);
}


void StepMotor::SetTimerVal(unsigned int usec)
{
    struct itimerval it_val;
  
    it_val.it_value.tv_sec      = 0;
    it_val.it_value.tv_usec     = usec; 
    it_val.it_interval.tv_sec   = 0;
    it_val.it_interval.tv_usec  = usec;
  
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1)
    {
        perror("error calling setitimer()");
		exit(1);
    }

	// update current position
	iCurPos += iRotationDir;

	//printf(" %i->%i\n", iSeq, usec);
}

// timer interrupt service routine
void StepMotor::timer_routine()
{
	if (mState == IDLE) return;

	if (mState == ACC)
	{
		if (iSeq >= iStepsAcc)
		{
			//printf("acc->march\n");

			iSeq = 0;
			mState = MARCH;
		}
		else
		{
			//printf("acc\n");

			SendStepPulseToDrive();
			
			SetTimerVal( aSpeedProfile[iSeq] );
			iSeq++;
		}
	}
	if (mState == MARCH)
	{
		if (iSeq >= iStepsMarch)
		{
			//printf("march->dec\n");

			iSeq = 0;
			mState = DECC;
		}
		else
		{
			//printf("march\n");

			SendStepPulseToDrive();
			
			SetTimerVal(aSpeedProfile[STEP_NUMBER - 1]);	// use the last value as the fastest speed
			
			iSeq++;
		}
	}
	if (mState == DECC)
	{
		if (iSeq >= iStepsDec)
		{
			SetTimerVal(0);	// stop timer (clock source=none)

			printf("Done\n");

			iSeq = 0;
			mState = IDLE;
		}
		else
		{
			//printf("dec\n");

			SendStepPulseToDrive();
			
			SetTimerVal( aSpeedProfile[iStepsDec-1-iSeq] );

			iSeq += 1;
		}
	}
}

