// This program implements a Task Manager that enables users to efficiently manage their tasks through various operations such as creating, editing, deleting, and viewing tasks.
// The Task class represents individual tasks, encapsulating attributes like priority, deadline, and category.
// The TaskManager class provides the core functionalities for task manipulation, including loading tasks from a file and saving updates back to the file.
// Users can perform multiple operations in the main.cpp file, including:
// - Viewing tasks sorted by date, priority, or category.
// - Editing task details such as priority, status, and deadlines.
// - Deleting tasks by their unique IDs.
// The program enhances user experience by offering color-coded output based on task priority and deadline proximity, making it easier to identify critical tasks at a glance.

#include <iostream>
#include <ctime>     // Used for time-related functions like time, mktime, and difftime to handle deadlines and time calculations
#include <string>    // String manipulation functions like getline()
#include <vector>    // Store a list of tasks managed by the TaskManager class
#include <sstream>   // Used to parse strings into tokens (loadTaskFromFile()) for extracting task details from lines read from a file
#include <fstream>   // Used to read from and write to files (loadTaskFromFile() and saveTaskToFile())
#include <algorithm> // Transformations such as converting strings to lowercase
#include <cctype>    // Used for the tolower(), applied during string transformations to convert characters to lowercase
#include <iomanip>   // Used for get_time in convertStringToTime()
#include <cmath>     // for abs()
#include "processor.cpp"  // Include the Task class implementation file

using namespace std;

// TaskManager class definition
class TaskManager
{
private:
    vector<Task> tasks; // List of tasks managed by the task manager

public:
    TaskManager() {} // Default constructor

    // Save a task to a file
    void saveTaskToFile(string filename, int taskID)
    {
        ofstream file(filename, ios::app); // Open the file in append mode

        // Check if the file exists
        if (!file)
        {
            // If the file doesn't exist, create it
            file.open(filename);
        }

        // Check if the file is successfully open for writing
        if (file.is_open())
        {
            /* find_if is a standard library algorithm used to search for an element in a range (tasks.begin() to tasks.end() in this case) that satisfies a specified condition.
            The lambda function [taskID](Task &task) { return task.getTaskID() == taskID; } is used as the condition. It captures the taskID variable by value and checks if the ID of the task (task.getTaskID()) matches the provided taskID.
            If a task with the specified ID is found within the tasks vector, the iterator it will point to that task. Otherwise, it will point to tasks.end(), indicating that the task with the specified ID was not found. */

            // 'auto' avoids explicitly specifying the type of it, letting the compiler infer it from the context (Task &task)
            auto it = find_if(tasks.begin(), tasks.end(), [taskID](Task &task)
                              { return task.getTaskID() == taskID; });

            // Check if the task with the specified ID was found
            if (it != tasks.end())
            {
                // Write task details to the file
                file << taskID << "," << it->getCategory() << "," << it->getTitle() << "," << it->getDescription() << "," << it->getDeadline() << "," << it->getPriority() << ","
                     << it->getStatus() << "," << it->getLabel();
                file << endl; // End the line
                cout << "Task with ID " << taskID << " saved to file." << endl;
            }
            else
            {
                cout << "Task with ID " << taskID << " not found." << endl;
            }
            file.close(); // Close the file
        }
        else
        {
            cout << "Unable to open file: " << filename << endl;
        }
    }

    // Load tasks from a file
    void loadTaskFromFile(string filename)
    {
        ifstream file(filename); // Open the file for reading

        // Check if the file was successfully opened
        if (file.is_open())
        {
            string line;
            // Read each line from the file
            while (getline(file, line))
            {
                // stringstream created to parse each line into individual tokens to extract specific details such as task ID, etc
                stringstream ss(line); // Create a stringstream to parse the line

                // Extract task details from the line
                string idStr;
                getline(ss, idStr, ','); // Read the task ID string until the comma delimiter
                int id = stoi(idStr);    // Convert the ID string to an integer

                Task task;          // Create a new Task object to store the extracted details
                task.setTaskID(id); // Set the task ID

                getline(ss, task.category, ',');    // Read the category string until the comma delimiter and store it in the task object
                getline(ss, task.title, ',');       // Read the title string until the comma delimiter and store it in the task object
                getline(ss, task.description, ','); // Read the description string until the comma delimiter and store it in the task object
                getline(ss, task.deadline, ',');    // Read the deadline string until the comma delimiter and store it in the task object

                string priorityStr;
                getline(ss, priorityStr, ','); // Extract the priority string from the stringstream

                // Convert priority string to TaskPriority enum and set it
                transform(priorityStr.begin(), priorityStr.end(), priorityStr.begin(), ::tolower);
                if (priorityStr == "low")
                {
                    task.setPriority("Low");
                }
                else if (priorityStr == "medium")
                {
                    task.setPriority("Medium");
                }
                else if (priorityStr == "high")
                {
                    task.setPriority("High");
                }

                string statusStr;
                getline(ss, statusStr, ','); // Extract status string

                // Convert status string to TaskStatus enum and set it
                transform(statusStr.begin(), statusStr.end(), statusStr.begin(), ::tolower);
                if (statusStr == "pending")
                {
                    task.setStatus("Pending");
                }
                else if (statusStr == "in progress")
                {
                    task.setStatus("In Progress");
                }
                else if (statusStr == "completed")
                {
                    task.setStatus("Completed");
                }

                getline(ss, task.label, ','); // Read the label until the comma delimiter and store it in the task object

                tasks.push_back(task); // Add the task to the vector
            }
            file.close(); // Close the file
        }
        else
        {
            cout << "Unable to open file." << endl;
        }
    }

    // Create a new task and add it to the task list
    void createTask()
    {
        Task newTask;
        cin >> newTask; // Utilize the operator>> to input task details

        // Load existing tasks from the file
        ifstream infile("project.txt");
        if (infile.is_open())
        {
            string line;
            while (getline(infile, line))
            {
                stringstream ss(line);
                string idStr;

                getline(ss, idStr, ',');
                int id = stoi(idStr);

                // Skip tasks with the same ID as the one being added
                if (id == newTask.getTaskID())
                {
                    cout << "Task with the same ID already exists! Please choose a different ID." << endl;
                    infile.close();
                    return;
                }
            }
            infile.close();
        }

        // Add the task to the task manager
        tasks.push_back(newTask);

        cout << "Task created successfully!" << endl;

        // Save the task to txt file
        saveTaskToFile("project.txt", newTask.getTaskID());
    }

    // Convert string representation of date to time_t object
    time_t convertStringToTime(string &dateString)
    {
        // Create a stringstream to parse the dateString
        istringstream ss(dateString);

        // Initialize a tm struct to store the parsed date
        struct tm tm = {};

        // Parse the dateString using get_time() to fill the tm struct with date components
        ss >> get_time(&tm, "%d/%m/%Y");

        // Check if parsing failed
        if (ss.fail())
        {
            // Handle parsing error by returning -1
            return -1;
        }
        else
        {
            // Convert the parsed tm struct into a time_t object and return it
            return mktime(&tm);
        }
    }

    // Check if the deadline is near (within a defined threshold)
    bool isDeadlineNear(string deadline)
    {
        // Convert the deadline string to a time_t object using convertStringToTime() function
        time_t deadlineTime = convertStringToTime(deadline);

        // Check if conversion failed (-1 indicates an error)
        if (deadlineTime == -1)
        {
            // Handle parsing error by returning false
            return false;
        }

        // Get the current time in seconds since the epoch (00:00:00 UTC on January 1, 1970)
        time_t currentTime = time(nullptr);

        // Calculate the time difference in seconds between the deadline and the current time
        double diffInSeconds = difftime(deadlineTime, currentTime);

        // Define the threshold for considering a deadline as near (e.g., 24 hours)
        double threshold = 24 * 3600; // 24 hours threshold, you can adjust this as needed

        // Check if the absolute value of the time difference is less than or equal to the threshold
        return abs(diffInSeconds) <= threshold;
    }

    // Print task details with colored output based on status and priority
    void printTaskWithColour(Task &task)
    {
        // Convert status and priority strings to lowercase for case insensitivity
        string status = task.getStatus();
        transform(status.begin(), status.end(), status.begin(), ::tolower);

        string priority = task.getPriority();
        transform(priority.begin(), priority.end(), priority.begin(), ::tolower);

        if (isDeadlineNear(task.getDeadline()) && status == "completed")
        {
            // Print task details with green colour for nearing deadline and completed status
            cout << "\033[1;32m"; // ANSI escape code for green colour
        }
        else if (isDeadlineNear(task.getDeadline()))
        {
            // Print task details with red colour for nearing deadline
            cout << "\033[1;31m"; // ANSI escape code for red colour
        }
        else if (status == "completed" && priority == "high")
        {
            // Print task details with green colour for high priority and completed status
            cout << "\033[1;32m"; // ANSI escape code for green colour
        }
        else if (status == "completed")
        {
            // Print task details with green colour if the task is completed
            cout << "\033[1;32m"; // ANSI escape code for green colour
        }
        else if (priority == "high")
        {
            // Print task details with red colour for high priority but not completed status
            cout << "\033[1;31m"; // ANSI escape code for red colour
        }
        else
        {
            // No colour for other cases
        }

        task.getTaskDetails(); // Call this once after setting the colour
        cout << "\033[0m";     // Reset colour after printing details
    }

    // View tasks sorted by date
    void viewTasksByDate()
    {
        // Clear existing tasks from the vector before loading new tasks
        tasks.clear();

        // Load tasks from the file "project.txt"
        loadTaskFromFile("project.txt");

        // Check if there are tasks to display
        if (tasks.empty())
        {
            cout << "No tasks present in the file!" << endl;
            return;
        }

        // Sort tasks by deadline using a lambda function as the comparison criterion
        sort(tasks.begin(), tasks.end(), [](Task &a, Task &b)
             {
        // The lambda function compares tasks based on their deadlines
        // It takes two task objects (a and b) as input parameters and returns a boolean value
        // The tasks are sorted in ascending order based on their deadlines

        // Return true if the deadline of task a is less than the deadline of task b
        return a.getDeadline() < b.getDeadline(); });

        // Display sorted tasks with color highlighting based on deadline proximity
        for (auto &task : tasks)
        {
            // Call printTaskWithColour() function to print each task with appropriate color
            printTaskWithColour(task);
            cout << endl;
        }
    }

    // View tasks sorted by priority
    void viewTasksByPriority()
    {
        tasks.clear(); // Clear existing tasks before loading

        // Load tasks from file
        loadTaskFromFile("project.txt");

        // Check if there are tasks to display
        if (tasks.empty())
        {
            cout << "No tasks present in the file!" << endl;
            return;
        }

        // Sort tasks by priority: HIGH > MEDIUM > LOW
        sort(tasks.begin(), tasks.end(), [](Task &a, Task &b)
             {
        // Convert priority strings to lowercase for case insensitivity
        string priorityA = a.getPriority();
        string priorityB = b.getPriority();

        transform(priorityA.begin(), priorityA.end(), priorityA.begin(), ::tolower);
        transform(priorityB.begin(), priorityB.end(), priorityB.begin(), ::tolower);

        // Define priority order
        int priorityOrderA = 0, priorityOrderB = 0;

        // Assign priority order based on priority string
        if (priorityA == "high") priorityOrderA = 3;
        else if (priorityA == "medium") priorityOrderA = 2;
        else if (priorityA == "low") priorityOrderA = 1;

        if (priorityB == "high") priorityOrderB = 3;
        else if (priorityB == "medium") priorityOrderB = 2;
        else if (priorityB == "low") priorityOrderB = 1;

        // Compare priorities based on the defined order
        return priorityOrderA > priorityOrderB; });

        // Display sorted tasks with colour based on priority
        for (auto &task : tasks)
        {
            printTaskWithColour(task);
            cout << endl;
        }
    }

    // View tasks sorted by category
    void viewTasksByCategory()
    {
        tasks.clear(); // Clear existing tasks before loading

        // Load tasks from file
        loadTaskFromFile("project.txt");

        // Check if there are tasks to display
        if (tasks.empty())
        {
            cout << "No tasks present in the file!" << endl;
            return;
        }

        // Sort tasks alphabetically by category
        sort(tasks.begin(), tasks.end(), [](Task &a, Task &b)
             { return a.getCategory() < b.getCategory(); });

        // Display sorted tasks with colour based on category
        for (auto &task : tasks)
        {
            printTaskWithColour(task);
            cout << endl;
        }
    }

    // View tasks based on the chosen sorting method
    void viewTask(int sortingMethod)
    {
        // Check the sorting method provided by the user
        if (sortingMethod == 1)
        {
            // Call viewTasksByDate function if '1' is passed
            viewTasksByDate();
        }
        else if (sortingMethod == 2)
        {
            // Call viewTasksByPriority function if '2' is passed
            viewTasksByPriority();
        }
        else if (sortingMethod == 3)
        {
            // Call viewTasksByCategory function if '3' is passed
            viewTasksByCategory();
        }
    }

    // Function to edit the priority and status of a task by its ID
    void editTaskPriorityAndStatus(int taskID)
    {
        // Clear the existing tasks and load them from file if the tasks vector is empty
        if (tasks.empty())
        {
            tasks.clear(); // Clear any existing tasks to avoid duplication
            loadTaskFromFile("project.txt");
        }

        // Find the task with the specified taskID
        auto it = find_if(tasks.begin(), tasks.end(), [taskID](Task &task)
                          { return task.getTaskID() == taskID; });

        // If the task is not found, throw an exception or handle the error
        if (it == tasks.end())
        {
            // Print an error message to standard error (cerr; alternative of cout for error messages only)
            cerr << "Task with ID " << taskID << " not found." << endl;

            // Exit the function as the task is not found
            return;
        }

        try
        {
            // Obtain a reference to the task found with the provided task ID
            Task &updatedTask = *it; // Reference the task to edit it directly

            // Prompt the user to enter the new Priority
            cout << "Enter the new Priority (Low, Medium, High): ";
            string priority;
            cin >> priority;

            // Convert input to lowercase
            transform(priority.begin(), priority.end(), priority.begin(), ::tolower);

            // Set the new priority based on user input
            if (priority == "low" || priority == "medium" || priority == "high")
            {
                updatedTask.setPriority(priority);
            }
            else
            {
                throw invalid_argument("Invalid priority. Priority must be 'Low', 'Medium', or 'High'.");
            }

            // Prompt the user to enter the new Status
            cout << "Enter the new Status (Pending, In Progress, Completed): ";
            string status;
            getline(cin >> ws, status); // Read entire line, including spaces

            // Convert input to lowercase
            transform(status.begin(), status.end(), status.begin(), ::tolower);

            // Set the new status based on user input
            if (status == "pending" || status == "in progress" || status == "completed")
            {
                updatedTask.setStatus(status);
            }
            else
            {
                throw invalid_argument("Invalid status. Status must be 'Pending', 'In Progress', or 'Completed'.");
            }

            cout << "Task edited successfully." << endl;

            // Write all tasks back to the file "project.txt"
            ofstream file("project.txt");

            // Check if the file is successfully opened
            if (file.is_open())
            {
                // Iterate over each task in the tasks vector
                for (auto &task : tasks)
                {
                    // Write task details to the file
                    file << task.getTaskID() << "," << task.getCategory() << "," << task.getTitle() << "," << task.getDescription() << ","
                         << task.getDeadline() << "," << task.getPriority() << "," << task.getStatus() << "," << task.getLabel() << endl;
                }
                // Close the file after writing all tasks
                file.close();
            }
            else
            {
                // Output an error message if the file cannot be opened
                cerr << "Unable to open file for writing." << endl;
            }
        }
        // Catch any exceptions that might occur during task editing
        catch (exception &ex)
        {
            // Output an error message with the exception details using cerr
            cerr << "Error: " << ex.what() << endl;

            // Exit the function early to avoid further processing if an error occurs
            return;
        }
    }

    // Function to delete a task by its ID
    void deleteTask(const string &filename, int taskID)
    {
        // Vector to store loaded tasks from the file
        vector<Task> loadedTasks;

        // Input file stream to read tasks from the file
        ifstream file(filename);

        // Flag to track if the task with the given ID is found
        bool taskFound = false;

        // Check if the file is open
        if (!file.is_open())
        {
            cerr << "Unable to open file: " << filename << endl;
            return;
        }

        string line;
        // Read each line of the file
        while (getline(file, line))
        {
            // Create a stringstream to parse the line containing task details
            stringstream ss(line);

            // String to store the task ID as a string
            string idStr;

            // Extract the task ID from the stringstream delimited by ','
            getline(ss, idStr, ',');

            // Convert the extracted ID string to an integer
            int id = stoi(idStr);

            // If the task ID matches the ID to be deleted, skip it
            if (id != taskID)
            {
                Task task;
                task.setTaskID(id);

                // Read and set other task details
                getline(ss, task.category, ',');
                getline(ss, task.title, ',');
                getline(ss, task.description, ',');
                getline(ss, task.deadline, ',');

                // Read and set priority
                string priorityStr;
                getline(ss, priorityStr, ',');
                if (priorityStr == "LOW")
                {
                    task.setPriority("LOW");
                }
                else if (priorityStr == "MEDIUM")
                {
                    task.setPriority("MEDIUM");
                }
                else if (priorityStr == "HIGH")
                {
                    task.setPriority("HIGH");
                }

                // Read and set status
                string statusStr;
                getline(ss, statusStr, ',');
                if (statusStr == "PENDING")
                {
                    task.setStatus("PENDING");
                }
                else if (statusStr == "IN_PROGRESS")
                {
                    task.setStatus("IN_PROGRESS");
                }
                else if (statusStr == "COMPLETED")
                {
                    task.setStatus("COMPLETED");
                }

                // Read and set label
                getline(ss, task.label, ',');

                // Add the task to the vector of loaded tasks
                loadedTasks.push_back(task);
            }
            else
            {
                // Mark task as found
                taskFound = true;
            }
        }
        file.close();

        // If task is found, rewrite the file without the deleted task
        if (taskFound)
        {
            ofstream outfile(filename);
            if (!outfile.is_open())
            {
                cerr << "Unable to open file for writing: " << filename << endl;
                return;
            }

            // Write remaining tasks to the file
            for (Task &task : loadedTasks)
            {
                outfile << task.getTaskID() << "," << task.getCategory() << "," << task.getTitle() << "," << task.getDescription() << ","
                        << task.getDeadline() << "," << task.getPriority() << "," << task.getStatus() << ","
                        << task.getLabel() << endl;
            }
            outfile.close();
            cout << "Task with ID " << taskID << " has been deleted." << endl;
        }
        else
        {
            cout << "Task with ID " << taskID << " not found." << endl;
        }
    }
};