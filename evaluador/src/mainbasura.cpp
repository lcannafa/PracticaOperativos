#include "IniciadorMemoria.cpp"
#include "ingresoelemento.cpp"
int main (){
    string nombre = "buffer";
    int i= 3;
    int ie = 5;
    int oe = 10;
    int q = 5;
    int b = 5;
    int d = 5;
    int s = 5;
    
    crearEspacio(nombre,i,ie,oe,q,b,d,s);
    struct registroentrada test;
    test.bandeja = 0;
    test.cantidad = 3;
    test.id = 8;
    test.tipo = 'D';

    struct registroentrada test2;
    test2.bandeja = 2;
    test2.cantidad = 3;
    test2.id = 5;
    test2.tipo = 'D';
    ingresarRegistro(test,"buffer",3,5,10);
    ingresarRegistro(test2,"buffer",3,5,10);
    recorrer("buffer",3,5,10);
    return 0;
    }