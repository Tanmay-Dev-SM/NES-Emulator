#pragma once
#include <cstdint> //-- helps me in defining 8bit, 16bit into explicited type
#include "olc6502.h"
#include <array>

class Bus
{
public:
	Bus();
	~Bus();

public:// Devices on bus
	olc6502 cpu;

	// 64KB RAM on bus
	std::array<uint8_t, 64 * 1024> ram;

public:// Functions for Reading and Writing to BUS
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr, bool bReadOnly = false);
};
