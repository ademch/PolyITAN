all: polyitan

polyitan:   ./obj/PolyITAN_Camera.o ./obj/Camera.o ./obj/GLSL_Pipeline.o \
			./obj/Bayer2rgbFBO.o ./obj/bilateralFBO.o ./obj/smx-20Sxx.o ./obj/smx_usb.o \
			./obj/SobelFBO.o ./obj/rgb2yuvFBO.o ./../../!!adGUI/obj/glfont.o \
			./../../!!adGlobals/obj/wdir.o ./obj/ParamsSubWindow.o ./obj/CameraSubWindow.o \
			./obj/HistogramSubWindow.o ./../../!!adExtensions/obj/extensions.o \
			./../../!!adExtensions/obj/Shaders.o ./../../!!adGlobals/obj/vector_math.o \
			./../../!!adGUI/obj/SubWindow.o ./../../!!adGlobals/obj/adOpenGLUtilities.o \
			./../../!!adGUI/obj/button.o ./../../!!adGlobals/obj/trackball.o \
			./../../!!adGUI/obj/label.o ./../../!!adGUI/obj/OnOffFlipSwitch.o \
			./../../!!adGUI/obj/fps.o ./../../!!adGlobals/obj/bmpHandler.o \
			./../../!!adGUI/obj/ComboBox.o ./../../!!adExtensions/obj/read_shader_file.o \
			./../../!!adGUI/obj/Histogram.o
	g++ -g -Wall ./obj/PolyITAN_Camera.o ./obj/Camera.o ./obj/GLSL_Pipeline.o \
			./obj/Bayer2rgbFBO.o ./obj/bilateralFBO.o ./obj/smx_usb.o ./obj/smx-20Sxx.o \
			./obj/SobelFBO.o ./obj/rgb2yuvFBO.o ./../../!!adGUI/obj/glfont.o \
			./../../!!adGlobals/obj/wdir.o ./obj/ParamsSubWindow.o ./obj/CameraSubWindow.o \
			./obj/HistogramSubWindow.o ./../../!!adExtensions/obj/extensions.o \
			./../../!!adExtensions/obj/Shaders.o ./../../!!adGlobals/obj/vector_math.o \
			./../../!!adGUI/obj/SubWindow.o ./../../!!adGlobals/obj/adOpenGLUtilities.o \
			./../../!!adGUI/obj/button.o ./../../!!adGlobals/obj/trackball.o \
			./../../!!adGUI/obj/label.o ./../../!!adGUI/obj/OnOffFlipSwitch.o \
			./../../!!adGUI/obj/fps.o ./../../!!adGlobals/obj/bmpHandler.o \
			./../../!!adGUI/obj/ComboBox.o ./../../!!adExtensions/obj/read_shader_file.o \
			./../../!!adGUI/obj/Histogram.o \
			-lusb-1.0 -lpthread -lglut -lOpenGL -lGLU -o PolyITAN_Camera
	
./obj/PolyITAN_Camera.o: PolyITAN_Camera.cpp PolyITAN_Camera.h
	g++ -g -Wall -c PolyITAN_Camera.cpp -o "./obj/PolyITAN_Camera.o"

./obj/Camera.o: Camera.cpp Camera.h
	g++ -g -Wall -c Camera.cpp -o "./obj/Camera.o"
	
./obj/smx-20Sxx.o: smx-20Sxx.cpp smx-20Sxx.h
	g++ -g -Wall -c smx-20Sxx.cpp -o ./obj/smx-20Sxx.o

./obj/smx_usb.o: smx_usb.cpp smx_usb.h
	g++ -g -Wall -c smx_usb.cpp -o ./obj/smx_usb.o

./obj/GLSL_Pipeline.o: GLSL_Pipeline.cpp GLSL_Pipeline.h
	g++ -g -Wall -c GLSL_Pipeline.cpp -o "./obj/GLSL_Pipeline.o"

./obj/Bayer2rgbFBO.o: Bayer2rgbFBO.cpp Bayer2rgbFBO.h
	g++ -g -Wall -c Bayer2rgbFBO.cpp -o "./obj/Bayer2rgbFBO.o"

./obj/bilateralFBO.o: bilateralFBO.cpp bilateralFBO.h
	g++ -g -Wall -c bilateralFBO.cpp -o "./obj/bilateralFBO.o"

./obj/SobelFBO.o: SobelFBO.cpp SobelFBO.h
	g++ -g -Wall -c SobelFBO.cpp -o "./obj/SobelFBO.o"

./obj/rgb2yuvFBO.o: rgb2yuvFBO.cpp rgb2yuvFBO.h
	g++ -g -Wall -c rgb2yuvFBO.cpp -o "./obj/rgb2yuvFBO.o"
	
./obj/ParamsSubWindow.o: ParamsSubWindow.cpp ParamsSubWindow.h
	g++ -g -Wall -c ParamsSubWindow.cpp -o ./obj/ParamsSubWindow.o

./obj/CameraSubWindow.o: CameraSubWindow.cpp CameraSubWindow.h
	g++ -g -Wall -c CameraSubWindow.cpp -o ./obj/CameraSubWindow.o

./obj/HistogramSubWindow.o: HistogramSubWindow.cpp HistogramSubWindow.h
	g++ -g -Wall -c HistogramSubWindow.cpp -o ./obj/HistogramSubWindow.o

./../../!!adGUI/obj/glfont.o: ./../../!!adGUI/glfont.cpp ./../../!!adGUI/glfont.h
	g++ -g -Wall -c ./../../!!adGUI/glfont.cpp -o ./../../!!adGUI/obj/glfont.o

./../../!!adGUI/obj/SubWindow.o: ./../../!!adGUI/SubWindow.cpp ./../../!!adGUI/SubWindow.h
	g++ -g -Wall -c ./../../!!adGUI/SubWindow.cpp -o ./../../!!adGUI/obj/SubWindow.o

./../../!!adGUI/obj/button.o: ./../../!!adGUI/button.cpp ./../../!!adGUI/button.h
	g++ -g -Wall -c ./../../!!adGUI/button.cpp -o ./../../!!adGUI/obj/button.o

./../../!!adGUI/obj/label.o: ./../../!!adGUI/label.cpp ./../../!!adGUI/label.h
	g++ -g -Wall -c ./../../!!adGUI/label.cpp -o ./../../!!adGUI/obj/label.o

./../../!!adGUI/obj/OnOffFlipSwitch.o: ./../../!!adGUI/OnOffFlipSwitch.cpp ./../../!!adGUI/OnOffFlipSwitch.h
	g++ -g -Wall -c ./../../!!adGUI/OnOffFlipSwitch.cpp -o ./../../!!adGUI/obj/OnOffFlipSwitch.o

./../../!!adGUI/obj/ComboBox.o: ./../../!!adGUI/ComboBox.cpp ./../../!!adGUI/ComboBox.h
	g++ -g -Wall -c ./../../!!adGUI/ComboBox.cpp -o ./../../!!adGUI/obj/ComboBox.o

./../../!!adGUI/obj/fps.o: ./../../!!adGUI/fps.cpp ./../../!!adGUI/fps.h
	g++ -g -Wall -c ./../../!!adGUI/fps.cpp -o ./../../!!adGUI/obj/fps.o

./../../!!adGUI/obj/Histogram.o: ./../../!!adGUI/Histogram.cpp ./../../!!adGUI/Histogram.h
	g++ -g -Wall -c ./../../!!adGUI/Histogram.cpp -o ./../../!!adGUI/obj/Histogram.o

./../../!!adGlobals/obj/wdir.o: ./../../!!adGlobals/wdir.cpp ./../../!!adGlobals/wdir.h
	g++ -g -Wall -c ./../../!!adGlobals/wdir.cpp -o ./../../!!adGlobals/obj/wdir.o

./../../!!adGlobals/obj/adOpenGLUtilities.o: ./../../!!adGlobals/adOpenGLUtilities.cpp ./../../!!adGlobals/adOpenGLUtilities.h
	g++ -g -Wall -c ./../../!!adGlobals/adOpenGLUtilities.cpp -o ./../../!!adGlobals/obj/adOpenGLUtilities.o

./../../!!adGlobals/obj/trackball.o: ./../../!!adGlobals/trackball.cpp ./../../!!adGlobals/trackball.h
	g++ -g -Wall -c ./../../!!adGlobals/trackball.cpp -o ./../../!!adGlobals/obj/trackball.o

./../../!!adGlobals/obj/bmpHandler.o: ./../../!!adGlobals/bmpHandler.cpp ./../../!!adGlobals/bmpHandler.h
	g++ -g -Wall -c ./../../!!adGlobals/bmpHandler.cpp -o ./../../!!adGlobals/obj/bmpHandler.o

./../../!!adExtensions/obj/extensions.o: ./../../!!adExtensions/extensions.cpp ./../../!!adExtensions/extensions.h
	g++ -g -Wall -c ./../../!!adExtensions/extensions.cpp -o ./../../!!adExtensions/obj/extensions.o

./../../!!adExtensions/obj/Shaders.o: ./../../!!adExtensions/Shaders.cpp ./../../!!adExtensions/Shaders.h
	g++ -g -Wall -c ./../../!!adExtensions/Shaders.cpp -o ./../../!!adExtensions/obj/Shaders.o

./../../!!adExtensions/obj/read_shader_file.o: ./../../!!adExtensions/read_shader_file.cpp ./../../!!adExtensions/read_shader_file.h
	g++ -g -Wall -c ./../../!!adExtensions/read_shader_file.cpp -o ./../../!!adExtensions/obj/read_shader_file.o

./../../!!adGlobals/obj/vector_math.o: ./../../!!adGlobals/vector_math.cpp ./../../!!adGlobals/vector_math.h
	g++ -g -Wall -c ./../../!!adGlobals/vector_math.cpp -o ./../../!!adGlobals/obj/vector_math.o

clean:
	rm -rf -v *.o
	rm -rf -v *.gch
	rm -rf -v ./obj/*.o
	rm -rf -v ./../../!!adExtensions/obj/*.o
	rm -rf -v ./../../!!adGlobals/obj/*.o
	rm -rf -v ./../../!!adGUI/obj/*.o
