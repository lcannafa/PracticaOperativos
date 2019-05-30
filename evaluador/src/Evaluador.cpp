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

    const char* n = "evaluator";
    // crea el espacio de memoria compartida, lanza error si ya existe
    int fd = shm_open(n, O_RDWR | O_CREAT | O_EXCL, 0660);

    if (fd < 0) {
    cerr << "Error creando la memoria compartida: 1 "
	 << errno << strerror(errno) << endl;
    exit(1);
  }


  if (ftruncate(fd, sizeof(struct BufferI)) != 0) {
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

  
  struct BufferIE test;

  struct BufferI *pBufferI = (struct BufferI *) dir;
  pBufferI->bufferI[0] = test;

  close(fd);
}
