#include "TASKGRAPH.h"
#include <iostream>
using namespace std;

TaskGraph::TaskGraph() {
    head = nullptr;
    size = 0;
}

TaskGraph::~TaskGraph() {
    // Deconstrcutor general para liberar el GraphNode y sus prerequisitos.
    while (head != nullptr) {
        GraphNode* tmp = head;
        head = head->next;
        while (tmp->prereqs != nullptr) {
            IntNode* pointer = tmp->prereqs;
            tmp->prereqs = pointer->next;
            delete pointer;
        }
        delete tmp;
    }
}

GraphNode* TaskGraph::findNode(int taskID) {
    GraphNode* pointer = head;
    while (pointer != nullptr) {
        if (pointer->taskID == taskID) return pointer;
        pointer = pointer->next;
    }
    return nullptr;
}

void TaskGraph::resetVisited() {
    GraphNode* pointer = head;
    while (pointer != nullptr) {
        pointer->visited = false;
        pointer = pointer->next;
    }
}

// DFS: ¿desde 'from' alcanzo 'target' siguiendo aristas?
bool TaskGraph::canReach(int from, int target) {
    GraphNode* node = findNode(from);
    if (node == nullptr) return false;
    if (node->visited) return false;
    node->visited = true;

    IntNode* pointer = node->prereqs;
    while (pointer != nullptr) {
        if (pointer->data == target) return true;
        if (canReach(pointer->data, target)) return true;
        pointer = pointer->next;
    }
    return false;
}

bool TaskGraph::addDependency(int taskID, int prereqID, HashTable& hash) {
    // validaciones
    if (taskID == prereqID) return false;
    if (!hash.contains(taskID) || !hash.contains(prereqID)) return false;

    // ¿ya existe la dependencia?
    GraphNode* node = findNode(taskID);
    if (node != nullptr) {
        IntNode* pointer = node->prereqs;
        while (pointer != nullptr) {
            if (pointer->data == prereqID) return false;
            pointer = pointer->next;
        }
    }

    // Se formo un ciclo? Si prereqID llega a taskID, el new branch cierra.
    resetVisited();
    if (canReach(prereqID, taskID)) return false;

    // crear el nodo de taskID si no existe
    if (node == nullptr) {
        node = new GraphNode;
        node->taskID = taskID;
        node->prereqs = nullptr;
        node->visited = false;
        node->next = head;
        head = node;
        size++;
    }

    // insertar prereqID en la cabeza de la lista de prereqs
    IntNode* newPrereq = new IntNode;
    newPrereq->data = prereqID;
    newPrereq->next = node->prereqs;
    node->prereqs = newPrereq;
    return true;
}

void TaskGraph::removeAllInvolving(int taskID) {
    // 1) borrar el nodo de taskID si existe
    GraphNode* pointer = head;
    GraphNode* prev = nullptr;
    while (pointer != nullptr) {
        if (pointer->taskID == taskID) {
            if (prev == nullptr) head = pointer->next;
            else prev->next = pointer->next;
            while (pointer->prereqs != nullptr) {
                IntNode* tmp = pointer->prereqs;
                pointer->prereqs = tmp->next;
                delete tmp;
            }
            delete pointer;
            size--;
            break;
        }
        prev = pointer;
        pointer = pointer->next; //recolocar el puntero para que no s equede colgando
    }

    // 2) quitar taskID de las listas de prereqs de los demas nodos
    GraphNode* node = head;
    while (node != nullptr) {
        IntNode* prereqPointer = node->prereqs;
        IntNode* prereqPrev = nullptr;
        while (prereqPointer != nullptr) {
            if (prereqPointer->data == taskID) {
                IntNode* tmp = prereqPointer;
                if (prereqPrev == nullptr) node->prereqs = prereqPointer->next;
                else prereqPrev->next = prereqPointer->next;
                prereqPointer = prereqPointer->next;
                delete tmp;
            } else {
                prereqPrev = prereqPointer;
                prereqPointer = prereqPointer->next;
            }
        }
        node = node->next;
    }
}

bool TaskGraph::canComplete(int taskID, HashTable& hash) {
    GraphNode* node = findNode(taskID);
    if (node == nullptr) return true;   // sin prereqs -> se puede completar sin pasar por ningun proceso anterior

    IntNode* pointer = node->prereqs;
    while (pointer != nullptr) {
        Task t;
        if (!hash.search(pointer->data, t)) return false;
        if (!t.isComplete()) return false;
        pointer = pointer->next;
    }
    return true;
}

// Recorre los prereqs y, por cada uno que no este completo (o no exista),
// imprime una linea para que el usuario sepa que le hace falta.
void TaskGraph::showMissingPrereqs(int taskID, HashTable& hash) { 
    // Para caso 6 en el menu, si los prerequisitos no estan completados se llama esta funcion para demostrar cuales falta.
    GraphNode* node = findNode(taskID);
    if (node == nullptr) return;   // no tiene prereqs registrados, nada que mostrar

    IntNode* pointer = node->prereqs;
    while (pointer != nullptr) {
        Task t;
        if (!hash.search(pointer->data, t)) {
            cout << "  - [" << pointer->data << "] (eliminada)" << endl;
        } else if (!t.isComplete()) {
            cout << "  - [" << pointer->data << "] " << t.getTitle() << " (incompleta)" << endl;
        }
        pointer = pointer->next;
    }
}
//Visualizacion de el grafo 
void TaskGraph::showGraph(HashTable& hash) {
    cout << "--- Grafo de dependencias (" << size << " tareas) ---" << endl;
    if (head == nullptr) {
        cout << "(vacio)" << endl;
        return;
    }
    GraphNode* node = head;
    while (node != nullptr) {
        Task t;
        string title = "(desconocida)";
        if (hash.search(node->taskID, t)) title = t.getTitle();

        cout << "[" << node->taskID << "] " << title << " -> depende de: ";
        IntNode* pointer = node->prereqs;
        if (pointer == nullptr) {
            cout << "(ninguna)";
        } else {
            while (pointer != nullptr) {
                cout << "[" << pointer->data << "]";
                if (pointer->next != nullptr) cout << ", ";
                pointer = pointer->next;
            }
        }
        cout << endl;
        node = node->next;
    }
}
// Escribe cada arista del grafo al stream, una por linea: DEP|taskID|prereqID
void TaskGraph::saveToFile(std::ostream& out) {
    GraphNode* node = head;
    while (node != nullptr) {
        IntNode* pointer = node->prereqs;
        while (pointer != nullptr) {
            out << "DEP|" << node->taskID << "|" << pointer->data << "\n";
            pointer = pointer->next;
        }
        node = node->next;
    }
}

//Getter simple para visualizar y manejar
int TaskGraph::getSize() {
    return size;
}
