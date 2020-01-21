#include <iostream>
#include <fstream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include "ProcMem.h"
#include "csgo.hpp"
#include "Hax.h"

using namespace hazedumper::signatures;
using namespace hazedumper::netvars;

std::string _CsPatchVersion = "1.37.3.7"; // current csgo changenumber

ProcMem _Mem;
Hax _Hax;
uintptr_t _LocalPlayer; // Our local player in the current session
uintptr_t _ClientDLL; // panorama_client.dll
uintptr_t _EngineDLL; // engine.dll

bool _Trigger, _Glow, _Bhop, _Flash;

//TODO: For some reason our it looks like that our localPlayer is not being updated ( correctly )

#pragma region debugging

void PrintCStates() {
	system("cls");
	std::cout << std::boolalpha << "Triggerbot: " << _Trigger << std::endl;
	std::cout << std::boolalpha << "Glow: " << _Glow << std::endl;
	std::cout << std::boolalpha << "bHop: " << _Bhop << std::endl;
	std::cout << std::boolalpha << "Enable flash: " << !_Flash << std::endl;
}

#pragma endregion

#pragma region input

//Key flags
int _F1 = 0, _F2 = 0, _F3 = 0, _F4 = 0;
void InputHandle() {
	if (GetAsyncKeyState(VK_F1) && _F1 == 0) {
		_F1 = 1;
		_Trigger = !_Trigger;
		PrintCStates();
	}

	if (GetAsyncKeyState(VK_F2) && _F2 == 0) {
		_F2 = 1;
		_Glow = !_Glow;
		PrintCStates();
	}

	if (GetAsyncKeyState(VK_F3) && _F3 == 0) {
		_F3 = 1;
		_Bhop = !_Bhop;
		PrintCStates();
	}

	if (GetAsyncKeyState(VK_F4) && _F4 == 0) {
		_F4 = 1;
		_Flash = !_Flash;
		PrintCStates();
	}

	if (GetAsyncKeyState(VK_F1) == 0) _F1 = 0;
	if (GetAsyncKeyState(VK_F2) == 0) _F2 = 0;
	if (GetAsyncKeyState(VK_F3) == 0) _F3 = 0;
	if (GetAsyncKeyState(VK_F4) == 0) _F4 = 0;
}

void DoActions() {
	// Input
	if (_Trigger) _Hax.Trigger(_LocalPlayer, _ClientDLL, _Mem);
	if (_Bhop) _Hax.Bhop(_LocalPlayer, _ClientDLL, _Mem);

	// Graphical
	if (_Glow) _Hax.Glow(_LocalPlayer, _ClientDLL, _Mem);
	if (_Flash) _Hax.Flash(_LocalPlayer, _Mem);
}

#pragma endregion

#pragma region Main

bool UpdateCheck() {
	std::cout << "Checking for updates...\n";
	std::string path(_Mem.GetExePath());

	path = path.substr(0, path.size() - 8);

	size_t pos;
	while ((pos = path.find("\\")) != std::string::npos) {
		path.replace(pos, 1, "/");
	}

	path.append("csgo/steam.inf");

	std::cout << path << std::endl;
	std::ifstream file(path);
	std::string conf;
	while (std::getline(file, conf)) {
		if (conf.find("PatchVersion=") != std::string::npos) {
			break;
		}
	}

	conf = conf.substr(13, conf.size() - 13);
	std::cout << conf << std::endl;

	if (conf != _CsPatchVersion) {
		return false;
	}

	std::cout << "You are fully up to date!\n";
	Sleep(500);
	system("cls");
	return true;
}

int main()
{
	SetConsoleTitle("pHax v200120 - PRIVATE ");

	bool processState = false;
	std::cout << "Waiting for CSGO to launch" << std::endl;

	while (!processState) {
		processState = _Mem.Process((char*)"csgo.exe");
	}

	if (!UpdateCheck()) {
		std::cout << "CSGO Has been updated!\nPlease download an updated version of pHax!";
		return 0;
	}

	std::cout << "CSGO Found!\n";

	_ClientDLL = _Mem.Module((char*)"client_panorama.dll");
	_EngineDLL = _Mem.Module((char*)"engine.dll");

	Sleep(500);
	system("cls");
	PrintCStates();

	while (_Mem.processState() == STILL_ACTIVE) {
		_LocalPlayer = _Mem.Read<uintptr_t>(_ClientDLL + dwLocalPlayer); // Updating our localPlayer

		InputHandle();
		DoActions();
		Sleep(1); // Decrease CPU Load
	}
}

#pragma endregion