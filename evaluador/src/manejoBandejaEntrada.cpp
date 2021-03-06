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

using namespace std;
// función que le entregan un registro a guardar en la memoria compartida de nombre

int ingresarRegistro(registroentrada registro, string nombre)
{

  //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor
  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  string mutex = "Mut" + nombre + to_string(registro.bandeja);
  string vacio = "Vacio" + nombre + to_string(registro.bandeja);
  string lleno = "Lleno" + nombre + to_string(registro.bandeja);
  arrayMut = sem_open(mutex.c_str(), 0);
  arrayVacio = sem_open(vacio.c_str(), 1);
  arrayLleno = sem_open(lleno.c_str(), 0);

  //accede a la memoria compartida
  // posición inicial
  char *dir = abrirMemoria(nombre);
  bool insertado = false;

  header *pHeader = (header *)dir;

  int i = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;
  int contador = pHeader->contador;

  // variable para recorrer la bandeja
  int recorrido = 0;

  // posición inicial de la bandeja i
  char *pos = (registro.bandeja * ie * sizeof(registroentrada)) + dir + sizeof(header);

  //hasta que no logre insertar intentar
  //Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
  //Soy consumidor
  sem_wait(arrayVacio);
  sem_wait(arrayMut);

  // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
  while (recorrido < ie)
  {
    //posición en la bandeja
    char *posn = (pos + (recorrido * sizeof(registroentrada)));
    registroentrada *pRegistro = (registroentrada *)posn;

    //si logra insertar se sale
    if (pRegistro->cantidad <= 0)
    {
      pRegistro->bandeja = registro.bandeja;
      pRegistro->id = registro.id;
      pRegistro->tipo = registro.tipo;
      pRegistro->cantidad = registro.cantidad;
      pHeader->contador = pHeader->contador + 1;
      pRegistro->time = rand() % ((10 + 1) - 1);
      //Soy productor
      sem_post(arrayMut);
      sem_post(arrayLleno);
      return EXIT_SUCCESS;
    }
    // sino sigue avanzando
    else
    {
      recorrido++;
    }
  }

  return 1;
}

// Método que imprime el contenido de las bandejas de entrada
int recorrer(string nombre)
{
  int temp1 = 0;
  int temp2 = 0;

  // posición inicial
  char *dir = abrirMemoria(nombre);
  bool insertado = false;
  header *pHeader = (header *)dir;

  int i = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;
  int contador = pHeader->contador;

  while (temp1 < i)
  {
    char *pos = (temp1 * ie * sizeof(registroentrada)) + dir + sizeof(header);
    while (temp2 < ie)
    {
      char *posn = (pos + (temp2 * sizeof(registroentrada)));
      registroentrada *pRegistro = (registroentrada *)posn;
      if (pRegistro->cantidad > 0)
      {
        cout << "[" << pRegistro->id << " " << pRegistro->bandeja << " " << pRegistro->tipo << " " << pRegistro->cantidad << "]" << endl;
      }
      temp2++;
    }
    temp1++;
    temp2 = 0;
  }
  return 0;
}

// función que le entregan un registro a guardar en la memoria compartida de nombre

registrosalida retirarRegistro(int bandeja, string nombre)
{

  //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor de las bandejas
  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  string mutex = "Mut" + nombre + to_string(bandeja);
  string vacio = "Vacio" + nombre + to_string(bandeja);
  string lleno = "Lleno" + nombre + to_string(bandeja);
  arrayMut = sem_open(mutex.c_str(), 0);
  arrayVacio = sem_open(vacio.c_str(), 1);
  arrayLleno = sem_open(lleno.c_str(), 0);

  //accede a la memoria compartida
  // posición inicial
  char *dir = abrirMemoria(nombre);
  header *pHeader = (header *)dir;

  int i = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;
  int contador = pHeader->contador;

  // variable para recorrer la bandeja
  int recorrido = 0;

  // posición inicial de la bandeja i
  char *pos = (bandeja * ie * sizeof(registroentrada)) + dir + sizeof(header);

  //Crear el registro de salida que devolver
  registrosalida registro;

  //hasta que no logre insertar intentar
  // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
  //Soy Productor
  sem_wait(arrayLleno);
  sem_wait(arrayMut);

  // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
  while (recorrido < ie)
  {
    //posición en la bandeja
    char *posn = (pos + (recorrido * sizeof(registroentrada)));
    registroentrada *pRegistro = (registroentrada *)posn;

    //si encuentro elemento a retirar
    if (pRegistro->cantidad > 0)
    {

      //asigno los valores a devolver
      registro.cantidad = pRegistro->cantidad;
      registro.id = pRegistro->id;
      registro.tipo = pRegistro->tipo;
      registro.bandeja = pRegistro->bandeja;
      registro.time = pRegistro->time;

      //Pongo basura donde estaba
      pRegistro->bandeja = bandeja;
      pRegistro->id = 0;
      pRegistro->tipo = '0';
      pRegistro->cantidad = 0;
      //soy consumidor
      sem_post(arrayMut);
      sem_post(arrayVacio);

      return registro;
    }
    // sino sigue avanzando
    else
    {
      recorrido++;
    }
  }

  return registro;
}

int retornarContador(string nombre)
{

  //accede a la memoria compartida
  // posición inicial
  char *dir = abrirMemoria(nombre);
  header *pHeader = (header *)dir;
  int contador = pHeader->contador;
  //cout << "Se ha ingesado un total de : " << pHeader->contador << " registros" << endl;
  return 0;
}
