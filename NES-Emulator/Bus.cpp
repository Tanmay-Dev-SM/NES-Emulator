#include "Bus.h"

Bus::Bus() {
	//--clearing RAM contents for safety
	for (auto& i : cpuRam) {
		i = 0x00;
	}

	//--Connect CPU to communication Bus
	cpu.ConnectBus(this);
}

Bus::~Bus() {
}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {
	//--guarding the ram contents
	if (cart->cpuWrite(addr, data))
	{
		// The cartridge "sees all" and has the facility to veto
		// the propagation of the bus transaction if it requires.
		// This allows the cartridge to map any address to some
		// other data, including the facility to divert transactions
		// with other physical devices. The NES does not do this
		// but I figured it might be quite a flexible way of adding
		// "custom" hardware to the NES in the future!
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		// System RAM Address Range. The range covers 8KB, though
		// there is only 2KB available. That 2KB is "mirrored"
		// through this address range. Using bitwise AND to mask
		// the bottom 11 bits is the same as addr % 2048.
		cpuRam[addr & 0x07FF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		// PPU Address range. The PPU only has 8 primary registers
		// and these are repeated throughout this range. We can
		// use bitwise AND operation to mask the bottom 3 bits,
		// which is the equivalent of addr % 8.
		ppu.cpuWrite(addr & 0x0007, data);
	}
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly) {
	uint8_t data = 0x00;

	//--guarding the ram contents
	if (cart->cpuRead(addr, data))
	{
		// Cartridge Address Range
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		data = cpuRam[addr & 0x07FF];//--System RAM address Range mirroring after every 2KB
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF) {
		data = ppu.cpuRead(addr & 0x0007, bReadOnly);//--PPU Address range, mirrored every 8
	}

	//--if reading outside the range we return zero
	return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->cart = cartridge; //--Cartridge to Main BUS
	ppu.ConnectCartridge(cartridge); //--Cartridge to PPU BUS
}

void Bus::reset()
{
	cpu.reset();
	nSystemClockCounter = 0;
}

void Bus::clock()
{
	ppu.clock();
	//--CPU run 3 times slower compared to PPU
	if (nSystemClockCounter % 3 == 0) {
		cpu.clock();
	}
	nSystemClockCounter++;
}