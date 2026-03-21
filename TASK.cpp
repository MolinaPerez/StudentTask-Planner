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

Task::Task(int ID, std::string title, std::string description,
           std::string course, int priority, std::string dueDate) {
    this->ID = ID;
    this->title = title;
    this->description = description;
    this->course = course;
    this->priority = priority;
    this->DueDate = DueDate;
    this->complete = false;
}

int Task::getID(){
    return ID;
}
string Task::getTitle(){
    return title;
}
string Task::getDescription(){
    return description;
}
string Task::getCourse(){
    return course;
}
int Task::getPriority(){
    return priority;
}
string Task::getDueDate(){
    return DueDate;
}
bool Task::isComplete(){
    return complete;
}

void Task::setTitle(string title){
    this -> title = title;
}
void Task::setDescription(string description){
    this -> description = description;
}
void Task::setCourse(string course){
    this -> course = course;
}
void Task::setPriority(int priority){
    this -> priority = priority;
}
void Task::setDueDate(int DueDate){
    this -> DueDate = DueDate;
}
void Task::checkedComplete(){
    complete = true;
}