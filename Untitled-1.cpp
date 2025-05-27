#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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
            cout << "ID: " << actual->proceso.id
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
        string linea;
        while (getline(archivo, linea)) {
            Proceso p;
            size_t pos = 0;

            pos = linea.find(',');
            p.id = stoi(linea.substr(0, pos));
            linea.erase(0, pos + 1);

            pos = linea.find(',');
            p.nombre = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            pos = linea.find(',');
            p.prioridad = stoi(linea.substr(0, pos));
            linea.erase(0, pos + 1);

            p.memoria = stoi(linea);

            insertarProceso(p);
        }
        archivo.close();
    }
};

void mostrarMenuPrincipal() {
    cout << "\n===== SISTEMA DE GESTION DE PROCESOS =====\n";
    cout << "1. Agregar proceso\n";
    cout << "2. Eliminar proceso\n";
    cout << "3. Buscar proceso por ID\n";
    cout << "4. Modificar prioridad\n";
    cout << "5. Mostrar todos los procesos\n";
    cout << "6. Guardar procesos en archivo\n";
    cout << "7. Cargar procesos desde archivo\n";

}

int main() {
    GestorProceso gestor;
    int opcion;

    do {
        mostrarMenuPrincipal();
        cin >> opcion;
        cin.ignore();

        if (opcion == 1) {
            Proceso p;
            cout << "Ingrese ID: ";
            cin >> p.id;
            cout << "Ingrese nombre: ";
            cin >> p.nombre;
            cout << "Ingrese prioridad: ";
            cin >> p.prioridad;
            cout << "Ingrese memoria (MB): ";
            cin >> p.memoria;
            gestor.insertarProceso(p);
            cout << "Proceso insertado correctamente.\n";
        }
        else if (opcion == 2) {
            int id;
            cout << "Ingrese ID del proceso a eliminar: ";
            cin >> id;
            if (gestor.eliminarProceso(id))
                cout << "Proceso eliminado correctamente.\n";
            else
                cout << "No se encontro el proceso.\n";
        }
        else if (opcion == 3) {
            int id;
            cout << "Ingrese ID del proceso a buscar: ";
            cin >> id;
            Proceso* p = gestor.buscarProceso(id);
            if (p)
                     << " | Prioridad: " << p->prioridad
                     << " | Memoria: " << p->memoria << " MB\n";
            else
                cout << "No se encontro el proceso.\n";
        }
        else if (opcion == 4) {
            int id, nuevaPrioridad;
            cout << "Ingrese ID del proceso a modificar: ";
            cin >> id;
            cout << "Ingrese nueva prioridad: ";
            cin >> nuevaPrioridad;
            if (gestor.modificarPrioridad(id, nuevaPrioridad))
                cout << "Prioridad modificada correctamente.\n";
            else
                cout << "No se encontro el proceso.\n";
        }
        else if (opcion == 5) {
            gestor.mostrarProcesos();
        }
        else if (opcion == 6) {
            string nombreArchivo;
            cout << "Ingrese nombre del archivo para guardar: ";
            cin >> nombreArchivo;
            gestor.guardarArchivo(nombreArchivo);
            cout << "Procesos guardados correctamente.\n";
        }
        else if (opcion == 7) {
            string nombreArchivo;
            cout << "Ingrese nombre del archivo para cargar: ";
            cin >> nombreArchivo;
            gestor.cargarArchivo(nombreArchivo);
            cout << "Procesos cargados correctamente.\n";
        }
        else if (opcion == 0) {
            cout << "Saliendo del programa...\n";
        }
        else {
            cout << "Opcion no valida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}