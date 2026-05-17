#ifndef HISTORY_H
#define HISTORY_H

#include "TASK.h"
#include "TASKLIST.h"
#include "HASHTABLE.h"
#include "TASKGRAPH.h"

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