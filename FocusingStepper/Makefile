all: focusingstepper

focusingstepper: main.o StepperMotor.o Command.o
	g++ -g -Wall main.o StepperMotor.o Command.o -lwiringPi -o stepperMotor
	
main.o: main.cpp PinsConfig.h
	g++ -g -Wall -c main.cpp
	
stepperMotor.o: StepperMotor.h StepperMotor.cpp
	g++ -g -Wall -c StepperMotor.cpp
	
Command.o: Command.h Command.cpp
	g++ -g -Wall -c Command.cpp

clean:
	rm -rf -v *.o
	rm -rf -v *.gch
