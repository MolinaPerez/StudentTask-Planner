#ifndef TASKGRAPH_H
#define TASKGRAPH_H

#include "HASHTABLE.h"

// Chain list de IDs, para teenr presente los prerequisitos de una tarea.
struct IntNode {
    int data;
    IntNode* next;
};

// GraphNode es una tarea y la lista de IDs de los que depende. (Encadenando la tarea especifica a esos IDs especificos y requeridos)
// Si existe esta conexion: taskID -> prereqID = "taskID necesita prereqID hecho primero".
struct GraphNode {
    int taskID;
    IntNode* prereqs;
    bool visited;          // Search Algorith Depth First Search para recorrer la lista de abajao hacia arriba. (El bool funciona para los ciclos)
    GraphNode* next;
};

class TaskGraph {
private:
    GraphNode* head;
    int size;

    GraphNode* findNode(int taskID);
    void resetVisited();                       // Resetea el bool para todo nodo para comenzar otro ciclo
    bool canReach(int from, int target);       // DFS: "from" hacia "target"

public:
    TaskGraph();
    ~TaskGraph();

    // Agrega una dependencia a una tarea: taskID -> prereqID.
    // Tira falso si: misma ID, ID inexistente, duplicado, o se culmina un ciclo.
    bool addDependency(int taskID, int prereqID, HashTable& hash);

    // Borra taskID y cualquier referencia a el (llamar al eliminar una tarea).
    void removeAllInvolving(int taskID);

    // Tira true si todos los prereqs existen en hashTABLE y estan marcados como completados.
    bool canComplete(int taskID, HashTable& hash);

    // Imprime los prereqs que aun no estan completos (o que fueron eliminados).
    // Util para informarle al usuario que le falta antes de poder completar la tarea.
    void showMissingPrereqs(int taskID, HashTable& hash);

    void showGraph(HashTable& hash);
    int getSize();
};

#endif
