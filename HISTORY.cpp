#include "HISTORY.h"
#include <stdexcept>
#include <iostream>

// HISTORY.h / HISTORY.cpp
// Undo/redo system implemented as two stacks of recorded actions.
// Supports reversing and re-applying ADD and REMOVE operations across
// the TaskList, HashTable, and TaskGraph simultaneously.
// Authors: Alex Molina Perez, Gustavo Ramirez Renta
// COMP 3075 - Introduction to Data Structures, RUM

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

    if (undoTop->action == "ADD") {  // Undo an ADD = remove the task from all 3 data structures.
        int ID = undoTop->data.getID();
        list.removeTask(ID);
        hash.remove(ID);
        graph.removeAllInvolving(ID);   // clean up ghost edges in the graph (fix bug #1 and #2)
    }
    else if (undoTop->action == "REMOVE"){  // Undo a REMOVE = put the task back.
        list.addTask(undoTop->data);        // The graph is not touched: since REMOVE didn't clean it,
        hash.insert(undoTop->data);         // the task's prereq edges still exist and are preserved.
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

    if (redoTop->action == "ADD") { // Redo an ADD = re-add the task to list and hash.
        list.addTask(redoTop->data);   // The graph is not touched: it was already clean
        hash.insert(redoTop->data);    // from the previous undo.
    }
    else if (redoTop->action == "REMOVE") {
        int ID = redoTop->data.getID();
        list.removeTask(ID);
        hash.remove(ID);
        graph.removeAllInvolving(ID);   // re-remove ghost edges from the graph (tasks left dangling by certain prereqs)
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