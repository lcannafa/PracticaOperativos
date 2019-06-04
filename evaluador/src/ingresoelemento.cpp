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

using namespace std;

int
ingresarRegistro(struct registroentrada registro, string nombre, int i, int ie, int oe) {
  

  //sem_t *vacios, *llenos;
  sem_t *mutex;
  //vacios = sem_open("vacios", 0);
  //llenos = sem_open("llenos", 0);
  mutex  = sem_open("mutex", 0);

  nombre = "/" + nombre;
  //accede a la memoria compartida
  int fd = shm_open(nombre.c_str(), O_RDWR, 0660);

  if (fd < 0) {
    cerr << "Error abriendo la memoria compartida: 4"
	 << errno << strerror(errno) << endl;
    exit(1);
  }
  // posición inicial
  int *dir;
  bool insertado = false;

  // saca en dir la posicion inicial del espacio de memoria
  if ((dir = (int *)(mmap(NULL, (sizeof(struct registroentrada)* i * ie ) + (sizeof(struct registrosalida) * oe) /* + (sizeof(struct variablesExtra))**/, PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0))) == MAP_FAILED) {
      cerr << "Error mapeando la memoria compartida: 5"
	         << errno << strerror(errno) << endl;
           exit(1);
  }
  // variable para recorrer la bandeja
  int n = 0;
  // posición inicial de la bandeja i
  int *pos = (registro.bandeja * ie * sizeof(registroentrada)) + dir;

    

  for(;;) {

    //hasta que no logre insertar intentar
    while(!insertado){
    // Espera la semaforo para insertar
      sem_wait(mutex);
      // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
      while(n < ie){

       //posición en la bandeja
       int *posn = (pos + (n*sizeof(registroentrada)));
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
int recorrer(string nombre, int i, int ie ,int oe){
  int temp1 = 0;
  int temp2 = 0;


  // Sube los semaforos
  //sem_t *vacios, *llenos;
  sem_t *mutex;
  //vacios = sem_open("vacios", 0);
  //llenos = sem_open("llenos", 0);
  mutex  = sem_open("mutex", 0);
 
  // open del espacio compartida
  nombre = "/" + nombre;
  int fd = shm_open(nombre.c_str(), O_RDWR, 0660);

  if (fd < 0) {
    cerr << "Error abriendo la memoria compartida: 4"
	 << errno << strerror(errno) << endl;
    exit(1);
  }
  // posición inicial
  int *dir;
  bool insertado = false;

  // saca en dir la posicion inicial del espacio de memoria
  if ((dir = (int *)(mmap(NULL, (sizeof(struct registroentrada)* i * ie ) + (sizeof(struct registrosalida) * oe) /* + (sizeof(struct variablesExtra))**/, PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0))) == MAP_FAILED) {
      cerr << "Error mapeando la memoria compartida: 5"
	         << errno << strerror(errno) << endl;
           exit(1);
  }


  while (temp1 < i){
    int *pos = (temp1 * ie * sizeof(registroentrada)) + dir;
    while(temp2 < ie){
    int *posn = (pos + (temp2 *sizeof(registroentrada)));
    struct registroentrada *pRegistro = (struct registroentrada *) posn;
    cout << pRegistro->id << pRegistro->tipo << pRegistro->cantidad << endl;
    temp2++;
    }
    temp1++;
    temp2 = 0;
  }
  return 0;
}
