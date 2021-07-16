// #include "main.hpp"
// #include "logs.hpp"
#include "readwrite.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void fooLoop()
{
  while (true)
  {
    write(400, read(400) + 1);
    Delay(200);
  }
}

#ifdef __cplusplus
}
#endif

// #include "main.cpp"
// #include "logs.cpp"
#include "readwrite.cpp"
