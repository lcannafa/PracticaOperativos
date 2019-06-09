#include "IniciadorMemoria.cpp"
#include "manejoBandejaEntrada.cpp"
#include "AbrirSemaforos.cpp"
//#include "retirarElemento.cpp"
#include "manejoQ.cpp"
#include "abrirHilo.cpp"
#include "manejoOE.cpp"
using namespace std;

int main()
{
    string nombre = "evaluator";
    int i = 3;
    int ie = 5;
    int oe = 10;
    int q = 5;
    int b = 5;
    int d = 5;
    int s = 5;

    cout << "Inicio" << endl;

    // init
    crearEspacio(nombre, i, ie, oe, q, b, d, s);
    crearSemaforo(nombre);
    crearQ(nombre);
    crearHilo(nombre);
    crearHiloProcesadores(nombre);


    // reg
    registroentrada testD;
    testD.bandeja = 0;
    testD.cantidad = 5;
    testD.id = 1;
    testD.tipo = 'D';
    registroentrada testB;
    testB.bandeja = 1;
    testB.cantidad = 4;
    testB.id = 2;
    testB.tipo = 'B';

    registroentrada testS;
    testS.bandeja = 2;
    testS.cantidad = 8;
    testS.id = 3;
    testS.tipo = 'S';

    ingresarRegistro(testD, nombre);
    ingresarRegistro(testB, nombre);
    ingresarRegistro(testS, nombre);
 
    sleep(5);
    cout << "" << endl;
    cout << " Bandejas OE Resultado" << endl;
    cout << "" << endl;

    // list
    recorrerOE(nombre);

    return 0;
}