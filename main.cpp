#include <iostream>
#include <string>
#include "TASK.h"
#include "TASKLIST.h"
#include "HISTORY.h"
#include "HASHTABLE.h"
#include "TASKGRAPH.h"
#include "STORAGE.h"

using namespace std;

// DD/MM/YYYY format (e.g., "30/04/2026")
// Returns true only if the format is correct AND it's a real calendar date.
bool isValidDate(string d) {
    // Must be exactly 10 characters
    if (d.length() != 10) {
        return false;
    }

    // Slashes must be in positions 2 and 5
    if (d[2] != '/' || d[5] != '/') {
        return false;
    }

    // Every other position must be a digit '0'..'9'
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5) {
            if (d[i] < '0' || d[i] > '9') {
                return false;
            }
        }
    }

    // Pulling out day, month, year as numbers
    int day   = stoi(d.substr(0, 2));   // first 2 chars
    int month = stoi(d.substr(3, 2));   // chars 3 and 4
    int year  = stoi(d.substr(6, 4));   // chars 6 through 9

    // Month must be 1-12
    if (month < 1 || month > 12) {
        return false;
    }

    // Day must be at least 1
    if (day < 1) {
        return false;
    }

    // Figure out how many days the month has
    int maxDay = 31;   // default for Jan, Mar, May, Jul, Aug, Oct, Dec

    // WEIRD MONTHS
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDay = 30;   // April, June, September, November
    }
    else if (month == 2) {
        // February: depends on leap year
        bool isLeap = false;
        if (year % 400 == 0) {
            isLeap = true;
        }
        else if (year % 100 == 0) {
            isLeap = false;
        }
        else if (year % 4 == 0) {
            isLeap = true;
        }

        if (isLeap) {
            maxDay = 29;
        }
        else {
            maxDay = 28;
        }
    }

    // Day must not be more than the month allows - VALIDATION
    if (day > maxDay) {
        return false;
    }

    return true;
}

int main (){
    // ASCII Art Banner
    cout << "  /$$$$$$                        /$$            /$$$            /$$$$$$                                      /$$" << endl;
    cout << " /$$__  $$                      | $$           /$$ $$          /$$__  $$                                    | $$" << endl;
    cout << "| $$  \\__/  /$$$$$$   /$$$$$$  /$$$$$$        |  $$$          | $$  \\__/ /$$  /$$  /$$  /$$$$$$   /$$$$$$  /$$$$$$" << endl;
    cout << "|  $$$$$$  /$$__  $$ /$$__  $$|_  $$_/         /$$ $$/$$      |  $$$$$$ | $$ | $$ | $$ /$$__  $$ /$$__  $$|_  $$_/" << endl;
    cout << " \\____  $$| $$  \\ $$| $$  \\__/  | $$          | $$  $$_/       \\____  $$| $$ | $$ | $$| $$$$$$$$| $$$$$$$$  | $$" << endl;
    cout << " /$$  \\ $$| $$  | $$| $$        | $$ /$$      | $$\\  $$        /$$  \\ $$| $$ | $$ | $$| $$_____/| $$_____/  | $$ /$$" << endl;
    cout << "|  $$$$$$/|  $$$$$$/| $$        |  $$$$/      |  $$$$/$$      |  $$$$$$/|  $$$$$/$$$$/|  $$$$$$$|  $$$$$$$  |  $$$$/" << endl;
    cout << " \\______/  \\______/ |__/         \\___/         \\____/\\_/       \\______/  \\_____/\\___/  \\_______/ \\_______/   \\___/" << endl << endl << endl;                                                                                                            

    string loop = "";
    int option = 0, option2 = 0;
    int ID = 0, priority = 0;
    string title = "", description = "", course = "",  dueDate = "";
    TaskList list;
    History history;
    HashTable hash;   // parallel structure: mirrors 'list', used for O(1) ID search
    TaskGraph taskGraph;   // Dependence Graph Between Tasks (hito #4)

    const string DATA_FILE = "planner_data.txt";   // Persistence File (hito #5)

    cout << "Welcome to Sort & Sweet. Your personal academic task planner." << endl << "Let's get started!" << endl;

    //Load previous data (if available) on startup.
    int loaded = loadData(DATA_FILE, list, hash, taskGraph);
    if (loaded > 0) {
        cout << endl << "Se cargaron " << loaded << " tarea(s) desde " << DATA_FILE << "." << endl;
    } else {
        cout << endl << "No se encontraron datos previos. Empezando de cero." << endl;
    }

    do {
        cout << endl;
        cout << "Consider the following for your planner: " << endl;
        cout << "1. Add a Task" << endl;
        cout << "2. Remove a Task" << endl;
        cout << "3. See Full Planner" << endl;
        cout << "4. Search for a Specific Task" << endl;
        cout << "5. See Planner History" << endl;
        cout << "6. Mark a Task as Completed" << endl;
        cout << "7. Add a Dependency between Tasks" << endl;
        cout << "8. Show Dependency Graph" << endl;
        cout << "9. Close planner" << endl << endl;
        cout << "What would you like to do? (Type 1-9) : " ;

        while (!(cin >> option) || (option < 1 || option > 9)) {
            cout << endl;
            cout << "Invalid Input. Expecting Integer" << endl << "Try Again: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        switch (option)
        {
            case 1: {
                cout << "Add task properties: ";
                cout << endl << "ID: ";
                while (!(cin >> ID) || ID < 0) {
                    cout << endl;
                    cout << "Invalid Input. Expecting non-negative Integer" << endl << "Try Again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }

                // Reject IDs that are already used so the list and the hash never get out of sync.
                while (hash.contains(ID)) {
                    cout << endl;
                    cout << "ID " << ID << " already exists. Pick a different one: ";
                    while (!(cin >> ID) || ID < 0) {
                        cout << endl;
                        cout << "Invalid Input. Expecting non-negative Integer" << endl << "Try Again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                    }
                }

                cin.ignore(1000, '\n');
                cout << endl << "Title: ";
                getline(cin, title);

                cout << endl << "Description: ";
                getline(cin, description);

                cout << endl << "course: ";
                getline(cin, course);

                cout << endl << "Priority (1=High, 2=Medium, 3=Low): ";
                while (!(cin >> priority) || priority < 1 || priority > 3) {
                    cout << endl;
                    cout << "Invalid input. Priority must be 1, 2, or 3." << endl << "Try Again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }

                cin.ignore(1000, '\n');
                cout << endl << "Due Date (DD/MM/YYYY): ";
                getline(cin, dueDate);
                while (!isValidDate(dueDate)) {
                    cout << endl;
                    cout << "Invalid date. Use DD/MM/YYYY (30/04/2026)." << endl
                         << "Try Again: ";
                    getline(cin, dueDate);
                }
                cout << endl;

                Task task (ID, title, description, course, priority, dueDate);
                list.addTask(task);
                hash.insert(task);          // keep hash in sync
                history.record("ADD", task);
                break;
            }

            case 2: {
                cout << "Enter ID of Undesired Task: ";
                while (!(cin >> ID) || ID < 0) {
                    cout << endl;
                    cout << "Invalid Input. Expecting non-negative Integer" << endl << "Try Again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
                try {
                   Task removed = list.removeTask(ID);
                    hash.remove(ID);                       // keep hash in sync
                    //The graph is not cleared here: we leave the live edges so 
                    //that the undo of REMOVE can restore them for free. If the 
                    //user never undoes, visual garbage remains in the graph (assumed)."
                    cout << endl << "Task Removed Successfully" << endl;
                    history.record("REMOVE", removed);
                }
                catch (const std::underflow_error& e) {
                    cout << "Error: " << e.what() << endl;
                } 
                catch (const std::invalid_argument& e) {
                    cout << "Error: " << e.what() << endl;
                }
                

                break;
            }

            case 3: {
                try {
                    // Show the planner sorted by priority (does NOT modify the list).
                    list.showByPriority();
                }
                catch (const std::underflow_error& e) {
                    cout << "Error: " << e.what() << endl;
                } 
                catch (const std::invalid_argument& e) {
                    cout << "Error: " << e.what() << endl;
                }

                break;
            }

            case 4: {
                cout << "Enter ID: ";
                while (!(cin >> ID) || ID < 0) {
                    cout << endl;
                    cout << "Invalid Input. Expecting non-negative Integer" << endl << "Try Again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
                // O(1) average-time lookup via the hash table.
                Task found;
                if (hash.search(ID, found)) {
                    cout << endl << "Task found:" << endl;
                    found.show();
                } else {
                    cout << endl << "Task ID not found" << endl;
                }

                break;
            }

            case 5: {
                do {
                    try {
                        history.showHistory();
                    }
                    catch (const std::underflow_error& e) {
                        cout << "Error: " << e.what() << endl;
                    } 
                    catch (const std::invalid_argument& e) {
                        cout << "Error: " << e.what() << endl;
                    }

                    cout << endl << "Do you wish to make any changes?" << endl;
                    cout << "1. Undo Last Change" << endl;
                    cout << "2. Redo the undoing" << endl;
                    cout << "3. Leave History Settings" << endl;
                    cout << "Type your choice: ";
                    
                    while (!(cin >> option2) || (option2 < 1 || option2 > 3)) {
                        cout << endl;
                        cout << "Invalid Input. Expecting Integer" << endl << "Try Again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                    }

                    switch (option2) {
                        case 1:
                            try {
                                if (history.canUndo())
                                    history.undo(list, hash, taskGraph);
                                else
                                    cout << "History is empty" << endl;
                            }
                            catch (const std::underflow_error& e) {
                                cout << "Error: " << e.what() << endl;
                            }
                            catch (const std::invalid_argument& e) {
                                cout << "Error: " << e.what() << endl;
                            }
                            break;

                        case 2:
                            try {
                                if(history.canRedo())
                                    history.redo(list, hash, taskGraph);
                                else
                                    cout << "Nothing to Redo" << endl;
                            }
                            catch (const std::underflow_error& e) {
                                cout << "Error: " << e.what() << endl;
                            } 
                            catch (const std::invalid_argument& e) {
                                cout << "Error: " << e.what() << endl;
                            }
                            break;

                        default:
                            break;
                    }
                } while (option2 != 3);
                break;
            }

            case 6: {
                //Will mark task as completed ONLY if all of its prereqs are ready.
                cout << "Enter ID of task to mark as completed: ";
                while (!(cin >> ID) || ID < 0) {
                    cout << endl;
                    cout << "Invalid Input. Expecting non-negative Integer" << endl << "Try Again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
                Task found;
                if (!hash.search(ID, found)) {
                    cout << endl << "Task ID not found" << endl;
                    break;
                }
                // Verificar prerrequisitos antes de marcar
                if (!taskGraph.canComplete(ID, hash)) {
                    cout << endl << "No se puede completar [" << ID << "] todavia." << endl;
                    cout << "Te falta terminar:" << endl;
                    taskGraph.showMissingPrereqs(ID, hash);
                    break;
                }
                found.checkedComplete();
                hash.insert(found);          // sobrescribe en hash
                list.markComplete(ID);       // actualiza tambien en la lista
                cout << endl << "Task [" << ID << "] marked as completed" << endl;
                break;
            }

            case 7: {
                // Add Dependency: TaskID depends on PrereqsID
                int prereqID = 0;
                cout << "Add Dependency" << endl;
                cout << "Task ID (la que depende): ";
                while (!(cin >> ID) || ID < 0) {
                    cout << endl;
                    cout << "Invalid Input. Expecting non-negative Integer" << endl << "Try Again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
                cout << "Prereq ID (la que debe hacerse primero): ";
                while (!(cin >> prereqID) || prereqID < 0) {
                    cout << endl;
                    cout << "Invalid Input. Expecting non-negative Integer" << endl << "Try Again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }

                if (taskGraph.addDependency(ID, prereqID, hash)) {
                    cout << endl << "Dependency added: [" << ID << "] -> [" << prereqID << "]" << endl;
                } else {
                    cout << endl << "Dependency rejected (misma ID, ID inexistente, duplicado, o cierra ciclo)" << endl;
                }
                break;
            }

            case 8: {
                // Show Dependency Graph
                cout << endl;
                taskGraph.showGraph(hash);
                break;
            }

            case 9: {
                // Save Data before closing (Automatic Persistence, hito #5).
                if (saveData(DATA_FILE, list, taskGraph)) {
                    cout << endl << "Datos guardados en " << DATA_FILE << "." << endl;
                } else {
                    cout << endl << "Aviso: no se pudieron guardar los datos." << endl;
                }
                cout << "Thank you for your time" << endl;
                return 0;
            }

            default:
                break;
        }
    } while (option != 9);

    // Fallback exit (should not normally be reached): also saves.
    saveData(DATA_FILE, list, taskGraph);
    cout << endl << "Thank you for your time" << endl;
    return 0;
}