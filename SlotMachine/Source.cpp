/************************************************
Slot machine gake made for CMP104 assessment. 

Aurthor: Joseph Lee
Sudent # 1903399

*************************************************/

// Includes
// ========

#include <iostream>
#include <fcntl.h> // To set 16 encoding
#include <io.h> // ???
#include <conio.h> // For reading key presses
#include <stdio.h> // Is this included in io.h?
#include <Windows.h> // To get access to console screen buffer etc.

using namespace std;

// Defines
// =======

// Function Prototypes
// ===================

void GraphicsSetup();
void DrawSlotMachine();
void ToggleSlotMachineLights(bool&);
void ToggleMachineName(bool&);
void PrintReel(int, wstring);
void TestPrintResults(int, int, int, int);


// Globals
// =======

// Player Globals

// Console Globals
CONSOLE_SCREEN_BUFFER_INFO console_info;
CONSOLE_CURSOR_INFO cursor_info;
HANDLE hconsole;
const int SIZE_X = 80; // Size of prinatble screen area X
const int SIZE_Y = 25; // Size of prinatble screen area Y
const int DEFAULT_TEXT_COLOR = 7;

// Game Controls
const char reel1Key = 'Z';
const char reel2Key = 'X';
const char reel3Key = 'C';

// Reels
const int REEL_LENGTH = 9;
const wstring REEL_VALUES[REEL_LENGTH] = { L"cd  ", L"sudo", L"ls  ", L"bash", L"vim ", L"ping", L"grep", L"ssh ", L"echo"};


// Main Game
// =========

int main()
{
	// Animation Variables
	bool lightStatus = false;
	bool nameStatus = true;
	int lightSpeed = 1000;
	int spinSpeed = 400;

	// Game Variables
	int gameActive = true;
	int gameState = 0; // 0 = Idle, 1 = Spin Reels 1+2+3, 2 = Spin Reels 2+3, 3 = Spin Reel 3, 4 = All Reels Stopped
	int reel1StopPos = 0;
	int reel2StopPos = 0;
	int reel3StopPos = 0;

	// Game Setup
	GraphicsSetup();

	DrawSlotMachine();

	while (gameActive)
	{
		// Idle State Loop
		while (gameState == 0)
		{
			ToggleMachineName(nameStatus);

			ToggleSlotMachineLights(lightStatus);

			if (_kbhit())
			{
				int key = _getch();

				if (key == ' ') 
				{
					gameState = 1;
				}

				if (key == 27)
				{
					gameActive = false;
					gameState = 6;
				}

			}

			Sleep(lightSpeed);
		}

		// Spin State Loop
		while (gameState > 0 && gameState <=3)
		{
			bool keepSpinning = true;

			while (keepSpinning)
			{
				for (int i = 0; i < REEL_LENGTH; i++)
				{
					if (gameState == 1)
					{
						PrintReel(1, REEL_VALUES[i]);
					}

					if (gameState == 1 || gameState == 2)
					{
						PrintReel(2, REEL_VALUES[i]);
					}

					if (gameState > 0 && gameState <= 3)
					{
						PrintReel(3, REEL_VALUES[i]);
					}

					if (_kbhit())
					{
						int key = towupper(_getch());

						if (key == 'Z' && gameState == 1)
						{
							reel1StopPos = i;
							gameState = 2;
							break;
						}

						if (key == 'X' && gameState == 2)
						{
							reel2StopPos = i;
							gameState = 3;
							break;
						}

						if (key == 'C' && gameState == 3)
						{
							keepSpinning = false;
							reel3StopPos = i;
							gameState = 4;
							break;
						}
					}

					Sleep(spinSpeed);
				}
			}

			spinSpeed -= 50;
			TestPrintResults(reel1StopPos, reel2StopPos, reel3StopPos, spinSpeed);
			gameState = 0;
		}
	}

	return 0;
}

// Functions
// =========

// Set up console graphics engine for the game
void GraphicsSetup()
{
	bool result = _setmode(_fileno(stdout), _O_U16TEXT); // Set UniCode Chars

	// Debug Message
	OutputDebugString("DEBUG: Initializing console graphics engine.");

	// Define Console Screen Size in COORD
	COORD console_size = { SIZE_X, SIZE_Y };

	// open i/o channel to console screen 
	// TODO: Look up how this actually works as it is an unknown line of code.
	hconsole = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0L, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 0L);

	// Set Screen Buffer Size For Console
	SetConsoleScreenBufferSize(hconsole, console_size);

	// Get Console Info
	GetConsoleScreenBufferInfo(hconsole, &console_info);

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

	wcout << L"  \\|/ ┌────────────────────┐ \\|/  " << endl;
	wcout << L"  (┌─┐)│                    │(┌─┐)" << endl;
	wcout << L" ╔═╧═╧═╧════════════════════╧═╧═╧═╗" << endl;
	wcout << L" ║ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ║ " << endl;
	wcout << L" ║  ╔════════╤════════╤════════╗  ║" << endl;
	wcout << L" ║  ║        │        │        ║  ║" << endl;
	wcout << L" ║  ╟        ┼        ┼        ╢  ║" << endl;
	wcout << L" ║  ║        │        │        ║  ║" << endl;
	wcout << L" ║  ╚════════╧════════╧════════╝  ║" << endl;
	wcout << L" ║ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ║" << endl;
	wcout << L" ╚═╤═╤═╤════════════════════╤═╤═╤═╝" << endl;
	wcout << L" ╔═╧═╧═╧════════════════════╧═╧═╧═╗" << endl;
	wcout << L" ║┌──────────────────────────────┐║" << endl;
	wcout << L" ║│   ┌───┐    ┌───┐    ┌───┐    │║" << endl;
	wcout << L" ║│   │ Z │    │ X │    │ C │    │║" << endl;
	wcout << L" ║│   └───┘    └───┘    └───┘    │║" << endl;
	wcout << L" ║└──────────────────────────────┘║" << endl;
	wcout << L" ╚═╤═╤═╤════════════════════╤═╤═╤═╝" << endl;
	wcout << L"   └─┴─┘                    └─┴─┘  " << endl;
}

// Toggle Slot Machine Lights
void ToggleSlotMachineLights(bool &status)
{

	if (status)
	{
		SetConsoleTextAttribute(hconsole, 12); // Set Lights Red

		// Light 1 top
		SetConsoleCursorPosition(hconsole, { 3, 1 });
		wcout << "\\|/";

		// Light 1 Left
		SetConsoleCursorPosition(hconsole, { 2,2 });
		wcout << "(";

		// Light 1 Right
		SetConsoleCursorPosition(hconsole, { 6,2 });
		wcout << ")";

		// Light 2 top
		SetConsoleCursorPosition(hconsole, { 30, 1 });
		wcout << "\\|/";

		// Light 2 Left
		SetConsoleCursorPosition(hconsole, { 29,2 });
		wcout << "(";

		// Light 2 Right
		SetConsoleCursorPosition(hconsole, { 33,2 });
		wcout << ")";

		SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default

		status = false;
	}
	else
	{
		// Light 1 top
		SetConsoleCursorPosition(hconsole, { 3, 1 });
		wcout << "   ";

		// Light 1 Left
		SetConsoleCursorPosition(hconsole, { 2,2 });
		wcout << " ";

		// Light 1 Right
		SetConsoleCursorPosition(hconsole, { 6,2 });
		wcout << " ";

		// Light 2 top
		SetConsoleCursorPosition(hconsole, { 30, 1 });
		wcout << "   ";

		// Light 2 Left
		SetConsoleCursorPosition(hconsole, { 29,2 });
		wcout << " ";

		// Light 2 Right
		SetConsoleCursorPosition(hconsole, { 33,2 });
		wcout << " ";

		status = true;
	}
}

// Cycle through colors for machine name
void ToggleMachineName(bool& status)
{
	if (status)
	{	
		SetConsoleCursorPosition(hconsole, { 12,2 });

		SetConsoleTextAttribute(hconsole, 11); // Set Name Light Cyan

		wcout << "HACK MACHINE";

		SetConsoleTextAttribute(hconsole, 10); // Set Name Light Green

		SetConsoleCursorPosition(hconsole, { 9,2 });

		wcout << "**";

		SetConsoleCursorPosition(hconsole, { 25,2 });

		wcout << "**";

		SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default

		status = false;
	}
	else
	{
		SetConsoleCursorPosition(hconsole, { 12,2 });

		SetConsoleTextAttribute(hconsole, 10); // Set Name Light Green

		wcout << "------------";

		SetConsoleTextAttribute(hconsole, 11); // Set Name Light Cyan

		SetConsoleCursorPosition(hconsole, { 9,2 });

		wcout << "**";

		SetConsoleCursorPosition(hconsole, { 25,2 });

		wcout << "**";

		SetConsoleTextAttribute(hconsole, DEFAULT_TEXT_COLOR); // Set Console Text Color to Default

		status = true;
	}

	
}

void PrintReel(int reelNumber, wstring reelValue)
{
	if (reelNumber > 0 && reelNumber <= 3)
	{
		if (reelNumber == 1)
		{
			SetConsoleCursorPosition(hconsole, { 7,7 });

			wcout << reelValue;
		}
		else if (reelNumber == 2)
		{
			SetConsoleCursorPosition(hconsole, { 16,7 });

			wcout << reelValue;
		}
		else if (reelNumber == 3)
		{
			SetConsoleCursorPosition(hconsole, { 25,7 });

			wcout << reelValue;
		}
	}
	else 
	{
		OutputDebugString("DEBUG: reelNumberValue out of range. It must be between 1-3.");
	}
}

// TODO: This is a test function remove when done
void TestPrintResults(int reel1, int reel2, int reel3, int speed) 
{
	SetConsoleCursorPosition(hconsole, { 0,22 });

	wcout << L"DEBUG: FUNCTION" << endl;
	wcout << L"The reel results were:" << endl;
	wcout << L"Reel 1: " << REEL_VALUES[reel1] << endl;
	wcout << L"Reel 2: " << REEL_VALUES[reel2] << endl;
	wcout << L"Reel 3: " << REEL_VALUES[reel3] << endl;
	wcout << endl;
	wcout << "The new spin speed is: " << speed << endl;
}

