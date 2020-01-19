#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include "ProcMem.h"
#include "csgo.hpp"
#include "Hax.h"

using namespace hazedumper::signatures;
using namespace hazedumper::netvars;

std::string csChangeNumber = "7544935"; // current csgo changenumber

ProcMem mem;
Hax hax;
DWORD LocalPlayer; // Our local player in the current session
DWORD clientDLL; // panorama_client.dll
DWORD engineDLL; // engine.dll

bool trigger, glow, bhop, flash;

//TODO: Pointer and health offset are working in CE but are not returning the right value!
//"client_panorama.dll"+0xD29B1C <- Local Player
//Health localplayer + 0x100

//TODO: For some reason our it looks like that our localPlayer is not being updated ( correctly )

#pragma region debugging

void PrintCStates() {
	system("cls");
	std::cout << std::boolalpha << "Triggerbot: " << trigger << std::endl;
	std::cout << std::boolalpha << "Glow: " << glow << std::endl;
	std::cout << std::boolalpha << "bHop: " << bhop << std::endl;
	std::cout << std::boolalpha << "Enable flash: " << !flash << std::endl;
}

#pragma endregion

#pragma region input

//Key flags
int f1 = 0, f2 = 0, f3 = 0, f4 = 0;
void InputHandle() {
	if (GetAsyncKeyState(VK_F1) && f1 == 0) {
		f1 = 1;
		trigger = !trigger;
		PrintCStates();
	}

	if (GetAsyncKeyState(VK_F2) && f2 == 0) {
		f2 = 1;
		glow = !glow;
		PrintCStates();
	}

	if (GetAsyncKeyState(VK_F3) && f3 == 0) {
		f3 = 1;
		bhop = !bhop;
		PrintCStates();
	}

	if (GetAsyncKeyState(VK_F4) && f4 == 0) {
		f4 = 1;
		flash = !flash;
		PrintCStates();
	}

	if (GetAsyncKeyState(VK_F1) == 0) f1 = 0;
	if (GetAsyncKeyState(VK_F2) == 0) f2 = 0;
	if (GetAsyncKeyState(VK_F3) == 0) f3 = 0;
	if (GetAsyncKeyState(VK_F4) == 0) f4 = 0;
}

void DoActions() {
	// Input
	if (trigger) hax.Trigger(LocalPlayer, clientDLL, mem);
	if (bhop) hax.Bhop(LocalPlayer, clientDLL, mem);

	// Graphical
	if (glow) hax.Glow(LocalPlayer, clientDLL, mem);
	if(flash) hax.Flash(LocalPlayer, mem);
}

#pragma endregion

#pragma region Main

bool UpdateCheck() {
	std::cout << "Checking for updates...\n";

	// Do csgo version check here

	std::cout << "You are fully up to date!\n";
	Sleep(500);
	system("cls");
	return true;
}

int main()
{
	SetConsoleTitle("pHaxor v200120 - PRIVATE ");

	if (!UpdateCheck()) {
		std::cout << "CSGO Has been updated!\nPlease download an updated version of pHax!";
		return 0;
	}

	bool processState = false;
	std::cout << "Waiting for CSGO to launch" << std::endl;

	while (!processState) {
		processState = mem.Process((char*)"csgo.exe");
	}

	std::cout << "CSGO Found!\n";

	clientDLL = mem.Module((char*)"client_panorama.dll");
	engineDLL = mem.Module((char*)"engine.dll");

	Sleep(500);
	system("cls");
	PrintCStates();

	while (mem.processState() == STILL_ACTIVE) {
		LocalPlayer = mem.Read<DWORD>(clientDLL + dwLocalPlayer); // Updating our localPlayer

		InputHandle();
		DoActions();
	}
}

#pragma endregion