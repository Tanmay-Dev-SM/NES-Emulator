#include "Bus.h"

Bus::Bus() {
	//--clearing RAM contents for safety
	for (auto& i : ram) {
		i = 0x00;
	}

	//--Connect CPU to communication Bus
	cpu.ConnectBus(this);
}

Bus::~Bus() {
}

void Bus::write(uint16_t addr, uint8_t data) {
	//--guarding the ram contents
	if (addr >= 0x0000 && addr <= 0xFFFF) {
		ram[addr] = data;
	}
}

uint8_t Bus::read(uint16_t addr, bool bReadOnly) {
	//--guarding the ram contents
	if (addr >= 0x0000 && addr <= 0xFFFF) {
		return ram[addr];
	}

	//--if reading outside the range we return zero
	return 0x00;
}