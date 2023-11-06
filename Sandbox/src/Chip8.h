#pragma once

#include "COSMAC.h"

#include <stack>
#include <array>
#include <string>

class Chip8
{
public:
	std::array<uint32_t, 64 * 32> framebuffer;
	std::array<uint16_t, 16> keypad;

	Chip8();
	~Chip8() {}

	void Restart();
	bool LoadProgram(const std::string& path);
	void FetchOpcode();
	void Tick();
	void UpdateTimers();

	//aux fxns
	std::string decToHex(int num);

	bool PAUSE = true;
	bool DRAW = false;

private:
	std::array<uint8_t, 4096> memory;
	std::array<uint8_t, 16> V;
	std::stack<uint16_t> stack;

	uint16_t I;
	uint16_t PC;
	uint8_t SP;
	uint8_t DT, ST;

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