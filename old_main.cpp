// Task Management System
// This program provides a comprehensive solution for managing tasks, allowing users to create, view, edit, and delete tasks.
// Users can interact with the system through a user-friendly, menu-driven interface, enabling easy navigation and operation.
// The core functionality is powered by the TaskManager class, which handles all task-related operations and stores task data in a file for persistent storage.
// The program offers multiple viewing options, allowing users to sort tasks by date, priority, or category.
// Additionally, users can update task details, mark tasks as completed, or delete tasks by their unique IDs, making task management efficient and flexible.

#include <iostream>
#include <limits> // For input validation
#include "manager.cpp" // Include the TaskManager class implementation

using namespace std;

// Constants for menu options
const int CREATE_TASK = 1;
const int VIEW_TASKS = 2;
const int EDIT_TASK = 3;
const int DELETE_TASK = 4;
const int EXIT_PROGRAM = 5;

// Function to print the main menu options
void printMainMenu()
{
    cout << "Main Menu:" << endl;
    cout << CREATE_TASK << ". Create Task" << endl;
    cout << VIEW_TASKS << ". View Tasks" << endl;
    cout << EDIT_TASK << ". Edit Task" << endl;
    cout << DELETE_TASK << ". Delete Task" << endl;
    cout << EXIT_PROGRAM << ". Exit Program" << endl;
    cout << endl << "Enter your choice (1-5): ";
}

// Function to print the view options
void printViewOptions()
{
    cout << "View Options:" << endl;
    cout << "1. View by Date" << endl;
    cout << "2. View by Priority" << endl;
    cout << "3. View by Category" << endl;
    cout << "Enter your choice (1-3): ";
}

// Function to get and validate integer input
int getValidatedIntegerInput(int min, int max, const string& errorMessage = "Invalid input. Please enter a valid number.")
{
    int choice;
    while (true)
    {
        cin >> choice;
        
        if (cin.fail() || choice < min || choice > max)
        {
            cin.clear();  // Clear the fail flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << errorMessage << endl;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
            return choice;
        }
    }
}

// Main function
int main()
{
    // Create an instance of the TaskManager class
    TaskManager taskManager;
    int choice;

    // Display welcome message
    cout << "Welcome To The Task Management System" << endl << endl;

    // Main program loop
    do
    {
        try
        {
            // Print main menu options
            printMainMenu();

            // Get user choice with validation
            choice = getValidatedIntegerInput(CREATE_TASK, EXIT_PROGRAM, "Please enter a number between 1 and 5.");
            cout << endl;

            // Process user choice
            switch (choice)
            {
            case CREATE_TASK:
                // Create a new task
                taskManager.createTask();
                break;

            case VIEW_TASKS:
            {
                // Print view options and get user input
                printViewOptions();
                int viewChoice = getValidatedIntegerInput(1, 3, "Please enter a number between 1 and 3.");
                cout << endl;

                // View tasks based on user choice
                taskManager.viewTask(viewChoice);
                break;
            }

            case EDIT_TASK:
            {
                // Prompt user for task ID to update
                cout << "Enter the task ID you want to update: ";
                int taskId = getValidatedIntegerInput(0, numeric_limits<int>::max(), "Please enter a valid task ID (positive integer).");

                // Edit task priority and status
                taskManager.editTaskPriorityAndStatus(taskId);
                break;
            }

            case DELETE_TASK:
            {
                // Prompt user for task ID to delete
                cout << "Enter the task ID you want to delete: ";
                int taskIdToDelete = getValidatedIntegerInput(0, numeric_limits<int>::max(), "Please enter a valid task ID (positive integer).");

                // Delete task by ID
                taskManager.deleteTask("project.txt", taskIdToDelete);
                break;
            }

            case EXIT_PROGRAM:
                // Exit the program
                cout << "Thank you for using our system! Have a nice day." << endl;
                break;

            default:
                // This will not be reached due to validation, but kept for completeness
                cout << "Invalid choice. Please enter a number between 1 and 5." << endl;
                break;
            }
            cout << endl;
        }
        catch (const exception& ex)
        {
            // Handle exceptions and display error message
            cerr << "Error: " << ex.what() << endl;
        }
    } while (choice != EXIT_PROGRAM); // Continue loop until user chooses to exit

    return 0; // Exit program
}
