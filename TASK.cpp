#include "TASK.h"

Task::Task() { //No Parameters
    ID = 0;
    title = " ";
    description = " ";
    course = " ";
    priority = 0;
    DueDate = " "; 
    complete = false;

    
};

Task::Task(int id, std::string title, std::string description,
           std::string course, int priority, std::string dueDate) {
    this->ID = id;
    this->title = title;
    this->description = description;
    this->course = course;
    this->priority = priority;
    this->DueDate = dueDate;
    this->complete = false;
}