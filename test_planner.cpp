// test_planner.cpp
// Comprehensive test harness for StudentTask-Planner.
// Compile: g++ -std=c++17 -Wall -Wextra test_planner.cpp TASK.cpp TASKLIST.cpp HASHTABLE.cpp HISTORY.cpp -o test_planner
// (paths are passed via include flags or by sitting next to the project files)

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "TASK.h"
#include "TASKLIST.h"
#include "HASHTABLE.h"
#include "HISTORY.h"

using namespace std;

// ---- tiny test framework ----------------------------------------------
static int g_pass = 0;
static int g_fail = 0;
static vector<string> g_failures;

#define CHECK(cond, name) do {                                  \
    if (cond) { ++g_pass; cout << "  [PASS] " << name << "\n";} \
    else      { ++g_fail; cout << "  [FAIL] " << name << "\n";  \
                g_failures.push_back(name); }                    \
} while (0)

#define SECTION(s) cout << "\n=== " << s << " ===\n"

// silence stdout for the duration of a scope (so showList() output doesn't drown the report)
struct CoutMute {
    streambuf* old; ostringstream sink;
    CoutMute()  { old = cout.rdbuf(sink.rdbuf()); }
    ~CoutMute() { cout.rdbuf(old); }
    string str() const { return sink.str(); }
};

// Same DD/MM/YYYY validator as main.cpp (copied so we can unit-test it).
bool isValidDate(string d) {
    if (d.length() != 10) return false;
    if (d[2] != '/' || d[5] != '/') return false;
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5) {
            if (d[i] < '0' || d[i] > '9') return false;
        }
    }
    int day   = stoi(d.substr(0, 2));
    int month = stoi(d.substr(3, 2));
    int year  = stoi(d.substr(6, 4));
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;
    int maxDay = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) maxDay = 30;
    else if (month == 2) {
        bool isLeap = false;
        if (year % 400 == 0) isLeap = true;
        else if (year % 100 == 0) isLeap = false;
        else if (year % 4 == 0) isLeap = true;
        maxDay = isLeap ? 29 : 28;
    }
    if (day > maxDay) return false;
    return true;
}

// ----------------------------------------------------------------------

static Task make(int id, const string& t = "T", int prio = 1, const string& due = "01/01/2026") {
    return Task(id, t, "desc", "CS101", prio, due);
}

void test_task_class() {
    SECTION("Task class");

    Task def;
    CHECK(def.getID() == 0,                "default ctor: ID = 0");
    CHECK(def.getPriority() == 0,          "default ctor: priority = 0");
    CHECK(def.isComplete() == false,       "default ctor: complete = false");

    Task t = make(7, "Essay", 2, "15/03/2026");
    CHECK(t.getID() == 7,                  "param ctor: ID");
    CHECK(t.getTitle() == "Essay",         "param ctor: title");
    CHECK(t.getPriority() == 2,            "param ctor: priority");
    CHECK(t.getDueDate() == "15/03/2026",  "param ctor: due date");
    CHECK(t.getCourse() == "CS101",        "param ctor: course");
    CHECK(t.isComplete() == false,         "param ctor: complete starts false");

    t.setTitle("New Title");
    t.setDescription("New desc");
    t.setCourse("MATH200");
    t.setPriority(3);
    t.setDueDate("31/12/2027");
    t.checkedComplete();

    CHECK(t.getTitle() == "New Title",     "setter: title");
    CHECK(t.getDescription() == "New desc","setter: description");
    CHECK(t.getCourse() == "MATH200",      "setter: course");
    CHECK(t.getPriority() == 3,            "setter: priority");
    CHECK(t.getDueDate() == "31/12/2027",  "setter: due date");
    CHECK(t.isComplete() == true,          "checkedComplete sets complete=true");
}

void test_tasklist() {
    SECTION("TaskList");

    TaskList list;
    CHECK(list.getSize() == 0,             "empty list: size 0");

    // peek on empty
    bool peekThrew = false;
    try { list.peekTask(); } catch (const underflow_error&) { peekThrew = true; }
    CHECK(peekThrew,                       "peekTask on empty throws underflow_error");

    // remove on empty
    bool rmThrew = false;
    int idGone = 5;
    try { list.removeTask(idGone); } catch (const underflow_error&) { rmThrew = true; }
    CHECK(rmThrew,                         "removeTask on empty throws underflow_error");

    // add 3 tasks
    list.addTask(make(1, "A"));
    list.addTask(make(2, "B"));
    list.addTask(make(3, "C"));
    CHECK(list.getSize() == 3,             "addTask: size grows to 3");
    CHECK(list.peekTask().getID() == 3,    "peekTask returns tail (last-added)");

    // search hits + miss
    int find2 = 2, find99 = 99;
    CHECK(list.searchTask(find2) == true,  "searchTask: existing ID found");
    CHECK(list.searchTask(find99) == false,"searchTask: missing ID returns false");

    // remove from middle
    int mid = 2;
    Task removed = list.removeTask(mid);
    CHECK(removed.getID() == 2,            "removeTask middle: returns the right Task");
    CHECK(list.getSize() == 2,             "removeTask middle: size decremented");

    // remove from head
    int head = 1;
    list.removeTask(head);
    CHECK(list.getSize() == 1,             "removeTask head: size decremented");
    CHECK(list.peekTask().getID() == 3,    "after head remove, only ID 3 remains");

    // remove last node
    int last = 3;
    list.removeTask(last);
    CHECK(list.getSize() == 0,             "removeTask last: size = 0");

    // searching on an empty list now returns false (no exception)
    CHECK(list.searchTask(1) == false,     "searchTask on empty returns false (no throw)");

    // remove non-existent ID throws invalid_argument when list non-empty
    list.addTask(make(10));
    bool nonExistThrew = false;
    int absent = 999;
    try { list.removeTask(absent); } catch (const invalid_argument&) { nonExistThrew = true; }
    CHECK(nonExistThrew,                   "removeTask absent ID throws invalid_argument");

    // copy constructor
    list.addTask(make(11));
    list.addTask(make(12));
    TaskList copy = list;                                  // deep copy
    CHECK(copy.getSize() == list.getSize(),                "copy ctor: same size");
    int eleven = 11;
    CHECK(copy.searchTask(eleven) == true,                 "copy ctor: copies elements");

    // mutate original, copy unaffected
    int rmFromOrig = 11;
    list.removeTask(rmFromOrig);
    CHECK(copy.searchTask(eleven) == true,                 "copy ctor: independent (deep copy)");
}

void test_hashtable_basic() {
    SECTION("HashTable basic");

    HashTable h;
    CHECK(h.getSize() == 0,                "empty hash: size 0");

    Task out;
    CHECK(h.search(1, out) == false,       "search on empty: false");
    CHECK(h.contains(1) == false,          "contains on empty: false");
    CHECK(h.remove(1) == false,            "remove on empty: false");

    h.insert(make(1, "Alpha"));
    h.insert(make(2, "Beta"));
    h.insert(make(3, "Gamma"));
    CHECK(h.getSize() == 3,                "insert: size=3");

    Task got;
    CHECK(h.search(2, got) == true,        "search existing: true");
    CHECK(got.getTitle() == "Beta",        "search existing: returns correct task");
    CHECK(h.contains(3) == true,           "contains existing");
    CHECK(h.contains(42) == false,         "contains absent");

    CHECK(h.remove(2) == true,             "remove existing returns true");
    CHECK(h.getSize() == 2,                "remove decrements size");
    CHECK(h.contains(2) == false,          "removed item is gone");
    CHECK(h.remove(2) == false,            "double-remove returns false");
}

void test_hashtable_collisions() {
    SECTION("HashTable collisions (BUCKET_COUNT = 17)");

    HashTable h;
    // IDs that all hash to bucket 0: 0, 17, 34, 51, ...
    h.insert(make(0,  "zero"));
    h.insert(make(17, "seventeen"));
    h.insert(make(34, "thirtyfour"));
    h.insert(make(51, "fiftyone"));
    CHECK(h.getSize() == 4,                "4 colliding inserts: size=4");

    Task got;
    CHECK(h.search(0,  got) && got.getTitle() == "zero",       "collision: find ID 0");
    CHECK(h.search(17, got) && got.getTitle() == "seventeen",  "collision: find ID 17");
    CHECK(h.search(34, got) && got.getTitle() == "thirtyfour", "collision: find ID 34");
    CHECK(h.search(51, got) && got.getTitle() == "fiftyone",   "collision: find ID 51");

    // remove middle of chain
    CHECK(h.remove(17) == true,            "collision: remove middle of chain");
    CHECK(h.contains(17) == false,         "collision: 17 gone after removal");
    CHECK(h.contains(34) == true,          "collision: 34 still present");
    CHECK(h.getSize() == 3,                "collision: size after removal");

    // duplicate insert (same ID) => overwrite, NOT a new node
    h.insert(make(34, "OVERWRITTEN"));
    CHECK(h.getSize() == 3,                "duplicate ID insert: size unchanged (overwrite)");
    CHECK(h.search(34, got) && got.getTitle() == "OVERWRITTEN", "duplicate insert: title overwritten");

    // negative ID supported (hash takes abs)
    h.insert(make(-5, "neg"));
    CHECK(h.contains(-5) == true,          "negative ID insert + contains");
    CHECK(h.remove(-5) == true,            "negative ID remove");
}

void test_history_undo_redo() {
    SECTION("History undo/redo");

    TaskList list; HashTable hash; History hist;

    CHECK(hist.canUndo() == false,         "fresh history: canUndo false");
    CHECK(hist.canRedo() == false,         "fresh history: canRedo false");
    CHECK(hist.undo(list, hash) == false,  "undo on empty: false");
    CHECK(hist.redo(list, hash) == false,  "redo on empty: false");

    // simulate ADD via main.cpp pattern
    Task t1 = make(1, "Task1");
    list.addTask(t1); hash.insert(t1); hist.record("ADD", t1);
    Task t2 = make(2, "Task2");
    list.addTask(t2); hash.insert(t2); hist.record("ADD", t2);

    CHECK(list.getSize() == 2,             "after 2 ADDs: list size 2");
    CHECK(hash.getSize() == 2,             "after 2 ADDs: hash size 2");
    CHECK(hist.canUndo(),                  "canUndo true after records");

    // undo last ADD => removes t2 from both
    CHECK(hist.undo(list, hash) == true,   "undo ADD #2 returns true");
    CHECK(list.getSize() == 1,             "after undo: list size 1");
    CHECK(hash.contains(2) == false,       "after undo: hash has no 2");
    CHECK(hist.canRedo() == true,          "canRedo true after undo");

    // redo => puts t2 back
    CHECK(hist.redo(list, hash) == true,   "redo ADD #2 returns true");
    CHECK(list.getSize() == 2,             "after redo: list size 2");
    CHECK(hash.contains(2) == true,        "after redo: hash has 2 again");

    // simulate REMOVE
    int rmId = 1;
    Task removed = list.removeTask(rmId);
    hash.remove(1);
    hist.record("REMOVE", removed);
    CHECK(list.getSize() == 1,             "after REMOVE: size 1");

    // undo REMOVE => task comes back
    CHECK(hist.undo(list, hash) == true,   "undo REMOVE returns true");
    CHECK(list.getSize() == 2,             "after undo REMOVE: size 2");
    CHECK(hash.contains(1) == true,        "after undo REMOVE: hash has 1");

    // recording a new action MUST clear redo stack
    Task t3 = make(3, "Task3");
    list.addTask(t3); hash.insert(t3); hist.record("ADD", t3);
    CHECK(hist.canRedo() == false,         "new record clears redo stack");
}

void test_history_show_empty_throws() {
    SECTION("History showHistory on empty");

    History hist;
    bool threw = false;
    {
        CoutMute mute;
        try { hist.showHistory(); } catch (const underflow_error&) { threw = true; }
    }
    CHECK(threw,                           "showHistory on empty throws underflow_error");
}

void test_isValidDate() {
    SECTION("isValidDate");

    CHECK(isValidDate("01/01/2026")  == true,  "valid: 01/01/2026");
    CHECK(isValidDate("31/12/2026")  == true,  "valid: 31/12/2026");
    CHECK(isValidDate("29/02/2024")  == true,  "valid leap: 29/02/2024");
    CHECK(isValidDate("29/02/2000")  == true,  "valid leap (div 400): 29/02/2000");
    CHECK(isValidDate("28/02/2026")  == true,  "valid Feb non-leap last day");

    CHECK(isValidDate("29/02/2026")  == false, "invalid: 29/02/2026 non-leap");
    CHECK(isValidDate("29/02/1900")  == false, "invalid: 29/02/1900 (div 100, not 400)");
    CHECK(isValidDate("31/04/2026")  == false, "invalid: 31/04 (April has 30)");
    CHECK(isValidDate("31/06/2026")  == false, "invalid: 31/06 (June has 30)");
    CHECK(isValidDate("00/01/2026")  == false, "invalid: day 0");
    CHECK(isValidDate("01/00/2026")  == false, "invalid: month 0");
    CHECK(isValidDate("01/13/2026")  == false, "invalid: month 13");
    CHECK(isValidDate("32/01/2026")  == false, "invalid: day 32");
    CHECK(isValidDate("1/1/2026")    == false, "invalid: missing leading zeros (length)");
    CHECK(isValidDate("01-01-2026")  == false, "invalid: dashes instead of slashes");
    CHECK(isValidDate("aa/bb/cccc")  == false, "invalid: letters");
    CHECK(isValidDate("")            == false, "invalid: empty string");
    CHECK(isValidDate("01/01/202")   == false, "invalid: short year");
    CHECK(isValidDate("01/01/20266") == false, "invalid: long year");
}

void test_integration_workflow() {
    SECTION("Integration: list/hash stay in sync across many ops");

    TaskList list; HashTable hash; History hist;

    for (int i = 1; i <= 25; ++i) {
        Task t = make(i, "T" + to_string(i), (i % 3) + 1);
        list.addTask(t); hash.insert(t); hist.record("ADD", t);
    }
    CHECK(list.getSize() == 25 && hash.getSize() == 25,
          "after 25 adds: list and hash both size 25");

    // remove every even ID
    for (int i = 2; i <= 24; i += 2) {
        Task removed = list.removeTask(i);
        hash.remove(i);
        hist.record("REMOVE", removed);
    }
    CHECK(list.getSize() == 13 && hash.getSize() == 13,
          "after removing 12 evens: both sizes drop to 13");

    // verify content
    bool consistent = true;
    for (int i = 1; i <= 25; ++i) {
        bool inHash = hash.contains(i);
        int idCopy = i;
        bool inList = false;
        try { inList = list.searchTask(idCopy); } catch (...) { inList = false; }
        bool shouldBe = (i % 2 == 1);
        if (inHash != shouldBe || inList != shouldBe) { consistent = false; break; }
    }
    CHECK(consistent,                        "list and hash agree on all 25 IDs");

    // undo all 12 REMOVEs
    int undone = 0;
    while (hist.canUndo() && undone < 12) {
        if (hist.undo(list, hash)) ++undone;
    }
    CHECK(undone == 12,                      "undid all 12 REMOVEs");
    CHECK(list.getSize() == 25 && hash.getSize() == 25,
          "after undoing 12 removes: back to 25/25");
}

// ---- known-bug demonstrations ----------------------------------------
// These tests document behavior that is suspicious / inconsistent.

void test_bug_searchTask_on_empty() {
    SECTION("Regression: searchTask returns false on empty (was: threw)");

    TaskList list;
    CHECK(list.searchTask(5) == false, "searchTask on empty returns false");
}

void test_bug_duplicate_id_desync() {
    SECTION("BUG demo: duplicate ID inserts desynchronize list and hash");

    TaskList list; HashTable hash;
    Task a = make(5, "first");
    Task b = make(5, "second");

    list.addTask(a); hash.insert(a);
    list.addTask(b); hash.insert(b);   // hash overwrites, list keeps both

    // Hash now has 1 entry with title "second"
    Task got;
    bool inHash = hash.search(5, got);
    CHECK(inHash && got.getTitle() == "second",
          "hash holds the latest task with ID=5 (overwrite semantics)");

    // List has 2 nodes with ID=5
    CHECK(list.getSize() == 2,
          "list has 2 nodes with the SAME ID=5 -- main.cpp does not enforce unique IDs");

    // Remove ID 5 from list once -> still has 1 left, but hash thinks empty
    int five = 5;
    list.removeTask(five);
    hash.remove(5);
    CHECK(list.getSize() == 1,
          "after one removal, list still has a duplicate (orphaned)");
    CHECK(hash.contains(5) == false,
          "hash no longer contains 5, but list still has an orphan -> DESYNC");
}

int main() {
    cout << "=========================================\n";
    cout << " StudentTask-Planner Test Suite\n";
    cout << "=========================================\n";

    test_task_class();
    test_tasklist();
    test_hashtable_basic();
    test_hashtable_collisions();
    test_history_undo_redo();
    test_history_show_empty_throws();
    test_isValidDate();
    test_integration_workflow();

    cout << "\n--- Bug demos (these may FAIL/PASS but illustrate known issues) ---\n";
    test_bug_searchTask_on_empty();
    test_bug_duplicate_id_desync();

    cout << "\n=========================================\n";
    cout << " Results: " << g_pass << " passed, " << g_fail << " failed\n";
    if (!g_failures.empty()) {
        cout << " Failures:\n";
        for (auto& f : g_failures) cout << "   - " << f << "\n";
    }
    cout << "=========================================\n";
    return g_fail == 0 ? 0 : 1;
}
