#pragma once
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include "elementos.h"
#include "AbrirMemoria.cpp"
#include "BandejasQ.cpp"
//Aca estoy io
//Aca estamos los melos
using namespace std;
// función que le entregan un registro a guardar en la memoria compartida de nombre

registrosalida retirarRegistroDeQ(struct registrosalida registroSalida,char tipo, string nombre)
{
  //accede a la memoria compartida
  // posición inicial
  char *dir = abrirMemoria(nombre);
  char *direx = abrirQ(nombre);
  struct header *pHeader = (struct header *)dir;

  int ie = pHeader->ie;
  int q = pHeader->q;
  int i = pHeader->i;
  //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor de las bandejas
  sem_t *arrayMut, *arrayVacio, *arrayLleno;
    int tipopipo;
    if (registroSalida.tipo == 'B')
    {
        tipopipo = i;
    }
    if (registroSalida.tipo == 'S')
    {
        tipopipo = i + 2;
    }
    if (registroSalida.tipo == 'D')
    {
        tipopipo = i + 1;
    }
  string mutex = "Mut" + nombre + to_string(tipopipo);
  string vacio = "Vacio" + nombre + to_string(tipopipo);
  string lleno = "Lleno" + nombre + to_string(tipopipo);
  cout << "pene marco" << endl;
  cout << mutex << endl;
  cout << vacio << endl;
  cout << lleno << endl;
  cout << "pene marco otra vez xd" << endl;
  arrayMut = sem_open(mutex.c_str(), 0);
  arrayVacio = sem_open(vacio.c_str(), 1);
  arrayLleno = sem_open(lleno.c_str(), 0);

  // variable para recorrer la bandeja
  int recorrido = 0;

  // posición inicial de la bandeja i
  char *pos = (tipopipo * sizeof(registrosalida)) + dir + sizeof(struct header);

  //Crear el registro de salida que d
  //char *pos = dir + sizeof(struct header);
  cout << "pipi james 1" << endl;
  struct registrosalida registro;
  cout << "pipi james 1.5" << endl;
  //hasta que no logre insertar intentar
  // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
  sem_wait(arrayLleno);
  cout << "pipi james 1.7" << endl;
  sem_wait(arrayMut);
  cout << "pipi james 2" << endl;
  // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
  while (recorrido < q)
  {
    cout << "pipi james 3" << endl;
    //posición en la bandeja
    char *posn = (pos + (recorrido * sizeof(registroentrada)));
    struct registroentrada *pRegistro = (struct registroentrada *)posn;
    cout << "pipi james 4" << endl;
    //si encuentro elemento a retirar
    if (pRegistro->cantidad > 0)
    {
      cout << "pipi james 5" << endl;
      //asigno los valores a devolver
      registro.cantidad = pRegistro->cantidad;
      registro.id = pRegistro->id;
      registro.tipo = pRegistro->tipo;

      //Pongo basura donde estaba
      //pRegistro->bandeja = bandeja;
      pRegistro->id = -1;
      pRegistro->tipo = 'a';
      pRegistro->cantidad = -1;
      cout << "pipi james 6" << endl;
      sem_post(arrayMut);
      sem_post(arrayVacio);

      return registro;
    }
    else
    {
      recorrido++;
    }
    cout << "pipi james 7" << endl;
    return registro;
  }
}