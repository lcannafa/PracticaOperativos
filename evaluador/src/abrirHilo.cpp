#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "elementos.h"
#include "AbrirMemoria.cpp"
#include "retirarElemento.cpp"
#include "manejoOE.cpp"
#include "BandejasQ.cpp"
#include "retiroBandejaQ.cpp"

using namespace std;
struct banHilo
{
    int bandeja;
    string nombre;
};

struct banPros
{
    char tipo;
    string nombre;
};

void *procesador(void *bandej)
{

    banHilo *producto = (banHilo *)bandej;
    int num_bandeja = producto->bandeja;
    string nom_memoria = producto->nombre;

    for (;;)
    {
        registrosalida registro = retirarRegistro(num_bandeja, nom_memoria);
        ingresarBandejaQ(registro, nom_memoria);
 
    }

    cout << "Sali?" << endl;

    pthread_exit(NULL);
}

void crearHilo(string n)
{
    //accede a la memoria compartida
    // posición inicial
    char *dir = abrirMemoria(n);

    struct header *pHeader = (struct header *)dir;

    int i = pHeader->i;

    // Instancia los elementos que van a hacer parte de los hilos
    // Instancia el arreglo de los hilos.
    pthread_t hiloP[i];
    banHilo bande;
    bande.nombre = n;
    string n_Hilo = "Hilo" + n;

    // Crea los hilos y les asigna la funcion
    for (int m = 0; m < i; ++m)
    {
        bande.bandeja = m;
        ostringstream namellen;
        namellen << n_Hilo << m;
        string realNameLlen(namellen.str());
        pthread_create(&hiloP[m], NULL, procesador, (void *)&bande);
        sleep(0.1);
    }

    //ESTO SE BORRA, SE DEJA AHORA POR DEBUGGER

    // if (pthread_join(hiloP[0], NULL))
    // {
    //     fprintf(stderr, "Error joining thread\n");
    //     return;
    // }

    return;
}

//------------------------------------------------------////------------------------------------------------------////

void *procesadorOE(void *bandej)
{

    banPros *producto = (banPros *)bandej;
    char tipo = producto->tipo;
    string nom_memoria2 = producto->nombre;

    for (;;)
    {   
        registrosalida registro = retirarRegistroDeQ(tipo, nom_memoria2);
        ingresarSalida(registro, nom_memoria2);
    }

    cout << "Sali?" << endl;

    pthread_exit(NULL);
}

void crearHiloProcesadores(string n)
{
    //accede a la memoria compartida
    // posición inicial
    char *dirq = abrirQ(n);

    headerQ *pHeaderQ = (headerQ *)dirq;

    int q = pHeaderQ->q;

    // Instancia los elementos que van a hacer parte de los hilos
    // Instancia el arreglo de los hilos.
    pthread_t hiloQ[q];
    banPros bande;
    bande.nombre = n;
    char tipoPros = 'B';
    string n_Hilo = "Hilo" + n;
    // Crea los hilos y les asigna la funcion
    for (int m = 0; m < 3; ++m)
    {
        if(m == 1) tipoPros = 'D';
        if(m == 2) tipoPros = 'S';
        bande.tipo = tipoPros;
        ostringstream namellen;
        namellen << n_Hilo << m;
        string realNameLlen(namellen.str());
        pthread_create(&hiloQ[m], NULL, procesadorOE, (void *)&bande);
        sleep(1);
    }


    //ESTO SE BORRA, SE DEJA AHORA POR DEBUGGER

    // if (pthread_join(hiloP[0], NULL))
    // {
    //     fprintf(stderr, "Error joining thread\n");
    //     return;
    // }

    return;
}