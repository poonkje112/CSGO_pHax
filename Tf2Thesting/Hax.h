#pragma once
#include <Windows.h>
#include "ProcMem.h"

class Hax {
public:
	void Trigger(DWORD LocalPlayer, DWORD clientDLL, ProcMem& mem);
	void Bhop(DWORD LocalPlayer, DWORD clientDLL, ProcMem& mem);
	void Glow(DWORD LocalPlayer, DWORD clientDLL, ProcMem& mem);
	void Flash(DWORD LocalPlayer, ProcMem& mem);


};
