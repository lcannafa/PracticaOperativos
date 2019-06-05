#pragma once
#include <semaphore.h>

struct registroentrada {
  int bandeja;
  int id;
  char tipo;
  int cantidad;
};

// externo -> nombrado
// interno -> anonimo
struct registrosalida{
  int id;
  char tipo;
  int cantidad;
};

struct semaforos{
 sem_t sem;
};
