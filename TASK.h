#ifndef TASK_H
#define TASK    

#include <string>
using namespace std;

class Task {
    private: 
    int ID;
    string title;
    string description;
    string course;
    int priority;
    string DueDate;
    bool complete;

    public: //Client will use this only
    Task();
};

#endif