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

Additional functionality can be added as needed. An example game from assessment sheet is shown below:

![Example Game Image](readmeImages/ExampleGame.PNG)

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

### Game 

The game will be *State Machine* controlled. So, depending on the game's states it will enter several different state dependant loops. To escape that loop the game's state will be changed from within the loop. All the game state loops are contained in a game active loop. So once a loop is escaped it will run through main loop until next state loop is entered. 

The major game states are:

0. `IDLE` - Plays slot machine idle animation. (Lights and Name flashing, colours?)
1. `SPIN_REELS_123` - All 3 reels are continuously spinning until reel 1 key hit.
2. `SPIN_REELS_23` - Reels 2 & 3 continuously spinning until reel 2 key hit.
3. `SPIN_REEL_3` - Reel 3 continuously spinning until reel 3 key hit.
4. `ALL_REELS_STOPPED` - All reels have been stopped and victory state is checked. Game is then returned to `IDLE`.
5. `GAME_OVER` - Max visibility has been reached, game is over. Displays Game over animations. (Flashing lights, mean message?). Reset button is pressed to reset scores and return game to `IDLE`.
6. `QUIT` - Game is quit, display quit message/ close console?

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

Due to the use of Unicode character set in this game the file mode has been set to translate the `stdout` to **UTF-16** so all all char, strings and output should use wide characters, standard UTF-8 characters may not display correctly to console. 

Globals
--------

### Console
`cursor_info` - Stores console cursor information

`hconsole` - Stores console handle information

`DEFAULT_TEXT_COLOUR` - Console's default text colour (White by default)

### Game
`DATA_PRIZE_2` - Data prize for 2 reels (3 by default)

`DATA_PRIZE_3` - Data prize for 2 reels (9 by default)

`DATA_PRIZE_VUN` - Data prize for vulnerability exploit (12 by default)

`SPIN_SPEED_PRIZE_2` - Spin speed reduction for 2 reels in ms (50 by defualt)

`SPIN_SPEED_PRIZE_3` - Spin speed reduction for 3 reels in ms (75 by defualt)

`START_SPIN_SPEED` - Spin speed at the start of game in ms (500 by default)

`DIFFICULTY` - Spin speed increase after each spin, this happens regardless of win or loose. In ms (50 by default).

`gameStates` - Enum with all possible game states.
```cpp
const enum gameStates {
	IDLE = 0,
	SPIN_REELS_123 = 1,
	SPIN_REELS_23 = 2,
	SPIN_REEL_3 = 3,
	ALL_REELS_STOPPED = 4,
	GAME_OVER = 5,
	QUIT = 6
};

```

### Controls
`REEL1_KEY` - Key to stop Reel 1 spinning ('Z' by default)

`REEL2_KEY` - Key to stop Reel 2 spinning ('X' by default)

`REEL3_KEY` - Key to stop Reel 3 spinning ('C' by default)

`RESET_KEY` - Key to reset game ('R' by default)

`PLAY_KEY` - Key to start game and start spins (`SPACE` by default)

`QUIT_KEY` - Key to quit game from `IDLE` (`Esc` by default)

### Reels
`REEL_LENGTH` - Defines the number of vaules in reels (9 ny default)

`REEL_VALUES` - Stores all the possible reel values.
```cpp
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

### Code

```cpp
// Console Globals
CONSOLE_CURSOR_INFO cursor_info;
HANDLE hconsole;
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

**NOTE: Due to reel box size, all reel values(commands) must be 4 characters long, if they are shorter then this right-hand padding should be added to keep all reel strings to 4 characters.**

Functions
=========

Game Drawing Functions
----------------------

### GraphicsSetup()

This function is used to set up the console grapics. This includes setting up `hconsole` handle to allow console I/O, cursor visibility and console character mode. 

```cpp
void GraphicsSetup()
{
	// Enables File Translation to U16 Text so console can display unicode
	// makes stout print in U16, so all chars and string will need to be 16-bit,
	// i.e wchar, wstring.

	// Return assigned to result to prevent complier warning, result not used. 
	bool result = _setmode(_fileno(stdout), _O_U16TEXT);


	// 	CONOUT$ file name selected to open handle to current console active screen
	// buffer. 
	hconsole = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0L, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 0L);

	// Get Cursor Info
	GetConsoleCursorInfo(hconsole, &cursor_info);

	cursor_info.bVisible = false; // Set cursor visability to false

	// Set Cursor Info
	SetConsoleCursorInfo(hconsole, &cursor_info);
}
```

`_setmode` used to translate the `stdout` from the default **UTF-8** to **UTF-16** to allow the use of Unicode characters in console. This does mean that all `stdout` chars and strings must be wide values (`wChar` and `wString`). It is assigned to `result` just to capture the return from function, this is not used in this function and is to avoid compilation error. It could be used to provide debug output to show if setting mode was successsful.

`CreateFile` is used to populate the hconsole handle with the current console's I/O information. The file name `CONOUT$` is used to select the current console's active screen buffer for I/O. 

The other parameters in `CreateFile` are, information taken from [Microsoft Dev Center](https://docs.microsoft.com/en-gb/windows/win32/api/fileapi/nf-fileapi-createfilea?redirectedfrom=MSDN).

```cpp
CreateFile(FileName, DesiredAccess, ShareMode, SecurityAtt,
	CreationDispostion, FlagsAndAttributes, TemplateFile);
```

So `hconsole` is set to console's active screen buffer, with read & write permissions (as standard in docs), with file share read write (again default from docs), 0 dWord security attribute (`0L` is used to ensure long int used to match dWord length), only opens if the file exists (default in doc, in this case if the console is not open then the screen buffer wouldn't exist), file is set to normal as it has no attributes, No templates used. 

This function also sets the console cursor visibility to `false` to prevent it displaying with the below:

```cpp
	// Get Cursor Info
	GetConsoleCursorInfo(hconsole, &cursor_info);

	cursor_info.bVisible = false; // Set cursor visability to false

	// Set Cursor Info
	SetConsoleCursorInfo(hconsole, &cursor_info);
```

### DrawSlotMachine()

The `DrawSlotMachine` function draws the outline of the slotmachine to the console. With a top-left position of (**1,1**).

```cpp
SetConsoleCursorPosition(hconsole, {1,1});

	wcout << L"   \\|/ ┌────────────────────┐ \\|/  " << endl;
	wcout << L"   (┌─┐)│                    │(┌─┐)" << endl;
	wcout << L"  ╔═╧═╧═╧════════════════════╧═╧═╧═╗" << endl;
	wcout << L"  ║ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ║ " << endl;
	wcout << L"  ║  ╔════════╤════════╤════════╗  ║" << endl;
	wcout << L"  ║  ║        │        │        ║  ║" << endl;
	wcout << L"  ║  ╟        ┼        ┼        ╢  ║" << endl;
	wcout << L"  ║  ║        │        │        ║  ║" << endl;
	wcout << L"  ║  ╚════════╧════════╧════════╝  ║" << endl;
	wcout << L"  ║ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ║" << endl;
	wcout << L"  ╚═╤═╤═╤════════════════════╤═╤═╤═╝" << endl;
	wcout << L"  ╔═╧═╧═╧════════════════════╧═╧═╧═╗" << endl;
	wcout << L"  ║┌──────────────────────────────┐║" << endl;
	wcout << L"  ║│                              │║" << endl;
	wcout << L"  ║│                              │║" << endl;
	wcout << L"  ║│                              │║" << endl;
	wcout << L"  ║└──────────────────────────────┘║" << endl;
	wcout << L"  ╚═╤═╤═╤════════════════════╤═╤═╤═╝" << endl;
	wcout << L"    └─┴─┘                    └─┴─┘  " << endl;
```

### DrawStatusBox()

The `DrawStatusBox` function draws the outline of the status box below the slotmachine. This status box displays players score (data) and visibility. It has a top-left position of (**0,20**).

```cpp
	SetConsoleCursorPosition(hconsole, { 0,20 });

	wcout << L" ╔════════╤═════════════════════════╗" << endl;
	wcout << L" ║ STATUS │ Visibility [          ] ║" << endl;
	wcout << L" ╟────────┘ Data: 0 GB              ║" << endl;
	wcout << L" ╚══════════════════════════════════╝" << endl;
```

### DrawTargetBox()

The `DrawTargetBox` function draws target box. This box is used to display level name and target description. It has a top-left position of (**40,0**). **TODO: This is still just a place holder and needs mechanics added to game.**

```cpp
void DrawTargetBox()
{
	wstring targetBox[7] = {
		L"╔════════╤═════════════════════════╗",
		L"║ TARGET │ LEVEL 1: Neighbour's    ║",
		L"╟────────┴─────────────────────────╢",
		L"║ In the post Brexit era any data  ║",
		L"║ can be worth something to the    ║",
		L"║ government.                      ║",
		L"╚══════════════════════════════════╝"
	};

	for (int i = 0; i < 7; i++)
	{
		short pos = 0 + i;

		SetConsoleCursorPosition(hconsole, { 40, pos});

		wcout << targetBox[i];
	}
}
```

### DrawVunBox()

The `DrawVunBox` draws the box that will be used to display the randomly generated vulnerability values for each spin. It has a top-left position of (**40,7**).

```cpp
void DrawVunBox()
{
	wstring vunBox[7] = {
	L"╔═══════════════╤══════════════════╗",
	L"║ VULNERABILITY │                  ║",
	L"╟───────────────┘                  ║",
	L"║  ┌──────┐   ┌──────┐   ┌──────┐  ║",
	L"║  │ ---- │   │ ---- │   │ ---- │  ║",
	L"║  └──────┘   └──────┘   └──────┘  ║",
	L"╚══════════════════════════════════╝"
	};

	for (int i = 0; i < 7; i++)
	{
		short pos = 7 + i;

		SetConsoleCursorPosition(hconsole, { 40, pos });

		wcout << vunBox[i];
	}
}
```

### DrawReel1Key()
### DrawReel2Key()
### DrawReel3Key()

**TODO: Can these be combined?**

### DrawDebugInfo()

The `DrawDebugInfo` function draws the mock debug info box that is used to display game instructions, comments, and victory state. It has a top-left position of (**40,14**).

```cpp
void DrawDebugInfo()
{
	wstring debugInfoBox[10] = {
	L"╔════════════╤═════════════════════╗",
	L"║ DEBUG INFO │                     ║",
	L"╟────────────┘                     ║",
	L"║                                  ║",
	L"║                                  ║",
	L"║                                  ║",
	L"║                                  ║",
	L"║                                  ║",
	L"║                                  ║",
	L"╚══════════════════════════════════╝"
	};

	for (int i = 0; i < 10; i++)
	{
		short pos = 14 + i;

		SetConsoleCursorPosition(hconsole, { 40, pos });

		wcout << debugInfoBox[i];
	}
}
```

Game Print Functions
--------------------

### PrintReel(int reelNumber, wstring reelValue)

The `PrintReel` function prints the `reelValue` string to the reel identified by `reelNumber`. For this game it can accept `reelNumber` 0, 1 and 2. It will not accept a value out of this range as there are only 3 reels on the slot machine.

```cpp
void PrintReel(int reelNumber, wstring reelValue)
{
	if (reelNumber > 0 && reelNumber <= 3)
	{
		if (reelNumber == 1)
		{
			SetConsoleCursorPosition(hconsole, { 8,7 });

			wcout << reelValue;
		}
		else if (reelNumber == 2)
		{
			SetConsoleCursorPosition(hconsole, { 17,7 });

			wcout << reelValue;
		}
		else if (reelNumber == 3)
		{
			SetConsoleCursorPosition(hconsole, { 26,7 });

			wcout << reelValue;
		}
	}
	else 
	{
		OutputDebugString("DEBUG: reelNumberValue out of range. It must be between 1-3.");
	}
}
```

### PrintVunReel()

THe `PrintVunReel` function prints the value of the `REEL_VALUES` at index `pos` to the box for `reel`. It then saves the index of pos to `reelPosStore` so it can be used to work out if the reels match. 

```cpp
void PrintVunReel(int reel, int pos, int& reelPosStore)
{
	switch (reel)
	{
	case 1:
		SetConsoleCursorPosition(hconsole, { 45 , 11 });
		break;

	case 2: 
		SetConsoleCursorPosition(hconsole, { 56 , 11 });
		break;

	case 3:
		SetConsoleCursorPosition(hconsole, { 67 , 11 });
		break;

	default:
		OutputDebugString("DEBUG: VunReel number out of range.");
		break;
	}

	// Print
	wcout << REEL_VALUES[pos];

	// Store
	reelPosStore = pos;
}
```

### PrintDebugInfoMessage(wString messageLine1, wString messageLine2)

The `PrintDebugInfoMessage` prints the game's fake debug info to the Debug Info Box to display instructions and information on the gameplay. It take in two strings and displays them on two separate lines. 

```cpp
void PrintDebugInfoMessage(wstring messageLine1, wstring messageLine2)
{
	SetConsoleCursorPosition(hconsole, { 43, 17 });
	wcout << "~$ " << messageLine1;

	SetConsoleCursorPosition(hconsole, { 43, 18 });
	wcout << "~$ " << messageLine2;
}
```

### ClearDebugInfoMessage()

The `ClearDebugInfoMessage` function clears the two Debug Info Box lines by writing spaces over the who line in the box. 

```cpp
void ClearDebugInfoMessage()
{
	PrintDebugInfoMessage(L"              ", L"              ");
}
```

### PrintData(int dataScore)

The `PrintData` function displays the `dataScore` value in the Status Box and adds the data **GB** unit. 

```cpp
void PrintData(int dataScore) 
{
	SetConsoleCursorPosition(hconsole, { 18,22 });
	wcout << dataScore << "         ";

	SetConsoleCursorPosition(hconsole, { 18,22 });
	wcout << dataScore << " GB";
}
```

### PrintVisibility(int visibilityScore)

The `PrintVisibility` takes in the `visibilityScore` and displays it as a bar in the Status Box. This bar is **RED** by default.

```cpp
void PrintVisibility(int visibilityScore)
{
	SetConsoleCursorPosition(hconsole, { 24,21 });
	wcout << L"          ";

	SetConsoleCursorPosition(hconsole, { 24,21 });
	SetConsoleTextAttribute(hconsole, 12); // Set bar Red

	for (int i = 0; i < visibilityScore; i++)
	{
		wcout << L"■";
	}

	SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default
}
```

Game Animation Functions
------------------------

### ToogleSlotMachineLights(bool& status)

```cpp
void ToggleSlotMachineLights(bool &status)
{

	if (status)
	{
		SetConsoleTextAttribute(hconsole, 12); // Set Lights Red

		// Light 1 top
		SetConsoleCursorPosition(hconsole, { 4, 1 });
		wcout << "\\|/";

		// Light 1 Left
		SetConsoleCursorPosition(hconsole, { 3,2 });
		wcout << "(";

		// Light 1 Right
		SetConsoleCursorPosition(hconsole, { 7,2 });
		wcout << ")";

		// Light 2 top
		SetConsoleCursorPosition(hconsole, { 31, 1 });
		wcout << "\\|/";

		// Light 2 Left
		SetConsoleCursorPosition(hconsole, { 30,2 });
		wcout << "(";

		// Light 2 Right
		SetConsoleCursorPosition(hconsole, { 34,2 });
		wcout << ")";

		SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default

		status = false;
	}
	else
	{
		// Light 1 top
		SetConsoleCursorPosition(hconsole, { 4, 1 });
		wcout << "   ";

		// Light 1 Left
		SetConsoleCursorPosition(hconsole, { 3,2 });
		wcout << " ";

		// Light 1 Right
		SetConsoleCursorPosition(hconsole, { 7,2 });
		wcout << " ";

		// Light 2 top
		SetConsoleCursorPosition(hconsole, { 31, 1 });
		wcout << "   ";

		// Light 2 Left
		SetConsoleCursorPosition(hconsole, { 30,2 });
		wcout << " ";

		// Light 2 Right
		SetConsoleCursorPosition(hconsole, { 34,2 });
		wcout << " ";

		status = true;
	}
}
```

The `ToggleSlotMachineLights` toggles the slot machine's lights animation depending on if the status is `true` or `false`. It then toggles `status` to swap animation each time it is called. Below shows the lighs in on and off states.

![Lights On](readmeImages/LightsActive.PNG)

![Lights Off](readmeImages/LighsInactive.PNG)

### ToggleMachineName(bool& status)

```cpp
if (status)
	{	
		SetConsoleCursorPosition(hconsole, { 13,2 });

		SetConsoleTextAttribute(hconsole, 11); // Set Name Light Cyan

		wcout << "HACK MACHINE";

		SetConsoleTextAttribute(hconsole, 10); // Set Name Light Green

		SetConsoleCursorPosition(hconsole, { 10,2 });

		wcout << "**";

		SetConsoleCursorPosition(hconsole, { 26,2 });

		wcout << "**";

		SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default

		status = false;
	}
	else
	{
		SetConsoleCursorPosition(hconsole, { 13,2 });

		SetConsoleTextAttribute(hconsole, 10); // Set Name Light Green

		wcout << "------------";

		SetConsoleTextAttribute(hconsole, 11); // Set Name Light Cyan

		SetConsoleCursorPosition(hconsole, { 10,2 });

		wcout << "**";

		SetConsoleCursorPosition(hconsole, { 26,2 });

		wcout << "**";

		SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default

		status = true;
	}	
}
```

The `ToggleMachineName` function displays the slot machine's name ans toggles if it is showing or not. In a similar way to the `ToggleSlotMachineLights`. Below images show the machine's name in both states. 

![Name Active](readmeImages/MachineNameActive.PNG)

![Inactive Name](readmeImages/MachineNameInactive.PNG)

Game Mechanics Functions
------------------------

### VictoryState(int reel1, int reel2, int reel3, int vun1, int vun2, int,vun3)

The `VictoryState` takes in the 3 reel positions and the 3 randomly generated vulnarability reel positions and check if they meet one of the victory conditions. It returns an **int** to represent this `victoryState`. 

There are four victory states:

0. No Win, none of the three reels match
1. Vulnerability Win, the reels match vun values, all 3 in the correct order.
2. 2 Reel Win, two reel match each other.
3. 3 Reel Win, three reels match each other. 

```cpp
int VictoryState(int reel1, int reel2, int reel3, int vun1, int vun2, int vun3)
{
	int victoryState = 0; // 0 = no win, 1 = vulnarability, 2 = 2 reels, 3 = 3 reels

	if (reel1 == vun1 && reel2 == vun2 && reel3 == vun3)
	{
		victoryState = 1;
	}
	// 3 Reel Victory
	else if (reel1 == reel2 && reel1 == reel3)
	{
		victoryState = 3;
	}

	// 2 Reel Victory
	else if (reel1 == reel2 || reel2 == reel3 || reel1 == reel3)
	{
		victoryState = 2;
	}

	return victoryState;
}
```

### IncreaseVisibility(int& vis)

The `IncreaseVisibility` function increases the value of `vis` **1** until a value of **10** it will not increase the value higher then **10** wich is the maximum visibility value that can be displayed in Status Box visibility bar.

```cpp
void IncreaseVisibility(int& vis) 
{
	if (vis <= 10) 
	{
		vis ++;
	}
}
```

### DecreaseVisibility(int& vis)

The `DecreaseVisibility` function will decrease the value of `vis` by **1** but it will not decrease the value below **0**.

```cpp
void DecreaseVisibility(int& vis)
{
	if (vis > 0)
	{
		vis--;
	}
}
```

### ResetVisibility(int& vis)

The `ResetVisibility` function resets the value of the visibility back to its start of game value, by default this is **0**.

```cpp
void ResetVisibility(int& vis)
{
	vis = 0;
}
```

### IncreaseData(int& data, int ammount)

The `IncreaseData` increases the `data` variable by `ammount`.

```cpp
void IncreaseData(int& data, int ammount)
{
	data += ammount;
}
```

### DecreaseData(int& data, int ammount)

The `DecreaeData` decreases the `data` variable by `ammount`. By will not reduce the score below **0**.

```cpp
void DecreaseData(int& data, int ammount)
{
	if ((data - ammount) >= 0)
	{
		data -= ammount;
	}
}
```

### ResetData(int& data)

The `ResetData` function resets the data to its start value.

```cpp
void ResetData(int& data)
{
	data = 0;
}
```

### IncreaseSpinSpeed(int& spinSpeed, int ammount)

The `IncreaseSpinSpeed` function increases the `spinSpeed` value by `ammount`, as the spin speed is the delay between printing each value in the reel this delay is reduced to increase the spin speed of reels. This value will be reduced to a miniumum value of **0**ms. 

```cpp
void IncreaseSpinSpeed(int& spinSpeed, int ammount)
{
	if ((spinSpeed - ammount) >= 0)
	{
		spinSpeed -= ammount;
	}
}
```

### DecreaseSpinSpeed(int& spinSpeed, int ammount)

The `DecreaseSpinSpeed` function deacreases the `spinSpeed` by `ammount` ms, to decrease the spin speed the delay is increased.

```cpp
void DecreaseSpinSpeed(int& spinSpeed, int ammount)
{
	spinSpeed += ammount;
}
```

### ResetSpinSpeed(int& spinSpeed)

The `ResetSpinSpeed` function resets the `spinSpeed` to its starting value `START_SPIN_SPEED` which by defualt is **500**ms.

```cpp
void ResetSpinSpeed(int& spinSpeed)
{
	spinSpeed = START_SPIN_SPEED;
}
```

### RandomReelPosition(int reelLength)

The `RandomReelPosition` function returns a randomly generated integer `randomPos` between **0** and `reelLength`. This function is used to randomly select vulnerability vaules.

```cpp
int RandomReelPosition(int reelLength) 
{
	int randomPos = 0;

	randomPos = rand() % reelLength;

	return randomPos;
}
```

Main Function
-------------

```cpp
int main()
{
	// Animation Variables
	bool lightStatus = false;
	bool nameStatus = true;
	int lightSpeed = 1000;
	int spinSpeed = START_SPIN_SPEED;

	// Game Variables
	int gameActive = true;
	int gameState = 0;
	int reel1StopPos = 0;
	int reel2StopPos = 0;
	int reel3StopPos = 0;
	int vunReel1 = 0;
	int vunReel2 = 0;
	int vunReel3 = 0;
	int data = 0;
	int visibility = 0;
	   
	// Game Setup
	srand(time(0));
	GraphicsSetup();
	DrawSlotMachine();
	DrawStatusBox();
	DrawTargetBox();
	DrawVunBox();
	DrawDebugInfo();
	PrintData(data);
	PrintVisibility(visibility);
	DrawReel1Key(false);
	DrawReel2Key(false);
	DrawReel3Key(false);
	

	while (gameActive)
	{
		// Idle State Loop
		while (gameState == gameStates::IDLE)
		{
			ToggleMachineName(nameStatus);

			ToggleSlotMachineLights(lightStatus);

			if (_kbhit())
			{
				int key = _getch();

				if (key == PLAY_KEY) 
				{
					gameState = gameStates::SPIN_REELS_123;
				}

				if (key == QUIT_KEY)
				{
					gameActive = false;
					gameState = gameStates::QUIT;
				}
			}

			Sleep(lightSpeed);
		}

		// Spin State Loop
		while (gameState > gameStates::IDLE && gameState <= gameStates::SPIN_REEL_3)
		{
			bool keepSpinning = true;

			ClearDebugInfoMessage();
			PrintDebugInfoMessage(L"Spinning ...");

			// Vun
			PrintVunReel(1, RandomReelPosition(REEL_LENGTH), vunReel1);
			PrintVunReel(2, RandomReelPosition(REEL_LENGTH), vunReel2);
			PrintVunReel(3, RandomReelPosition(REEL_LENGTH), vunReel3);

			while (keepSpinning)
			{
				for (int i = 0; i < REEL_LENGTH; i++)
				{
					if (gameState == gameStates::SPIN_REELS_123)
					{
						PrintReel(1, REEL_VALUES[i]);
						DrawReel1Key(true);
					}

					if (gameState == gameStates::SPIN_REELS_123 || gameState == gameStates::SPIN_REELS_23)
					{
						PrintReel(2, REEL_VALUES[i]);

						if(gameState == gameStates::SPIN_REELS_23)
						{
							DrawReel1Key(false);
							DrawReel2Key(true);
							DrawReel3Key(false);
						}
					}

					if (gameState > gameStates::IDLE && gameState <= gameStates::SPIN_REEL_3)
					{
						PrintReel(3, REEL_VALUES[i]);

						if (gameState == gameStates::SPIN_REEL_3)
						{
							DrawReel1Key(false);
							DrawReel2Key(false);
							DrawReel3Key(true);
						}
					}

					if (_kbhit())
					{
						int key = towupper(_getch());

						if (key == REEL1_KEY && gameState == gameStates::SPIN_REELS_123)
						{
							reel1StopPos = i;
							gameState = gameStates::SPIN_REELS_23;
							break;
						}

						if (key == REEL2_KEY && gameState == gameStates::SPIN_REELS_23)
						{
							reel2StopPos = i;
							gameState = gameStates::SPIN_REEL_3;
							break;
						}

						if (key == REEL3_KEY && gameState == gameStates::SPIN_REEL_3)
						{
							keepSpinning = false;
							reel3StopPos = i;
							gameState = gameStates::ALL_REELS_STOPPED;
							DrawReel3Key(false);
							break;
						}
					}

					Sleep(spinSpeed);
				}
			}

			IncreaseSpinSpeed(spinSpeed, DIFFICULTY);

			switch (VictoryState(reel1StopPos, reel2StopPos, reel3StopPos, vunReel1, vunReel2, vunReel3))
			{
				case 1:
					ClearDebugInfoMessage();
					PrintDebugInfoMessage(L"You win!", L"Vun Exploited");
					IncreaseData(data, DATA_PRIZE_VUN);
					ResetVisibility(visibility);
					ResetSpinSpeed(spinSpeed);
					break;

				case 2:
					ClearDebugInfoMessage();
					PrintDebugInfoMessage(L"You win!", L"2 Commands");
					IncreaseData(data, DATA_PRIZE_2);
					DecreaseVisibility(visibility);
					DecreaseSpinSpeed(spinSpeed, SPIN_SPEED_PRIZE_2);
					break;

				case 3:
					ClearDebugInfoMessage();
					PrintDebugInfoMessage(L"You win!", L"3 Commands");
					IncreaseData(data, DATA_PRIZE_3);
					ResetVisibility(visibility);
					DecreaseSpinSpeed(spinSpeed, SPIN_SPEED_PRIZE_3);
					break;

				default:
					ClearDebugInfoMessage();
					PrintDebugInfoMessage(L"Failed!", L"Invalid Cmd");
					IncreaseVisibility(visibility);
					break;
			}

			// Update Scores
			if (gameState != gameStates::IDLE && gameState != gameStates::GAME_OVER)
			{
				PrintData(data);

				if (visibility <= 10)
				{
					PrintVisibility(visibility);
					gameState = gameStates::IDLE;
				}
				else
				{
					gameState = gameStates::GAME_OVER;
				}
			}

			// Game Over State
			while (gameState == gameStates::GAME_OVER)
			{
				ClearDebugInfoMessage();
				PrintDebugInfoMessage(L"BUSTED!", L"Run Fast!");

				ToggleSlotMachineLights(lightStatus);

				if (_kbhit())
				{
					int key = towupper(_getch());

					// Reset Game
					if (key == RESET_KEY)
					{
						gameState = gameStates::IDLE;						
						ResetData(data);
						ResetVisibility(visibility);
						ResetSpinSpeed(spinSpeed);
						PrintData(data);
						PrintVisibility(visibility);
						break;
					}
				}

				Sleep(lightSpeed);
			}
		}
	}

	return 0;
}
```


