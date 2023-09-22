#include "Chip8.h"

#include "COSMAC/Core/Log.h"

#include <fstream>
#include <cmath>

std::array<uint8_t, 0x50> font = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,
	0x60, 0x20, 0x20, 0x20, 0x70,
	0xF0, 0x10, 0xF0, 0x80, 0xF0,
	0xF0, 0x10, 0xF0, 0x10, 0xF0,
	0xA0, 0xA0, 0xF0, 0x20, 0x20,
	0xF0, 0x80, 0xF0, 0x10, 0xF0,
	0xF0, 0x80, 0xF0, 0x90, 0xF0,
	0xF0, 0x10, 0x10, 0x10, 0x10,
	0xF0, 0x90, 0xF0, 0x90, 0xF0,
	0xF0, 0x90, 0xF0, 0x10, 0xF0,
	0xF0, 0x90, 0xF0, 0x90, 0x90,
	0xF0, 0x50, 0x70, 0x50, 0xF0,
	0xF0, 0x80, 0x80, 0x80, 0xF0,
	0xF0, 0x50, 0x50, 0x50, 0xF0,
	0xF0, 0x80, 0xF0, 0x80, 0xF0,
	0xF0, 0x80, 0xF0, 0x80, 0x80,
};

Chip8::Chip8()
{
	PC = 0x200;
	I = 0;
	SP = 0;

	//Clear display
	for (auto& i : framebuffer)
		i = 0x00;

	//Clear stack
	for (auto& i : stack)
		i = 0x00;

	//Clear general purpose registers V[x]
	for (auto& i : V)
		i = 0x00;

	//Clear memory
	for (auto& i : memory)
		i = 0x00;

	//Load fontset into memory (0x050 - 0x0A0)
	for (int i = 0; i < 0x050; ++i)
		memory[0x050 + i] = font[i];

	//Reset timers
	DT = 0x00;
	ST = 0x00;
}

Chip8::~Chip8()
{
}

bool Chip8::LoadProgram()
{
	const std::string path = "..\\..\\COSMAC\\Sandbox\\src\\corax.ch8";
	std::ifstream file(path, std::ios::binary);

	if (!file.is_open()) {
		COSMAC_ERROR("Failed to open the file.");
		return false;
	}

	COSMAC_INFO("{0} loaded succesfully.", path);

	// Get the size of the file
	file.seekg(0, std::ios::end);
	std::streampos size = file.tellg();
	file.seekg(0, std::ios::beg);

	// Ensure that the ROM will fit in memory without exceeding its bounds
	if (size > (memory.size() - 0x200)) {
		COSMAC_ERROR("ROM file size exceeds available memory.");
		return false;
	}

	// Read the ROM contents into the memory array starting at 0x200
	file.read(reinterpret_cast<char*>(&memory[0x200]), size);
	file.close();

	return true;
}

void Chip8::FetchOpcode()
{
	opcode = memory[PC] << 8 | memory[PC + 1];
	PC += 2;
}

void Chip8::Tick()
{
	//Read next 2 opcodes from memory
	FetchOpcode();

	//Decode
	switch (opcode & 0xF000)
	{
	case 0x0000: 
		switch (opcode & 0x000F)
		{
		case 0x0000: OP_00E0(); break;
		case 0x000E: OP_00EE(); break;
		default: break;
		}
		break;
	case 0x1000: OP_1nnn(); break;
	case 0x2000: OP_2nnn(); break;
	case 0x3000: OP_3xkk(); break;
	case 0x4000: OP_4xkk(); break;
	case 0x5000: OP_5xy0(); break;
	case 0x6000: OP_6xkk(); break;
	case 0x7000: OP_7xkk(); break;
	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000: OP_8xy0(); break;
		case 0x0001: OP_8xy1(); break;
		case 0x0002: OP_8xy2(); break;
		case 0x0003: OP_8xy3(); break;
		case 0x0004: OP_8xy4(); break;
		case 0x0005: OP_8xy5(); break;
		case 0x0006: OP_8xy6(); break;
		case 0x0007: OP_8xy7(); break;
		case 0x000E: OP_8xyE(); break;
		default: break;
		}
		break;
	case 0x9000: OP_9xy0(); break;
	case 0xA000: OP_Annn(); break;
	case 0xB000: OP_Bnnn(); break;
	case 0xC000: OP_Cxkk(); break;
	case 0xD000: OP_Dxyn(); break;
	case 0xE000:
		COSMAC_WARN("SKP | SKNP - 0xEX00");
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x001E: OP_Fx1E();  break;
		case 0x0033: OP_Fx33(); break;
		case 0x0055: OP_Fx55(); break;
		case 0x0065: OP_Fx65(); break;

		default:
			break;
		}

	default:
		break;
	}

	//Update timers
	if (DT > 0)
		--DT;

	if (ST > 0)
		--ST;
}


void Chip8::OP_00E0()
{
	COSMAC_WARN("CLS - 0x00E0");
	for (auto i = 0; i < framebuffer.size(); i++)
		framebuffer[i] = 0;
}
void Chip8::OP_00EE()
{
	COSMAC_WARN("RET - 0x00EE");
	--SP;
	PC = stack[SP];
}

void Chip8::OP_1nnn()
{
	COSMAC_WARN("JP - 0x1NNN");
	PC = opcode & 0x0FFF;
}

void Chip8::OP_2nnn()
{
	COSMAC_WARN("CALL - 0x2NNN");
	stack[SP] = PC;
	++SP;
	PC = opcode & 0x0FFF;
}

void Chip8::OP_3xkk()
{
	COSMAC_WARN("SE - 0x3XKK");
	if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
		PC += 2;
}

void Chip8::OP_4xkk()
{
	COSMAC_WARN("SNE - 0x4XKK");
	if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
		PC += 2;
}

void Chip8::OP_5xy0()
{
	COSMAC_WARN("SE - 0x5XY0");
	if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
		PC += 2;
}

void Chip8::OP_6xkk()
{
	COSMAC_WARN("LD - 0x6XKK");
	V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
}

void Chip8::OP_7xkk()
{
	COSMAC_WARN("ADD - 0x7XKK");
	V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
}


void Chip8::OP_8xy0()
{
	COSMAC_WARN("LD - 0x8XY0");
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
}

void Chip8::OP_8xy1()
{
	COSMAC_WARN("OR - 0x8XY1");
	V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
}

void Chip8::OP_8xy2()
{
	COSMAC_WARN("AND - 0x8XY2");
	V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
}

void Chip8::OP_8xy3()
{
	COSMAC_WARN("XOR - 0x8XY3");
	V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
}

void Chip8::OP_8xy4()
{
	COSMAC_WARN("ADD - 0x8XY4");
	if ((V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) > 255)
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) & 0xFF;
}

void Chip8::OP_8xy5()
{
	COSMAC_WARN("SUB - 0x8XY5");
	if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
}

void Chip8::OP_8xy6()
{
	COSMAC_WARN("SHR - 0x8XY6");
	V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
	V[(opcode & 0x0F00) >> 8] >>= 1;
}

void Chip8::OP_8xy7()
{
	COSMAC_WARN("SUBN - 0x8XY7");
	if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
}

void Chip8::OP_8xyE()
{
	COSMAC_WARN("SHL - 0x8XYE");
	V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x80) >> 7;
	V[(opcode & 0x0F00) >> 8] <<= 1;
}


void Chip8::OP_9xy0()
{
	COSMAC_WARN("SNE - 0x9XY0");
	if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
		PC += 2;
}

void Chip8::OP_Annn()
{
	COSMAC_WARN("LD - 0xANNN");
	I = opcode & 0x0FFF;
}

void Chip8::OP_Bnnn()
{
	COSMAC_WARN("JP - 0xBNNN");
	PC = V[0] + (opcode & 0x0FFF);
}

void Chip8::OP_Cxkk()
{
	COSMAC_WARN("SE - Cxkk");

	if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
		PC += 2;
}

void Chip8::OP_Dxyn()
{
	COSMAC_WARN("DRW - 0xDXYN");

	V[0xF] = 0;

	uint8_t x = V[(opcode & 0x0F00) >> 8];
	uint8_t y = V[(opcode & 0x00F0) >> 4];
	uint8_t n = opcode & 0x000F;

	for (auto a = 0; a < n; a++)
	{
		for (auto b = 0; b < 8; b++)
		{
			auto target = ((x + b) % 64) + ((y + a) % 32) * 64;
			auto source = memory[I + a] >> (7 - b) & 0x1;

			if (source)
			{
				if (framebuffer[target] == 0xFFFFFFFF)
					V[0xF] = 1;

				framebuffer[target] = 0xFFFFFFFF;
			}
		}
	}
}


void Chip8::OP_ExA1()
{
}

void Chip8::OP_Ex9E()
{
}


void Chip8::OP_Fx07()
{
}

void Chip8::OP_Fx0A()
{
}

void Chip8::OP_Fx15()
{
}

void Chip8::OP_Fx18()
{
}

void Chip8::OP_Fx1E()
{
	COSMAC_WARN("ADD - 0xFX1E");
	I += V[(opcode & 0x0F00) >> 8];
}

void Chip8::OP_Fx29()
{
}

void Chip8::OP_Fx33()
{
	COSMAC_WARN("LD - 0xFX33");
	memory[I] = floor((V[(opcode & 0x0F00) >> 8] / 100) % 10);
	memory[I + 1] = floor((V[(opcode & 0x0F00) >> 8] / 10) % 10);
	memory[I + 2] = floor((V[(opcode & 0x0F00) >> 8]) % 10);
}

void Chip8::OP_Fx55()
{
	COSMAC_WARN("LD - 0xFX55");
	for (auto i = 0; i <= (opcode & 0x0F00) >> 8; ++i)
		memory[I + i] = V[i];
}

void Chip8::OP_Fx65()
{
	COSMAC_WARN("LD - 0xFX65");
	for (auto i = 0; i <= (opcode & 0x0F00) >> 8; ++i)
		V[i] = memory[I + i];
}