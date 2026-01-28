#include <cmath>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <chrono>
#include <cstring>
#include <random>
#include <sys/types.h>
#include <cmath>
#include <thread>
#include <SDL2/SDL.h>
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
    void updateTimer();
    void cycle();
    void OP_00E0();
    void OP_00EE();
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XNN();
    void OP_4XNN();
    void OP_5XY0();
    void OP_6XNN();
    void OP_7XNN();
    void OP_8XY0();
    void OP_8XY1();
    void OP_8XY2();
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    void OP_8XYE();
    void OP_9XY0();
    void OP_ANNN();
    void OP_BNNN();
    void OP_CXNN();
    void OP_DXYN();
    void OP_EX9E();
    void OP_EXA1();
    void OP_FX07();
    void OP_FX0A();
    void OP_FX15();
    void OP_FX18();
    void OP_FX1E();
    void OP_FX29();
    void OP_FX33();
    void OP_FX55();
    void OP_FX65();
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
  for(int i=0;i<FONTSET_SIZE;i++){
    memory[0x50+i]=fontset[i];
  }
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
    pc+=2;
  }
}

void Chip8::OP_4XNN(){
  
  uint8_t vx=(opcode & 0x0F00)>>8;
  uint8_t NN =opcode & 0x00FF;

  if(registers[vx]!=NN)
    pc+=2;
}

void Chip8::OP_5XY0(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  if (registers[x]==registers[y] )
    pc+=2;
}

void Chip8::OP_6XNN(){
  uint8_t vx=(opcode & 0x0F00)>>8;
  uint8_t NN =opcode & 0x00FF;
  registers[vx]=NN;
}

void Chip8::OP_7XNN(){
  uint8_t vx=(opcode & 0x0F00)>>8;
  uint8_t NN =opcode & 0x00FF;
  registers[vx]+=NN;
}

void Chip8::OP_8XY0(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  registers[x]=registers[y];
}

void Chip8::OP_8XY1(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  registers[x]=registers[x] | registers[y];
}

void Chip8::OP_8XY2(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  registers[x]=registers[x] & registers[y];
}

void Chip8::OP_8XY3(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  registers[x]=registers[x] ^ registers[y];
}
 
void Chip8::OP_8XY4(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  uint16_t sum=registers[x] + registers[y];
  registers[0xF] = (sum > 255);
  registers[x] = sum & 0xFF;
}


void Chip8::OP_8XY5(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  uint16_t diff=registers[x] - registers[y];
  registers[0xF] = (registers[x] > registers[y]);
  registers[x] = diff & 0xFF;
}

void Chip8::OP_8XY6(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  uint8_t LSB=registers[y] & 1;
  registers[0xF] = LSB;
  registers[x] =   (registers[y] >> 1) ;
}

void Chip8::OP_8XY7(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  uint16_t diff=registers[y] - registers[x];
  registers[0xF] = (registers[y] > registers[x]);
  registers[x] = diff & 0xFF;
}

void Chip8::OP_8XYE(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  uint8_t MSB=(registers[y] >>7) & 1;
  registers[0xF] = MSB;
  registers[x] =   (registers[y] << 1) ;
}

void Chip8::OP_9XY0(){
  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;

  if(registers[x]!=registers[y])
    pc+=2;
}

void Chip8::OP_ANNN(){
  uint16_t NNN=opcode & 0x0FFF;
  index=NNN;
}


void Chip8::OP_BNNN(){
  uint16_t NNN=opcode & 0x0FFF;
  pc= NNN +registers[0];
}


void Chip8::OP_CXNN(){
  uint16_t NN=opcode & 0x00FF;
  u_int8_t x=(opcode & 0x0F00)>>8;
  registers[x]=randByte(randGen) & NN ;
}


void Chip8::OP_DXYN(){

  uint8_t x=(opcode & 0x0F00)>>8;
  uint8_t y=(opcode & 0x00F0)>>4;
  uint8_t N=opcode & 0x000F;
  uint8_t vx=registers[x];
  uint8_t vy=registers[y];
  uint16_t I =index;

        registers[0xF]=0;
  for(int  row=0;row<N;row++){
    uint8_t spriteByte=memory[I+row];
    for(int col =0;col<8;col++){
      uint8_t colbit=(spriteByte & (0x80>>col));
        if (spriteByte & (0x80 >> col))
            {
                uint8_t px = (vx + col) % 64;
                uint8_t py = (vy + row) % 32;
                uint32_t pixelIndex = py * 64 + px;

                if (video[pixelIndex] == 1)
                    registers[0xF] = 1;

                video[pixelIndex] ^= 1;
           
            }
    }
  }
}

void Chip8::OP_EX9E(){
  uint8_t x = (opcode & 0x0F00)>>8;
  uint8_t key = registers[x] & 0x0F;
  if(keypad[key]){
    pc+=2;
  }
}

void Chip8::OP_EXA1(){
  uint8_t x = (opcode & 0x0F00)>>8;
  uint8_t key = registers[x] & 0x0F;
  if(!keypad[key]){
    pc+=2;
  }
}

void Chip8::OP_FX07(){
  uint8_t x=(opcode & 0x0F00)>>8;
  registers[x]=delaytimer;
}

void Chip8::OP_FX0A(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  bool keyPressed = false;
  
  for(int i = 0; i < 16; i++){
    if(keypad[i]){
      registers[Vx] = i;
      keyPressed = true;
      break;
    }
  }
  if(!keyPressed){
    pc -= 2;  // Repeat this instruction
  }
}

void Chip8::OP_FX15(){
  uint8_t x= (opcode & 0x0F00)>>8;
  delaytimer =registers[x];
}


void Chip8::OP_FX18(){
  uint8_t x= (opcode & 0x0F00)>>8;
  soundtimer=registers[x];
}


void Chip8::OP_FX1E(){
  uint8_t x= (opcode & 0x0F00)>>8;
  index =index+registers[x];
}


void Chip8::OP_FX29(){
  uint8_t x= (opcode & 0x0F00)>>8;
    uint8_t digit = registers[x] & 0x0F;
    index = 0x50 + (digit * 5);
}


void Chip8::OP_FX33(){
  uint8_t x= (opcode & 0x0F00)>>8;
  uint8_t value = registers[x]; 
  memory[index]=value/100;
  memory[index+1]=(value%100)/10;
  memory[index+2]=value%10;
}

void Chip8::OP_FX55(){
  uint8_t x= (opcode & 0x0F00)>>8;
  for(int i =0 ;i<=x;i++){
    memory[index+i]=registers[i];
  }
}

void Chip8::OP_FX65(){
  uint8_t x= (opcode & 0x0F00)>>8;
  for(int i =0 ;i<=x;i++){
    registers[i]=memory[index+i];
  }
}

void Chip8::updateTimer(){
  if(delaytimer>0){
    --delaytimer;
  }
  if(soundtimer>0){
    --soundtimer;
  }
}


void drawScreen(SDL_Renderer* renderer, uint32_t* video)
{
    const int SCALE = 20;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            if (video[y * 64 + x])
            {
                SDL_Rect pixel;
                pixel.x = x * SCALE;
                pixel.y = y * SCALE;
                pixel.w = SCALE;
                pixel.h = SCALE;

                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Chip8::cycle()
{
    opcode = (memory[pc] << 8u) | memory[pc + 1];

    pc += 2;

    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode & 0x00FF)
            {
                case 0x00E0: OP_00E0(); break;
                case 0x00EE: OP_00EE(); break;
            }
            break;

        case 0x1000: OP_1NNN(); break;
        case 0x2000: OP_2NNN(); break;
        case 0x3000: OP_3XNN(); break;
        case 0x4000: OP_4XNN(); break;
        case 0x5000: OP_5XY0(); break;
        case 0x6000: OP_6XNN(); break;
        case 0x7000: OP_7XNN(); break;

        case 0x8000:
            switch (opcode & 0x000F)
            {
                case 0x0: OP_8XY0(); break;
                case 0x1: OP_8XY1(); break;
                case 0x2: OP_8XY2(); break;
                case 0x3: OP_8XY3(); break;
                case 0x4: OP_8XY4(); break;
                case 0x5: OP_8XY5(); break;
                case 0x6: OP_8XY6(); break;
                case 0x7: OP_8XY7(); break;
                case 0xE: OP_8XYE(); break;
            }
            break;

        case 0x9000: OP_9XY0(); break;
        case 0xA000: OP_ANNN(); break;
        case 0xB000: OP_BNNN(); break;
        case 0xC000: OP_CXNN(); break;
        case 0xD000: OP_DXYN(); break;

        case 0xE000:
            switch (opcode & 0x00FF)
            {
                case 0x9E: OP_EX9E(); break;
                case 0xA1: OP_EXA1(); break;
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FF)
            {
                case 0x07: OP_FX07(); break;
                case 0x0A: OP_FX0A(); break;
                case 0x15: OP_FX15(); break;
                case 0x18: OP_FX18(); break;
                case 0x1E: OP_FX1E(); break;
                case 0x29: OP_FX29(); break;
                case 0x33: OP_FX33(); break;
                case 0x55: OP_FX55(); break;
                case 0x65: OP_FX65(); break;
            }
            break;
    }
}


int main()
{
    Chip8 chip8;
    chip8.LoadRom("../Chip-8/games/TETRIS");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        64 * 20,
        32 * 20,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    auto lastTimer = std::chrono::high_resolution_clock::now();


    while (running)
    {

      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        if (event.type == SDL_QUIT)
            running = false;

        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            bool pressed = (event.type == SDL_KEYDOWN);

            switch (event.key.keysym.sym)
            {
                case SDLK_1: chip8.keypad[0x1] = pressed; break;
                case SDLK_2: chip8.keypad[0x2] = pressed; break;
                case SDLK_3: chip8.keypad[0x3] = pressed; break;
                case SDLK_4: chip8.keypad[0xC] = pressed; break;

                case SDLK_q: chip8.keypad[0x4] = pressed; break;
                case SDLK_w: chip8.keypad[0x5] = pressed; break;
                case SDLK_e: chip8.keypad[0x6] = pressed; break;
                case SDLK_r: chip8.keypad[0xD] = pressed; break;

                case SDLK_a: chip8.keypad[0x7] = pressed; break;
                case SDLK_s: chip8.keypad[0x8] = pressed; break;
                case SDLK_d: chip8.keypad[0x9] = pressed; break;
                case SDLK_f: chip8.keypad[0xE] = pressed; break;

                case SDLK_z: chip8.keypad[0xA] = pressed; break;
                case SDLK_x: chip8.keypad[0x0] = pressed; break;
                case SDLK_c: chip8.keypad[0xB] = pressed; break;
                case SDLK_v: chip8.keypad[0xF] = pressed; break;
            }
        }
      }


      chip8.cycle();
      drawScreen(renderer, chip8.video);

        // 🔹 TIMERS (60Hz)
        auto now = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<double>(now - lastTimer).count() >= 1.0 / 60.0)
        {
            chip8.updateTimer();
            lastTimer = now;
        }

        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
