#pragma once

#include <array>

class Chip8
{
public:
	std::array<uint32_t, 64 * 32> framebuffer;

	Chip8();
	~Chip8();

	bool LoadProgram();
	void FetchOpcode();
	void Tick();

private:
	// 0x000 - 0x1FF - Chip 8 interpreter(contains font set in emu)
	// 0x050 - 0x0A0 - Used for the built in 4x5 pixel font set(0 - F)
	// 0x200 - 0xFFF - Program ROM and work RAM
	std::array<uint8_t, 4096> memory;

	//16 general purpose 8-bit registers
	std::array<uint8_t, 16> V;

	//16 16-bit value stack
	std::array<uint16_t, 16> stack;

	uint16_t I;
	uint16_t PC;
	uint8_t SP;
	uint8_t DT;
	uint8_t ST;

	uint16_t opcode;

	void OP_00E0();
	void OP_00EE();

	void OP_1nnn();
	void OP_2nnn();
	void OP_3xkk();
	void OP_4xkk();
	void OP_5xy0();
	void OP_6xkk();
	void OP_7xkk();

	void OP_8xy0();
	void OP_8xy1();
	void OP_8xy2();
	void OP_8xy3();
	void OP_8xy4();
	void OP_8xy5();
	void OP_8xy6();
	void OP_8xy7();
	void OP_8xyE();

	void OP_9xy0();
	void OP_Annn();
	void OP_Bnnn();
	void OP_Cxkk();
	void OP_Dxyn();

	void OP_ExA1();
	void OP_Ex9E();

	void OP_Fx07();
	void OP_Fx0A();
	void OP_Fx15();
	void OP_Fx18();
	void OP_Fx1E();
	void OP_Fx29();
	void OP_Fx33();
	void OP_Fx55();
	void OP_Fx65();
};