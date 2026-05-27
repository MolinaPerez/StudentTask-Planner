#include "STORAGE.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
using namespace std;

bool saveData(const string& filename, TaskList& list, TaskGraph& graph) {
    ofstream out(filename.c_str());
    if (!out) {
        return false;   // no se pudo abrir para escribir
    }
    list.saveToFile(out);    // primero las tareas (lineas TASK|...)
    graph.saveToFile(out);   // despues las dependencias (lineas DEP|...)
    out.close();
    return true;
}

int loadData(const string& filename, TaskList& list, HashTable& hash, TaskGraph& graph) {
    ifstream in(filename.c_str());
    if (!in) {
        return 0;   // archivo no existe (primera ejecucion): empezar de cero
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

            // Manejo de error: si la linea esta corrupta, la saltamos sin reventar.
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
        // --- Dependencia ---
        else if (type == "DEP") {
            string aStr, bStr;
            getline(ss, aStr, '|');
            getline(ss, bStr, '|');
            try {
                int a = stoi(aStr);
                int b = stoi(bStr);
                graph.addDependency(a, b, hash);   // valida existencia y ciclos solo
            } catch (const exception&) {
                cout << "Aviso: linea de dependencia corrupta ignorada." << endl;
            }
        }
        // cualquier otro prefijo se ignora silenciosamente
    }
    in.close();
    return taskCount;
}
