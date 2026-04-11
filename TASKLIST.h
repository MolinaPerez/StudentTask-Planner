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
    Task removeTask(int &ID);
    Task peekTask();
    void showList();
    bool searchTask(int& ID);
    void setSize(int Size);
    int getSize();
};

#endif