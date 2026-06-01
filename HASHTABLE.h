#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "TASK.h"

// HASHTABLE.h / HASHTABLE.cpp
// Hash table using separate chaining for O(1) average-time task lookup by ID.
// Runs parallel to the TaskList to enable fast search without traversing the list.
// Authors: Alex Molina Perez, Gustavo Ramirez Renta
// COMP 3075 - Introduction to Data Structures, RUM

// HashNode is simply a node in a bucket
// We use SEPARATE CHAINING for buckets
// Multiple tasks whose IDs hash to the same bucket can coexist.
struct HashNode {
    Task data;
    HashNode* next;
};

class HashTable {
private:
    static const int BUCKET_COUNT = 17;   // Hashing with 17 allows for a solid, spread out hash.
    HashNode* buckets[BUCKET_COUNT];      // array of bucket-heads (chains)
    int size;                             // total number of tasks stored in the Table

    // Maps integer ID to a bucket index ([0, BUCKET_COUNT-1]).
    int hash(int ID);

public:
    HashTable();
    ~HashTable();

    // Insert a task
    void insert(Task t);

    // Remove the task with the given ID.
    bool remove(int ID);

    // Search by ID. If found, copies the task into 'out'.
    bool search(int ID, Task& out);

    // Quick true or false for speed
    bool contains(int ID);

    //Visualization
    int  getSize();
    void showTable();   // prints each bucket and its contents
};

#endif
