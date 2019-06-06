#pragma once
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <semaphore.h>
#include "elementos.h"
#include "AbrirSemaforos.cpp"
using namespace std;

int
main(void) {
  string nombre = "buffer";
  char *dir = abrirMemoria(nombre);
  struct header *pHeader = (struct header *) dir;
  
    int i  = pHeader->i;

  int h = 0;
  string mut = "Mut";
  string llen = "LLen";
  string vac = "Vac";

  sem_unlink("vacios");
  sem_unlink("llenos");
  sem_unlink("mutex");
  shm_unlink("/buffer");

  while(h < i){ 
    
    ostringstream namemut;
    namemut << mut << h;
    string realNameMut(namemut.str());
    sem_unlink(realNameMut.c_str());

    ostringstream namellen;
    namellen << llen << h;
    string realNameLlen(namellen.str());
    sem_unlink(realNameLlen.c_str());
    
    ostringstream namevac;
    namevac << vac << h;
    string realNameVac(namevac.str());
    sem_unlink(realNameVac.c_str());
    h++;
  }
  return EXIT_SUCCESS;
}