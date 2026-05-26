#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include "TASKLIST.h"
#include "HASHTABLE.h"
#include "TASKGRAPH.h"

// Guarda todas las tareas y dependencias en 'filename'.
// Devuelve true si se pudo escribir el archivo.
bool saveData(const std::string& filename, TaskList& list, TaskGraph& graph);

// Carga tareas y dependencias desde 'filename' hacia las 3 estructuras.
// Devuelve cuantas tareas se cargaron (0 si el archivo no existe todavia).
int loadData(const std::string& filename, TaskList& list, HashTable& hash, TaskGraph& graph);

#endif
