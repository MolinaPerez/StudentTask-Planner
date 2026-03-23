#ifndef TASKLIST_H
#define TASKLIST_H

#include "TASK.h"
struct Node {
    Task data;
    Node* next;
};

class TaskList {
private:
    Node* head;
    int size;

public:
    TaskList();
    TaskList(const TaskList& other);
    void addTask(const Task& t);
    Task removeTask();
    Task peekTask();
    bool searchTask(const Task &ID);
    int setSize();
    int getSize();
};

#endif