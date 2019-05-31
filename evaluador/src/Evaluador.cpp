#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "elementos.h"

using namespace std;



int
main(void) {

    // variables a modificar en la ejecución
    const char* n = "evaluator";
    int i = 5;
    int ie = 6;


    // crea el espacio de memoria compartida, lanza error si ya existe el mode_t define derechos 0660 el usuario puede leer y escribir
    int fd = shm_open(n, O_RDWR | O_CREAT | O_EXCL, 0660);

    if (fd < 0) {
    cerr << "Error creando la memoria compartida: 1 "
	  << errno << strerror(errno) << endl;
    exit(1);
  }

    // para evitar error truncamos para el tamaño requerido
    if (ftruncate(fd, sizeof(struct elemento) * i * ie  ) != 0) {
    cerr << "Error creando la memoria compartida: 2 "
	  << errno << strerror(errno) << endl;
    exit(1);
  }

  void *dir;
  // mapea el espacio de memoria reservado y guarda la dirreción en el apuntador dir
  if ((dir = mmap(NULL, sizeof(struct elemento)*i*ie, PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida: 3 "
	  << errno << strerror(errno) << endl;
    exit(1);
  }

  close(fd);
}
