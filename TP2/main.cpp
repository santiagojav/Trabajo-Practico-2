#include <iostream>
#include <fstream> 
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

//lectura
fstream& operator >> (fstream &fs, Registro& reg){
    reg.accion = readstring(fs, laccion);
    
    fs.read((char *) &reg.plazo, sizeof(reg.plazo));
	
	reg.bolsa = readstring(fs, lbolsa);
	
    fs.read((char *) &reg.precio, sizeof(reg.precio));
    fs.read((char *) &reg.cantidad, sizeof(reg.cantidad));

	return fs;
}

// Para encontrar el plazo por codigo
string nombrePlazo(int codigo){
    switch(codigo){
        case 0: return "CI";
        case 1: return "24hs";
        case 2: return "48hs";
        case 3: return "72hs";
        default: return "Desconocido";
    }
}

// sobrecarga
ostream& operator<<(ostream &os, const Operacion& op){
    os << (op.cantidad > 0 ? "Venta" : "Compra")
       << op.accion << "\t" << op.cantidad << "\t" << op.precio;
    return os; 
}
ostream& operator<<(ostream &os, const Bolsa& bolsa){
    os << bolsa.nombre << "\t" << bolsa.montoTotal;
    return os;
}
ostream& operator<<(ostream &os, const Plazo& plazo){
    os << "Plazo: " << nombrePlazo(plazo.codigo) << "\n"
       << "Ventas: " << plazo.cantVentas << "\n"
       << "Compras: " << plazo.cantCompras << "\n";
    return os; 
}


bool criterioPlazo(const Plazo& a, const Plazo& b){
    return a.codigo < b.codigo;
}

bool criterioBolsa(const Bolsa& a, const Bolsa& b){
    return a.nombre.compare(b.nombre); 
}

void iniciarPlazos(Nodo<Plazo>* &lplazo){
        for(int i = 0; i < 4; i++){ 
            plazo.codigo = i; 
            insertarOrdenado(plazo, lplazo, criterioPlazo);
        }
}

void listaPrincipal(Nodo<Plazo> *lplazo){
    Nodo<Plazo>* pp = lplazo;

    while(pp){
        cout << "Plazo: " << pp->dato.codigo << "  Compras: " 
            << pp->dato.cantCompras << "  Ventas: " << pp->dato.cantVentas << "\n\n";

        Nodo<Bolsa>* bp = pp->dato.lbolsa;
    
        while (bp) {
            cout << bp->dato.nombre << "\t" << bp->dato.montoTotal << "\n";

            Nodo<Operacion>* op = bp->dato.loperacion;

            if(op){
                // imprimir solo si hay operaciones
                cout << "Acción "<<"\t"<<"Plazo Bolsa"<<"\t"<<"Pre.Un."<<"\t"<<"Cant.\n";
            }

            while (op) {
                cout << (op->dato.cantidad > 0 ? "Venta " : "Compra ") 
                        << op->dato.accion << "\t"
                        << nombrePlazo(pp->dato.codigo) << "\t"
                        << bp->dato.nombre << "\t"
                        << op->dato.precio << "\t"
                        << op->dato.cantidad << "\n";
                    op = op->sig;
            }
            cout << "\n"; // espacio entre bolsas
            bp = bp->sig;
        }
        cout << "\n"; // espacio entre plazos
        pp = pp->sig;
    }
}

int main(){
    fstream arch; 
    arch.open("Datos.bin", ios::in | ios::binary); 
    if(!arch){
        cout << "No se puede abrir el archivo cabezon..." << endl; 
        return EXIT_FAILURE; 
    }

    Nodo<Plazo> *lplazo = nullptr;
	Nodo<Bolsa> *lbolsa = nullptr; 
    Nodo<Plazo> *pp; 
    Nodo<Bolsa> *bp;
    Operacion op; 
    Bolsa bolsa; 
    Plazo plazo; 
    Registro reg; 

    iniciarPlazos(lplazo);

    while (arch >> reg){
        plazo.codigo = reg.plazo;
        pp = buscar(plazo, lplazo, criterioPlazo);  //encuentro el plazo que quiero
        
        if(re.cantidad > 0){ // actualizo contadores
            pp->dato.cantVentas++;
        }else{
            pp->dato.cantCompras++;
        }

        bolsa.nombre = reg.bolsa; 
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
        agregar(op, bp->dato.loperacion);
    }
    arch.close();

    listaPrincipal(lplazo);

    return 0; 
}
