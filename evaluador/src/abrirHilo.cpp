#include <cerrno>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <pthread.h>
#include <semaphore.h>
#include "AbrirMemoria.cpp"

using namespace std;

//Método debugging
void *procesador(void *arg)
{

    printf("Corre 2\n");
    /* increment x to 100 */
    int *x_ptr = (int *)arg;
    int x = 0;
    while(++(*x_ptr) < 100);
    printf("x: %d\n",x);
    ++x;
    printf("x increment finished\n");

    /* the function must return something - NULL will do */
    return NULL;

}

int main(int i)
{
    i=2;
    int x = 0;
    pthread_t hiloP[i];
    printf("Corre x:%d\n", x);
    
    printf("Hace hilo x:%d\n",x);
    string llen = "Hilo";
    for (int n = 0; n < i; n++)
    {
        
        ostringstream namellen;
        namellen << llen << n;
        string realNameLlen(namellen.str());
        pthread_create(&hiloP[n], NULL, procesador, &x);
    }
    if (pthread_join(hiloP[0], NULL))
    {

        fprintf(stderr, "Error joining thread\n");
        return 2;
    }
    printf("Hace hilo x:%d\n",x);
        
}


