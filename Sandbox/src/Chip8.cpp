#include "Chip8.h"

#include <fstream>
#include <cmath>
#include <random>

std::array<uint8_t, 0x50> font = {
/* '0' */ 0xF0, 0x90, 0x90, 0x90, 0xF0,
/* '1' */ 0x20, 0x60, 0x20, 0x20, 0x70,
/* '2' */ 0xF0, 0x10, 0xF0, 0x80, 0xF0,
/* '3' */ 0xF0, 0x10, 0xF0, 0x10, 0xF0,
/* '4' */ 0x90, 0x90, 0xF0, 0x10, 0x10,
/* '5' */ 0xF0, 0x80, 0xF0, 0x10, 0xF0,
/* '6' */ 0xF0, 0x80, 0xF0, 0x90, 0xF0,
/* '7' */ 0xF0, 0x10, 0x20, 0x40, 0x40,
/* '8' */ 0xF0, 0x90, 0xF0, 0x90, 0xF0,
/* '9' */ 0xF0, 0x90, 0xF0, 0x10, 0xF0,
/* 'A' */ 0xF0, 0x90, 0xF0, 0x90, 0x90,
/* 'B' */ 0xE0, 0x90, 0xE0, 0x90, 0xE0,
/* 'C' */ 0xF0, 0x80, 0x80, 0x80, 0xF0,
/* 'D' */ 0xE0, 0x90, 0x90, 0x90, 0xE0,
/* 'E' */ 0xF0, 0x80, 0xF0, 0x80, 0xF0,
/* 'F' */ 0xF0, 0x80, 0xF0, 0x80, 0x80,
};

Chip8::Chip8()
{
	PC = 0x200;
	I = 0;
	SP = 0;
	DT = 0;
	ST = 0;

	//Clear display
	for (auto& i : framebuffer)
		i = 0;

	//Clear stack
	while (!stack.empty())
		stack.pop();

	//Clear general purpose registers V[x]
	for (auto& i : V)
		i = 0;

	//Clear memory
	for (auto& i : memory)
		i = 0;

	//Load fontset into memory (0x050 - 0x0A0)
	for (int i = 0; i < 0x050; ++i)
		memory[i] = font[i];
}

void Chip8::Restart()
{
	PC = 0x200;
	I = 0;
	SP = 0;
	DT = 0;
	ST = 0;

	//Clear display
	for (auto& i : framebuffer)
		i = 0;
	DRAW = true;

	//Clear stack
	while (!stack.empty())
		stack.pop();

	//Clear general purpose registers V[x]
	for (auto& i : V)
		i = 0;

	//Clear memory
	for (auto& i : memory)
		i = 0;

	//Load fontset into memory (0x050 - 0x0A0)
	for (int i = 0; i < 0x050; ++i)
		memory[i] = font[i];
}

bool Chip8::LoadProgram(const std::string& path)
{
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

	PAUSE = false;

	return true;
}

std::string Chip8::decToHex(int n)
{
	// ans string to store hexadecimal number
	std::string ans = "";

	while (n != 0) {
		// remainder variable to store remainder
		int rem = 0;

		// ch variable to store each character
		char ch;
		// storing remainder in rem variable.
		rem = n % 16;

		// check if temp < 10
		if (rem < 10)
			ch = rem + 48;
		else
			ch = rem + 55;

		// updating the ans string with the character variable
		ans += ch;
		n = n / 16;
	}

	// reversing the ans string to get the final result
	int i = 0, j = ans.size() - 1;
	while (i <= j)
	{
		std::swap(ans[i], ans[j]);
		i++;
		j--;
	}
	return ans;
}

void Chip8::FetchOpcode()
{
	//16 bit instruction ($FFFF hex num) into opcode to be evaluated
	opcode = memory[PC] << 8 | memory[PC + 1];
	//Increase program counter by 2
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
		switch (opcode)
		{
		case 0x00E0: OP_00E0();  break;
		case 0x00EE: OP_00EE(); break;
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
		switch (opcode & 0x00FF)
		{
		case 0x009E: OP_Ex9E(); break;
		case 0x00A1: OP_ExA1(); break;
		default:
			break;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007: OP_Fx07(); break;
		case 0x000A: OP_Fx0A(); break;
		case 0x0015: OP_Fx15(); break;
		case 0x0018: OP_Fx18(); break;
		case 0x001E: OP_Fx1E(); break;
		case 0x0029: OP_Fx29(); break;
		case 0x0033: OP_Fx33(); break;
		case 0x0055: OP_Fx55(); break;
		case 0x0065: OP_Fx65(); break;
		default: break;
		}
		break;
	}
}

void Chip8::UpdateTimers()
{
	if (DT > 0)
		DT--;

	if (ST > 0)
		ST--;
}

void Chip8::OP_00E0()
{
	for (auto& i : framebuffer)
		i = 0;
}

void Chip8::OP_00EE()
{
	//COSMAC_WARN("RET - 0x00EE");
	PC = stack.top();
	stack.pop();
}

void Chip8::OP_1nnn()
{
	std::string s = decToHex((int)opcode);
	//COSMAC_WARN("{0} ${1} - JP {2}{3}{4}", decToHex(PC), s, s[1], s[2], s[3]);
	PC = opcode & 0x0FFF;
}

void Chip8::OP_2nnn()
{
	//COSMAC_WARN("CALL - 0x2NNN");
	stack.push(PC);
	PC = opcode & 0x0FFF;
}

void Chip8::OP_3xkk()
{
	//COSMAC_WARN("SE - 0x3XKK");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t kk = opcode & 0x00FF;

	if (V[x] == kk)
		PC += 2;
}

void Chip8::OP_4xkk()
{
	//COSMAC_WARN("SNE - 0x4XKK");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t kk = opcode & 0x00FF;

	if (V[x] != kk)
		PC += 2;
}

void Chip8::OP_5xy0()
{
	//COSMAC_WARN("SE - 0x5XY0");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	if (V[x] == V[y])
		PC += 2;
}

void Chip8::OP_6xkk()
{
	//COSMAC_WARN("LD - 0x6XKK");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t kk = opcode & 0x00FF;

	V[x] = kk;
}

void Chip8::OP_7xkk()
{
	//COSMAC_WARN("ADD - 0x7XKK");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t kk = opcode & 0x00FF;

	V[x] += kk;
}

void Chip8::OP_8xy0()
{
	//COSMAC_WARN("LD - 0x8XY0");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	V[x] = V[y];
}

void Chip8::OP_8xy1()
{
	//COSMAC_WARN("OR - 0x8XY1");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	V[x] |= V[y];

	V[0xF] = 0;
}

void Chip8::OP_8xy2()
{
	//COSMAC_WARN("AND - 0x8XY2");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	V[x] &= V[y];

	V[0xF] = 0;
}

void Chip8::OP_8xy3()
{
	//COSMAC_WARN("XOR - 0x8XY3");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	V[x] ^= V[y];

	V[0xF] = 0;
}

void Chip8::OP_8xy4()
{
	//COSMAC_WARN("ADD - 0x8XY4");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;
	uint16_t sum = V[x] + V[y];

	V[x] = sum & 0xFF;

	V[0xF] = (sum > 255);
}

void Chip8::OP_8xy5()
{
	//COSMAC_WARN("SUB - 0x8XY5");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;
	uint16_t sub = V[x] - V[y];
	uint8_t carry = V[x] > V[y];

	V[x] = sub & 0x00FF;
	V[0xF] = carry;
}

void Chip8::OP_8xy6()
{
	//COSMAC_WARN("SHR - 0x8XY6");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	uint8_t carry = V[y] & 0x1;

	V[x] = V[y] >> 1;
	
	V[0xF] = carry;
}

void Chip8::OP_8xy7()
{
	//COSMAC_WARN("SUBN - 0x8XY7");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;
	uint16_t sub = V[y] - V[x];
	uint8_t carry = V[y] > V[x];

	V[x] = sub & 0xFF;
	V[0xF] = carry;
}

void Chip8::OP_8xyE()
{
	//COSMAC_WARN("SHL - 0x8XYE");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	uint8_t carry = (V[y] & 0x80) != 0;

	V[x] = V[y] << 1;

	V[0xF] = carry;
}

void Chip8::OP_9xy0()
{
	//COSMAC_WARN("SNE - 0x9XY0");
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	if (V[x] != V[y])
		PC += 2;
}

void Chip8::OP_Annn()
{
	//COSMAC_WARN("LD - 0xANNN");
	I = opcode & 0x0FFF;
}

void Chip8::OP_Bnnn()
{
	//COSMAC_WARN("JP - 0xBNNN");
	PC = (V[0] + (opcode & 0x0FFF)) & 0xFFF;
}

void Chip8::OP_Cxkk()
{
	//COSMAC_WARN("SE - Cxkk");
	uint8_t x = (opcode & 0x0F00) >> 8;

	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distribution(0, 255);

	unsigned int rng = distribution(generator);

	V[x] = rng & (opcode & 0x00FF);
}

void Chip8::OP_Dxyn()
{
	//COSMAC_WARN("DRW - 0xDXYN");

	uint8_t x = V[(opcode & 0x0F00) >> 8] % 64; //sprite x
	uint8_t y = V[(opcode & 0x00F0) >> 4] % 32; //sprite y
	uint8_t n = opcode & 0x000F; //sprite height

	V[0xF] = 0;

	for (auto a = 0; a < n; a++) //y-axis line
	{
		auto byte = memory[I + a];

		for (auto b = 0; b < 8; b++) //x-axis line
		{
			auto target = (x + b) + (y + a) * 64;
			auto source = byte & (0x80 >> b);

			if ((x % 64) + b >= 64 || (y % 32) + a >= 32)
				source = 0;

			if (source != 0)
			{
				if (framebuffer[target] != 0)
					V[0xF] = 1;

				framebuffer[target] ^= 0xFFFFFFFF;
			}
		}
	}
}

void Chip8::OP_Ex9E()
{
	std::string s = decToHex((int)opcode);
	//COSMAC_WARN("{0} ${1} - SKP V{2}", decToHex(PC), s, s[1]);
	uint8_t key = V[(opcode & 0x0F00) >> 8];

	if (keypad[key] == 1)
		PC += 2;
}

void Chip8::OP_ExA1()
{
	std::string s = decToHex((int)opcode);
	//COSMAC_WARN("{0} ${1} - SKNP V{2}", decToHex(PC), s, s[1]);
	uint8_t key = V[(opcode & 0x0F00) >> 8];

	if (keypad[key] == 0)
		PC += 2;
}

void Chip8::OP_Fx07()
{
	//COSMAC_WARN("LD - 0xFx07");
	uint8_t x = (opcode & 0x0F00) >> 8;

	V[x] = DT;
}

void Chip8::OP_Fx0A()
{
	std::string s = decToHex((int)opcode);
	//COSMAC_WARN("LD V{0}, K - ${1}", s[1], s);
	uint8_t x = (opcode & 0x0F00) >> 8;
	
	if (keypad[0])
		V[x] = 0;
	else if (keypad[1])
		V[x] = 1;
	else if (keypad[2])
		V[x] = 2;
	else if (keypad[3])
		V[x] = 3;
	else if (keypad[4])
		V[x] = 4;
	else if (keypad[5])
		V[x] = 5;
	else if (keypad[6])
		V[x] = 6;
	else if (keypad[7])
		V[x] = 7;
	else if (keypad[8])
		V[x] = 8;
	else if (keypad[9])
		V[x] = 9;
	else if (keypad[10])
		V[x] = 10;
	else if (keypad[11])
		V[x] = 11;
	else if (keypad[12])
		V[x] = 12;
	else if (keypad[13])
		V[x] = 13;
	else if (keypad[14])
		V[x] = 14;
	else if (keypad[15])
		V[x] = 15;
	else
		PC -= 2;
}

void Chip8::OP_Fx15()
{
	uint8_t x = (opcode & 0x0F00) >> 8;

	DT = V[x];
}

void Chip8::OP_Fx18()
{
	uint8_t x = (opcode & 0x0F00) >> 8;

	ST = V[x];
}

void Chip8::OP_Fx1E()
{
	uint8_t x = (opcode & 0x0F00) >> 8;

	I += V[x];
}

void Chip8::OP_Fx29()
{
	//COSMAC_WARN("LD F, Vx - 0xFx29");
	uint8_t x = (opcode & 0x0F00) >> 8;

	I = (V[x] & 0x0F) * 5;
}

void Chip8::OP_Fx33()
{
	//COSMAC_WARN("LD - 0xFX33");
	uint8_t x = (opcode & 0x0F00) >> 8;

	memory[I] = (V[x] / 100) % 10;
	memory[I + 1] = (V[x] / 10) % 10;
	memory[I + 2] = V[x] % 10;
}

void Chip8::OP_Fx55()
{
	//COSMAC_WARN("LD - 0xFX55");
	uint8_t x = (opcode & 0x0F00) >> 8;

	for (auto i = 0; i <= x; i++)
	{
		// Might use this later
		//memory[I + i] = V[i];
		memory[I++] = V[i];
	}
}

void Chip8::OP_Fx65()
{
	//COSMAC_WARN("LD - 0xFX65");
	uint8_t x = (opcode & 0x0F00) >> 8;

	for (auto i = 0; i <= x; i++)
	{
		// Might use this later
		// V[i] = memory[I + i];
		V[i] = memory[I++];

	}
}