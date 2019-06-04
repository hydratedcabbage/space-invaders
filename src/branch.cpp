#include "cpu.hpp"

using i8080::Cpu;

void Cpu::jmp(uint8_t l, uint8_t h)
{
	uint16_t adr {pair(h, l)};
	pc_ = adr-1; // emulate_op increments the pc by one for each instruction
	cycles_ += 10;
}

void Cpu::j_condition(uint8_t cf, uint8_t l, uint8_t h)
{
	int old_cycles {cycles_};
	if (cf)
		jmp(l, h);
	else
		pc_ += 2;
	cycles_ = old_cycles + 10;
}

void Cpu::call(uint8_t l, uint8_t h)
{
	mem_[sp_ - 1] = static_cast<uint8_t>((pc_+3) >> 8);
	mem_[sp_ - 2] = static_cast<uint8_t>((pc_+3) & 0xff);
	sp_ -= 2;
	uint16_t adr {pair(h, l)};
	pc_ = adr-1;
	cycles_ += 17;
}

void Cpu::c_condition(uint8_t cf, uint8_t l, uint8_t h)
{
	if (cf)
	{
		call(l, h);
	}
	else
	{
		pc_ += 2;
		cycles_ += 11;
	}
}

void Cpu::ret()
{
	uint16_t pcl {static_cast<uint16_t>(mem_[sp_])};
	uint16_t pch = static_cast<uint16_t>(mem_[sp_ + 1]) << 8;
	pc_ = (pch | pcl) - 1;
	sp_ += 2;
	cycles_ += 10;
}

void Cpu::r_condition(uint8_t cf)
{
	if (cf)
	{
		ret();
		cycles_ += 1;
	}
	else
		cycles_ += 5 ;
}

void Cpu::rst(int n)
{
	mem_[sp_ - 1] = static_cast<uint8_t>((pc_+1) >> 8);
	mem_[sp_ - 2] = static_cast<uint8_t>((pc_+1) & 0xff);
	sp_ -= 2;
	pc_ = 8*n - 1;
	cycles_ += 11;
}

void Cpu::pchl()
{
	uint16_t adr {pair(h_, l_)};
	pc_ = adr-1;
	cycles_ += 5;
}
