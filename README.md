# Cow Simulator with OpenMP

- This is a project for CS-309. For a better formatted description of the project, see my Google Classroom submission.


CS-309 Final Project Writeup
David Sulpice

Project Description
The goal of this program is to simulate the fluctuations in population experienced by a community of cows in a bounded field with a limited amount of grass. As the cow population increases, the total grass consumption of the community increases as well, making grass harder to find for hungry cows. If the cows are not able to find a substantial amount of food, their numbers will start to decline. This in turn allows grass to grow back, making life possible for the next generation of cows. This cycle can continue indefinitely unless all of the cows go extinct, at which point the grass population is free to claim its territory for the rest of the simulation.

Tools Used
For rendering the graphics, I decided to use SFML (Simple and Fast Multimedia Library) since I already had some previous experience using it. (I used some basic starter code that I wrote in the past so that I did not have to start with a completely blank file.) You can find the library here: https://www.sfml-dev.org/
For drawing the sprites, I used Microsoft’s Paint 3D.
For the parallelization for the serial program, I decided to use OpenMP.

- Explanation of Files in Submission
- SimSeq.exe runs the program sequentially without graphics.
- SimSeqVisual.exe runs the program sequentially with graphics.
- SimOmp.exe runs the parallelized program without graphics.
- SimOmpVisual.exe runs the parallelized program with graphics.
- Main.cpp is the sequential code.
- ParallelMain2.cpp is the parallel code. (ParallelMain.cpp was not successful, and I started over)
- reference.cpp is a file that I wrote for SFML a while ago that I used as a reference. (Just to remember some of the basics of SFML and C++ that I had forgotten)
- README.md (this) is a copy of the Google Classroom submission. Please refer to that document instead.

Note: The programs are compiled to render a 1600x900 window. If your screen is smaller, some of the data may appear to be cut out.

Implementation Details for the Serial Program
Here are the basic properties of each class in the simulation code:

CustomTexture: Holds a texture, as well as an x and y coordinate for the texture’s origin. (This class was included in the starter code that I wrote in the past.)

CustomSprite: Holds the basic information for a sprite. While SFML already has the Sprite class, I have implemented my own so that I’m able to easily control collision, texture orientation, etc. (This class was included in the starter code that I wrote in the past.)

Grass: The Grass class holds a grid (locationArray) that keeps track of all the locations of the Grass objects. This grid is static and accessible to all Grass objects. The grid is used when checking for neighboring grass objects during reproduction. On each update, a grass object will determine if it is ready to reproduce. If it is, then it will look to see if one of its four adjacent neighbor spots is empty. The place it ultimately decides to use is random. If there is an empty spot, it will place its child in the cell in the 2D grid corresponding to that spot, and then return the child from the reproduce() method so that it can be added to the grasses vector, which is used for iteratively updating the Grass objects. Grass has two states: normal and degraded. Each Grass starts as normal and becomes degraded once a cow eats it. If given time to recover, it will return to its normal state. Otherwise, if it is eaten again while degraded, it will be removed from locationArray.

Cow: Cows are not bound to a grid like grass is, and can instead move around the screen freely. Each cow starts with a certain amount of food. As the simulation progresses, this amount of food steadily decreases. If a cow’s food count drops to zero, then it starves and is removed from the simulation. Cows can obtain more food by eating grass, but how a cow decides to search for grass depends on its situation. If a cow is above the DESPERATION_THRESHOLD, it will try to find grass that is in optimal shape (represented by the bright-green grass sprite). If a cow is below the DESPERATION_THRESHOLD, it is content looking for less-nutritious degraded grass that has been partially eaten by other cows (represented by the muddied grass sprite). Cows will always go to the closest piece of grass that it is content with eating. If it can not find a piece of grass nearby, it goes to a random location on the map in the hope that it will find a new grass to pursue.

The main loop: Each frame, the grass is updated first followed by the cows. All objects are drawn onto the screen, updated, and checked for conditions that would lead to their removal. Cows are responsible for removing grass from the grasses vector when they eat. At the end of the program, the populations of the grass and the cows over time are displayed on the screen. The green line represents the grass population, and the white line represents the cow population.

Please note: If you have any trouble getting the graphics to work for you, let me know. I can send you a video of the program in action or figure something else out. (I’m making this on Windows, and I’m not sure if it is compatible with Mac. It should be since SFML is multiplatform, but I’m unsure.) The window is made for a 1600x900 screen.

Parallelization
For the parallelized version of the program, I decided to not pursue determinism. Thus, when the parallel version of the program is run multiple times with the same beginning scenario, it is not guaranteed to output the same thing. Because this program is meant to be a rough simulation, taking the liberty of nondeterminism does not seem unreasonable. While this took some of the pressure off of the parallelized implementation, I still encountered some unexpected issues. For one, in the update loops used to produce or delete cows or grass, I originally had it so that new objects were added to the vectors being iterated over as the loop was running. This could be a problem if multiple threads are accessing the same vector while it is being updated. I was able to find ways to avoid this issue. In the case of the grass, I made it so that the new grass that is produced is stored in a temporary vector (protected by a critical section), and then transferred into the grasses vector with a sequential loop. For the cows, I parallelized the loop in main that iterates over cows and makes them search for grass, and then moved the deletion of grass by cows to a separate sequential loop. Originally, I tried to make the loop inside the Cow class that searches for grass run in parallel, but this caused the program to slow down instead of speed up. I suspect this is because many cows are iterated over each frame, meaning that new threads would have to be created and destroyed several times each frame as well.

Parallelization Stats
Iterations: 10,000
Original time: 20.31
Average parallelized time: 2.88
Speedup: 7.05

Iterations: 20,000
Original time: 108.36
Average parallelized time: 31.13
Speedup: 3.48

These stats were obtained from running the program on my personal computer, which has 4 cores. (https://www.techpowerup.com/cpu-specs/core-i7-3720qm.c1013) The parallelized time was taken from running the program three times and taking the average (because of its non-deterministic behavior). The sequential time was taken from running the program three times and taking the best time. Visual representations of the cows and grass during the simulation were turned off for gathering this data.

While the simulation with 10,000 iterations appears to demonstrate superlinear speedup, larger problem sizes seem to produce a more realistic efficiency. I’ve looked carefully at the parallelized code and the results it produces, and it does not appear to have any unfair advantage. 

Websites Visited for this Assignment
https://www.w3schools.com/cpp/cpp_output.asp
https://stackoverflow.com/questions/72050034/nothing-prints-on-console-in-sfml
https://stackoverflow.com/questions/12002304/how-to-compile-openmp-using-g
https://www.sfml-dev.org/documentation/2.6.1/classsf_1_1RenderWindow.php
https://cplusplus.com/forum/beginner/242227/
https://cplusplus.com/reference/cstdlib/rand/
https://www.sfml-dev.org/documentation/2.6.1/classsf_1_1Vector2.php
https://cplusplus.com/reference/cmath/sqrt/
https://stackoverflow.com/questions/17042487/sfml-fails-in-multithreading
https://www.sfml-dev.org/tutorials/2.0/graphics-draw.php#drawing-from-threads
https://en.cppreference.com/w/cpp/language/static
https://stackoverflow.com/questions/4981197/2d-array-values
https://stackoverflow.com/questions/39191765/qualified-id-in-declaration-before-token
https://en.cppreference.com/w/cpp/container/vector
https://stackoverflow.com/questions/37900109/adding-an-element-to-a-vector-while-iterating-over-it
https://www.learncpp.com/cpp-tutorial/constructors-and-initialization-of-derived-classes/
https://en.cppreference.com/w/cpp/container/vector/erase
https://www.sfml-dev.org/tutorials/2.6/graphics-shape.php
https://www.sfml-dev.org/tutorials/2.6/graphics-vertex-array.php
https://cplusplus.com/reference/cstdlib/srand/
https://cplusplus.com/reference/ctime/
https://stackoverflow.com/questions/4264127/correct-format-specifier-for-double-in-printf
https://en.cppreference.com/w/cpp/chrono/time_point/time_since_epoch
https://learn.microsoft.com/en-us/cpp/parallel/concrt/reference/concurrency-namespace?view=msvc-170
https://www.youtube.com/watch?app=desktop&v=5afpq2TkOHc
https://www.youtube.com/watch?v=Cyc2gRb5MaQ
https://www.boost.org/users/history/version_1_82_0.html
https://stackoverflow.com/questions/24782038/omp-max-reduction-with-storage-of-index
https://www.openmp.org/spec-html/5.0/openmpsu107.html
https://www.boost.org/doc/libs/1_53_0/boost/container/vector.hpp
https://stackoverflow.com/questions/39067764/sfml-static-vs-dynamic-library-loading-how-does-visual-studio-specify-this
https://learn.microsoft.com/en-gb/windows/win32/dlls/dynamic-link-libraries
https://en.wikipedia.org/wiki/Object_file
https://stackoverflow.com/questions/6353149/does-vectorerase-on-a-vector-of-object-pointers-destroy-the-object-itself
https://bpoplauschi.github.io/2021/10/24/Intro-to-static-and-dynamic-libraries-frameworks.html


## Things I Learned/Relearned...

- The -mwindows flag removes the console
- add "public" before inherited class to make inherited methods available
