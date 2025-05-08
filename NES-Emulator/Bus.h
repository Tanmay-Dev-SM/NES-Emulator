#pragma once
#include <array>
#include <cstdint>//-- helps me in defining 8bit, 16bit into explicited type
#include <memory>

#include "olc6502.h"
#include "olc2C02.h"
#include "Cartridge.h"

class Bus
{
public:
	Bus();
	~Bus();

public:// Devices on bus
	//--6502 CPU
	olc6502 cpu;
	//--2C02 PPU
	olc2C02 ppu;
	//--2KB CPU_RAM on bus
	std::array<uint8_t, 2048> cpuRam;
	//--Cartridge on Bus
	std::shared_ptr<Cartridge> cart;

public:// Functions for Reading and Writing to BUS
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public:// System Interface
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	void clock();

private:
	// A count of how many clocks have passed
	uint32_t nSystemClockCounter = 0;
};
