#include "HISTORY.h"
#include <stdexcept>
#include <iostream>

History::History(){
    undoTop = nullptr;
    redoTop = nullptr;
}
History::~History(){
    while (undoTop != nullptr){
        actionNode* temp = undoTop;
        undoTop = undoTop->next;
        delete temp;
    }
    clearRedo();
}

void History::clearRedo(){
    while (redoTop != nullptr){
        actionNode* temp= redoTop;
        redoTop = redoTop->next;
        delete temp;
    }
}

void History::record(string action, const Task &t){
    clearRedo();

    actionNode* newNode = new actionNode;
    newNode->action = action;
    newNode->data = t;
    newNode->next = undoTop;
    undoTop = newNode;
}

bool History::undo(TaskList& list, HashTable& hash, TaskGraph& graph){ //Also included taskgarph for dependencies
    if (undoTop == nullptr) {
        return false;
    }

    actionNode* temp= undoTop;

    if (undoTop->action == "ADD") {  // Deshacer un ADD = quitar la tarea de las 3 estructuras.
        int ID = undoTop->data.getID();
        list.removeTask(ID);
        hash.remove(ID);
        graph.removeAllInvolving(ID);   // limpiar fantasmas en el grafo (fix bug #1 y #2)
    }
    else if (undoTop->action == "REMOVE"){ // Deshacer un REMOVE = poner la tarea de vuelta.
        list.addTask(undoTop->data);     // El grafo no se toca: como REMOVE no lo limpio,
        hash.insert(undoTop->data);        //Siguen existiendo los prereqs de la tarea no se borran.
    }

    actionNode* redoNode = new actionNode;
    redoNode->action = undoTop->action;
    redoNode->data = undoTop->data;
    redoNode->next = redoTop;
    redoTop = redoNode;

    undoTop = undoTop-> next;
    delete temp;
    return true;
}

bool History::redo(TaskList& list, HashTable& hash, TaskGraph& graph){
    if(redoTop == nullptr){
        return false;
    }

    actionNode* temp = redoTop;

    if (redoTop->action == "ADD") { // Rehacer un ADD = re-agregar a list y hash.
        list.addTask(redoTop->data);   // El grafo no se toca: ya estaba limpio
        hash.insert(redoTop->data);    // desde el undo previo.
    }
    else if (redoTop->action == "REMOVE") {
        int ID = redoTop->data.getID();
        list.removeTask(ID);
        hash.remove(ID);
        graph.removeAllInvolving(ID);   // re-quitar fantasmas del grafo (TAreas que se quedan colgantes por ciertos prereqs)
    }
    actionNode* undoNode = new actionNode;
    undoNode->action = redoTop->action;
    undoNode->data = redoTop-> data;
    undoNode->next = undoTop;
    undoTop = undoNode;

    redoTop = redoTop->next;
    delete temp;
    return true;
}

bool History::canUndo(){
    if(undoTop != nullptr)
        return true;
    return false;
}

bool History::canRedo(){
    if(redoTop != nullptr)
        return true;
    return false;
}

void History::showHistory(){
    if(undoTop == nullptr){
        throw std::underflow_error("History is empty");
    }

    actionNode* ptr = undoTop;
    while (ptr != nullptr){
        cout << endl;
        cout << "Action : " << ptr->action << endl;
        ptr->data.show();
        ptr = ptr->next;
    }
}