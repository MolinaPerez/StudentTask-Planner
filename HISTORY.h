#ifndef HISTORY_H
#define HISTORY_H

#include "TASK.h"
#include "TASKLIST.h"
#include "HASHTABLE.h"
#include "TASKGRAPH.h"

// HISTORY.h / HISTORY.cpp
// Undo/redo system implemented as two stacks of recorded actions.
// Supports reversing and re-applying ADD and REMOVE operations across
// the TaskList, HashTable, and TaskGraph simultaneously.
// Authors: Alex Molina Perez, Gustavo Ramirez Renta
// COMP 3075 - Introduction to Data Structures, RUM

struct actionNode{
    string action;
    Task data;
    actionNode* next;
};

class History{
private:
    actionNode* undoTop;
    actionNode* redoTop;

public:
    History();
    ~History();

    void record(string action, const Task &t);
    // undo/redo reciben tambien el grafo para mantener todo sincronizado. (TaskGraph)
    bool undo(TaskList& t, HashTable& h, TaskGraph& g);
    bool redo(TaskList& t, HashTable& h, TaskGraph& g);
    void clearRedo();
    bool canUndo();
    bool canRedo();
    void showHistory();
};

#endif