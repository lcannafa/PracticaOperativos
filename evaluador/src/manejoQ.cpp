#pragma once
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include "elementos.h"
#include "AbrirMemoria.cpp"

using namespace std;

int crearQ(string nombre)
{
    //accede a la memoria compartida
    // posición inicial
    char *dir = abrirMemoria(nombre);
    header *pHeader = (header *)dir;
    int q = pHeader->q;
    int i = pHeader->i;
    int b = pHeader->b;
    int d = pHeader->d;
    int s = pHeader->s;

    // Abrir espacio de memoria para usar, usando el nombre n
    nombre = nombre + "Q";

    int fd = shm_open(nombre.c_str(), O_RDWR | O_CREAT | O_EXCL, 0660);
    if (fd < 0)
    {
        cerr << "Error creando la memoria compartida: Q1"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    //Acorta la region de memoria, de acuerdo al tamaño requerido
    if (ftruncate(fd, sizeof(headerQ) != 0))
    {
        cerr << "Error creando la memoria compartida: Q2"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    char *dirQ;

    if ((dirQ = (char *)mmap(NULL, sizeof(headerQ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q3"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    headerQ *pHeaderQ = (headerQ *)dirQ;
    pHeaderQ->q = q;
    pHeaderQ->i = i;
    pHeaderQ->b = b;
    pHeaderQ->d = d;
    pHeaderQ->s = s;

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

    if ((dir = (char *)(mmap(NULL, sizeof(headerQ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q5"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    headerQ *pHeaderQ = (headerQ *)dir;
    int q = pHeaderQ->q;
    int i = pHeaderQ->i;
    int b = pHeaderQ->b;
    int d = pHeaderQ->d;
    int s = pHeaderQ->s;

    munmap((void *)pHeaderQ, sizeof(headerQ));
    size_t memorysize = sizeof(headerQ) + (sizeof(registrosalida) * q * 3);

    if ((dir = (char *)(mmap(NULL, memorysize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q6"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    return dir;
}

int recorrerQ(string nombre)
{

    int temp1 = 0;
    int temp2 = 0;
    int recorrido = 0;

    // posición inicial
    char *dire = abrirQ(nombre);
    headerQ *pHeaderQ = (headerQ *)dire;
    int q = pHeaderQ->q;
    int b = pHeaderQ->b;
    int d = pHeaderQ->d;
    int s = pHeaderQ->s;

    while (recorrido < 3)
    {
        char *pos = dire + sizeof(headerQ) + (recorrido * sizeof(registrosalida) * q);

        while (temp2 < q)
        {
            char *posn = pos + (temp2 * sizeof(registrosalida));
            registrosalida *pRegistroSalida = (registrosalida *)posn;
            cout << pRegistroSalida->id << pRegistroSalida->tipo << pRegistroSalida->cantidad << endl;
            temp2++;
        }

        recorrido++;
        temp2 = 0;
    }
    return 0;
}

int ingresarBandejaQ(registrosalida registro, string nombre)
{
    //accede a la memoria compartida
    // posición inicial
    char *dire = abrirQ(nombre);
    headerQ *pHeaderQ = (headerQ *)dire;

    int q = pHeaderQ->q;
    int i = pHeaderQ->i;
    int b = pHeaderQ->b;
    int d = pHeaderQ->d;
    int s = pHeaderQ->s;

    //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor
    sem_t *arrayMut, *arrayVacio, *arrayLleno;
    int tipopipo;
    if (registro.tipo == 'B')
    {
        tipopipo = i;
    }
    if (registro.tipo == 'D')
    {
        tipopipo = i + 1;
    }
    if (registro.tipo == 'S')
    {
        tipopipo = i + 2;
    }
    string mutex = "Mut" + nombre + to_string(tipopipo);
    string vacio = "Vacio" + nombre + to_string(tipopipo);
    string lleno = "Lleno" + nombre + to_string(tipopipo);
    arrayMut = sem_open(mutex.c_str(), 0);
    arrayVacio = sem_open(vacio.c_str(), 0);
    arrayLleno = sem_open(lleno.c_str(), 0);

    // variable para recorrer la bandeja
    int recorrido = 0;

    // posición inicial de la bandeja según el tipo
    int posBandeja = tipopipo - i;
    char *pos = dire + sizeof(headerQ) + (posBandeja * sizeof(registrosalida) * q);

    //hasta que no logre insertar intentar
    // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
    //Soy consumidor
    sem_wait(arrayVacio);
    sem_wait(arrayMut);
    // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
    while (recorrido < q)
    {
        //posición en la bandeja
        char *posn = (pos + (recorrido * sizeof(registrosalida)));
        registrosalida *pRegistroSalida = (registrosalida *)posn;
        //si logra insertar se sale
        if (pRegistroSalida->cantidad <= 0)
        {
            pRegistroSalida->id = registro.id;
            pRegistroSalida->tipo = registro.tipo;
            pRegistroSalida->cantidad = registro.cantidad;
            pRegistroSalida->bandeja = registro.bandeja;
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

// función que le entregan un registro a guardar en la memoria compartida de nombre

registrosalida retirarRegistroDeQ(char tipo, string nombre)
{
    //accede a la memoria compartida
    // posición inicial
    char *dirQ = abrirQ(nombre);
    headerQ *pHeaderQ = (headerQ *)dirQ;

    int q = pHeaderQ->q;
    int i = pHeaderQ->i;
    int b = pHeaderQ->b;
    int d = pHeaderQ->d;
    int s = pHeaderQ->s;

    //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor de las bandejas
    sem_t *arrayMut, *arrayVacio, *arrayLleno, *arrayReact;
    int pos_tipo;
    int pos_bandejaQ;
    if (tipo == 'B')
    {
        pos_tipo = i;
        pos_bandejaQ = 0;
    }
    if (tipo == 'D')
    {
        pos_tipo = i + 1;
        pos_bandejaQ = 1;
    }
    if (tipo == 'S')
    {
        pos_tipo = i + 2;
        pos_bandejaQ = 2;
    }

    string mutex = "Mut" + nombre + to_string(pos_tipo);
    string vacio = "Vacio" + nombre + to_string(pos_tipo);
    string lleno = "Lleno" + nombre + to_string(pos_tipo);
    string reactivo = "Reactivo" + nombre + to_string(pos_bandejaQ);

    arrayMut = sem_open(mutex.c_str(), 0);
    arrayVacio = sem_open(vacio.c_str(), 0);
    arrayLleno = sem_open(lleno.c_str(), 0);
    arrayReact = sem_open(reactivo.c_str(), 0);

    // variable para recorrer la bandeja
    int recorrido = 0;

    // posición inicial de la bandeja B|D|S
    char *pos = dirQ + sizeof(headerQ) + (pos_bandejaQ * q * sizeof(registrosalida));

    //Crear el registro de salida que d
    registrosalida registro;

    //hasta que no logre insertar intentar
    // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
    //Soy productor

    sem_wait(arrayLleno);
    sem_wait(arrayMut);
    // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
    while (recorrido < q)
    {

        //posición en la bandeja
        char *posn = (pos + (recorrido * sizeof(registrosalida)));
        registrosalida *pRegistro = (registrosalida *)posn;

        //si encuentro elemento a retirar
        if (pRegistro->cantidad > 0)
        {

            //GENERACION del costo en reativo según tipo
            int costo;
            char tipodelregistro = pRegistro->tipo;
            for (int f = 0; f < pRegistro->cantidad; ++f)
            {
                if (tipodelregistro == 'B')
                    costo += rand() % ((7 + 1) - 1);
                if (tipodelregistro == 'D')
                    costo += 5 + rand() % ((20 + 1) - 5);
                if (tipodelregistro == 'S')
                    costo += 8 + rand() % ((25 + 1) - 8);
            }
            // Si es tipo B, Si no tengo suficiente reactivo libero el mutex y lo espero de nuevo
            if (tipodelregistro == 'B')
            {
                while (pHeaderQ->b < costo)
                {
                    sem_post(arrayMut);
                    sem_wait(arrayMut);
                }
                pHeaderQ->b -= costo;
            }
            // Si es tipo D, Si no tengo suficiente reactivo libero el mutex y lo espero de nuevo
            if (tipodelregistro == 'D')
            {
                while (pHeaderQ->d < costo)
                {
                    sem_post(arrayMut);
                    sem_wait(arrayMut);
                }
                pHeaderQ->d -= costo;
            }
            // Si es tipo S, Si no tengo suficiente reactivo libero el mutex y lo espero de nuevo
            if (tipodelregistro == 'S')
            {
                while (pHeaderQ->s < costo)
                {
                    sem_post(arrayMut);
                    sem_wait(arrayMut);
                }
                pHeaderQ->s -= costo;
            }

            //asigno los valores a devolver
            registro.cantidad = pRegistro->cantidad;
            registro.id = pRegistro->id;
            registro.tipo = pRegistro->tipo;
            registro.bandeja = pRegistro->bandeja;

            //Pongo basura donde estaba
            pRegistro->id = 0;
            pRegistro->tipo = '0';
            pRegistro->cantidad = 0;
            pRegistro->bandeja = 0;
            //Soy consumidor
            sem_post(arrayMut);
            sem_post(arrayVacio);

            return registro;
        }
        else
        {
            recorrido++;
        }
    }
    return registro;
}

int IngresarReactivo(string nombre, int cantidad, char tipo)
{
    //accede a la memoria compartida
    // posición inicial
    char *dirQ = abrirQ(nombre);
    headerQ *pHeaderQ = (headerQ *)dirQ;

    char b = pHeaderQ->b;
    char d = pHeaderQ->d;
    char s = pHeaderQ->s;

    sem_t *arrayMut;
    int pos_tipo;
    int pos_bandejaQ;
    if (tipo == 'B')
        pos_bandejaQ = 0;
    if (tipo == 'D')
        pos_bandejaQ = 1;
    if (tipo == 'S')
        pos_bandejaQ = 2;
    string mutex = "Mut" + nombre + to_string(pos_tipo);
    arrayMut = sem_open(mutex.c_str(), 0);

    sem_wait(arrayMut);
    if (tipo == b)
    {
        pHeaderQ->b += cantidad;
    }
    if (tipo == d)
    {
        pHeaderQ->d += cantidad;
    }
    if (tipo == s)
    {
        pHeaderQ->s += cantidad;
    }
    sem_post(arrayMut);

    return 0;
}

int ImprimirReactivo(string nombre)
{
    char *dirQ = abrirQ(nombre);
    headerQ *pHeaderQ = (headerQ *)dirQ;

    cout << "Tenemos " << pHeaderQ->b << " de reactivo B" << endl;
    cout << "Tenemos " << pHeaderQ->d << " de reactivo D" << endl;
    cout << "Tenemos " << pHeaderQ->s << " de reactivo S" << endl;
    return 0;
}