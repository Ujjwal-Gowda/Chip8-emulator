#include <cstdint>
#include <fstream>
#include <ios>
#include <string>
class Chip8{
  public:
    std::uint8_t registers[8]{};
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
    
    void LoadRom(const std::string& filename);
};
  

void Chip8::LoadRom(const std::string& filename)
{
  std::ifstream file(filename , std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0,std::ios::beg);
  if(size>(4096-0x200)){
    return;
  };
  file.read(reinterpret_cast<char*>(&memory[0x200]), size);
}


int main () {
  Chip8 chip8;
  chip8.LoadRom("pong.ch8");
  return 0;
}

