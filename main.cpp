#include <iostream>
#include <string>
#include "TASK.h"
#include "TASKLIST.h"

using namespace std;

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
    int option = 0;
    int ID = 0, priority = 0;
    string title = "", description = "", course = "",  dueDate = "";
    TaskList list;
    do {
        cout << "Welcome to Sort & Sweet. Your personal academic task planner." << endl << "Let's get started!" << endl << endl;
        cout << "Consider the following for your planner: " << endl;
        cout << "1. Add a Task" << endl;
        cout << "2. Remove a Task" << endl;
        cout << "3. See Full Planner" << endl;
        cout << "4. Search for a Specific Task" << endl;
        cout << "5. See Planner History (Not Working Currently)" << endl;
        cout << "6. Close planner" << endl << endl;
        cout << "What would you like to do? (Type 1-5)" << endl;

        cin >> option;
        switch (option)
        {
            case 1: {
                cout << "Add task properties: ";
                cout << endl << "ID: ";
                cin >> ID;

                cin.ignore(1000, '\n');
                cout << endl << "Title: ";
                getline(cin, title);

                cout << endl << "Description: ";
                getline(cin, description);

                cout << endl << "course: ";
                getline(cin, title);

                cout << endl << "Priority: ";
                cin >> priority;

                cin.ignore(1000, '\n');
                cout << endl << "Due Date: " << endl;
                getline(cin, dueDate);
                cout << endl;

                Task task (ID, title, description, course, priority, dueDate);
                list.addTask(task);
                
                break;
            }

            case 2: {
                list.removeTask();
                break;
            }
            case 3: {
                list.showList();
                break;
            }
            case 4: {
                cout << "Enter ID: ";
                cin >> ID;
                if(list.searchTask(ID))
                    cout << "Task ID found";
                break;
            }
            case 5: {
                
                break;
            }
            case 6: {
                cout << endl << "Thank you for your time" << endl;
                return 0;
            }
            default:
                break;
        }

        cout << "Would you like to do more? (Y/N)" << endl;
        cin >> loop;
    } while (loop == "y" || loop == "Y");
    return 0;
}