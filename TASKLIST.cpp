#include "TASKLIST.h"
#include <stdexcept>
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
        if (head == nullptr) {
            throw std::underflow_error("List is empty");
        }

        if (head->next == nullptr){
            Task removedTask = head->data;
            delete head;
            head = nullptr;
            
            size--;
            
            return removedTask;
        }

        Node* ptr = head;
        while(ptr->next->next != nullptr){
            ptr = ptr->next;
        }
        Task removedTask= ptr->next->data;
        delete ptr->next;
        ptr->next = nullptr;

        size--;
        
        return removedTask;
    }

    Task TaskList::peekTask() {
        if (head == nullptr) {
            throw std::underflow_error("List is empty");
        }

        Node* ptr = head;
        while(ptr->next != nullptr) {
            ptr = ptr->next;
        }

        return ptr->data;
    }

    void TaskList::showList() {
        if (head == nullptr) {
            throw std::underflow_error("List is empty");
        }

        Node* ptr = head;
        while (ptr != nullptr) {
            ptr->data.show();
            ptr = ptr->next;
        }
    }

    bool TaskList::searchTask(Task &t) {
        if (head == nullptr) {
            throw std::underflow_error("List is empty");
        }
        
        Node* ptr = head;
        while (ptr != nullptr) {
            if(ptr->data.getID() == t.getID()) {
                return true;
            }
            ptr = ptr->next;
        }
        return false;
    }

    void TaskList::setSize(int Size) {
        size = Size;
    }

    int TaskList::getSize() {
        return size;
    }