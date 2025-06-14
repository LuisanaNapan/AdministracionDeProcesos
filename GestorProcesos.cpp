#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // Para NULL
#include <limits> //para validar el número entero (Luisana)
using namespace std;
//para pila 
#define MAX_PILA 100         //Tamaño máximo
int bloques[MAX_PILA];       // almacena los bloques asignados
int siguiente[MAX_PILA];     // enlaza cada posición con la anterior 
int tope = -1;               // índice del tope de la pila
int libre = 0;               // índice del siguiente espacio libre

struct Proceso {
    int id ;
    string nombre;
    int prioridad;
    int memoria;
};

struct NodoLista {
    Proceso proceso;
    NodoLista* siguiente;
    NodoLista() : siguiente(NULL) {} 
};

struct NodoCola {
    Proceso proceso;
    NodoCola* siguiente;
};

NodoCola* frente = NULL;
NodoCola* final = NULL;

//cola
void encolar(NodoCola*& frente, NodoCola*& final, Proceso p) {
    NodoCola* nuevo = new NodoCola{p, NULL};
    if (!final) {
        frente = final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
}

bool desencolar(NodoCola*& frente, NodoCola*& final) {
    if (!frente) return false;
    NodoCola* temp = frente;
    frente = frente->siguiente;
    if (!frente) final = NULL;
    delete temp;
    return true;
}

void mostrarCola(NodoCola* frente) {
    if (!frente) {
        cout << "\tCola vac�a.\n";
        return;
    }
    NodoCola* actual = frente;
    while (actual) {
        Proceso p = actual->proceso;
        cout << "\tID: " << p.id << " | Nombre: " << p.nombre
             << " | Prioridad: " << p.prioridad << " | Memoria: " << p.memoria << "MB\n";
        actual = actual->siguiente;
    }
}

class GestorProceso {
private:
    NodoLista* cabeza;

    NodoLista* buscarNodo(int id) {
        NodoLista* actual = cabeza;
        while (actual) {
            if (actual->proceso.id == id)
                return actual;
            actual = actual->siguiente;
        }
        return NULL;
    }

public:
    GestorProceso() : cabeza(NULL) {}

    void insertarProceso(Proceso p) {
        NodoLista* nuevo = new NodoLista;
        nuevo->proceso = p;
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    bool eliminarProceso(int id) {
        if (!cabeza) return false;
        if (cabeza->proceso.id == id) {
            NodoLista* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            return true;
        }
        NodoLista* actual = cabeza;
        while (actual->siguiente) {
            if (actual->siguiente->proceso.id == id) {
                NodoLista* temp = actual->siguiente;
                actual->siguiente = temp->siguiente;
                delete temp;
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    Proceso* buscarProceso(int id) {
        NodoLista* nodo = buscarNodo(id);
        return nodo ? &(nodo->proceso) : NULL;
    }

    bool modificarPrioridad(int id, int nuevaPrioridad) {
        NodoLista* nodo = buscarNodo(id);
        if (nodo) {
            nodo->proceso.prioridad = nuevaPrioridad;
            return true;
        }
        return false;
    }

    void mostrarProcesos() {
        NodoLista* actual = cabeza;
        while (actual) {
            cout << "\t| ID: " << actual->proceso.id
                 << " | Nombre: " << actual->proceso.nombre
                 << " | Prioridad: " << actual->proceso.prioridad
                 << " | Memoria: " << actual->proceso.memoria << " MB\n";
            actual = actual->siguiente;
        }
    }

    void guardarArchivo(const string& nombreArchivo) {
        ofstream archivo(nombreArchivo.c_str());
        NodoLista* actual = cabeza;
        while (actual) {
            archivo << actual->proceso.id << ","
                    << actual->proceso.nombre << ","
                    << actual->proceso.prioridad << ","
                    << actual->proceso.memoria << "\n";
            actual = actual->siguiente;
        }
        archivo.close();
    }

    void cargarArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str());
        if (!archivo) {
            cout << "\tError al abrir el archivo para cargar.\n";
            return;
        }
        liberarLista();
        string linea;
        while (getline(archivo, linea)) {
            Proceso p;
            size_t pos = 0;

            pos = linea.find(',');
            if (pos == string::npos) continue;
            p.id = atoi(linea.substr(0, pos).c_str());
            linea.erase(0, pos + 1);

            pos = linea.find(',');
            if (pos == string::npos) continue;
            p.nombre = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            pos = linea.find(',');
            if (pos == string::npos) continue;
            p.prioridad = atoi(linea.substr(0, pos).c_str());
            linea.erase(0, pos + 1);

            p.memoria = atoi(linea.c_str());

            agregarProceso();
        }
        archivo.close();
    }
};

//Funciones para Pila - Dayana
void asignarBloque() {
    if (libre >= MAX_PILA) {
            cout << "\tError: Pila llena" << "\n";
        } else {
            int idBloque;
            cout << "\tID del bloque: ";
            cin >> idBloque;
            bloques[libre] = idBloque;
            siguiente[libre] = tope;    // enlaza con el bloque anterior
            tope = libre;              // actualiza el tope al nuevo bloque
            libre++;                  // actualiza índice del espacio libre
            cout << "\tBloque " << idBloque << " asignado correctamente.\n";
        }
}

void verTope() {
    if (tope == -1) {
            cout << "\tError: Pila vacia\n";
        } else {
            cout << "\tBloque en tope: " << bloques[tope] << "\n";
        }
}

//Funciones para Pila - Yatzuri
void LiberarBloque() {
    // Verifica si la pila esta vacia
    if (tope == -1) {
        cout << "\tError: Pila vacia\n";
    } else {
        // Muestra el bloque que será liberado (el que está en el tope)
        cout << "\tLiberando bloque: " << bloques[tope] << endl;
        // Actualiza el tope apuntando al bloque anterior en la pila
        tope = siguiente[tope]; // Retrocede al anterior bloque
    }
}

void VerificarVacia() {
    // Si el tope es -1, la pila no tiene elementos
    if (tope == -1) {
        cout << "\tVerdadero (vacía)\n";
    } else {
        // Si el tope no es -1, hay al menos un bloque en la pila
        cout << "\tFalso (con datos)\n";
    }
}

void mostrarMenuPrincipal() {
    cout << "\n\t===== SISTEMA DE GESTION DE PROCESOS =====\n";
    cout << "\t-------------------------------------------";
    cout << "\n\t[1]. Agregar proceso\n";
    cout << "\t[2]. Eliminar proceso\n";
    cout << "\t[3]. Buscar proceso por ID\n";
    cout << "\t[4]. Modificar prioridad\n";
    cout << "\t[5]. Mostrar todos los procesos\n";
    cout << "\t[6]. Guardar procesos en archivo\n";
    cout << "\t[7]. Cargar procesos desde archivo\n";
    cout << "\t[8]. Cola de procesos\n"; //una opcion del menú para las colas (Luisana)
    cout << "\t[9]. Pila de bloques\n"; // opcion para pila (Dayana y Yatzuri)

    cout << "\t[0]. Salir\n"; //agregar menu de salida (Luisana)
};

//Creando una funcion para validar si es numero entero y que podremos usarlo en cualquier momento (Luisana)
int pedirEntero(const string& mensaje){
    int valor; //creamos una variable "valor" que será ENTERO
    while (true) {
        cout << mensaje; //imprime el mensaje que está en el cout original
        if(cin >> valor){ //si el valor que escribe el usuario es...
            return valor; //un numero entero, retorna "valor"
        } else{
            cout << "\tERROR, Ingrese un numero.\n"; // si el valor ingresado no es un numero ENTERO
            cin.clear(); //limpia el bucle que se ejecutó con el error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignora lo anterior y repite el proceso
        }
    }
}

void SubMenuCola(GestorProceso& gestor) {
    setlocale(LC_CTYPE, "Spanish");
    int opColita;
    do {
        cout << "\n\t************ COLA DE PROCESOS ************\n";
        cout << "\t-------------------------------------------";
        cout << "\n\t[1]. Ingresar nuevo proceso\n";
        cout << "\t[2]. Ejecutar proceso (desencolar)\n";
        cout << "\t[3]. Ver cola de procesos\n";
        cout << "\t[4]. Volver\n";
        cout << "\t[5]. Salir\n";
        cout << "\n\tElija una opcion: ";
        cin >> opColita;
        cin.ignore();

        switch (opColita) {
            case 1: {
                Proceso p;
                p.id = pedirEntero("\n\tIngrese ID\t    : ");
                cout << "\tIngrese nombre\t    : ";
                cin.ignore();
                getline(cin, p.nombre);
                p.prioridad = pedirEntero("\tIngrese prioridad   : ");
                p.memoria = pedirEntero("\tIngrese memoria (MB): ");

                gestor.insertarProceso(p);
                encolar(frente, final, p);
                cout << "\t-- Proceso insertado correctamente. --\n";
                break;
            }
            case 2: {
                if (frente) {
                    Proceso p = frente->proceso;
                    desencolar(frente, final);
                    gestor.eliminarProceso(p.id);
                    cout << "\tProceso con ID " << p.id << " ejecutado y eliminado.\n";
                } else {
                    cout << "\tLa cola est� vac�a.\n";
                }
                break;
            }
            case 3: {
                mostrarCola(frente);
                break;
            }
            case 4:
                cout << "\tEstas volviendo al menu principal...\n";
                break;
            case 5:
                cout << "\tSaliendo del programa...\n";
                exit(0);
            default:
                cout << "\tERROR, Ingrese una opcion v�lida del submenu.\n";
        }
    } while (opColita != 4);
}

void SubMenuPila() {
    int opcion;
    do {
        cout << "\n\t************ PILA DE BLOQUES ************\n";
        cout << "\t-------------------------------------------";
        cout << "\n\t[1]. Asignar bloque";
        cout << "\n\t[2]. Liberar Bloque";
        cout << "\n\t[3]. Ver tope";
        cout << "\n\t[4]. Verificar Vacía";
        cout << "\n\t[5]. Volver\n";
        cout << "\n\tElija una opción: ";
        opcion = pedirEntero("");  

        switch(opcion) {
            case 1: asignarBloque(); break;
            case 2: LiberarBloque(); break;
            case 3: verTope(); break;
            case 4: VerificarVacia(); break;
            case 5: cout << "\tVolviendo al menú principal...\n"; break;
            default: cout << "\tOpción inválida\n";
        }
    } while (opcion != 5);
}

int main() {
    setlocale(LC_CTYPE, "Spanish");
    GestorProceso gestor;
    int opcion;

    do {
        mostrarMenuPrincipal();
        cout << "\n\tSeleccione una opcion del menu: "; //(Luisana)
        cin >> opcion;
        cin.ignore();

        if (opcion == 1) {
            Proceso p;
            cout << "\n\tIngrese ID\t    : ";
            cin >> p.id;
            cout << "\tIngrese nombre\t    : ";
            cin >> p.nombre;
            cout << "\tIngrese prioridad   : ";
            cin >> p.prioridad;
            cout << "\tIngrese memoria (MB): ";
            cin >> p.memoria;
            gestor.insertarProceso(p);
            encolar(frente, final, p); //para que se agregen los procesos ingresados a la cola(Luisana)
            cout << "\tProceso insertado correctamente.\n";
        }
        else if (opcion == 2) {
            int id;
            cout << "\tIngrese ID del proceso a eliminar: ";
            cin >> id;
            if (gestor.eliminarProceso(id))
                cout << "\tProceso eliminado correctamente.\n";
            else
                cout << "\tNo se encontro el proceso.\n";
        }
        else if (opcion == 3) {
            int id;
            cout << "\tIngrese ID del proceso a buscar: ";
            cin >> id;
            Proceso* p = gestor.buscarProceso(id);
            if (p)
                cout << "\t| ID     : " << p->id
                     << "| Nombre   : " << p->nombre
                     << "| Prioridad: " << p->prioridad
                     << "| Memoria  : " << p->memoria << " MB\n";
            else
                cout << "\tNo se encontro el proceso.\n";
        }
        else if (opcion == 4) {
            int id, nuevaPrioridad;
            cout << "\tIngrese ID del proceso a modificar: ";
            cin >> id;
            cout << "\tIngrese nueva prioridad: ";
            cin >> nuevaPrioridad;
            if (gestor.modificarPrioridad(id, nuevaPrioridad))
                cout << "\tPrioridad modificada correctamente.\n";
            else
                cout << "\tNo se encontro el proceso.\n";
        }
        else if (opcion == 5) {
            gestor.mostrarProcesos();
        }
        else if (opcion == 6) {
            string nombreArchivo;
            cout << "\tIngrese nombre del archivo para guardar: ";
            cin >> nombreArchivo;
            gestor.guardarArchivo(nombreArchivo);
            cout << "\tProcesos guardados correctamente.\n";
        }
        else if (opcion == 7) {
            string nombreArchivo;
            cout << "\tIngrese nombre del archivo para cargar: ";
            cin >> nombreArchivo;
            gestor.cargarArchivo(nombreArchivo);
            cout << "\tProcesos cargados correctamente.\n";
        }
        else if (opcion == 8) {
            SubMenuCola(gestor); //llama a la funcion de cola(Luisana)
        }
        else if (opcion == 9) {
            SubMenuPila(); // Llama a submenú de pila (Dayana y Yatzuri)
        }
        //para salir del programa
        else if (opcion == 0) {
            cout << "\tSaliendo del programa...\n";
        }
        else {
            cout << "\tOpcion no valida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}
