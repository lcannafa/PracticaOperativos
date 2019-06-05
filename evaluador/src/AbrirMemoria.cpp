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

using namespace std;

int* abrirMemoria(string nombre, int i, int ie, int oe){
    int fd = shm_open(nombre.c_str(), O_RDWR, 0660);
    if (fd < 0) {
    cerr << "Error abriendo la memoria compartida: 4"
	 << errno << strerror(errno) << endl;
    exit(1);
  }
  int *dir;
  if ((dir = (int *)(mmap(NULL, (sizeof(struct registroentrada)* i * ie) + (sizeof(struct registrosalida) * oe), PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0))) == MAP_FAILED) {
      cerr << "Error mapeando la memoria compartida: 5"
	         << errno << strerror(errno) << endl;
           exit(1);
  }
  return dir;
}