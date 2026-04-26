#include <iostream>
#include <string>
#include "TASK.h"
#include "TASKLIST.h"
#include "HISTORY.h"
#include "HASHTABLE.h"

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

    // Pull out day, month, year as numbers
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
    
    cout << "Welcome to Sort & Sweet. Your personal academic task planner." << endl << "Let's get started!" << endl;
    do {
        cout << endl;
        cout << "Consider the following for your planner: " << endl;
        cout << "1. Add a Task" << endl;
        cout << "2. Remove a Task" << endl;
        cout << "3. See Full Planner" << endl;
        cout << "4. Search for a Specific Task" << endl;
        cout << "5. See Planner History" << endl;
        cout << "6. Close planner" << endl << endl;
        cout << "What would you like to do? (Type 1-6) : " ;

        while (!(cin >> option) || (option < 1 || option > 6)) {
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
                while (!(cin >> ID)) {
                    cout << endl;
                    cout << "Invalid Input. Expecting Integer" << endl << "Try Again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
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
                while (!(cin >> ID)) {
                    cout << endl;
                    cout << "Invalid Input. Expecting Integer" << endl << "Try Again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
                try {
                   Task removed = list.removeTask(ID);
                    hash.remove(ID);            // keep hash in sync
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
                    list.showList();
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
                while (!(cin >> ID)) {
                    cout << endl;
                    cout << "Invalid Input. Expecting Integer" << endl << "Try Again: ";
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
                                    history.undo(list, hash);
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
                                    history.redo(list, hash);
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
                cout << endl << "Thank you for your time" << endl;
                return 0;
            }

            default:
                break;
        }
    } while (option != 6);
    cout << endl << "Thank you for your time" << endl;
    return 0;
}