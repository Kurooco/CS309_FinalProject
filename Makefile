all: compile link run

compile:
	g++ -c main.cpp -I"C:\Users\DJsul\OneDrive\Documents\libraries\SFML-2.5.1\include" -DSFML_STATIC -fopenmp

link:
	g++ main.o -L"C:\Users\DJsul\OneDrive\Documents\libraries\SFML-2.5.1\lib" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lopengl32 -lfreetype -lwinmm -lgdi32 -openal32 -fopenmp -o Main

run:
	main.exe

clean:
	rm Main.exe
	

