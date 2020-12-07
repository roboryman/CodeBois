main:
	g++ -c libs/*/*.cpp libs/*/*.c libs/libheatmap/colorschemes/RdGy.c libs/libheatmap/colorschemes/Spectral.c *.cpp
	g++ *.o -o bin/covid -lsfml-graphics -lsfml-window -lsfml-system -lSFGUI
	rm *.o