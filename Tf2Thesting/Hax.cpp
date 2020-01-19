#include "Hax.h"
#include "csgo.hpp"

using namespace hazedumper::signatures;
using namespace hazedumper::netvars;

//const DWORD EntLoopDist = 0x10; // The distance we want to loop through the entity list

void Hax::Trigger(DWORD LocalPlayer, DWORD clientDLL, ProcMem& mem)
{
	int crosshairId = mem.Read<int>(LocalPlayer + m_iCrosshairId); // Here we get the entity id what our crosshair is aiming at

	if (crosshairId > 0 && crosshairId < 32) { // Here we check if the id is a player
		mem.Write(clientDLL + 0x316EC9C, 5); // Here we virtualy do +attack
		Sleep(50);
		mem.Write(clientDLL + 0x316EC9C, 4); // Here we virtualy do -attack to prepare for the next shot
	}
}

void Hax::Bhop(DWORD LocalPlayer, DWORD clientDLL, ProcMem& mem)
{
	byte flag = mem.Read<byte>(LocalPlayer + m_fFlags);

	if (GetAsyncKeyState(VK_SPACE) && flag & (1 << 0)) { // Check if we are on the ground
		mem.Write<DWORD>(clientDLL + dwForceJump, 6); // Just forcing the jump
	}
}

void Hax::Glow(DWORD LocalPlayer, DWORD clientDLL, ProcMem& mem)
{
	int myTeam = mem.Read<int>(LocalPlayer + m_iTeamNum); // Getting our team number
	DWORD glowObject = mem.Read<DWORD>(clientDLL + dwGlowObjectManager); // Getting the Glowobject manager

	for (int i = 0; i < 64; i++) {

		DWORD entity = mem.Read<DWORD>(clientDLL + dwEntityList + i * 0x10); // Get the entity from the entity list

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

void Hax::Flash(DWORD LocalPlayer, ProcMem& mem)
{
	if (mem.Read<float>(LocalPlayer + m_flFlashDuration) > 0) // Checking if there was/is a flashbang
	{
		mem.Write<float>(LocalPlayer + m_flFlashDuration, 0); // Reset the flashbang duriation to 0
	}
}