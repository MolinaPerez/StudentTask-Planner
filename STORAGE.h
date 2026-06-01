#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include "TASKLIST.h"
#include "HASHTABLE.h"
#include "TASKGRAPH.h"

// STORAGE.h / STORAGE.cpp
// Handles saving and loading planner data to and from a local text file.
// Tasks are stored as TASK|... lines and dependencies as DEP|... lines.
// Authors: Alex Molina Perez, Gustavo Ramirez Renta
// COMP 3075 - Introduction to Data Structures, RUM

// Guarda todas las tareas y dependencias en 'filename'.
// Devuelve true si se pudo escribir el archivo.
bool saveData(const std::string& filename, TaskList& list, TaskGraph& graph);

// Carga tareas y dependencias desde 'filename' hacia las 3 estructuras.
// Devuelve cuantas tareas se cargaron (0 si el archivo no existe todavia).
int loadData(const std::string& filename, TaskList& list, HashTable& hash, TaskGraph& graph);

#endif
