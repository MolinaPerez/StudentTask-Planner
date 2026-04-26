#ifndef HISTORY_H
#define HISTORY_H

#include "TASK.h"
#include "TASKLIST.h"
#include "HASHTABLE.h"

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
    // undo/redo now take the HashTable too so the hash stays in sync with tasklist.
    bool undo(TaskList& t, HashTable& h);
    bool redo(TaskList& t, HashTable& h);
    void clearRedo();
    bool canUndo();
    bool canRedo();
    void showHistory();
};

#endif