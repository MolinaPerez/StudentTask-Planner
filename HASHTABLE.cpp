#include "HASHTABLE.h"
#include <iostream>

using namespace std;

HashTable::HashTable() {
    // Start every bucket as an empty
    for (int i = 0; i < BUCKET_COUNT; i++) {
        buckets[i] = nullptr;
    }
    size = 0;
}

HashTable::~HashTable() {
    // Data leak prevention, freeing up before use.
    for (int i = 0; i < BUCKET_COUNT; i++) {
        HashNode* ptr = buckets[i];
        while (ptr != nullptr) {
            HashNode* temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
        buckets[i] = nullptr;
    }
}

// Hashing TEchnique
// Absolute value first so a negative ID does not produce a negative hash index.
int HashTable::hash(int ID) {
    if (ID < 0) ID = -ID;
    return ID % BUCKET_COUNT;
}

void HashTable::insert(Task t) {
    int idx = hash(t.getID());

    // If task with same ID already in bucket, overwrite it.
    // (Duplicate entry prevention with the same key)
    HashNode* ptr = buckets[idx];
    while (ptr != nullptr) {
        if (ptr->data.getID() == t.getID()) {
            ptr->data = t;
            return;
        }
        ptr = ptr->next;
    }

    // Adding the new chain node 
    HashNode* newNode = new HashNode;
    newNode->data = t;
    newNode->next = buckets[idx];
    buckets[idx] = newNode;
    size++;
}

bool HashTable::remove(int ID) {
    int idx = hash(ID);
    HashNode* ptr  = buckets[idx];
    HashNode* prev = nullptr;

    while (ptr != nullptr) {
        if (ptr->data.getID() == ID) {
            // either we're at the head, or somewhere in the middle or end
            if (prev == nullptr) {
                buckets[idx] = ptr->next;
            } else {
                prev->next = ptr->next;
            }
            delete ptr;
            size--;
            return true;
        }
        prev = ptr;
        ptr  = ptr->next;
    }
    return false;   // ID does not exist in the table
}

bool HashTable::search(int ID, Task& out) {
    int idx = hash(ID);
    HashNode* ptr = buckets[idx];

    while (ptr != nullptr) {
        if (ptr->data.getID() == ID) {
            out = ptr->data;   // copy task back to the caller
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

bool HashTable::contains(int ID) {
    Task tmp;
    return search(ID, tmp);
}

int HashTable::getSize() {
    return size;
}

// Prints each bucket and its chain.
// collisions land in the same bucket as multiple chained nodes.
void HashTable::showTable() {
    cout << "--- Hash Table (" << size << " tasks across "
         << BUCKET_COUNT << " buckets) ---" << endl;

    for (int i = 0; i < BUCKET_COUNT; i++) {
        cout << "Bucket [" << i << "]: ";
        HashNode* ptr = buckets[i];

        if (ptr == nullptr) {
            cout << "(empty)" << endl;
            continue;
        }

        while (ptr != nullptr) {
            cout << "ID=" << ptr->data.getID()
                 << " (" << ptr->data.getTitle() << ")";
            if (ptr->next != nullptr) cout << " -> ";
            ptr = ptr->next;
        }
        cout << endl;
    }
}
