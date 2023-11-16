all: focusingstepper

focusingstepper: testlibusb.o
	g++ -g -Wall testlibusb.o -lusb-1.0 -o sumix
	
testlibusb.o: testlibusb.cpp
	g++ -g -Wall -c testlibusb.cpp
	
clean:
	rm -rf *.o
	rm -rf *.gch
