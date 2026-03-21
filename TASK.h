#ifndef TASK_H
#define TASK_H    

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
        void setDueDate(int DueDate);
        void checkedComplete();


};

#endif