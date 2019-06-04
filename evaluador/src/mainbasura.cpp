#include "IniciadorMemoria.cpp"
#include "ingresoelemento.cpp"
int main (){
    crearEspacio("buffer",3, 5, 10);
    struct registroentrada test;
    test.bandeja = 1;
    test.cantidad = 3;
    test.id = 8;
    test.tipo = 'D';

    struct registroentrada test2;
    test2.bandeja = 1;
    test2.cantidad = 3;
    test2.id = 5;
    test2.tipo = 'D';
    ingresarRegistro(test,"buffer",3,5,10);
    ingresarRegistro(test2,"buffer",3,5,10);
    recorrer("buffer",3,5,10);
    return 0;
    }