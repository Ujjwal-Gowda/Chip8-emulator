#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <chrono>
#include <cstring>
#include <random>
#include <stack>
class Chip8{
  public:
  
  Chip8();
	std::default_random_engine randGen;
	std::uniform_int_distribution<uint8_t> randByte;
    std::uint8_t registers[16]{};
    std::uint8_t memory[4096]{};
    std::uint16_t index{};
    std::uint16_t pc{};
    std::uint8_t sp{};
    std::uint16_t stack[16]{};
    std::uint8_t delaytimer{};
    std::uint16_t keypad[16]{};
    std::uint8_t soundtimer{};
    std::uint32_t video[64*32]{};
    std::uint16_t opcode;
    void OP_00E0();
    void OP_00EE();
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XNN();
    void LoadRom(const std::string& filename);
};
  
void Chip8::LoadRom(const std::string& filename)
{
  std::ifstream file(filename , std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    std::cout<<"failed to load file"<<std::endl;
  }
  std::streamsize size = file.tellg();
  file.seekg(0,std::ios::beg);
  if(size>(4096-0x200)){
    return;
  };
  std::cout<<size<<std::endl;

  file.read(reinterpret_cast<char*>(&memory[0x200]), size);
}

Chip8::Chip8()
    : randGen(std::chrono::system_clock::now().time_since_epoch().count()),
      randByte(0, 255),
      pc(0x200)
{
}

void Chip8::OP_00E0()
{
  memset(video,0,sizeof(video));
}


void Chip8::OP_00EE()
{
  --sp;
  pc=stack[sp];
}



void Chip8::OP_1NNN()
{
  uint16_t address =opcode & 0x0FFFu;
  pc=address;
}

void Chip8::OP_2NNN()
{
  uint16_t address =opcode & 0x0FFFu;
  stack[sp]=pc;
  ++sp;
  pc=address;
}

void Chip8::OP_3XNN(){
  
  uint8_t vx=(opcode & 0x0F00)>>8;
  uint8_t NN =opcode & 0x00FF;

  if(registers[vx]==NN){
    pc+=4;
  }else{
    pc+=2;
  }
}




const unsigned int FONTSET_SIZE = 80;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

int main () {
  Chip8 chip8;
  chip8.LoadRom("../Chip-8/games/TETRIS");

  return 0;
}

