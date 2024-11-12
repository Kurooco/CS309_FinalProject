all: parallel link_parallel run

all_serial: serial link_serial run

serial:
	g++ -c main.cpp -I"C:\Users\DJsul\OneDrive\Documents\libraries\SFML-2.5.1\include" -I"C:\Program Files\boost\boost_1_82_0\boost_1_82_0" -DSFML_STATIC -fopenmp

parallel:
	g++ -c parallelmain.cpp -I"C:\Users\DJsul\OneDrive\Documents\libraries\SFML-2.5.1\include" -I"C:\Program Files\boost\boost_1_82_0\boost_1_82_0" -DSFML_STATIC -fopenmp

link_parallel:
	g++ parallelmain.o -L"C:\Users\DJsul\OneDrive\Documents\libraries\SFML-2.5.1\lib" -L"C:\Program Files\boost\boost_1_82_0\boost_1_82_0" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lopengl32 -lfreetype -lwinmm -lgdi32 -openal32 -fopenmp -o Main

link_serial:
	g++ main.o -L"C:\Users\DJsul\OneDrive\Documents\libraries\SFML-2.5.1\lib" -L"C:\Program Files\boost\boost_1_82_0\boost_1_82_0" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lopengl32 -lfreetype -lwinmm -lgdi32 -openal32 -fopenmp -o Main

run:
	main.exe

clean:
	rm Main.exe
	

