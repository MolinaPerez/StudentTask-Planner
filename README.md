# Welcome to the Sort & Sweet Student Task Manager:
## Description

This is terminal-based software that utilizes an ensemble of data structures to process user-created tasks for your convenience. 

The purpose of this program is to allow students to have an environment where they can organize their pending work load in a digestible manner that may help with decision fatigue and overall productivity. 

It allows for editing via a history composed of two stacks that behaves like the keyboard shortcut "ctrl + z/y", efficient searching via a hash table utilizing the task IDs, and the ability to add prerequisites for tasks via directed graphs. The program allows for local data storage so that the user doesn't have to worry about losing their data once the program is terminated.

This will all be accessible via the menu that displays on the system terminal.

Furthermore, the point of this project was to practice and apply all we've learned in our endeavors to understand Data Structures. The concept of tasks and lists lend themselves quite well to linked lists and its many forms. We approached this by making sure to select feautures that we could link to the data structures targets intended for practice.

## Features
**History**- Undo/Redo: Allows for undoing and redoing changes made to the data.
> Note: you may make as many changes as necessary with the ability to reverse the changes, until you add a new task via the Add Task menu option in which the ability to redo changes won't be possible just as with "ctrl + z/y".

**Efficient Searching**- Just input the ID of the task and it will let you know if it's present in the program. It's particularly fast because the Hash Table utilized has a time complexity of O(1).

**Adding Dependencies**- You'll be prompted with inputting the ID of the task you wish to make dependent of another, followed by the ID of the task you're making a prerequisite to the task previously selected.

**Save Data**- For the program persistence there is a file located within the program directory as planner_data.txt, be sure not to delete it.

## How to Compile and Run
To be able to compile and run this program, you must run the following commands using g++ (for transparency, this was used in the VsCode terminals):

**Windows:**
```bash
g++ -o planner main.cpp HASHTABLE.cpp HISTORY.cpp STORAGE.cpp TASK.cpp TASKGRAPH.cpp TASKLIST.cpp -std=c++17 && planner.exe
```

**Mac / Linux:**
```bash
g++ -o planner main.cpp HASHTABLE.cpp HISTORY.cpp STORAGE.cpp TASK.cpp TASKGRAPH.cpp TASKLIST.cpp -std=c++17 && ./planner
```
## Collaborators
** https://github.com/MolinaPerez
** https://github.com/FlanMan04
