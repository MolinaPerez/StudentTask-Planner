#ifndef TASK_H
#define TASK_H    

#include <string>
using namespace std;

// TASK.h / TASK.cpp
// Defines the Task class, the core data unit of the planner.
// Each task holds an ID, title, description, course, priority, due date, and completion status.
// Authors: Alex Molina Perez, Gustavo Ramirez Renta
// COMP 3075 - Introduction to Data Structures, RUM

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
    Task(int ID, string title, string description, string course, 
        int priority, string DueDate);

        int getID();
        string getTitle();
        string getDescription();
        string getCourse();
        int getPriority();
        string getDueDate();
        bool isComplete();

        void setTitle(string title);
        void setDescription(string description);
        void setCourse(string course);
        void setPriority(int priority);
        void setDueDate(string DueDate);
        void checkedComplete();

        //FOR TASK VIEWING

        void show();
};

#endif
