#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> //Para NULL
using namespace std;
//para pila 
#define MAX_PILA 100      //Tama�o m�ximo
int procesos[MAX_PILA];   //Almacena los IDs de procesos con memoria asignada
int siguiente[MAX_PILA];  //enlaza cada posici�n con la anterior
int tope = -1;           //�ltimo bloque asignado
int libre = 0;          //Primer espacio libre disponible
int memoriaUsada = 0; // Memoria total ocupada actualmente

struct Proceso {
    int id;
    string nombre;
    int prioridad;
    int memoria;
};

struct NodoLista {
    Proceso proceso;
    NodoLista* siguiente;
    NodoLista() : siguiente(NULL) {}
};
NodoLista* cabeza = NULL; //puntero al inicio de la lista

//creamos una estructura llamada NodoCola
struct NodoCola {
    Proceso proceso;
    NodoCola* siguiente;
};

NodoCola* frente = NULL; //decimos que el frente de la cola
NodoCola* final = NULL; //y el final de la cola est�n vacios

//creamos una funcion encolar con los parametros y referencias que renombraremos dentro
void encolar(NodoCola*& frente, NodoCola*& final, Proceso p) {
    NodoCola* nuevo = new NodoCola; //creamos un puntero y copiaremos los campos de la estructura NodoCola
    nuevo->proceso = p;
    nuevo->siguiente = NULL;

    if (!final) {
	//cuando la cola est� vac�a el nuevo es el frente y el final
        frente = final = nuevo;
	return;
    }
    //para ordenar la prioridad dependiendo del n�mero, si es menor al que est� al frente entonces intercambian lugares
    if (p.prioridad < frente->proceso.prioridad) {
        nuevo->siguiente = frente; //ahora el nuevo apunta al que est� al frente
        frente = nuevo; //El frente toma el lugar del que reci�n entr� convirtiendose ahora en el nuevo
        return;
    }
    
    NodoCola* actual = frente; //creamos un puntero llamado "actual" ahora el actual es el nuevo frente
    NodoCola* anterior = NULL;
    
    // Avanzamos hasta encontrar el lugar correcto para insertar
    while (actual && actual->proceso.prioridad <= p.prioridad) {
        if (actual->proceso.prioridad == p.prioridad) {
            // Si la prioridad es igual, seguimos para insertarlo al final de este grupo
            while (actual->siguiente && actual->siguiente->proceso.prioridad == p.prioridad) {
                actual = actual->siguiente;
            }
            // Insertamos despu�s del �ltimo de igual prioridad
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
            if (nuevo->siguiente == NULL) {
                final = nuevo;
            }
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    // Insertar entre anterior y actual (donde actual tiene prioridad mayor que p.prioridad)
    nuevo->siguiente = actual;
    anterior->siguiente = nuevo;

    // Si se insert� al final, actualizar final
    if (nuevo->siguiente == NULL) {
        final = nuevo;
    }
}

bool desencolar(NodoCola*& frente, NodoCola*& final) {
    if (!frente) return false; //si el frente sea igual a NULL, no hay nada que eliminar
    NodoCola* temp = frente; //creamos un puntero temporal que apuntar� al frente de la cola
    frente = frente->siguiente; // el frente es igual al nodo que sigue y si luego de eso
    if (!frente) final = NULL; //el frente es NULL entonces el final tambi�n lo es
    delete temp; //Eliminamos el temporal(inicio de la cola)
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
        cout << "\t| ID: " << p.id << " | Nombre: " << p.nombre
             << " | Prioridad: " << p.prioridad << " | Memoria: " << p.memoria << "MB\n";
        actual = actual->siguiente;
    }
}

//eliminar por ID
bool eliminarDeColaPorId(NodoCola*& frente, NodoCola*& final, int id) {
    if (!frente) return false; //si el frente es NULL no hay para para eliminar
    NodoCola* actual = frente; //puntero actual apunta al que est� al frente
    NodoCola* anterior = NULL; //anterior es NULL porque apunta al que est� antes del frente

    while (actual) { //mientra el puntero actual no sea NULL
        if (actual->proceso.id == id) { //si el nodo actual es el ID que quieres eliminar
            if (anterior == NULL) { //y el anterior es NULL y
                // El frente ahora es el nodo que sigue
                frente = actual->siguiente; //pero
                if (!frente) final = NULL; //si el frente es NULL, el final tambi�n lo es
            } else {
                anterior->siguiente = actual->siguiente; //el anterior y el actual ahora son el mismo
                if (actual == final) { //si el actual es igual al final
                    final = anterior; //el final entonces ser� igual al anterior
                }
            }
            delete actual; //eliminamos el actual
            return true;
        }
        anterior = actual; //el anterior toma el lugar del actual
        actual = actual->siguiente; //el actual ahora es el nodo que sigue
    }
    return false;
}

// Funciones para Pila - Dayana
void asignarMemoria(NodoLista* cabeza) {
    int idBuscado;
    cout << "\tIngrese el ID del proceso al que desea asignar memoria: ";
    cin >> idBuscado;
    // Busca el proceso en la lista
    NodoLista* actual = cabeza;
    while (actual && actual->proceso.id != idBuscado) {
        actual = actual->siguiente;
    }

    if (!actual) {
        cout << "\tEl proceso con ID "<<idBuscado<<" no existe.\n";
        return;
    }

    if (libre == -1) {
        cout << "\tNo hay bloques de memoria disponibles.\n";
        return;
    }
    
        // Validar memoria disponible
    if (memoriaUsada + actual->proceso.memoria > MAX_PILA) {
        cout << "\tNo hay suficiente memoria disponible. Solo quedan " 
             << (MAX_PILA - memoriaUsada) << " MB.\n";
        return;
    }

    if (libre == -1) {
        cout << "\tNo hay bloques de memoria disponibles.\n";
        return;
    }

    // Asigna el bloque al proceso
    int nuevoBloque = libre;
    libre = siguiente[libre]; // Actualiza el siguiente espacio libre

    procesos[nuevoBloque] = actual->proceso.id;
    siguiente[nuevoBloque] = tope; // El nuevo bloque apunta al anterior tope
    tope = nuevoBloque;            // El nuevo bloque ahora es el tope

    memoriaUsada += actual->proceso.memoria;      // Suma la memoria del proceso a la memoria�total�usada

    cout << "\tMemoria asignada correctamente al proceso " << actual->proceso.nombre << " (ID: " << actual->proceso.id << ").\n";
}

void verTope() {
    if (tope == -1) {
        cout << "\tLa pila de memoria est� vac�a.\n";
        return;
    }
    cout << "\tEl bloque en el tope est� asignado al proceso con ID: " << procesos[tope] << "\n";
}

// Funci�n para liberar memoria asignada a un proceso en la pila - Yatzuri
// Funci�n para liberar memoria asignada a un proceso en la pila - Yatzuri
void liberarMemoria(NodoLista* cabeza) {
    // Verifica si la pila est� vac�a antes de liberar
    if (tope == -1) {
        cout << "\tError: la pila de memoria est� vac�a. No hay nada que liberar.\n";
        return;
    }

    int idLiberar;
    cout << "\tIngrese el ID del proceso cuya memoria desea liberar: ";
    cin >> idLiberar;

    // Verificar primero si est� en el tope (comportamiento LIFO normal)
    if (procesos[tope] == idLiberar) {
        // Buscar el proceso en la lista para saber cu�nta memoria liberar
        NodoLista* nodoProceso = cabeza;
        while (nodoProceso && nodoProceso->proceso.id != idLiberar) {
            nodoProceso = nodoProceso->siguiente;
        }

        if (!nodoProceso) {
            cout << "\tError: no se encontr� el proceso en la lista (ID: " << idLiberar << ").\n";
            return;
        }

        // Restar la memoria liberada del total usado
        memoriaUsada -= nodoProceso->proceso.memoria;

        // Muestra el proceso liberado
        cout << "\tLiberando memoria del proceso: " << nodoProceso->proceso.nombre 
             << " (ID: " << idLiberar << ", " 
             << nodoProceso->proceso.memoria << " MB).\n";

        // Liberar el bloque del tope
        int bloqueLiberado = tope;
        tope = siguiente[tope];           // Retrocede en la pila
        siguiente[bloqueLiberado] = libre; // Conecta el bloque liberado a la lista de libres
        libre = bloqueLiberado;           // Ese bloque ahora est� disponible
        return;
    }

    // Si no est� en el tope, preguntar si quiere forzar la liberaci�n
    char opcion;
    cout << "\tEl proceso no est� en el tope. �Desea liberarlo de todas formas? (s/n): ";
    cin >> opcion;

    if (opcion != 's' && opcion != 'S') {
        cout << "\tOperaci�n cancelada. Solo puede liberar el proceso en el tope (LIFO).\n";
        return;
    }

    // Buscar el proceso en la pila
    int actual = tope;
    int anterior = -1;
    bool encontrado = false;

    // Recorremos la pila para encontrar el proceso
    while (actual != -1) {
        if (procesos[actual] == idLiberar) {
            encontrado = true;
            break;
        }
        anterior = actual;
        actual = siguiente[actual];
    }

    if (!encontrado) {
        cout << "\tError: no se encontr� el proceso con ID " << idLiberar << " en la memoria.\n";
        return;
    }

    // Buscar el proceso en la lista para saber cu�nta memoria liberar
    NodoLista* nodoProceso = cabeza;
    while (nodoProceso && nodoProceso->proceso.id != idLiberar) {
        nodoProceso = nodoProceso->siguiente;
    }

    if (!nodoProceso) {
        cout << "\tError: no se encontr� el proceso en la lista (ID: " << idLiberar << ").\n";
        return;
    }

    // Restar la memoria liberada del total usado
    memoriaUsada -= nodoProceso->proceso.memoria;

    // Muestra el proceso liberado
    cout << "\tLiberando memoria del proceso: " << nodoProceso->proceso.nombre 
         << " (ID: " << idLiberar << ", " 
         << nodoProceso->proceso.memoria << " MB).\n";

    // Liberar el bloque: actualizar los punteros y manejar la lista de espacios libres
    if (anterior == -1) {
        // Caso cuando el proceso est� en el tope (no deber�a pasar aqu�)
        int bloqueLiberado = tope;
        tope = siguiente[tope];
        siguiente[bloqueLiberado] = libre;
        libre = bloqueLiberado;
    } else {
        // Caso cuando el proceso est� en medio de la pila
        siguiente[anterior] = siguiente[actual]; // Salta el bloque a liberar
        siguiente[actual] = libre;               // El bloque liberado apunta al antiguo libre
        libre = actual;                          // Ahora el libre es este bloque
    }
}

void VerificarVacia() {
    // Verifica si la pila de memoria est� vac�a
    if (tope == -1) {
        cout << "\tVerdadero (vac�a)\n";
    } else {
        cout << "\tFalso (con datos). Estado de la memoria:\n";
        cout << "\t- Memoria usada: " << memoriaUsada << " MB\n";// Muestra la cantidad total de memoria actualmente en uso (en MB)
        cout << "\t- Memoria disponible: " << (MAX_PILA - memoriaUsada) << " MB\n";// Calcula y muestra la memoria disponible
        cout << "\t- Procesos en memoria (del m�s reciente al m�s antiguo):\n";
        
        int actual = tope;// Inicia desde el tope de la pila (bloque m�s reciente)
        int contador = 0;// Contador para llevar registro de bloques asignados
        
        while (actual != -1) {// Recorre la pila mientras haya bloques asignados (actual != -1)
            cout << "\t  Bloque " << actual << ": Proceso ID " << procesos[actual] << "\n";
            actual = siguiente[actual];// Avanza al bloque anterior en la pila (usando el arreglo siguiente[])
            contador++;// Incrementa el contador de bloques asignados
        }
        cout << "\t- Total de bloques asignados: " << contador << "\n";
        cout << "\t- Bloques libres: ";
        
        if (libre == -1) {// Verifica si hay bloques libres (libre == -1 indica que no hay)
            cout << "No hay bloques libres\n";
        } else {
            
            int temp = libre;// Si hay bloques libres, comienza desde el primero (�ndice libre)
            while (temp != -1) {
                cout << temp << " ";// Muestra el n�mero del bloque libre
                temp = siguiente[temp];// Avanza al siguiente bloque libre usando el arreglo siguiente[]
            }
            cout << "\n";
        }
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

    void liberarLista() {
        NodoLista* actual = cabeza;
        while (actual) {
            NodoLista* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
        cabeza = NULL;
    }

public:
    GestorProceso() : cabeza(NULL) {}

    ~GestorProceso() {
        liberarLista();
    }
    
    NodoLista* getCabeza() const {
        return cabeza;
    }
	//Funci�n para agregar un nuevo proceso a la lista ligada 
	void agregarProceso() { //Dayana
		Proceso p;
	    //Validar que el ID sea un n�mero de 3 d�gitos entre 100 y 999
	    do {
	        cout << "\n\tIngrese ID del proceso\t\t: ";
	        cin >> p.id;
	        if (p.id < 100 || p.id > 999) {
	            cout << "\tERROR, el ID debe ser un n�mero de 3 d�gitos (100 a 999)\n";
	        }
	    } while (p.id < 100 || p.id > 999);
	
	    //Solicitar nombre del proceso
	    cout << "\tIngrese nombre del proceso\t: ";
	    cin >> p.nombre;
	
	    //Validar que la prioridad est� entre 1 y 5
	    do {
	        cout << "\tIngrese prioridad (1 a 5)\t: ";
	        cin >> p.prioridad;
	        if (p.prioridad < 1 || p.prioridad > 5) {
	            cout << "\tERROR, ingrese una prioridad v�lida (entre 1 y 5)\n";
	        }
	    } while (p.prioridad < 1 || p.prioridad > 5);
	
	    //Validar que la memoria sea un n�mero positivo
	    do {
	        cout << "\tIngrese memoria (en MB)\t\t: ";
	        cin >> p.memoria;
	        if (p.memoria <= 0) {
	            cout << "\tERROR, la memoria debe ser un n�mero positivo.\n";
	        }
	    } while (p.memoria <= 0);
	
	    //Crear un nuevo nodo para la lista ligada y asignar el proceso
	    NodoLista* nuevoNodo = new NodoLista();
	    nuevoNodo->proceso = p;
	    nuevoNodo->siguiente = NULL;
	
	    //Insertar el nodo al final de la lista
	    if (cabeza == NULL) {
	        cabeza = nuevoNodo;  //Si la lista est� vac�a, el nuevo nodo es la cabeza
	    } else {
	        NodoLista* actual = cabeza;
	        // Recorrer hasta el �ltimo nodo
	        while (actual->siguiente != NULL) {
	            actual = actual->siguiente;
	        }
	        actual->siguiente = nuevoNodo;  //Enlazar el nuevo nodo al final
	    }
	
	    cout << "\t-- Proceso insertado correctamente. --\n";
	    encolar(frente, final, p);
	}

    // Funci�n para eliminar un proceso de la lista ligada por su ID
    bool eliminarProceso(int id) {
        if (!cabeza) return false;
        if (cabeza->proceso.id == id) {
            NodoLista* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
	    eliminarDeColaPorId(frente, final, id);
            return true;
        }
        NodoLista* actual = cabeza;
        while (actual->siguiente) {
            if (actual->siguiente->proceso.id == id) {
                NodoLista* temp = actual->siguiente;
                actual->siguiente = temp->siguiente;
                delete temp;
		eliminarDeColaPorId(frente, final, id);
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }
// Funci�n que busca un proceso por su ID y retorna un puntero al proceso si lo encuentra
    Proceso* buscarProceso(int id) { 
        NodoLista* nodo = buscarNodo(id); // Busca el nodo que contiene el proceso con ese ID
        return nodo ? &(nodo->proceso) : NULL; // Si lo encuentra, retorna direcci�n del proceso; si no, retorna NULL
    }
// Funci�n que modifica la prioridad de un proceso identificado por su ID
    bool modificarPrioridad(int id, int nuevaPrioridad) {
        NodoLista* nodo = buscarNodo(id);  // Busca el nodo correspondiente al ID dado
        if (nodo) {
            nodo->proceso.prioridad = nuevaPrioridad; // Actualiza la prioridad del proceso

 // Para mantener la cola de prioridad actualizada,
// se elimina el proceso de la cola y se vuelve a insertar (reencolar)
            eliminarDeColaPorId(frente, final, id); // Elimina el proceso antiguo de la cola
            encolar(frente, final, nodo->proceso); // Vuelve a encolarlo con la nueva prioridad

            return true;  // Operaci�n exitosa
        }
        return false; // No se encontr� el proceso con ese ID
    }
// Funci�n que muestra todos los procesos registrados en la lista enlazada
    void mostrarProcesos() {
        if (!cabeza) { // Verifica si la lista est� vac�a
            cout << "\tNo hay procesos para mostrar.\n";
            return;
        }
        NodoLista* actual = cabeza; // Puntero auxiliar para recorrer la lista desde la cabeza
        while (actual) {
            cout << "\t| ID: " << actual->proceso.id  // Muestra el ID del proceso
                 << " | Nombre: " << actual->proceso.nombre // Muestra el nombre
                 << " | Prioridad: " << actual->proceso.prioridad  // Muestra la prioridad
                 << " | Memoria: " << actual->proceso.memoria << " MB\n"; // Muestra la memoria
            actual = actual->siguiente; // Avanza al siguiente nodo de la lista
        }
    }
// Funci�n para guardar los procesos de la lista en un archivo de texto
    void guardarArchivo(const string& nombreArchivo) {
        ofstream archivo(nombreArchivo.c_str());
// Se abre el archivo en modo escritura
        if (!archivo) {
            cout << "Error al abrir el archivo para guardar.\n";
            return;
        }
 // Se inicia desde el principio de la lista
        NodoLista* actual = cabeza;
// Se recorre la lista enlazada y se escriben los datos de cada proceso en el archivo
        while (actual) {
            archivo << actual->proceso.id << ","       // Escribe el ID del proceso
                    << actual->proceso.nombre << ","   // Escribe el nombre del proceso
                    << actual->proceso.prioridad << "," // Escribe la prioridad del proceso
                    << actual->proceso.memoria << "\n"; // Escribe la memoria del proceso y salta de l�nea
            actual = actual->siguiente;  // Avanza al siguiente nodo
        }
        archivo.close();  // Cierra el archivo despu�s de escribir
    }
// Funci�n para cargar los procesos desde un archivo de texto a la lista enlazada
   void cargarArchivo(const string& nombreArchivo) {
 // Se abre el archivo en modo lectura
        ifstream archivo(nombreArchivo.c_str());
 // Si no se pudo abrir el archivo, se muestra un mensaje de error
        if (!archivo) {
            cout << "\tError al abrir el archivo para cargar.\n";
            return;
        }
        liberarLista(); // Se limpia la lista actual antes de cargar nuevos datos
        string linea; // Variable para guardar cada l�nea del archivo
 // Se lee el archivo l�nea por l�nea
        while (getline(archivo, linea)) {
            Proceso p; // Estructura donde se almacenar� cada proceso
            size_t pos = 0;
 // Extraer el ID del proceso
            pos = linea.find(',');
            if (pos == string::npos) continue; // Si no se encuentra una coma, se omite la l�nea
            p.id = atoi(linea.substr(0, pos).c_str());  // Convierte el texto en entero
            linea.erase(0, pos + 1); // Elimina la parte ya le�da
 // Extraer el nombre del proceso
            pos = linea.find(',');
            if (pos == string::npos) continue; // Guarda el nombre
            p.nombre = linea.substr(0, pos);
            linea.erase(0, pos + 1);
 // Extraer la prioridad del proceso
            pos = linea.find(',');
            if (pos == string::npos) continue;
            p.prioridad = atoi(linea.substr(0, pos).c_str());
            linea.erase(0, pos + 1);
 // Extraer la memoria del proceso (lo que queda)
            p.memoria = atoi(linea.c_str());
 // Agrega el proceso a la lista enlazada
            agregarProceso();
        }
        archivo.close(); ; // Cierra el archivo despu�s de leer
    }
};

//Funcion de SubMenu cola cuando el usuario ingrese la opcion 8
void SubMenuCola(GestorProceso& gestor) {
    setlocale(LC_CTYPE, "Spanish");
    int opColita;
    do {
	//mostramos las opciones para el SubMenuCola
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
				gestor.agregarProceso();       
			    break;
			}
            case 2: {
                if (frente) {
                    Proceso p = frente->proceso;
                    desencolar(frente, final);
                    gestor.eliminarProceso(p.id);
                    cout << "\tProceso con ID " << p.id << " ejecutado y eliminado.\n";
                } else {
                    cout << "\tLa cola esta vacia.\n";
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
                cout << "\tERROR, Ingrese una opcion valida del submenu.\n";
        }
    } while (opColita != 4);
}

void SubMenuPila(GestorProceso gestor) {
    int opcion;
    do {
        cout << "\n\t************ PILA DE MEMORIA ************\n";
        cout << "\t-------------------------------------------";
        cout << "\n\t[1]. Asignar Memoria";
        cout << "\n\t[2]. Liberar Memoria";
        cout << "\n\t[3]. Ver tope";
        cout << "\n\t[4]. Verificar Vac�a";
        cout << "\n\t[5]. Volver\n";
        cout << "\n\tElija una opci�n: "; 
        cin>>opcion;
        
        switch(opcion) {
            case 1:
				asignarMemoria(gestor.getCabeza());		
			break;
            case 2: 
				liberarMemoria(gestor.getCabeza()); 
			break;
            case 3: 
				verTope(); 
			break;
            case 4: 
				VerificarVacia(); 
			break;
            case 5: cout << "\tVolviendo al men� principal...\n"; break;
            default: cout << "\tOpci�n inv�lida\n";
        }
    } while (opcion != 5);
}

void mostrarMenuPrincipal() {
    cout << "\n\t===== SISTEMA DE GESTION DE PROCESOS =====\n";
    cout << "\t[1]. Agregar proceso\n";
    cout << "\t[2]. Eliminar proceso\n";
    cout << "\t[3]. Buscar proceso por ID\n";
    cout << "\t[4]. Modificar prioridad\n";
    cout << "\t[5]. Mostrar todos los procesos\n";
    cout << "\t[6]. Guardar procesos en archivo\n";
    cout << "\t[7]. Cargar procesos desde archivo\n";
    cout << "\t[8]. Cola de procesos\n"; //agregamos la opci�n 8 en el men� principal para que sea visible por el usuario.
    cout << "\t[9]. Pila de Memoria\n"; // opcion para pila (Dayana y Yatzuri)
    cout << "\t[0]. Salir\n";
    cout << "\tSeleccione una opci�n: ";
}

int main() {
    setlocale(LC_CTYPE,"Spanish");
    GestorProceso gestor;
    int opcion;

    do {
        mostrarMenuPrincipal();
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
		    case 1: {
                gestor.agregarProceso();
                break;
			break;
		    }
            case 2: {
                int id;
                cout << "\tIngrese ID del proceso a eliminar: ";
                cin >> id;
                cin.ignore();
                if (gestor.eliminarProceso(id))
                    cout << "\tProceso eliminado correctamente.\n";
                else
                    cout << "\tNo se encontr� el proceso.\n";
                break;
            }
            case 3: {
                int id;
                cout << "\tIngrese ID del proceso a buscar: ";
                cin >> id;
                cin.ignore();
                Proceso* p = gestor.buscarProceso(id);
                if (p)
                    cout << "\t| ID: " << p->id
                         << " | Nombre: " << p->nombre
                         << " | Prioridad: " << p->prioridad
                         << " | Memoria: " << p->memoria << " MB\n";
                else
                    cout << "\tNo se encontr� el proceso.\n";
                break;
            }
            case 4: {
                int id, nuevaPrioridad;
                cout << "\tIngrese ID del proceso a modificar: ";
                cin >> id;
                cout << "\tIngrese nueva prioridad: ";
                cin >> nuevaPrioridad;
                cin.ignore();
                if (gestor.modificarPrioridad(id, nuevaPrioridad))
                    cout << "\tPrioridad modificada correctamente.\n";
                else
                    cout << "\tNo se encontr� el proceso.\n";
                break;
            }
            case 5:
                gestor.mostrarProcesos();
                break;
            case 6: {
                string nombreArchivo;
                cout << "\tIngrese nombre del archivo para guardar: ";
                cin.ignore(); // Agregado por seguridad
                getline(cin, nombreArchivo);
                gestor.guardarArchivo(nombreArchivo);
                cout << "\tProcesos guardados correctamente.\n";
                break;
            }
            case 7: {
                string nombreArchivo;
                cout << "\tIngrese nombre del archivo para cargar: ";
                cin.ignore(); // Agregado por seguridad
                getline(cin, nombreArchivo);
                gestor.cargarArchivo(nombreArchivo);
                cout << "\tProcesos cargados correctamente.\n";
                break;
            }
            case 8: {
                SubMenuCola(gestor); //llamamos a la funcion de submenuCola cuando el usuario ingrese la opci�n 8 del men� principal
			break;
            }
            case 9: {
                SubMenuPila(gestor); 
			break;
            }
            case 0:
                cout << "\tSaliendo del programa...\n";
                break;
            default:
                cout << "\tOpci�n no v�lida. Intente de nuevo.\n";
        }
    } while (opcion != 0);
    return 0;
}
