#pragma once
#include <Windows.h>
#include "ProcMem.h"
#include "csgo.hpp"

using namespace hazedumper::signatures;
using namespace hazedumper::netvars;

class Hax {
public:
	void Trigger(uintptr_t localPlayer, uintptr_t clientDll, ProcMem& mem);
	void Bhop(uintptr_t localPlayer, uintptr_t clientDll, ProcMem& mem);
	void Glow(uintptr_t localPlayer, uintptr_t clientDll, ProcMem& mem);
	void Flash(uintptr_t localPlayer, ProcMem& mem);
	void Radar(uintptr_t clientDll, ProcMem& mem);
	void Aimbot(uintptr_t localPLayer, ProcMem& mem);
};
