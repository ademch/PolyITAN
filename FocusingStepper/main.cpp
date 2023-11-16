/*
 * setitimer.c - simple use of the interval timer
 */

#include <sys/time.h>   /* for setitimer */
#include <unistd.h>     /* for pause */
#include <signal.h>     /* for signal */
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <wiringPi.h>

#include "StepperMotor.h"
#include "Command.h"


using namespace std;


StepMotor stepMotor;

void TIMER3_COMPA_vect(void)
{
    stepMotor.timer_routine();
}


int main(int argc, char *argv[])
{
    wiringPiSetup();
    pinMode(PIN_STEPDRIVER_STEP, OUTPUT);
    pinMode(PIN_STEPDRIVER_DIR,  OUTPUT);
    pinMode(PIN_STEPDRIVER_EN,   OUTPUT);
    
    digitalWrite(PIN_STEPDRIVER_STEP, LOW);
    digitalWrite(PIN_STEPDRIVER_DIR,  LOW);
    digitalWrite(PIN_STEPDRIVER_EN,   HIGH);
    
    // Upon SIGALRM, call TIMER3_COMPA_vect()
    if (signal(SIGALRM, (void (*)(int))TIMER3_COMPA_vect) == SIG_ERR)
    {
        perror("Unable to catch SIGALRM");
        exit(1);
    }

    setbuf(stdout, NULL);

    while (1)
    {
      ShowMenu();
      
      unsigned int i;
      cin >> i;
    
      if (cin.fail())
      {
          string input;
          std::cout << "Integer command expected" << endl;
          std::cin.clear();     // clear fail state
          getline(cin, input);  // eat failed buffer
        
          continue;
      }
      
      if (i<CMD_COUNT)
          ProcessMenuItemOkClick(i);
      else
          printf("Command 1->%d expected\n", CMD_COUNT-1);
   }

}


