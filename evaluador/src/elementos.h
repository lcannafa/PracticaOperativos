#pragma once

#define ie 6
#define i 5


struct elemento {
  int registro[2];
};

struct BufferIE {
  int entra;
  int sale;
  int contiene;
  struct elemento bufferIE[ie];
};

struct BufferI {
  struct BufferIE bufferI[i];
};