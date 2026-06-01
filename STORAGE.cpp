#include "STORAGE.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
using namespace std;

// STORAGE.h / STORAGE.cpp
// Handles saving and loading planner data to and from a local text file.
// Tasks are stored as TASK|... lines and dependencies as DEP|... lines.
// Authors: Alex Molina Perez, Gustavo Ramirez Renta
// COMP 3075 - Introduction to Data Structures, RUM

bool saveData(const string& filename, TaskList& list, TaskGraph& graph) {
    ofstream out(filename.c_str());
    if (!out) {
        return false;   // no se pudo abrir para escribir
    }
    list.saveToFile(out);    // First the Tasks (lineas TASK|...)
    graph.saveToFile(out);   // Then the Dependencies (lineas DEP|...)
    out.close();
    return true;
}

int loadData(const string& filename, TaskList& list, HashTable& hash, TaskGraph& graph) {
    ifstream in(filename.c_str());
    if (!in) {
        return 0;   // file does not exist (primera ejecucion): start from zero
    }

    int taskCount = 0;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string type;
        getline(ss, type, '|');

        // --- Tarea ---
        if (type == "TASK") {
            string idStr, title, desc, course, prioStr, due, completeStr;
            getline(ss, idStr, '|');
            getline(ss, title, '|');
            getline(ss, desc, '|');
            getline(ss, course, '|');
            getline(ss, prioStr, '|');
            getline(ss, due, '|');
            getline(ss, completeStr, '|');

            // Error Handling : If the line is corrupt, we skip it without crashing.
            try {
                int id   = stoi(idStr);
                int prio = stoi(prioStr);
                Task t(id, title, desc, course, prio, due);
                if (completeStr == "1") t.checkedComplete();
                list.addTask(t);
                hash.insert(t);
                taskCount++;
            } catch (const exception&) {
                cout << "Aviso: linea de tarea corrupta ignorada." << endl;
            }
        }
        // --- Dependence ---
        else if (type == "DEP") {
            string aStr, bStr;
            getline(ss, aStr, '|');
            getline(ss, bStr, '|');
            try {
                int a = stoi(aStr);
                int b = stoi(bStr);
                graph.addDependency(a, b, hash);   // Validates Existence and Cycles only
            } catch (const exception&) {
                cout << "Aviso: linea de dependencia corrupta ignorada." << endl;
            }
        }
        // any other prefix is silently ignored
    }
    in.close();
    return taskCount;
}
