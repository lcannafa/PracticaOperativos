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
#include "IniciadorMemoria.cpp"

using namespace std;

int
main() {
  
  struct registroentrada registro;
  int ie = 3;
  int oe = 3;
  //sem_t *vacios, *llenos, *mutex;
  int i = 3;
  int n = 0;
  int m = 0;
  //vacios = sem_open("vacios", 0);
  //llenos = sem_open("llenos", 0);
  //mutex  = sem_open("mutex", 0);

  crearEspacio(i,ie,oe);

  int fd = shm_open("/buffer", O_RDWR, 0660);

  if (fd < 0) {
    cerr << "Error abriendo la memoria compartida: 4"
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  int *dir;

  if ((dir = (int *)(mmap(NULL, (sizeof(struct registroentrada)* i * ie ) + (sizeof(struct registrosalida) * oe) , PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0))) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida: 5"
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  int *inicio = dir;
  struct registroentrada *pRegistro = (struct registroentrada *) dir;
  
  for(;;) {
    //sem_wait(vacios);
    //sem_wait(mutex);
    while( n < i){
    dir = ( inicio + (sizeof(struct registroentrada) * (i * ie))  +  (n * sizeof(struct registroentrada))   );
    struct registroentrada *pRegistro = (struct registroentrada *) dir;
    pRegistro->id = n;
    cout << "Estamos en bello"<< endl;
    //cout << n                 << endl;
    while( m < ie){
      cout << pRegistro->id     << endl;
      m++;
    }
    
    n++;
    m=0;
    }
    
    
    
    //sem_post(mutex);
    //sem_post(llenos);
    sleep(2);
  }

  return EXIT_SUCCESS;
}
