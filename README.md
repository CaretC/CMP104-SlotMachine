CMP 104 - Fruit Machine Assignment
==================================

Overview
--------

Assignment is to create a fruit machine game in the console, which the user can play for prizes. The minimum functionality is shown below:
* 3 Columns (Reels)
* When the game beings all 3 reels will 'spin'
* User chooses when to stop reels 1, 2 & 3
* If 3 match big jackpot
* If 2 match smaller jackpot

Additional functionality can be added as needed.

Concept For My Machine
----------------------

To bring the concept of the fruit machine and the course Ethical Hacking I thought it would be fun to make it a hacking themed fruit machine. The machine can be made to look cool with ASCII art. 

Fruit machine in this game is called the 'hack machine' and the reels will have console commands displayed on them. The prize/ currancy in this game is data, data stolen by successful hacks. Each time an invalid command is selected (i.e. 3 reels do not match) the player's visibility increases, once the visibility is at maximum the player is caught and the game is over. 

An added unique mechanic will be added to the game to add an extra challenge to game. Each time a spin is started a random 3 reel vulnerability is generated. This value will yeild the highest prize but the highest risk.

Concept of game:

![Game Concept Image](readmeImages/GameConcept.PNG)

Game Mechanics
--------------

### Reels
The reel hacking themed commands (a little cheesey):
1. cd
2. sudo
3. ls
4. bash
5. vim
6. ping
7. grep
8. ssh
9. echo

### Controls
The main game controls are:
* `Space` - Start Spins
* `Esc` - Quit Game
* `Z` - Stop Reel 1
* `X` - Stop Reel 2
* `C` - Stop Reel 3
* `R` - Restart Game 

### General Game Flow
A normal game would flow like below:
* Game intro and main screen displays with all the instruction games.
* `Space` to start game.
* Pints slot machine with animations.
* `Esc` to quit from this state back to main screen, `Space` starts reels spinning.
* All 3 reels keep spinning and vulnerability generated.
* Press `Z` key to stop Reel 1 spinning, other 2 reels keep spinning.
* Press `X` key to stop Reel 2 spinning, reel 3 keeps spinning.
* Press `C` key to stop Reel 3 Spinning. 
* Once all reels stopped values are checked against victory conditions.
* Data value increased if a winning score, visibility increases.
* Each spin attempt reel spinning speed increases, this is more drematic the higher the level. 
* Winning will also reduce the spin speed increase. 
* If Max visibility reached, game is over. 
* If game is over `R` can be pressed to restart the game.

### Game States
The major game states are:

0. `IDLE`
1. `SPIN_REELS_123`
2. `SPIN_REELS_23`
3. `SPIN_REEL_3`
4. `ALL_REELS_STOPPED`
5. `GAME_OVER`
6. `QUIT`

These sates are stored in an enum class to make the states very obvious when used later in the code:

```cpp
	enum gameStates 
	{ 
		IDLE = 0,
		SPIN_REELS_123 = 1,
		SPIN_REELS_23 = 2,
		SPIN_REEL_3 = 3,
		ALL_REELS_STOPPED = 4,
		GAME_OVER = 5,
		QUIT = 6
	};
```
### Debug Info Window

The 'DEBUG INFO' window is used to display the game state, instructions and other little bits in a terminal prompt style to make it feel a little more hacker like. 

Main Game Loop Outline
-----------------------

A rough plan of the main game loop

* While `gameActive`
	* Select Action Dependant on `gameState`.
		* Idle State
			* Loop Animation
		* Spin State
			* Spin All
			* Spin 2 & 3
			* Spin 3
		* Game Over State
* End of While

Includes
---------

```cpp
#include <iostream> // For Console i/o
#include <fcntl.h> // To set 16 encoding
#include <io.h> // Used to get _setmode()
#include <conio.h> // For reading key presses
#include <stdio.h> 
#include <Windows.h> // To get access to console screen buffer etc.
#include <cstdlib> // Used for Random Number Generation
#include <ctime> // Date and time info (Used as random Number Seed)
```

General Notes
-------------

Note about unicode

Globals
--------

```cpp
// Console Globals
CONSOLE_SCREEN_BUFFER_INFO console_info;
CONSOLE_CURSOR_INFO cursor_info;
HANDLE hconsole;
const int SIZE_X = 80; // Size of prinatble screen area X
const int SIZE_Y = 25; // Size of prinatble screen area Y
const int DEFAULT_TEXT_COLOR = 7; // Default Console Text Colour;

// Game Consts
const int DATA_PRIZE_2 = 3; // Data Prize For 2 Reels
const int DATA_PRIZE_3 = 9; // Data Prize For 3 Reels
const int DATA_PRIZE_VUN = 12; // Data Prize Vun
const int SPIN_SPEED_PRIZE_2 = 50;
const int SPIN_SPEED_PRIZE_3 = 75;
const int START_SPIN_SPEED = 500;
const int DIFFICULTY = 50; // Ammount Reel Spin Speed Incrases in ms // TODO: Make this more mean each level.
const enum gameStates {
	IDLE = 0,
	SPIN_REELS_123 = 1,
	SPIN_REELS_23 = 2,
	SPIN_REEL_3 = 3,
	ALL_REELS_STOPPED = 4,
	GAME_OVER = 5,
	QUIT = 6
};

// Game Controls
const char REEL1_KEY = 'Z';
const char REEL2_KEY = 'X';
const char REEL3_KEY = 'C';
const char RESET_KEY = 'R';
const char PLAY_KEY = ' ';
const char QUIT_KEY = 27; // Esc Key.

// Reels
const int REEL_LENGTH = 9;
const wstring REEL_VALUES[REEL_LENGTH] = { 
	L"cd  ",
	L"sudo",
	L"ls  ",
	L"bash",
	L"vim ",
	L"ping",
	L"grep",
	L"ssh ",
	L"echo"
};
```