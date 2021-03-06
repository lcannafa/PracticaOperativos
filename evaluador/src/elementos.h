#pragma once
#include <semaphore.h>

//Header ingresado por consola
struct header
{
  int i;
  int ie;
  int oe;
  int q;
  int b;
  int d;
  int s;
  char n[30];
  int contador;
};

//
struct headerQ
{
  int q;
  int i;
  int b;
  int d;
  int s;
};

//Contenido de una posicion ie en la bandeja de entrada
struct registroentrada
{
  int bandeja;
  int id;
  char tipo;
  int cantidad;
  int time;
};

// externo -> nombrado
// interno -> anonimo
//Contenido de una posicion en la bandeja de salida.
struct registrosalida
{
  int id;
  int bandeja;
  char tipo;
  int cantidad;
  int time;
  char resultado;
};
