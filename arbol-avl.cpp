#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <windows.h>

using namespace std;

typedef struct Nodo {
    int dato;
    struct Nodo* izquierda;
    struct Nodo* derecha;
    int altura;
} stcNodo;

stcNodo* raiz = NULL;

int obtenerAltura(stcNodo* nodo) {
    return nodo ? nodo->altura : -1;
}

int obtenerMayor(int a, int b) {
    return (a > b) ? a : b;
}

stcNodo* nuevoNodo(int dato) {
    stcNodo* nodo = (stcNodo*)malloc(sizeof(stcNodo));
    nodo->dato = dato;
    nodo->izquierda = nodo->derecha = NULL;
    nodo->altura = 0;
    return nodo;
}

stcNodo* rotarDerecha(stcNodo* y) {
    stcNodo* b = y->izquierda;
    stcNodo* c = b->derecha;

    b->derecha = y;
    y->izquierda = c;

    y->altura = obtenerMayor(obtenerAltura(y->izquierda), obtenerAltura(y->derecha)) + 1;
    b->altura = obtenerMayor(obtenerAltura(b->izquierda), obtenerAltura(b->derecha)) + 1;

    return b;
}

stcNodo* rotarIzquierda(stcNodo* x) {
    stcNodo* z = x->derecha;
    stcNodo* y = z->izquierda;

    z->izquierda = x;
    x->derecha = y;

    x->altura = obtenerMayor(obtenerAltura(x->izquierda), obtenerAltura(x->derecha)) + 1;
    z->altura = obtenerMayor(obtenerAltura(z->izquierda), obtenerAltura(z->derecha)) + 1;

    return z;
}

int obtenerBalance(stcNodo* nodo) {
    return nodo ? obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha) : 0;
}

stcNodo* insertar(stcNodo* nodo, int dato) {
    if (!nodo){
        cout << "El valor ["<< dato <<"] se ha ingresado correctamente" << endl;
        return nuevoNodo(dato);
    }
    if (dato < nodo->dato)
        nodo->izquierda = insertar(nodo->izquierda, dato);
    else if (dato > nodo->dato)
        nodo->derecha = insertar(nodo->derecha, dato);
    else{
        cout << "El valor ya existe, por lo que no se ha ingresado en el arbol" << endl;
        return nodo;
    }

    nodo->altura = 1 + obtenerMayor(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));

    int balance = obtenerBalance(nodo);

    if (balance > 1 && dato < nodo->izquierda->dato)
        return rotarDerecha(nodo);

    if (balance < -1 && dato > nodo->derecha->dato)
        return rotarIzquierda(nodo);

    if (balance > 1 && dato > nodo->izquierda->dato) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }

    if (balance < -1 && dato < nodo->derecha->dato) {
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

stcNodo* nodoMasDerecho(stcNodo* nodo) {
    stcNodo* actual = nodo;
    while (actual->derecha != NULL)
        actual = actual->derecha;
    return actual;
}

stcNodo* eliminar(stcNodo* nodoTemp, int dato) {
    if (!nodoTemp){
        cout << "ERROR! no se ha encontrado el valor en el arbol, no se ha podido eliminar" << endl;
        return nodoTemp;
    }

    if (dato < nodoTemp->dato)
        nodoTemp->izquierda = eliminar(nodoTemp->izquierda, dato);
    else if (dato > nodoTemp->dato)
        nodoTemp->derecha = eliminar(nodoTemp->derecha, dato);
    else {
        if ((!nodoTemp->izquierda) || (!nodoTemp->derecha)) {
            stcNodo* temp = nodoTemp->izquierda ? nodoTemp->izquierda : nodoTemp->derecha;

            if (!temp) {
                temp = nodoTemp;
                nodoTemp = NULL;
                free(nodoTemp);
            } else
                *nodoTemp = *temp;

            free(temp);
            cout << "El valor [" << dato << "] se ha eliminado satisfactoriamente" << endl;
        } else {
            stcNodo* temp = nodoMasDerecho(nodoTemp->izquierda);
            nodoTemp->dato = temp->dato;
            nodoTemp->izquierda = eliminar(nodoTemp->izquierda, temp->dato);
        }
    }

    if (!nodoTemp)
        return nodoTemp;

    nodoTemp->altura = 1 + obtenerMayor(obtenerAltura(nodoTemp->izquierda), obtenerAltura(nodoTemp->derecha));

    int balance = obtenerBalance(nodoTemp);

    if (balance > 1 && obtenerBalance(nodoTemp->izquierda) >= 0)
        return rotarDerecha(nodoTemp);

    if (balance > 1 && obtenerBalance(nodoTemp->izquierda) < 0) {
        nodoTemp->izquierda = rotarIzquierda(nodoTemp->izquierda);
        return rotarDerecha(nodoTemp);
    }

    if (balance < -1 && obtenerBalance(nodoTemp->derecha) <= 0)
        return rotarIzquierda(nodoTemp);

    if (balance < -1 && obtenerBalance(nodoTemp->derecha) > 0) {
        nodoTemp->derecha = rotarDerecha(nodoTemp->derecha);
        return rotarIzquierda(nodoTemp);
    }

    return nodoTemp;
}

void imprimirAVL() {
    if (raiz == NULL) {
        cout << "ERROR! El arbol esta vacio, no hay nada para mostrar." << endl;
        return;
    }

    vector<vector<string>> lineas;
    vector<stcNodo*> nivel;
    vector<stcNodo*> siguiente;
    nivel.push_back(raiz);

    int noNodos = 1;
    size_t maxDigitos = 0;

    while (noNodos != 0) {
        vector<string> linea;
        noNodos = 0;

        for (stcNodo* nodoTemp : nivel) {
            if (nodoTemp == NULL) {
                linea.push_back("");
                siguiente.push_back(NULL);
                siguiente.push_back(NULL);
            } else {
                string aa = "(" + to_string(nodoTemp->dato) + ")";
                linea.push_back(aa);
                if (aa.length() > maxDigitos) 
                    maxDigitos = aa.length();
                siguiente.push_back(nodoTemp->izquierda);
                siguiente.push_back(nodoTemp->derecha);
                if (nodoTemp->izquierda != NULL) 
                    noNodos++;
                if (nodoTemp->derecha != NULL) 
                    noNodos++;
            }
        }

        if (maxDigitos % 2 == 1) maxDigitos++;

        lineas.push_back(linea);

        vector<stcNodo*> tmp = nivel;
        nivel = siguiente;
        siguiente = tmp;
        siguiente.clear();
    }

    size_t porPieza = lineas.back().size() * (maxDigitos + 4);
    for (size_t i = 0; i < lineas.size(); i++) {
        vector<string> linea = lineas[i];
        size_t mediaPieza = floor(porPieza / 2.0) - 1;

        if (i > 0) {
            for (size_t j = 0; j < linea.size(); j++) {
                char c = ' ';
                if (j % 2 == 1) {
                    if (linea[j - 1] != "") {
                        c = (linea[j] != "") ? ' ' : ' ';
                    } else {
                        if (j < linea.size() && linea[j] != "") c = ' ';
                    }
                }
                cout << c;

                if (linea[j] == "") {
                    for (size_t k = 0; k < porPieza - 1; k++) {
                        cout << " ";
                    }
                } else {
                    for (size_t k = 0; k < mediaPieza; k++) {
                        cout << (j % 2 == 0 ? " " : " ");
                    }
                    cout << (j % 2 == 0 ? "/" : "\\");
                    for (size_t k = 0; k < mediaPieza; k++) {
                        cout << (j % 2 == 0 ? " " : " ");
                    }
                }
            }
            cout << endl;
        }

        for (size_t j = 0; j < linea.size(); j++) {
            string f = linea[j];
            size_t gap1 = ceil(porPieza / 2.0 - f.length() / 2.0);
            size_t gap2 = floor(porPieza / 2.0 - f.length() / 2.0);

            for (size_t k = 0; k < gap1; k++) {
                cout << " ";
            }
            cout << f;
            for (size_t k = 0; k < gap2; k++) {
                cout << " ";
            }
        }
        cout << endl;

        porPieza /= 2;
    }
}

stcNodo* buscarNodo(int valor, stcNodo *temp){
    if(temp->dato == valor){
        return temp;
    }else{
        if(temp->izquierda == NULL && temp->derecha == NULL){
            return NULL;
        }else if(valor < temp->dato){
            return buscarNodo(valor, temp->izquierda);
        }else{
            return buscarNodo(valor, temp->derecha);
        }
    }
}

void buscar (int valor){
    stcNodo* ptrNuevo = (stcNodo*)malloc(sizeof(stcNodo));
    ptrNuevo = buscarNodo(valor, raiz);

    if(ptrNuevo == NULL){
        cout << "El valor ingresado no se ha podido encontrar en el arbol :(" << endl;
    }else{
        cout << "El valor [" << valor << "] ha sido encontrado!" << endl;
    }
    getch();
}

int main() {
    int opcion, valor;

    do {
        system("cls");
        cout << "*****MANEJO DE ARBOL AVL*****\n\n";
        cout << "1. Insertar valor\n";
        cout << "2. Buscar valor\n";
        cout << "3. Eliminar valor\n";
        cout << "4. Mostrar arbol\n";
        cout << "5. Salir\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                system("cls");
                cout << "Ingrese el valor a insertar: ";
                cin >> valor;
                raiz = insertar(raiz, valor);
                getch();
            break;
            case 2:
                if(!raiz){
                    system("cls");
                    cout << "ERROR! no se puede buscar ya que el arbol esta vacio" << endl;
                    getch();
                }else{
                    system("cls");
                    cout << "Ingrese el valor que desea buscar: ";
                    cin >> valor;
                    cout << endl;
                    buscar(valor);
                }
            break;
            case 3:
                system("cls");
                    if (!raiz){
                        cout << "ERROR! no se puede eliminar ya que el arbol esta vacio" << endl;
                    }else{
                        cout << "Ingrese el valor a eliminar: ";
                        cin >> valor;
                        raiz = eliminar(raiz, valor);
                    }
                getch();
            break;
            case 4:
                system("cls");
                cout << "Mostrar Arbol AVL:\n";
                imprimirAVL();
                getch();
            break;
            case 5:
                cout << "Saliendo...\n";
            break;
            default:
                cout << "Opcion no valida.\n";
            break;
        }
    } while (opcion != 5);

    return 0;
}
