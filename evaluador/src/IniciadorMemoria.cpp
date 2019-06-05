#pragma once
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "elementos.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

int
crearEspacio(string nombre ,int i, int ie, int oe) {


  size_t memorysize = (sizeof(struct registroentrada)* i * ie) + (sizeof(struct registrosalida) * oe);
  // instanciar los semaforos, el valor define cuanto empieza
  sem_t *vacios = sem_open("vacios", O_CREAT | O_EXCL, 0660, i*ie);
  sem_t *llenos = sem_open("llenos", O_CREAT | O_EXCL, 0660, 0);
  sem_t *mutex  = sem_open("mutex", O_CREAT | O_EXCL, 0660, 1);


  // Abrir espacio de memoria para usar, usando el nombre n
  int fd = shm_open( nombre.c_str(), O_RDWR | O_CREAT | O_EXCL, 0660);

  if (fd < 0) {
    cerr << "Error creando la memoria compartida: 1"
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  if (ftruncate(fd, memorysize != 0) ) {
    cerr << "Error creando la memoria compartida: 2"
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  void *dir;

  if ((dir = mmap(NULL, memorysize , PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida: 3"
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  close(fd);

  return EXIT_SUCCESS;
}