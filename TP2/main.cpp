#include <iostream>
#include <cassert> 
#include "listas.hpp"
#include "rwstring.hpp"

using namespace std;

struct Registro{
    string accion;
    int plazo;
    string bolsa; 
    float precio;
    int cantidad;
};
const int laccion = 12; 
const int lbolsa = 14;

struct Operacion{
    string accion;
    int cantidad;   //negativo es compra y positivo venta
    float precio;
};

struct Bolsa{
    string nombre; 
    float montoTotal = 0;   //sumo cantidad * precio
    Nodo<Operacion> *loperacion = nullptr;
};

struct Plazo{
    int codigo; 
    int cantVentas = 0; 
    int cantCompras = 0;
    Nodo<Bolsa> *lbolsa = nullptr; 
};

fstream& operator >> (fstream &fs, Registro& reg){
    reg.accion = readstring(fs, laccion);
    reg.bolsa = readstring(fs, lbolsa); 

    fs.read((char *) &reg.plazo, sizeof(reg.plazo));
    fs.read((char *) &reg.precio, sizeof(reg.precio));
    fs.read((char *) &reg.cantidad, sizeof(reg.cantidad));

	return fs;
}

int criterioPlazo(const Plazo& a, const Plazo& b){
    return a.codigo.compare(b.codigo);
}

int criterioBolsa(const Bolsa& a, const Bolsa& b){
    return a.nombre.compare(b.nombre); 
}

int main(){
    fstream arch; 
    arch.open("Datos.bin", ios::in | ios::binary); 
    if(!arch){
        cout << "No se puede abrir el archivo cabezon..." << endl; 
        return EXIT_FAILURE; 
    }

    Nodo<Operacion> *lop = nullptr; 
    Nodo<Plazo> *lplazo = nullptr; 
    Nodo<Plazo> *pp; 
    Nodo<Bolsa> *lbolsa = nullptr; 
    Nodo<Bolsa> *bp;
    Operacion op; 
    Bolsa bolsa; 
    Plazo plazo; 
    Registro. reg; 

    void iniciarPlazos(Nodo<Plazo>* &lplazo){
        for(int i = 0; i < 4; i++){ 
            plazo.codigo = i; 
            insertarOrdenado(plazo, lista, criterioPlazo);
        }
    }

    while (arch >> reg){
        plazo.codigo = reg.codigo;
        pp = buscar(plazo, lplazo, criterioPlazo);  //encuentro el plazo que quiero
        
        if(re.cantidad > 0){ // actualizo contadores
            pp->dato.cantVentas++;
        }else{
            pp->dato.cantCompras++;
        }

        bolsa.nombre = reg.nombre; 
        bp = buscar(bolsa, pp->dato.lbolsa, criterioBolsa); // busco la bolsa 
        if(!bp){    //inserta bolsa dentro del plazo
            insertarOrdenado(bolsa, pp->dato.lbolsa, criterioBolsa); 
            bp = buscar(bolsa, pp->dato.lbolsa, criterioBolsa);
        }
        bp->dato.montoTotal += reg.cantidad * reg.precio; //sumo monto total de la bolsa

        //agrego operacion en orden original de la operacion
        op.accion = reg.accion;
        op.cantidad = reg.cantidad;
        op.precio = reg.precio;
        agregar(op, bp->dato.lop);
    }

    return 0; 
}
