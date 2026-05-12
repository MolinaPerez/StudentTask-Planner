# StudentTask-Planner — Test Report

## TL;DR
- 107 / 107 logic tests pass.
- Compiles clean under `-Wall -Wextra -Wpedantic`.
- Scripted end-to-end run of `main.cpp` survives bad menu input, bad priority, invalid date, missing-ID removal, and full undo/redo cycles.
- AddressSanitizer found **a real memory leak (5,440 bytes / 34 allocations)** — `TaskList` has no destructor.
- Two design issues worth fixing: `searchTask` throws on empty list, and duplicate IDs are not enforced (they desync the list and the hash).

How to reproduce: `g++ -std=c++17 -Wall -Wextra test_planner.cpp TASK.cpp TASKLIST.cpp HASHTABLE.cpp HISTORY.cpp -o test_planner && ./test_planner`

---

## Bugs (ranked)

### 1. `TaskList` leaks every node it ever allocated — HIGH
Every `addTask` and copy-constructor `new Node` is leaked when the `TaskList` goes out of scope, because there is no `~TaskList()`. ASan output:

```
SUMMARY: AddressSanitizer: 5440 byte(s) leaked in 34 allocation(s).
  #1 ... TaskList::addTask(Task const&) at TASKLIST.cpp:37
  #1 ... TaskList::TaskList(TaskList const&) at TASKLIST.cpp:16
```

Fix — declare and implement a destructor (and you should add `operator=` to be safe under the rule of three):

```cpp
// TASKLIST.h
~TaskList();
TaskList& operator=(const TaskList& other);

// TASKLIST.cpp
TaskList::~TaskList() {
    Node* ptr = head;
    while (ptr) { Node* tmp = ptr; ptr = ptr->next; delete tmp; }
    head = nullptr;
    size = 0;
}
```

### 2. `TaskList::searchTask` throws on empty list — MEDIUM
Currently throws `underflow_error` when the list is empty. Most callers expect "not found" to be a normal `false` return. The hash version (`HashTable::search`) already returns `false` on empty — make these consistent.

```cpp
bool TaskList::searchTask(int& ID) {
    Node* ptr = head;
    while (ptr) { if (ptr->data.getID() == ID) return true; ptr = ptr->next; }
    return false;   // no exception
}
```

### 3. Duplicate IDs desync `TaskList` and `HashTable` — MEDIUM
`main.cpp` doesn't check that an ID is unique before inserting. If you add ID 5 twice, the hash overwrites (size stays 1) but the list keeps both nodes (size 2). After one removal the list still has an orphan that the hash can't see — the two structures are no longer in sync.

Fix — check uniqueness in the input loop before constructing the Task, e.g.:
```cpp
if (hash.contains(ID)) {
    cout << "ID already exists. Pick a different one.\n";
    continue;
}
```

### 4. `removeTask`/`searchTask` take `int&` instead of `int` — LOW
Forces the caller to put the ID in a variable; you can't call `list.removeTask(5)` with a literal. Cosmetic but worth simplifying — change to `int` since you don't mutate the parameter anywhere.

### 5. Tail-append is O(n) per insert — LOW
`TaskList::addTask` walks to the tail every time, making N inserts O(N²). For a planner that's fine in practice but a tail pointer is one line of code.

---

## What was tested (107 cases, all green)

Task class — default + parameter constructors, every getter, every setter, `checkedComplete`.

TaskList — empty operations (peek/remove/search), add/peek/search hits and misses, remove from head/middle/tail, remove on absent ID, copy constructor produces an independent deep copy.

HashTable — basic CRUD, search/contains/remove on empty, collisions on bucket 0 with IDs 0/17/34/51 (BUCKET_COUNT = 17), removing a node from the middle of a chain, duplicate-ID inserts (overwrite semantics), negative IDs.

History — `canUndo`/`canRedo` on empty, undo/redo of ADD and REMOVE keeping the list and hash in sync, recording a new action correctly clearing the redo stack, `showHistory` throws on empty.

isValidDate — leap-year edge cases (`29/02/2024` valid, `29/02/1900` invalid, `29/02/2000` valid), 30-day months, day 0, month 0/13, day 32, missing leading zeros, dashes instead of slashes, letters, empty string, year too short or too long.

Integration — 25 adds, remove every even ID, full content audit between the list and the hash, undo all 12 removals back to a synced state.

End-to-end — `./planner` driven by a scripted input file with deliberately bad menu input ("abc"), out-of-range option (9), invalid priority (9), invalid date (`31/02/2026`), missing ID lookup (99), undo + redo + leave history; exits cleanly.

---

## Files
- `test_planner.cpp` — the harness (drop-in, no extra dependencies).
- Compile command at the top of the file.
