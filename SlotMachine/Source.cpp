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
void toggleSlotMachineLights(bool&);
void ToggleMachineName(bool&);


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

// Game State Globals
int gameState = 0; // 0 = Idle, 1 = Play

// Main Game Loop
// ==============

int main()
{
	// Animation
	bool lightStatus = false;
	bool nameStatus = true;
	int lightSpeed = 1000;

	COORD slotMachineTopLeft = { 1,1 };

	GraphicsSetup();

	DrawSlotMachine();



	while (gameState == 0)
	{
		ToggleMachineName(nameStatus);

		toggleSlotMachineLights(lightStatus);

		Sleep(lightSpeed);
	}

	return 0;
}

// Functions
// =========

// Runs to set up console graphics engine for the game
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
	wcout << L" ║  ║        │        │        ║  ║" << endl;
	wcout << L" ║  ║        │        │        ║  ║" << endl;
	wcout << L" ║  ╚════════╧════════╧════════╝  ║" << endl;
	wcout << L" ║ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ║" << endl;
	wcout << L" ╚═╤═╤═╤════════════════════╤═╤═╤═╝" << endl;
	wcout << L" ╔═╧═╧═╧════════════════════╧═╧═╧═╗" << endl;
	wcout << L" ║┌──────────────────────────────┐║" << endl;
	wcout << L" ║│   ┌───┐    ┌───┐    ┌───┐    │║" << endl;
	wcout << L" ║│   │ A │    │ S │    │ D │    │║" << endl;
	wcout << L" ║│   └───┘    └───┘    └───┘    │║" << endl;
	wcout << L" ║└──────────────────────────────┘║" << endl;
	wcout << L" ╚═╤═╤═╤════════════════════╤═╤═╤═╝" << endl;
	wcout << L"   └─┴─┘                    └─┴─┘" << endl;
}

// Toggle Slot Machine Lights
void toggleSlotMachineLights(bool &status)
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

