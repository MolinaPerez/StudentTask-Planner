#ifndef HISTORY_H
#define HISTORY_H

#include "TASK.h"
#include "TASKLIST.h"

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
    bool undo(TaskList& t);
    bool redo(TaskList& t);
    void clearRedo();
    bool canUndo();
    bool canRedo();
};

#endif