main:
	g++ -c libs/*/*.cpp libs/*/*.c src/*.cpp
	g++ *.o -o bin/covid -lsfml-graphics -lsfml-window -lsfml-system -lSFGUI
	rm *.o