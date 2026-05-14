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
    ~TaskList();                       // free all nodes when the list is destroyed (fixes memory leak)
    void addTask(const Task& t);
    Task removeTask(int ID);           // take ID by value so callers can pass a literal like removeTask(5)
    Task peekTask();
    void showList();
    void showByPriority();             // print tasks sorted by priority (1->3) without modifying
    bool searchTask(int ID);           // take ID by value so callers can pass a literal
    void setSize(int Size);
    int getSize();
};

#endif