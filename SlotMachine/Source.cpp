/************************************************
Slot machine made for CMP104 assessment. 

Aurthor: Joseph Lee
Sudent # 1903399

*************************************************/

// TODO: Add a little more back story

// Includes
// ========

#include <iostream> // For Console i/o
#include <fcntl.h> // To set 16-bit character encoding
#include <io.h> // Used to get _setmode()
#include <stdio.h> // Used with above.
#include <conio.h> // For reading key presses
#include <Windows.h> // To get access to console screen buffer etc.
#include <cstdlib> // Used for Random Number Generation
#include <ctime> // Date and time info (Used as random Number Seed)

using namespace std;

// Function Prototypes
// ===================

// TODO: Neaten up the order of these to match the functions section

void GraphicsSetup();
void IntroScreen();
void DrawSlotMachine();
void DrawStatusBox();
void DrawTargetBox();
void DrawVunBox();
void DrawDebugInfo();
void ToggleSlotMachineLights(bool&);
void ToggleMachineName(bool&);
void PrintReel(int, int);
int PreviousReelValue(int);
int NextReelValue(int);
void PrintDebugInfoMessage(wstring, wstring, wstring);
void ClearDebugInfoMessage();
int VictoryState(int, int, int, int, int, int);
void PrintData(int);
void PrintVisibility(int);
void IncreaseVisibility(int&);
void DecreaseVisibility(int&);
void ResetVisibility(int&);
void IncreaseData(int&, int);
void DecreaseData(int&, int);
void ResetData(int&);
void IncreaseSpinSpeed(int&, int);
void DecreaseSpinSpeed(int&, int);
void ResetSpinSpeed(int&);
void DrawReelKey(int, bool);
void DrawReel1Key(bool);
void DrawReel2Key(bool);
void DrawReel3Key(bool);
int RandomReelPosition(int);
void PrintVunReel(int, int, int&);
void PrintLevelInfo(int);
int SelectLevel(int dataScore);
void SetDifficulty(int&, int);
void PrintMachineBanner(wstring, bool);
void QuitScreen(int);

// Globals
// =======

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
const int START_SPIN_SPEED = 350;
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


// Main Game
// =========

int main()
{
	// Animation Variables
	bool lightStatus = false;
	bool nameStatus = true;
	int lightSpeed = 1000;
	int spinSpeed = START_SPIN_SPEED;

	// Game Variables
	int gameActive = false; // Remain in game loop
	int gameState = 0; // Store game state
	int reel1StopPos = 0; // The position reel 1 has been stopped in
	int reel2StopPos = 0; // The position reel 2 has been stopped in
	int reel3StopPos = 0; // The position reel 3 has been stopped in
	int vunReel1 = 0; // Reel 1 vulnerability position
	int vunReel2 = 0; // Reel 2 vulnerability position
	int vunReel3 = 0; // Reel 3 vulnerability position
	int data = 0; // Data score
	int visibility = 0; // Visibility Level
	int level = 1; // Current level
	int difficulty = 50; // Ammount Reel Spin Speed Incrases in ms.
	


	// Game Setup
	srand(time(0)); // Use time as random number seed
	GraphicsSetup();

	// Intro
	IntroScreen();

	// Wait for user to press space to play game
	while (!gameActive)
	{
		if (_kbhit())
		{
			int key = _getch();

			if (key == PLAY_KEY)
			{
				gameActive = true;
			}
		}
	}

	system("CLS"); // Clear console screen


	// Game
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
	PrintDebugInfoMessage(L"Welcome to HACK MACHINE", L"Press SPACE to spin reels", L"Press Esc to quit");
	PrintLevelInfo(level);
	PrintReel(1, 0); // Set Reel 1 Initial Position
	PrintReel(2, 0); // Set Reel 2 Initial Position
	PrintReel(3, 0); // Set Reel 3 Initial Position


	// Main Game Loop
	while (gameActive)
	{
		// Idle State Loop
		while (gameState == gameStates::IDLE)
		{
			ToggleMachineName(nameStatus);

			ToggleSlotMachineLights(lightStatus);
			
			// Check for user input
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

			// Vun
			PrintVunReel(1, RandomReelPosition(REEL_LENGTH), vunReel1);
			PrintVunReel(2, RandomReelPosition(REEL_LENGTH), vunReel2);
			PrintVunReel(3, RandomReelPosition(REEL_LENGTH), vunReel3);

			// Instructions
			ClearDebugInfoMessage();
			PrintDebugInfoMessage(L"HACKING!", L"Press Z, X, C to stop reels", L" ");

			// Banner
			PrintMachineBanner(L"   HACKING  ", false);

			// Spinning Animation
			while (keepSpinning)
			{
				for (int i = 0; i < REEL_LENGTH; i++)
				{
					// Reel 1
					if (gameState == gameStates::SPIN_REELS_123)
					{
						PrintReel(1, i);
						DrawReelKey(1, true);
					}

					// Reel 2
					if (gameState == gameStates::SPIN_REELS_123 || gameState == gameStates::SPIN_REELS_23)
					{
						PrintReel(2, i);

						if(gameState == gameStates::SPIN_REELS_23)
						{
							DrawReelKey(1, false);
							DrawReelKey(2, true);
							DrawReelKey(3, false);
						}
					}

					// Reel 3
					if (gameState > gameStates::IDLE && gameState <= gameStates::SPIN_REEL_3)
					{
						PrintReel(3, i);

						if (gameState == gameStates::SPIN_REEL_3)
						{
							DrawReelKey(1, false);
							DrawReelKey(2, false);
							DrawReelKey(3, true);
						}
					}

					// Check for user input
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
							DrawReelKey(3, false);
							break;
						}
					}

					Sleep(spinSpeed);
				}
			}

			IncreaseSpinSpeed(spinSpeed, difficulty); // Increase after each spin attempt

			// Check Visctory State
			switch (VictoryState(reel1StopPos, reel2StopPos, reel3StopPos, vunReel1, vunReel2, vunReel3))
			{
				// Vulnerability Exploited
				case 1:
					ClearDebugInfoMessage();
					PrintDebugInfoMessage(L"ACCESS GRANTED!", L"Vulnerability Exploited!", L"12GB of data downloaded.");
					IncreaseData(data, DATA_PRIZE_VUN);
					ResetVisibility(visibility);
					ResetSpinSpeed(spinSpeed);
					break;

				// 2 Reel Combo
				case 2:
					ClearDebugInfoMessage();
					PrintDebugInfoMessage(L"ACCESS GRANTED!", L"Two Command Combo", L"3GB of data downloaded.");
					IncreaseData(data, DATA_PRIZE_2);
					DecreaseSpinSpeed(spinSpeed, SPIN_SPEED_PRIZE_2);
					break;

				// 3 Reel Combo
				case 3:
					ClearDebugInfoMessage();
					PrintDebugInfoMessage(L"ACCESS GRANTED!", L"Three Command Combo", L"9GB of data downloaded.");
					IncreaseData(data, DATA_PRIZE_3);
					ResetVisibility(visibility);
					DecreaseSpinSpeed(spinSpeed, SPIN_SPEED_PRIZE_3);
					break;

				// No win condition
				default:
					ClearDebugInfoMessage();
					PrintDebugInfoMessage(L"ACCESS DENIED!", L"Invalid Command Entered", L"Visibility Increased...");
					IncreaseVisibility(visibility);
					break;
			}

			// Update Scores
			if (gameState != gameStates::IDLE && gameState != gameStates::GAME_OVER)
			{
				PrintData(data);

				// Check if Game is Over
				if (visibility <= 10)
				{
					PrintVisibility(visibility);
					gameState = gameStates::IDLE;

					// Check if level increases
					level = SelectLevel(data);

					// Update Level
					PrintLevelInfo(level);

					// Set difficulty using level
					SetDifficulty(difficulty, level);
				}
				else
				{
					// Game Over
					gameState = gameStates::GAME_OVER;
					ClearDebugInfoMessage();
					PrintDebugInfoMessage(L"GAME OVER!", L"Press R to replay game", L"Press Esc to quit");
					PrintMachineBanner(L" GAME OVER  ", true);
				}
			}

			// Game Over State
			while (gameState == gameStates::GAME_OVER)
			{
				ToggleSlotMachineLights(lightStatus);

				// Check for user input
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
						level = 1;
						PrintLevelInfo(level);
						PrintDebugInfoMessage(L"Welcome to HACK MACHINE", L"Press SPACE to spin reels", L"Press Esc to quit");
						break;
					}

					// Quit Game
					if (key == QUIT_KEY)
					{
						gameActive = false;
						gameState = gameStates::QUIT;
					}
				}

				Sleep(lightSpeed);
			}
		}
	}
	
	// Display Quit Screen
	system("CLS"); // Clear Console 
	QuitScreen(data);

	return 0; // Return zero for int main()
}

// Functions
// =========

// TODO: Neaten up the order of these

// Set up console graphics engine for the game
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

// Print Intro Screen
void IntroScreen()
{
	wstring gameTitle[7] = {
		L" #     #    #     #####  #    #          #     #    #     #####  #     # ### #     # #######",
		L" #     #   # #   #     # #   #           ##   ##   # #   #     # #     #  #  ##    # #      ",
		L" #     #  #   #  #       #  #            # # # #  #   #  #       #     #  #  # #   # #      ",
		L" ####### #     # #       ###             #  #  # #     # #       #######  #  #  #  # #####  ",
		L" #     # ####### #       #  #            #     # ####### #       #     #  #  #   # # #      ",
		L" #     # #     # #     # #   #           #     # #     # #     # #     #  #  #    ## #      ",
		L" #     # #     #  #####  #    #          #     # #     #  #####  #     # ### #     # #######",
	};

	// Print Title
	SetConsoleTextAttribute(hconsole, 10); // Set Name Light Green

	for(int i = 0; i < 7; i++)
	{
		short pos = 1 + i;

		SetConsoleCursorPosition(hconsole, { 10, pos});
		wcout << gameTitle[i];
	}

	SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default

	wcout << endl;
	wcout << endl;
	wcout << L"  Welcome to HACK MACHINE. It may look like a simple fruit machine but behind the cover is and advanced brute-force" << endl;
	wcout << L"  hacking device. Due to the current oppressive government policies on tech access and internet freedoms it has been" << endl;
	wcout << L"  put together with bit of old machinery found in the abandoned ruins of the city. However, with hard times comes " << endl;
	wcout << L"  opportunity. There is now a very lucrative market in trading old data, it has become the everyday currency of the" << endl;
	wcout << L"  age, and everyone is at it! So join in with your HACK MACHINE and make some cash in this post-Brexit wasteland!" << endl;
	wcout << endl;
	wcout << L"  To use the HACK MACHINE, use the SPACE to start the machine hacking, press Z, X and C to stop the reels. Each" << endl;
	wcout << L"  machine has a vulnerability that can be exploited for BIG rewards. The machine also has 2 command and 3 command" << endl;
	wcout << L"  which yield some data access, but no the full data dump gained from exploiting a vulnerability." << endl;
	wcout << endl;
	wcout << L"  However, care should be taken as the government are on the lookout for hackers and each failed attempt to hack a" << endl;
	wcout << L"  device makes you a little more visible. However, a nifty 3 command combo or vulnerability exploit can hide you again." << endl;
	wcout << L"  Each hack means the reels spin faster due to the machine's construction, only data downloads can slow it down. As the " << endl;
	wcout << L"  devices get harder to hack the amount this reel spin speed increases after each failed attempt increases. So, take" << endl;
	wcout << L"  your time and hack those machines first time." << endl;
	wcout << endl;
	wcout << endl;
	wcout << L"                                             PRESS SPACE TO PLAY ... " << endl;
}

// Print Quit Screen
void QuitScreen(int score)
{
	wcout << endl;
	wcout << endl;
	wcout << L"            YOU ESCAPED WITH " << score << "GBs OF DATA!" << endl;
	wcout << L"                    ______________" << endl;
	wcout << L"                   |[]            |" << endl;
	wcout << L"                   |  __________  |" << endl;
	wcout << L"                   |  |        |  |" << endl;
	wcout << L"                   |  |  DATA  |  |" << endl;
	wcout << L"                   |  |________|  |" << endl;
	wcout << L"                   |   ________   |" << endl;
	wcout << L"                   |   [ [ ]  ]   |" << endl;
	wcout << L"                   \\___[_[_]__]___|" << endl;
}

// Draws Slot Machine
void DrawSlotMachine()
{
	SetConsoleCursorPosition(hconsole, {1,1});

	wcout << L"   \\|/ ┌────────────────────┐ \\|/  " << endl;
	wcout << L"   (┌─┐)│                    │(┌─┐)" << endl;
	wcout << L"  ╔═╧═╧═╧════════════════════╧═╧═╧═╗" << endl;
	wcout << L"  ║ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ║ " << endl;
	wcout << L"  ║  ╔════════╤════════╤════════╗  ║" << endl;
	wcout << L"  ║  ║        │        │        ║  ║" << endl;
	wcout << L"  ║  ╟─      ─┼─      ─┼─      ─╢  ║" << endl;
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
}

// Draws Status Box
void DrawStatusBox()
{
	SetConsoleCursorPosition(hconsole, { 0,20 });

	wcout << L" ╔════════╤═════════════════════════╗" << endl;
	wcout << L" ║ STATUS │ Visibility [          ] ║" << endl;
	wcout << L" ╟────────┘ Data: 0 GB              ║" << endl;
	wcout << L" ╚══════════════════════════════════╝" << endl;
}

// Draw Target Box
void DrawTargetBox()
{
	wstring targetBox[7] = {
		L"╔════════╤═════════════════════════╗",
		L"║ TARGET │                         ║",
		L"╟────────┴─────────────────────────╢",
		L"║                                  ║",
		L"║                                  ║",
		L"║                                  ║",
		L"╚══════════════════════════════════╝"
	};

	for (int i = 0; i < 7; i++)
	{
		short pos = 0 + i;

		SetConsoleCursorPosition(hconsole, { 40, pos});

		wcout << targetBox[i];
	}
}

// Draw Vun Box
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

// Draws Reel Key
void DrawReelKey(int reel, bool active)
{
	switch (reel)
	{
		case 1:
			DrawReel1Key(active);
			break;
		case 2:
			DrawReel2Key(active);
			break;
		case 3:
			DrawReel3Key(active);
			break;
		default:
			OutputDebugString("DEBUG: Invalid reel number selected.");
			break;
	}
}

// Draw Reel Key 1
void DrawReel1Key(bool active) 
{
	wstring reel1Key[3] = {
	L"┌───┐",
	L"│ Z │",
	L"└───┘"
	};

	if (active) 
	{
		SetConsoleTextAttribute(hconsole, 10); // Set Name Light Green
	}
	else
	{
		SetConsoleTextAttribute(hconsole, 12); // Set Lights Red
	}
	   	  
	for (int i = 0; i < 3; i++)
	{
		short pos = 14 + i;
		SetConsoleCursorPosition(hconsole, { 7, pos });
		wcout << reel1Key[i];
	}

	SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default
}

// Draw Reel Key 2
void DrawReel2Key(bool active)
{
	wstring reel2Key[3] = {
	L"┌───┐",
	L"│ X │",
	L"└───┘"
	};

	if (active)
	{
		SetConsoleTextAttribute(hconsole, 10); // Set Name Light Green
	}
	else
	{
		SetConsoleTextAttribute(hconsole, 12); // Set Lights Red
	}

	for (int i = 0; i < 3; i++)
	{
		short pos = 14 + i;
		SetConsoleCursorPosition(hconsole, { 16, pos });
		wcout << reel2Key[i];
	}

	SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default
}

// Draw Reel Key 3
void DrawReel3Key(bool active)
{
	wstring reel3Key[3] = {
	L"┌───┐",
	L"│ C │",
	L"└───┘"
	};

	if (active)
	{
		SetConsoleTextAttribute(hconsole, 10); // Set Name Light Green
	}
	else
	{
		SetConsoleTextAttribute(hconsole, 12); // Set Lights Red
	}

	for (int i = 0; i < 3; i++)
	{
		short pos = 14 + i;
		SetConsoleCursorPosition(hconsole, { 25, pos });
		wcout << reel3Key[i];
	}

	SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default
}

// Draw Debug Info Box
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

// Toggle Slot Machine Lights
void ToggleSlotMachineLights(bool& rStatus)
{

	if (rStatus)
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

		rStatus = false;
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

		rStatus = true;
	}
}

// Cycle through colors for machine name
void ToggleMachineName(bool& rStatus)
{
	if (rStatus)
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

		rStatus = false;
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

		rStatus = true;
	}

	
}

// Print Machine Banner Message
void PrintMachineBanner(wstring message, bool isWarning)
{
	if (message.length() <= 12) 
	{
		if (isWarning)
		{
			SetConsoleTextAttribute(hconsole, 12); // Set Lights Red
		}

		else
		{
			SetConsoleTextAttribute(hconsole, 10); // Set Name Light Green
		}

		SetConsoleCursorPosition(hconsole, { 13,2 });

		wcout << message;

		SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default
	}

	else
	{
		OutputDebugString("DEBUG: MachineBanner message too long");
	}
}

// Print a reel
void PrintReel(int reelNumber, int pos)
{
	int previousReel = PreviousReelValue(pos);
	int nextReel = NextReelValue(pos);

	if (reelNumber > 0 && reelNumber <= 3)
	{
		if (reelNumber == 1)
		{
			SetConsoleCursorPosition(hconsole, { 8,6 });
			wcout << REEL_VALUES[previousReel];
			SetConsoleCursorPosition(hconsole, { 8,7 });
			wcout << REEL_VALUES[pos];
			SetConsoleCursorPosition(hconsole, { 8,8 });
			wcout << REEL_VALUES[nextReel];
		}

		else if (reelNumber == 2)
		{
			SetConsoleCursorPosition(hconsole, { 17,6 });
			wcout << REEL_VALUES[previousReel];
			SetConsoleCursorPosition(hconsole, { 17,7 });
			wcout << REEL_VALUES[pos];
			SetConsoleCursorPosition(hconsole, { 17,8 });
			wcout << REEL_VALUES[nextReel];
		}
		else if (reelNumber == 3)
		{
			SetConsoleCursorPosition(hconsole, { 26,6 });
			wcout << REEL_VALUES[previousReel];
			SetConsoleCursorPosition(hconsole, { 26,7 });
			wcout << REEL_VALUES[pos];
			SetConsoleCursorPosition(hconsole, { 26,8 });
			wcout << REEL_VALUES[nextReel];
		}
	}
	else 
	{
		OutputDebugString("DEBUG: reelNumberValue out of range. It must be between 1-3.");
	}
}

// Returns the previous reel value
int PreviousReelValue(int currentPos)
{
	if ((currentPos - 1) >= 0)
	{
		return (currentPos - 1);
	}

	else
	{
		return (REEL_LENGTH - 1);
	}
}

// Returns the next reel value
int NextReelValue(int currentPos)
{
	if ((currentPos + 1) < REEL_LENGTH)
	{
		return (currentPos + 1);
	}

	else
	{
		return 0;
	}
}

// Print Debug Info Message
void PrintDebugInfoMessage(wstring messageLine1, wstring messageLine2, wstring messageLine3)
{
	SetConsoleCursorPosition(hconsole, { 43, 17 });
	wcout << "~$ " << messageLine1;

	SetConsoleCursorPosition(hconsole, { 43, 18 });
	wcout << "~$ " << messageLine2;

	SetConsoleCursorPosition(hconsole, { 43, 19 });
	wcout << "~$ " << messageLine3;
}

// Print Vun Reel
void PrintVunReel(int reel, int pos, int& rReelPosStore)
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
	rReelPosStore = pos;
}

// Print Level Info
void PrintLevelInfo(int levelValue)
{
	if (levelValue > 0 && levelValue <= 6)
	{
		wstring levelNames[6] = {
			L"Level 1: Neighbour's",
			L"Level 2: Small Office",
			L"Level 3: Big Office",
			L"Level 4: Small Website",
			L"Level 5: Large Website",
			L"Level 6: Government"
		};

		wstring levelDescriptions[6] = {
			L"Stating things out easy.",
			L"Upping the stakes?",
			L"Now the real game begins!",
			L"You're no script kiddie now!",
			L"You're aiming big now!",
			L"AHHH! You're crazy!"
		};


		// Print Level Name
		SetConsoleCursorPosition(hconsole, { 51 , 1 });
		wcout << L"                        ";

		SetConsoleCursorPosition(hconsole, { 51 , 1 });
		wcout << levelNames[(levelValue - 1)];

		// Print Level Description
		SetConsoleCursorPosition(hconsole, { 42 , 3 });
		wcout << L"                                ";

		SetConsoleCursorPosition(hconsole, { 42 , 3 });
		wcout << levelDescriptions[(levelValue -1)];
	}

	else 
	{
		OutputDebugString("DEBUG: Invalid Level Selected");
	}	
}

// Select Level
int SelectLevel(int dataScore) 
{
	if (dataScore > 70)
	{
		return 6;
	}

	else if (dataScore > 55)
	{
		return 5;
	}

	else if (dataScore > 40)
	{
		return 4;
	}

	else if (dataScore > 25)
	{
		return 3;
	}

	else if (dataScore > 10)
	{
		return 2;
	}

	else
	{
		return 1;
	}
}

// Set difficulty
void SetDifficulty(int& rDifficulty, int level)
{
	rDifficulty = 50 * level;
}

// Clear Debug Info Message
void ClearDebugInfoMessage()
{
	PrintDebugInfoMessage(L"                           ", L"                           ", L"                           ");
}

// Victory State
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

// Print Data
void PrintData(int dataScore) 
{
	SetConsoleCursorPosition(hconsole, { 18,22 });
	wcout << dataScore << "         ";

	SetConsoleCursorPosition(hconsole, { 18,22 });
	wcout << dataScore << " GB";
}

// Print Visibility
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

// Increase Visibility
void IncreaseVisibility(int& rVis) 
{
	if (rVis <= 10) 
	{
		rVis ++;
	}
}

// Decrease Visibility
void DecreaseVisibility(int& rVis)
{
	if (rVis > 0)
	{
		rVis--;
	}
}

// Reset Visibility
void ResetVisibility(int& rVis)
{
	rVis = 0;
}

// Increase Score
void IncreaseData(int& rData, int ammount)
{
	rData += ammount;
}

// Decrease Score
void DecreaseData(int& rData, int ammount)
{
	if ((rData - ammount) >= 0)
	{
		rData -= ammount;
	}
}

// Reset Score
void ResetData(int& rData)
{
	rData = 0;
}

// Increase Spin Speed
void IncreaseSpinSpeed(int& rSpinSpeed, int ammount)
{
	if ((rSpinSpeed - ammount) >= 0)
	{
		rSpinSpeed -= ammount;
	}
}

// Decrease Spin Speed
void DecreaseSpinSpeed(int& rSpinSpeed, int ammount)
{
	rSpinSpeed += ammount;
}

// Reset Spin Speed
void ResetSpinSpeed(int& rSpinSpeed)
{
	rSpinSpeed = START_SPIN_SPEED;
}

// RandomReelPos
int RandomReelPosition(int reelLength) 
{
	int randomPos = 0;

	randomPos = rand() % reelLength;

	return randomPos;
}
