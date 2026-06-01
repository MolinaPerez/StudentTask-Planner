#ifndef TASKGRAPH_H
#define TASKGRAPH_H

#include "HASHTABLE.h"
#include <iosfwd>

// TASKGRAPH.h / TASKGRAPH.cpp
// Directed graph of task dependencies. Each edge taskID -> prereqID means
// taskID cannot be marked complete until prereqID is finished.
// Uses DFS cycle detection to prevent circular dependencies.
// Authors: Alex Molina Perez, Gustavo Ramirez Renta
// COMP 3075 - Introduction to Data Structures, RUM

// Linked list of IDs, used to track the prerequisites of a task.
struct IntNode {
    int data;
    IntNode* next;
};

// GraphNode represents a task and the list of IDs it depends on.
// (Linking the specific task to its required prerequisite IDs)
// If this connection exists: taskID -> prereqID = "taskID needs prereqID done first".
struct GraphNode {
    int taskID;
    IntNode* prereqs;
    bool visited;          // Used by the Depth First Search algorithm to traverse the list and detect cycles.
    GraphNode* next;
};

class TaskGraph {
private:
    GraphNode* head;
    int size;

    GraphNode* findNode(int taskID);
    void resetVisited();                       // Resets the visited flag on every node before starting a new traversal
    bool canReach(int from, int target);       // DFS: can we reach "target" from "from"

public:
    TaskGraph();
    ~TaskGraph();

    // Adds a dependency to a task: taskID -> prereqID.
    // Returns false if: same ID, nonexistent ID, duplicate, or would close a cycle.
    bool addDependency(int taskID, int prereqID, HashTable& hash);

    // Removes taskID and any reference to it (call this when deleting a task).
    void removeAllInvolving(int taskID);

    // Returns true if all prereqs exist in the hash table and are marked as complete.
    bool canComplete(int taskID, HashTable& hash);

    // Prints the prereqs that are not yet complete (or have been deleted).
    // Useful for telling the user what they still need before completing a task.
    void showMissingPrereqs(int taskID, HashTable& hash);

    void showGraph(HashTable& hash);
    void saveToFile(std::ostream& out);  // writes each edge as a line "DEP|task|prereq"
    int getSize();
};

#endif
