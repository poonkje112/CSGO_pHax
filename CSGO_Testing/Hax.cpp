#include "Hax.h"

void Hax::Trigger(uintptr_t localPlayer, uintptr_t clientDll, ProcMem& mem)
{
	int crosshairId = mem.Read<int>(localPlayer + m_iCrosshairId); // Here we get the entity id what our crosshair is aiming at
	int myTeam = mem.Read<int>(localPlayer + m_iTeamNum); // Getting our team number

	if (crosshairId > 0 && crosshairId < 32) { // Here we check if the id is a player
		uintptr_t entity = mem.Read<uintptr_t>(clientDll + dwEntityList + ((crosshairId - 1) * 16)); // Get the entity from the entity list

		if (entity != NULL)
		{
			int entityTeam = mem.Read<int>(entity + m_iTeamNum); // Get the entity team
			if (entityTeam != myTeam) {
				mem.Write(clientDll + 0x316EC9C, 5); // Here we virtualy do +attack
				Sleep(50);
				mem.Write(clientDll + 0x316EC9C, 4); // Here we virtualy do -attack to prepare for the next shot
			}
		}
	}
}

void Hax::Bhop(uintptr_t localPlayer, uintptr_t clientDll, ProcMem& mem)
{
	byte flag = mem.Read<byte>(localPlayer + m_fFlags);

	if (GetAsyncKeyState(VK_SPACE) && flag & (1 << 0)) { // Check if we are on the ground
		mem.Write<uintptr_t>(clientDll + dwForceJump, 6); // Just forcing the jump
	}
}

void Hax::Glow(uintptr_t localPlayer, uintptr_t clientDll, ProcMem& mem)
{
	int myTeam = mem.Read<int>(localPlayer + m_iTeamNum); // Getting our team number
	uintptr_t glowObject = mem.Read<uintptr_t>(clientDll + dwGlowObjectManager); // Getting the Glowobject manager

	for (int i = 0; i < 64; i++) {

		uintptr_t entity = mem.Read<uintptr_t>(clientDll + dwEntityList + i * 0x10); // Get the entity from the entity list

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

void Hax::Flash(uintptr_t localPlayer, ProcMem& mem)
{
	if (mem.Read<float>(localPlayer + m_flFlashDuration) > 0) // Checking if there was/is a flashbang
	{
		mem.Write<float>(localPlayer + m_flFlashDuration, 0); // Reset the flashbang duriation to 0
	}
}

void Hax::Radar(uintptr_t clientDll, ProcMem& mem)
{
	for(int i = 0; i < 64; i++)
	{
		uintptr_t entity = mem.Read<uintptr_t>(clientDll + dwEntityList + (i * 0x10)); // Get the entity from the entity list

		if(entity > 0)
		{
			mem.Write<int>(entity + m_bSpotted, 1);
		}
	}
}

void Hax::Aimbot(uintptr_t localPLayer, ProcMem& mem)
{

}
