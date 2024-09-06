#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

class Task {
public:
    string task;
    string assignee;
    string status;
    string date;

    Task(string task, string assignee, string status, string date)
        : task(task), assignee(assignee), status(status), date(date) {}

    void display() const {
        cout << "Task: " << task << "\nAssignee: " << assignee << "\nStatus: " << status
            << "\nDate: " << date << "\n";
    }
};

class TaskManager {
private:
    ofstream tasksFile;
    vector<Task*> tasks;

public:
    TaskManager() {
        tasksFile.open("tasks.txt", ios::app);
        if (!tasksFile.is_open()) {
            cerr << "[!] Error: Unable to open file" << endl;
        }
    }

    ~TaskManager() {
        tasksFile.close();
        deleteAllTasks();
    }

    TaskManager(const TaskManager& other) {
        tasksFile.open("tasks.txt", ios::app);
        if (!tasksFile.is_open()) {
            cerr << "[!] Error: Unable to open file" << endl;
        }

        for (const Task* task : other.tasks) {
            Task* newTask = new Task(*task);
            tasks.push_back(newTask);
            setTaskToFile(*newTask);
        }
    }

    void insertTask(const Task& newTask) {
        newTask.display();
        tasks.push_back(new Task(newTask));
        setTaskToFile(newTask);
    }

    void deleteTask() {
        if (!tasks.empty()) {
            delete tasks.back();
            tasks.pop_back();
            updateTasksFile();
            cout << "Last task deleted.\n";
        }
        else {
            cout << "No tasks to delete.\n";
        }
    }

    void extractData() const {
        ifstream file("tasks.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        }
        else {
            cerr << "[!] Error: Unable to open file" << endl;
        }
        cout << "\n";
        system("pause");
        system("cls");
    }

    void managerMenu() {
        int option;
        do {
            banner();
            cout << "\nManager Menu:\n" << endl;
            cout << "\t 1 - View current tasks" << endl;
            cout << "\t 2 - Enter a new task" << endl;
            cout << "\t 3 - Delete last task" << endl;
            cout << "\t 4 - Exit" << endl;

            cout << "\nEnter an option (1-4): ";
            cin >> option;

            switch (option) {
            case 1:
                extractData();
                break;
            case 2:
                insertTaskMenu();
                break;
            case 3:
                deleteTask();
                break;
            }
        } while (option != 4);
    }

    void employeeMenu() {
        int option;
        do {
            banner();
            cout << "\nEmployee Menu:\n" << endl;
            cout << "\t 1 - View current tasks" << endl;
            cout << "\t 2 - Update task progress" << endl;
            cout << "\t 3 - Exit" << endl;

            cout << "\nEnter an option (1-3): ";
            cin >> option;

            switch (option) {
            case 1:
                extractData();
                break;
            case 2:
                updateTaskProgress();
                break;
            }
        } while (option != 3);
    }
void banner() {
        cout << "----------------------------------------------------------------" << endl;
        cout << "|                    Task Management Tool                       |" << endl;
        cout << "|                                                               |" << endl;
        cout << "----------------------------------------------------------------" << endl;
    }
private:
    void insertTaskMenu() {
        string task;
        string assignee;
        string status;
        string date;

        cin.ignore();
        cout << "[*] Set a task: ";
        getline(cin, task);

        cout << "[*] Assign to: ";
        getline(cin, assignee);

        cout << "[*] Set task status (To Do, In Progress, Completed): ";
        getline(cin, status);

        cout << "[*] Set a finish time: ";
        getline(cin, date);

        Task newTask(task, assignee, status, date);
        insertTask(newTask);

        cout << "\nTask added successfully.\n";
        system("cls");
    }

    void updateTaskProgress() {
        if (tasks.empty()) {
            cout << "No tasks available to update.\n";
            return;
        }

       
        cout << "Tasks:\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << i + 1 << ". ";
            tasks[i]->display();
            cout << "-----------------------------\n";
        }

        cout << "Select a task to update (1-" << tasks.size() << "): ";
        int taskIndex;
        cin >> taskIndex;

        
        if (taskIndex < 1 || taskIndex > tasks.size()) {
            cout << "Invalid task selection.\n";
            return;
        }

        cout << "Update task status (To Do, In Progress, Completed): ";
        string newStatus;
        cin.ignore(); 
        getline(cin, newStatus);

        Task* selectedTask = tasks[taskIndex - 1];
        selectedTask->status = newStatus;
        updateTasksFile();

        cout << "Task status updated successfully.\n";
    }


    void setTaskToFile(const Task& task) {
        time_t rawtime;
        time(&rawtime);

        if (tasksFile.is_open()) {
            char buffer[80];
            struct tm timeinfo;
            localtime_s(&timeinfo, &rawtime);
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

            tasksFile << "Task: " << task.task << "\nAssignee: " << task.assignee
                << "\nStatus: " << task.status << "\nDate: " << task.date
                << "\nCreated: " << buffer << "\n";
        }
        else {
            cerr << "[!] Error: Unable to open file" << endl;
        }
    }

    void updateTasksFile() {
        tasksFile.close();
        tasksFile.open("tasks.txt", ios::app); // Open in append mode
        if (!tasksFile.is_open()) {
            cerr << "[!] Error: Unable to open file" << endl;
        }

        for (Task* task : tasks) {
            setTaskToFile(*task);
        }
    }


    void deleteAllTasks() {
        for (Task* task : tasks) {
            delete task;
        }
        tasks.clear();
        updateTasksFile();
    }

    
};

int main() {
    int selection = 0;
    TaskManager tasking = TaskManager();
    do {
        tasking.banner();
        cout << "\nSelect an option:\n" << endl;
        cout << "\t 1 - Manager Login" << endl;
        cout << "\t 2 - Employee Login" << endl;
        cout << "\t 0 - Exit" << endl;

        cout << "\nEnter an option (0-2): ";
        cin >> selection;

        switch (selection) {
        case 1:
            tasking.managerMenu();
            break;
        case 2:
            tasking.employeeMenu();
            break;
        }
    } while (selection != 0);

    return 0;
}
