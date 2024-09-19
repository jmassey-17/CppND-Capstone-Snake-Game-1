# CPPND: Capstone Snake Game

This is the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

In this project, I used what I learnt in the C++ Nanodegree to extend the Snake game. 

## New Features
* Obstacles:
  * Obstacles are now part of the game and are added to the board every time the snake eats food. 
* Game modes:
	The game comes with 4 possible modee, which are selected in the console before the game initializes. The modes are: 
  * 1: Classic Snake
  * 2: Snake With Static Obstacles
  * 3: Snake with randomly moving obstacles
  * 4: Snake with Obstacles that chase the head of the snake round the board. 

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

## Rubric Points
### Loops, Functions, I/O
* Project accepts user input to determine game mode
* Project is organised into functions. 
* Project uses arrays, vectors and constant variables. 

### Object Oriented Programming
* All functions within classes have their purpose and function documented.
* Project is organized into classes, with 
* Constructor and destructor of the game class have been expanded. 

### Memory Management 
* Destructor of the game class is used to tidy up threads.
* References are called to manage implementation of obstacles vector and manage the threads. 
* Snake is implemented as a unique pointer. 

### Concurrency
* Project splits the snake and the obstacle movement into two threads and runs them concurrently. 
* Mutex and a lock is used for this, as is a conditional variable. 



