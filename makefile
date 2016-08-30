all: fbo_demo

fbo_demo:	main.o
		g++ -g -o fbo_demo main.o shader_utils.o -lglfw -lGLEW -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread

main.o:	main.cpp
	g++ -pipe -Wall -Wextra -c shader_utils.cpp main.cpp

clean:
		rm fbo_demo *.o
