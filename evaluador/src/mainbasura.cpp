#pragma once
#include "IniciadorMemoria.cpp"
#include "ingresoelemento.cpp"
#include "AbrirSemaforos.cpp"
#include "retirarElemento.cpp"
using namespace std;

int main()
{
    string nombre = "buffer";
    int i = 3;
    int ie = 5;
    int oe = 10;
    int q = 5;
    int b = 5;
    int d = 5;
    int s = 5;
    
    crearEspacio(nombre, i, ie, oe, q, b, d, s);
    crearSemaforo(nombre);
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
    ingresarRegistro(test, nombre);
    ingresarRegistro(test2, nombre);
    recorrer(nombre);
    retirarRegistro(2, nombre);
    recorrer(nombre);
    return 0;
}