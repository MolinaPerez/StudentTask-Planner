#include "TASKGRAPH.h"
#include <iostream>
using namespace std;

// TASKGRAPH.h / TASKGRAPH.cpp
// Directed graph of task dependencies. Each edge taskID -> prereqID means
// taskID cannot be marked complete until prereqID is finished.
// Uses DFS cycle detection to prevent circular dependencies.
// Authors: Alex Molina Perez, Gustavo Ramirez Renta
// COMP 3075 - Introduction to Data Structures, RUM

TaskGraph::TaskGraph() {
    head = nullptr;
    size = 0;
}

TaskGraph::~TaskGraph() {
    // General Destructor to free the GraphNode and its prerequisites.
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

// DFS: can we reach 'target' from 'from' by following edges?
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

    // Would this form a cycle? If prereqID can reach taskID, the new edge would close a loop.
    resetVisited();
    if (canReach(prereqID, taskID)) return false;

    // Creates the TaskID node if it doesn't exist
    if (node == nullptr) {
        node = new GraphNode;
        node->taskID = taskID;
        node->prereqs = nullptr;
        node->visited = false;
        node->next = head;
        head = node;
        size++;
    }

    // insert prereqID in the head of the prerequisite list
    IntNode* newPrereq = new IntNode;
    newPrereq->data = prereqID;
    newPrereq->next = node->prereqs;
    node->prereqs = newPrereq;
    return true;
}

void TaskGraph::removeAllInvolving(int taskID) {
    // 1) delete the taskID node if it exists
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
        pointer = pointer->next; //relocates the pointer to avoid it from hanging
    }

    // 2) remove taskID from the prereq lists of all other nodes
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
    if (node == nullptr) return true;   // no prerequisites -> can be completed without going through any prior task"

    IntNode* pointer = node->prereqs;
    while (pointer != nullptr) {
        Task t;
        if (!hash.search(pointer->data, t)) return false;
        if (!t.isComplete()) return false;
        pointer = pointer->next;
    }
    return true;
}

// Iterates through prerequisites and, for each one that isn't complete (or no longer exists),
// prints a line so the user knows what they still need to finish.
void TaskGraph::showMissingPrereqs(int taskID, HashTable& hash) { 
    // Called from menu option 6 when prerequisites are incomplete, to show the user which ones are still missing.
    GraphNode* node = findNode(taskID);
    if (node == nullptr) return;   // Has no prerequisites registered, nothing to show

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
//Graph Visualization
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
// Writes each graph edge to the stream, one per line: DEP|taskID|prereqID
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

// Simple getter for display and management purposes"
int TaskGraph::getSize() {
    return size;
}
