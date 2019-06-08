#include "IniciadorMemoria.cpp"
#include "ingresoelemento.cpp"
#include "AbrirSemaforos.cpp"
#include "retirarElemento.cpp"
#include "retiroBandejaQ.cpp"
#include "BandejasQ.cpp"
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

    cout << "1" << endl;
    crearEspacio(nombre, i, ie, oe, q, b, d, s);
    crearSemaforo(nombre);
    crearQ(nombre);
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
    

    recorrer(nombre);
    crearHilo(nombre);
    cout << "" << endl;
    cout << "Bandejas Q" << endl;
    cout << "" << endl;
    sleep(2);
    recorrerQ(nombre);
    registrosalida D = retirarRegistroDeQ('D', nombre);
    registrosalida B = retirarRegistroDeQ('B', nombre);
    registrosalida S = retirarRegistroDeQ('S', nombre);
    cout << "" << endl;
    cout << " Bandejas Q x2" << endl;
    cout << "" << endl;
    sleep(2);
    recorrerQ(nombre);

    cout << "" << endl;
    cout << " Bandeja OE" << endl;
    cout << "" << endl;
    ingresarSalida(D, nombre);
    ingresarSalida(B, nombre);
    ingresarSalida(S, nombre);
    recorrerOE(nombre);


    return 0;
}