#include <semaphore.h>
#include <cerrno>
#include <cstring>
#include "AbrirMemoria.cpp"
#include <sstream>

int crearSemaforo(string nombre)
{

    char *dir = abrirMemoria(nombre);
    struct header *pHeader = (struct header *)dir;

    int i = pHeader->i;
    int ie = pHeader->ie;
    int oe = pHeader->oe;
    int q = pHeader->q;
    int b = pHeader->b;
    int d = pHeader->d;
    int s = pHeader->s;

    sem_t **arraySem0 = new sem_t *[4 + i];
    sem_t **arraySem1 = new sem_t *[4 + i];
    sem_t **arraySem2 = new sem_t *[4 + i];

    string mut = "Mut";
    string llen = "LLen";
    string vac = "Vac";

    for (int j = 0; j < i; j++)
    {

        ostringstream namemut;
        namemut << mut << j;
        string realNameMut(namemut.str());
        arraySem0[j] = sem_open(realNameMut.c_str(), O_CREAT | O_EXCL, 0660, 1);

        ostringstream namellen;
        namellen << llen << j;
        string realNameLlen(namellen.str());
        arraySem1[j] = sem_open(realNameLlen.c_str(), O_CREAT | O_EXCL, 0660, 1);

        ostringstream namevac;
        namevac << vac << j;
        string realNameVac(namevac.str());
        arraySem2[j] = sem_open(realNameVac.c_str(), O_CREAT | O_EXCL, 0660, 1);

        if (arraySem0[j] == SEM_FAILED)
        {
            cerr << "Error creando los semaforos 'mutex'" << endl;
        }
        if (arraySem1[j] == SEM_FAILED)
        {
            cerr << "Error creando los semaforos 'vacios'" << endl;
        }
        if (arraySem2[j] == SEM_FAILED)
        {
            cerr << "Error creando los semaforos 'llenos'" << endl;
        }
    }

    return 0;
}