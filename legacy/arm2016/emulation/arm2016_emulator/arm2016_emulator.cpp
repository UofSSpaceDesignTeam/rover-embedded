// arm2016_emulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "system_emulator.h"

std::vector<MotorEmulator> Emulator::motors;

int main()
{
	Emulator::initialize();
	Emulator::emulateMovement(0, 0, 0, 550, 350, 1006);
    return 0;
}

