#include "TASKLIST.h"

TaskList::TaskList() {}
TaskList::TaskList(const TaskList& other) {
    head = nullptr;
    size = 0;

    Node* current = other.head;
    while (current != nullptr) {
        Node* newNode = new Node;
        newNode->data = current->data;
        newNode->next = current->next;

        if(head == nullptr) {
            head = newNode;
        }
        else {
            Node* tail = head;
            while (tail->next != nullptr) {
                tail = tail->next;
            }
            tail->next = newNode;
        }
        
        size++;
        current = current->next;
    }
}

    void TaskList::addTask(const Task& t){
        Node* newNode = new Node;
        newNode->data = t;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* tail = head;
            while(tail->next != nullptr) {
                tail = tail->next;
            }
            tail->next =newNode;
        }
        size++;
    }

    Task TaskList::removeTask() {

    }

    Task TaskList::peekTask() {

    }

    bool TaskList::searchTask(const Task &ID) {

    }

    void TaskList::setSize(int Size) {
        size = Size;
    }

    int TaskList::getSize() {
        return size;
    }