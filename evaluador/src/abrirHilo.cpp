#include <cerrno>
#include <cstring>
#include <sstream>
#include <pthread.h>
#include <semaphore.h>
#include "AbrirMemoria.cpp"

using namespace std;

static void *procesador(void *arg);
int main(int i)
{
    pthread_t hiloP[i];

    string llen = "Hilo";
    for (int n = 0; n < i; n++)
    {
        //pues no
        ostringstream namellen;
        namellen << llen << n;
        string realNameLlen(namellen.str());
        pthread_create(&hiloP[n], NULL, procesador, NULL);
    }
        
}

static void *procesador(void *arg)
{

    /* increment x to 100 */
    int *x_ptr = (int *)arg;
    while(++(*x_ptr) < 100);

    printf("x increment finished\n");

    /* the function must return something - NULL will do */
    return NULL;

}
