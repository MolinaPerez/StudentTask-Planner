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

bool History::undo(TaskList& list){
    if (undoTop == nullptr) {
        return false;
    }

    actionNode* temp= undoTop;
    
    if (undoTop->action == "ADD") {
        int ID = undoTop->data.getID();
        list.removeTask(ID);
    }
    else if (undoTop->action == "REMOVE"){
        list.addTask(undoTop->data);
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

bool History::redo(TaskList& list){
    if(redoTop == nullptr){
        return false;
    }

    actionNode* temp = redoTop;

    if (redoTop->action == "ADD") {
        list.addTask(redoTop->data);
    }
    else if (redoTop->action == "REMOVE") {
        int ID = redoTop->data.getID();
        list.removeTask(ID);
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