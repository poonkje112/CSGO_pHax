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

const DWORD EntLoopDist = 0x10; // The distance we want to loop through the entity list

ProcMem mem;
DWORD LocalPlayer; // Our local player in the current session
DWORD clientDLL; // panorama_client.dll
DWORD engineDLL; // engine.dll

bool trigger, glow, bhop;

//TODO: Pointer and health offset are working in CE but are not returning the right value!
//"client_panorama.dll"+0xD29B1C <- Local Player
//Health localplayer + 0x100

//TODO: For some reason our it looks like that our localPlayer is not being updated ( correctly )

#pragma region Hax

void Trigger() {
	int crosshairId = mem.Read<int>(LocalPlayer + m_iCrosshairId); // Here we get the entity id what our crosshair is aiming at

	if (crosshairId > 0 && crosshairId < 32) { // Here we check if the id is a player
		mem.Write(clientDLL + 0x316EC9C, 5); // Here we virtualy do +attack
		Sleep(50);
		mem.Write(clientDLL + 0x316EC9C, 4); // Here we virtualy do -attack to prepare for the next shot
	}
}

void Glow() {
	int myTeam = mem.Read<int>(LocalPlayer + m_iTeamNum); // Getting our team number
	DWORD glowObject = mem.Read<DWORD>(clientDLL + dwGlowObjectManager); // Getting the Glowobject manager

	for (int i = 0; i < 64; i++) {

		DWORD entity = mem.Read<DWORD>(clientDLL + dwEntityList + i * EntLoopDist); // Get the entity from the entity list

		if (entity != NULL) { // Check if we got an entity

			int glowIndex = mem.Read<int>(entity + m_iGlowIndex); // Get the glow index form the entity
			int entityTeam = mem.Read<int>(entity + m_iTeamNum); // Get the entity team
			int entityHealth = mem.Read<int>(entity + m_iHealth); // Get our entity health - UNUSED

			if (entityTeam != myTeam) { // Checking if the entity is on the same team as our localPlayer

				mem.Write<float>(glowObject + ((glowIndex * 0x38) + 0x4), 2); // This is the R value of the glowobject
				mem.Write<float>(glowObject + ((glowIndex * 0x38) + 0x8), 0); // This is the G value of the glowobject
				mem.Write<float>(glowObject + ((glowIndex * 0x38) + 0xC), 0); // This is the B value of the glowobject
				mem.Write<float>(glowObject + ((glowIndex * 0x38) + 0x10), 0.5f); // This is the A value of the glowobject

			}
			else {

				mem.Write<float>(glowObject + ((glowIndex * 0x38) + 0x4), 0); // This is the R value of the glowobject
				mem.Write<float>(glowObject + ((glowIndex * 0x38) + 0x8), 2); // This is the G value of the glowobject
				mem.Write<float>(glowObject + ((glowIndex * 0x38) + 0xC), 0); // This is the B value of the glowobject
				mem.Write<float>(glowObject + ((glowIndex * 0x38) + 0x10), 0.5f); // This is the A value of the glowobject

			}

			mem.Write<bool>(glowObject + ((glowIndex * 0x38) + 0x24), true);
			mem.Write<bool>(glowObject + ((glowIndex * 0x38) + 0x25), false);
		}
	}
}

void Bhop() {
	byte flag = mem.Read<byte>(LocalPlayer + m_fFlags);

	if (GetAsyncKeyState(VK_SPACE) && flag & (1 << 0)) {
		mem.Write<DWORD>(clientDLL + dwForceJump, 6);
	}
}

#pragma endregion

#pragma region debugging

void PrintCStates() {
	system("cls");
	std::cout << std::boolalpha << "Triggerbot: " << trigger << std::endl;
	std::cout << std::boolalpha << "Glow: " << glow << std::endl;
	std::cout << std::boolalpha << "bHop: " << bhop << std::endl;
}

#pragma endregion

#pragma region input

//Key flags
int f1 = 0, f2 = 0, f3 = 0;
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

	if (GetAsyncKeyState(VK_F1) == 0) f1 = 0;
	if (GetAsyncKeyState(VK_F2) == 0) f2 = 0;
	if (GetAsyncKeyState(VK_F3) == 0) f3 = 0;
}

void DoActions() {
	if (trigger) Trigger();
	if (glow) Glow();
	if (bhop) Bhop();
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
	SetConsoleTitle("pHaxor 160120 - PRIVATE ");

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

	LocalPlayer = mem.Read<DWORD>(clientDLL + dwLocalPlayer); // Getting our localPlayer
	while (mem.processState() == STILL_ACTIVE) {
		LocalPlayer = mem.Read<DWORD>(clientDLL + dwLocalPlayer); // Updating our localPlayer

		InputHandle();
		DoActions();
	}
}

#pragma endregion