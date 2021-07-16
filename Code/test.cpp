#include <iostream>
#include <functional>
#include <stdint.h>

uint16_t PSW[4096] = {1};
uint16_t PSB[64] = {1};

template<typename T>
class cell
{
protected:
  const uint32_t adr = 0;
  std::function<T (const uint32_t)> read;
  std::function<void (const uint32_t adr, const T value)> write;
    
  cell(const uint32_t adr, 
      std::function<T (const uint32_t)> read,
      std::function<void (const uint32_t, const T value)> write,
      std::function<bool (const uint32_t, const uint16_t, T *)> reads,
      std::function<bool (const uint32_t, const uint16_t, T *)> writes)
    : adr(adr), read(read), write(write), reads(reads), writes(writes)
  { }

public:
  std::function<bool (const uint32_t, const uint16_t, T *)> reads;
  std::function<bool (const uint32_t, const uint16_t, T *)> writes;

  void operator=(const T value) 
  { 
    this->write(this->adr, value); 
  }
  operator T() 
  { 
    return this->read(this-> adr); 
  }
  cell operator[](const uint32_t adr) 
  { 
    return cell(adr, this->read, this->write, this->reads, this->writes); 
  }
};

class psb_t : public cell<bool>
{
public:
  psb_t(void) : cell(0, 
    [](const uint32_t adr)->bool { return PSB[adr/16] & (1 << (adr%16)); }, 
    [](const uint32_t adr, const bool value)->void 
    { 
      PSB[adr/16] = value ? PSB[adr/16] | (1 << (adr%16)) : PSB[adr/16] & ~(1 << (adr%16)); 
    }, 
    [&](const uint32_t adr, const uint16_t cnt, bool * ptr)->bool 
    { 
      for(size_t i = 0; i < cnt; i++) ptr[i] = this->read(adr+i);
      return true;
    }, 
    [&](const uint32_t adr, const uint16_t cnt, bool * ptr)->bool
    { 
      for(size_t i = 0; i < cnt; i++) this->operator[](adr+i) = ptr[i];
      return true;
    })
  { }
};

class psw_t : public cell<uint16_t>
{
public:
  psw_t(void) : cell(0, 
    [](const uint32_t adr)->uint16_t { return PSW[adr]; }, 
    [](const uint32_t adr, const uint16_t value)->void { PSW[adr] = value; }, 
    [](const uint32_t adr, const uint16_t cnt, uint16_t * ptr)->bool 
    { 
      for(size_t i = 0; i < cnt; i++) ptr[i] = PSW[adr+i];
      return true;
    }, 
    [](const uint32_t adr, const uint16_t cnt, uint16_t * ptr)->bool
    { 
      for(size_t i = 0; i < cnt; i++) PSW[adr+i] = ptr[i];
      return true;
    })
  { }
};

using namespace std;

psb_t psb;
psw_t psw;

int main()
{

  bool buff[100] = {0};
  for(int i = 0; i < 100; i++)
  {
    buff[i] = rand() % 2;
  }
  psb.writes(20, 100, buff);

  bool writeBuff[100] = {0};
  psb.reads(20, 100, writeBuff);
  for(int i = 0; i < 100; i++)
  {
    cout << writeBuff[i] << " " << buff[i] << endl;
  }

  return 0;
}