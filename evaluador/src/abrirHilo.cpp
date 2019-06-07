#include <cerrno>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <pthread.h>
#include <semaphore.h>
#include "AbrirMemoria.cpp"
#include "retirarElemento.cpp"

using namespace std;

struct banNam
{
    int bandeja;
    string name;
};

//Método debugging
void *procesador(void *bandej)
{
    for (;;)
    {
        struct banNam *producto = (struct banNam *)bandej;
        //printf("%d\n", producto -> bandeja);
        //cout << producto->name << "\n" << endl;
        struct registrosalida registro = retirarRegistro(producto->bandeja, producto->name);
        // insertarRegistroSalida(registrosalida, nombre)
    }
    return NULL;
}

int instancearHilos(int i, string nombre)
{
   
    pthread_t hiloP[i];
    struct banNam bande;
    bande.bandeja = i;
    bande.name = nombre;
    string llen = "Hilo" + nombre;
    for (int n = 0; n < i; n++)
    {

        ostringstream namellen;
        namellen << llen << n;
        string realNameLlen(namellen.str());
        pthread_create(&hiloP[n], NULL, procesador, (void *)&bande);
    }
    if (pthread_join(hiloP[0], NULL))
    {

        fprintf(stderr, "Error joining thread\n");
        return 2;
    }
    printf("Hace hilo x:%d\n", x);
}
