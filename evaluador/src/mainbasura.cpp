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
    int i = 5;
    int ie = 6;
    int oe = 10;
    int q = 6;
    int b = 100;
    int d = 100;
    int s = 100;

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
    testD.cantidad = 1;
    testD.id = 1;
    testD.tipo = 'B';
    registroentrada testB;
    testB.bandeja = 1;
    testB.cantidad = 4;
    testB.id = 2;
    testB.tipo = 'D';

    registroentrada testS;
    testS.bandeja = 2;
    testS.cantidad = 2;
    testS.id = 3;
    testS.tipo = 'S';

    ImprimirReactivo(nombre);

    ingresarRegistro(testD, nombre);
    ingresarRegistro(testB, nombre);
    ingresarRegistro(testS, nombre);
    ingresarRegistro(testD, nombre);
    
 
     cout <<endl;
     cout <<endl;
    
    sleep(5);
    cout << "" << endl;
    cout << " Bandejas OE Resultado" << endl;
    cout << "" << endl;
    recorrerOE(nombre);
    ImprimirReactivo(nombre);
    RetornarContador(nombre);

    return 0;
}