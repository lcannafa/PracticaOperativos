#pragma once
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include "elementos.h"
#include "AbrirMemoria.cpp"

using namespace std;
// función que le entregan un registro a guardar en la memoria compartida de nombre, pero para facil manipulación requiere i ie y oe de dicha memoria
int
ingresarRegistro(struct registroentrada registro, string nombre) {
  

  sem_t *mutex;
  mutex  = sem_open("mutex", 0);

  //accede a la memoria compartida
  // posición inicial
  char *dir = abrirMemoria(nombre);
  bool insertado = false;

  struct header *pHeader = (struct header *) dir;
  
    int i  = pHeader->i;
    int ie = pHeader->ie;
    int oe = pHeader->oe;

  // variable para recorrer la bandeja
  int n = 0;
  // posición inicial de la bandeja i
  char *pos = (registro.bandeja * ie * sizeof(registroentrada)) + dir + sizeof(struct header);

    

  for(;;) {

      //hasta que no logre insertar intentar
      while(!insertado){
      // Espera la semaforo para insertar
      sem_wait(mutex);
      // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
      while(n < ie){

       //posición en la bandeja
       char *posn = (pos + (n*sizeof(registroentrada)));
       struct registroentrada *pRegistro = (struct registroentrada *) posn;

        //si logra insertar se sale
        if(pRegistro->cantidad <= 0  && !insertado){
         pRegistro->bandeja = registro.bandeja;
         pRegistro->id = registro.id;
         pRegistro->tipo = registro.tipo;
         pRegistro->cantidad = registro.cantidad;
         insertado = true;
         sem_post(mutex);
         return 0;
        }
       // sino sigue avanzando
       else{n++;}
      }
       // si no logro insertar pero recorri la bandeja notifico y espero
      if(n >= ie && !insertado){
      cout << "registro full" << endl;
      sem_post(mutex);
      sleep(5);
      n = 0;
      }
      if(insertado){
      sem_post(mutex);
      return 0;
      }
    }
  }
    return EXIT_SUCCESS;
}

// Método que imprime el contenido de las bandejas de entrada
int recorrer(string nombre){
  int temp1 = 0;
  int temp2 = 0;


  // Sube los semaforos
  sem_t *mutex;
  mutex  = sem_open("mutex", 0);
 
  // posición inicial
  char *dir = abrirMemoria(nombre);
  bool insertado = false;
  struct header *pHeader = (struct header *) dir;
  
    int i  = pHeader->i;
    int ie = pHeader->ie;
    int oe = pHeader->oe;


  while (temp1 < i){
    char *pos = (temp1 * ie * sizeof(registroentrada)) + dir + sizeof(struct header);
    while(temp2 < ie){
    char *posn = (pos + (temp2 *sizeof(registroentrada)));
    struct registroentrada *pRegistro = (struct registroentrada *) posn;
    cout << pRegistro->id << pRegistro->tipo << pRegistro->cantidad << endl;
    temp2++;
    }
    temp1++;
    temp2 = 0;
  }
  return 0;
}
