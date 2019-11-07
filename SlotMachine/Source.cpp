﻿/************************************************
Slot machine made for CMP104 assessment. 

Aurthor: Joseph Lee
Sudent # 1903399

*************************************************/

// Includes
// ========

#include <iostream> // For Console i/o
#include <fcntl.h> // To set 16 encoding
#include <io.h> // Used to get _setmode()
#include <stdio.h> // Used with above.
#include <conio.h> // For reading key presses
#include <Windows.h> // To get access to console screen buffer etc.
#include <cstdlib> // Used for Random Number Generation
#include <ctime> // Date and time info (Used as random Number Seed)

using namespace std;

// Function Prototypes
// ===================

void GraphicsSetup();
void DrawSlotMachine();
void DrawStatusBox();
void DrawTargetBox();
void DrawVunBox();
void DrawDebugInfo();
void ToggleSlotMachineLights(bool&);
void ToggleMachineName(bool&);
void PrintReel(int, wstring);
void PrintDebugInfoMessage(wstring);
void PrintDebugInfoMessage(wstring, wstring);
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
void DrawReel1Key(bool);
void DrawReel2Key(bool);
void DrawReel3Key(bool);
int RandomReelPosition(int);
void PrintVunReel(int, int, int&);


void TestPrintResults(int, int, int, int, int);


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

// Functions
// =========

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
	wstring reel1Key[3] = {
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
		wcout << reel1Key[i];
	}

	SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default
}

// Draw Reel Key 3
void DrawReel3Key(bool active)
{
	wstring reel1Key[3] = {
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
		wcout << reel1Key[i];
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

// Cycle through colors for machine name
void ToggleMachineName(bool& status)
{
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

// Print a reel
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

// Print Debug Info Message
void PrintDebugInfoMessage(wstring message) 
{
	SetConsoleCursorPosition(hconsole, { 43, 17 });
	wcout << "~$ " << message;
}

void PrintDebugInfoMessage(wstring messageLine1, wstring messageLine2)
{
	SetConsoleCursorPosition(hconsole, { 43, 17 });
	wcout << "~$ " << messageLine1;

	SetConsoleCursorPosition(hconsole, { 43, 18 });
	wcout << "~$ " << messageLine2;
}

// Print Vun Reel
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

// Clear Debug Info Message
void ClearDebugInfoMessage()
{
	PrintDebugInfoMessage(L"              ", L"              ");
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
void IncreaseVisibility(int& vis) 
{
	if (vis <= 10) 
	{
		vis ++;
	}
}

// Decrease Visibility
void DecreaseVisibility(int& vis)
{
	if (vis > 0)
	{
		vis--;
	}
}

// Reset Visibility
void ResetVisibility(int& vis)
{
	vis = 0;
}

// Increase Score
void IncreaseData(int& data, int ammount)
{
	data += ammount;
}

// Decrease Score
void DecreaseData(int& data, int ammount)
{
	if ((data - ammount) >= 0)
	{
		data -= ammount;
	}
}

// Reset Score
void ResetData(int& data)
{
	data = 0;
}

// Increase Spin Speed
void IncreaseSpinSpeed(int& spinSpeed, int ammount)
{
	if ((spinSpeed - ammount) >= 0)
	{
		spinSpeed -= ammount;
	}
}

// Decrease Spin Speed
void DecreaseSpinSpeed(int& spinSpeed, int ammount)
{
	spinSpeed += ammount;
}

// Reset Spin Speed
void ResetSpinSpeed(int& spinSpeed)
{
	spinSpeed = START_SPIN_SPEED;
}

// RandomReelPos
int RandomReelPosition(int reelLength) 
{
	int randomPos = 0;

	randomPos = rand() % reelLength;

	return randomPos;
}



// TODO: This is a test function remove when done
void TestPrintResults(int reel1, int reel2, int reel3, int speed, int vis) 
{
	SetConsoleCursorPosition(hconsole, { 0,24 });

	wcout << L"DEBUG: INFO" << endl;
	wcout << L"Reel 1: " << REEL_VALUES[reel1] << endl;
	wcout << L"Reel 2: " << REEL_VALUES[reel2] << endl;
	wcout << L"Reel 3: " << REEL_VALUES[reel3] << endl;
	wcout << "The new spin speed is: " << speed ;
	wcout << L" Visibility: " << vis;
}


