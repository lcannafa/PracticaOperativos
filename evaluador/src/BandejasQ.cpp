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

int crearQ(string nombre, int q)
{

    // Abrir espacio de memoria para usar, usando el nombre n
    nombre = nombre +"Q";
    int fd = shm_open(nombre.c_str(), O_RDWR | O_CREAT | O_EXCL, 0660);

    if (fd < 0)
    {
        cerr << "Error creando la memoria compartida: Q1"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    if (ftruncate(fd, sizeof(struct headerQ) != 0))
    {
        cerr << "Error creando la memoria compartida: Q2"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    char *dir;

    if ((dir = (char *)mmap(NULL, sizeof(struct headerQ), PROT_READ | PROT_WRITE, MAP_SHARED,
                            fd, 0)) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q3"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    struct headerQ *pHeader = (struct headerQ *)dir;
    pHeader->q = q;
    close(fd);
    return EXIT_SUCCESS;
}

// Permite abrir el espacio de memoria compartida Q y devuelve la posición inicial
char *abrirQ(string nombre)
{
    nombre = "/" + nombre + "Q";
    int fd = shm_open(nombre.c_str(), O_RDWR, 0660);
    if (fd < 0)
    {
        cerr << "Error abriendo la memoria compartida: Q4"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    char *dir;

    if ((dir = (char *)(mmap(NULL, sizeof(struct headerQ), PROT_READ | PROT_WRITE, MAP_SHARED,
                             fd, 0))) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q5"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    struct headerQ *pHeader = (struct headerQ *)dir;
    int q = pHeader->q;

    munmap((void *)pHeader, sizeof(struct headerQ));
    size_t memorysize = sizeof(struct headerQ) + (sizeof(struct registrosalida) * q * 3);

    if ((dir = (char *)(mmap(NULL, memorysize, PROT_READ | PROT_WRITE, MAP_SHARED,
                             fd, 0))) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q6"
             << errno << strerror(errno) << endl;
        exit(1);
    }
}