// This program defines a Task class to manage tasks, including their details like ID, title, description, deadline, priority, status, category, and label.
// It allows users to input task details and display task information.

#include <iostream>
#include <string>    // String manipulation functions like getline() and substr()
#include <algorithm> // Transformations such as converting strings to lowercase

using namespace std;

// The enums below represent the possible status and priority levels of a task.
// Makes the code more readable and maintainable, as it provides clear names for each state or priority level.

// Enumeration for task status
enum class TaskStatus
{
    PENDING,     // Task is pending
    IN_PROGRESS, // Task is in progress
    COMPLETED    // Task is completed
};

// Enumeration for task priorities
enum class TaskPriority
{
    LOW,    // Low priority
    MEDIUM, // Medium priority
    HIGH    // High priority
};

// The Task class represents a single task with various attributes such as ID, title, description, deadline, priority, status, category, and label.
// It provides methods to set and get these attributes, as well as to input task details from the user and display task information.
class Task
{
private:
    int taskID;            // Unique identifier for the task
    string title;          // Title of the task
    string description;    // Description of the task
    string deadline;       // Deadline for the task
    TaskPriority priority; // Priority of the task (LOW, MEDIUM, HIGH)
    TaskStatus status;     // Status of the task (PENDING, IN_PROGRESS, COMPLETED)
    string category;       // Category of the task (Personal, Work, etc)
    string label;          // Label of the task

public:
    // Constructor to initialize task properties
    Task(int id, string t, string desc, string dl, TaskPriority prio, TaskStatus stat, string l, string c)
        : taskID(id), title(t), description(desc), deadline(dl), priority(prio), status(stat), category(c), label(l) {}

    Task() {}

    // Setters

    // Setter for Task ID
    void setTaskID(int i)
    {
        if (i <= 0)
        {
            throw invalid_argument("Task ID must be a positive integer!");
        }
        taskID = i;
    }

    // Setter for priority
    void setPriority(string p)
    {
        // Convert input to lowercase
        transform(p.begin(), p.end(), p.begin(), ::tolower);

        // Check the input and assign the appropriate priority
        if (p == "low")
        {
            priority = TaskPriority::LOW;
        }
        else if (p == "medium")
        {
            priority = TaskPriority::MEDIUM;
        }
        else if (p == "high")
        {
            priority = TaskPriority::HIGH;
        }
        else
        {
            throw invalid_argument("Invalid priority! Priority must be 'Low', 'Medium', or 'High'");
        }
    }

    // Setter for status
    void setStatus(string s)
    {
        // Convert input to lowercase
        transform(s.begin(), s.end(), s.begin(), ::tolower);

        // Check the input and assign the appropriate status
        if (s == "pending")
        {
            status = TaskStatus::PENDING;
        }
        else if (s == "in progress")
        {
            status = TaskStatus::IN_PROGRESS;
        }
        else if (s == "completed")
        {
            status = TaskStatus::COMPLETED;
        }
        else
        {
            throw invalid_argument("Invalid status! Status must be 'Pending', 'In Progress', or 'Completed'");
        }
    }

    // Setter for category
    void setCategory(string c)
    {
        category = c;
    }

    // Setter for label
    void setLabel(string l)
    {
        label = l;
    }

    // Getters

    // Getter for Task ID
    int getTaskID()
    {
        return taskID;
    }

    // Getter for Task Title
    string getTitle()
    {
        return title;
    }

    // Getter for Task Description
    string getDescription()
    {
        return description;
    }

    // Getter for Task Deadline
    string getDeadline()
    {
        return deadline;
    }

    // Convert TaskPriority enum to string
    string getPriority()
    {
        return taskPriorityToString(priority);
    }

    // Convert TaskStatus enum to string
    string getStatus()
    {
        return taskStatusToString(status);
    }

    // Getter for Task Category
    string getCategory()
    {
        return category;
    }

    // Getter for Task Label
    string getLabel()
    {
        return label;
    }

    // The TaskManager class is a friend of the Task class to allow TaskManager to access private members of Task directly.
    // This friendship enables TaskManager to perform operations on Task objects without violating encapsulation.
    friend class TaskManager;

    // Task Priority and Status Conversion functions convert TaskPriority and TaskStatus enums to their corresponding string representations.
    // They are needed to facilitate input/output operations and ensure consistency in displaying task details.
    // By centralizing the conversion logic in these functions, the code becomes more modular and easier to maintain.

    // Convert TaskPriority enum to string
    string taskPriorityToString(TaskPriority priority)
    {
        switch (priority)
        {
        case TaskPriority::LOW:
            return "Low";
        case TaskPriority::MEDIUM:
            return "Medium";
        case TaskPriority::HIGH:
            return "High";
        default:
            return "Unknown";
        }
    }

    // Convert TaskStatus enum to string
    string taskStatusToString(TaskStatus status)
    {
        switch (status)
        {
        case TaskStatus::PENDING:
            return "Pending";
        case TaskStatus::IN_PROGRESS:
            return "In Progress";
        case TaskStatus::COMPLETED:
            return "Completed";
        default:
            return "Unknown";
        }
    }

    // Display task details
    void getTaskDetails()
    {
        cout << "Category: " << category << endl;
        cout << "Label: " << label << endl;

        cout << endl;
        cout << "Task ID: " << taskID << endl;
        cout << "Title: " << title << endl;
        cout << "Description: " << description << endl;
        cout << "Deadline: " << deadline << endl;
        cout << "Priority: " << taskPriorityToString(priority) << endl;
        cout << "Status: " << taskStatusToString(status) << endl;
    }

    friend istream &operator>>(istream &is, Task &task); // Allow input operator overload to access private members of Task
};

// Overloading >> operator to input Task objects from the standard input stream
istream &operator>>(istream &is, Task &task)
{
    try
    {
        // Prompt the user to enter task details
        cout << "Enter the Task Category (Personal, Work, etc.): ";
        // Read and store task category
        getline(is >> ws, task.category);

        // Prompt the user to add a label for the task
        cout << "Add a Label: ";
        // Read and store task label
        getline(is, task.label);

        // Prompt the user to enter the task ID
        cout << endl;
        cout << "Enter the Task ID: ";
        // Read and store the task ID
        is >> task.taskID;

        // Validate the task ID
        if (task.taskID < 0)
        {
            throw invalid_argument("Task ID must be a positive integer");
        }

        // Clear the input buffer
        is.ignore(numeric_limits<streamsize>::max(), '\n');

        // Prompt the user to enter the task title
        cout << "Enter Title: ";
        // Read and store the task title
        getline(is, task.title);

        // Prompt the user to enter the task description
        cout << "Enter Description: ";
        // Read and store the task description
        getline(is, task.description);

        // Validate the task deadline
        bool validDate = false;
        while (!validDate)
        {
            // Prompt the user to enter the task deadline
            cout << "Enter Deadline (DD/MM/YYYY): ";
            // Read and store the task deadline
            getline(is >> ws, task.deadline);

            // Validate the date format
            if (task.deadline.size() != 10 || task.deadline[2] != '/' || task.deadline[5] != '/')
            {
                cout << "Invalid date format! Please enter in DD/MM/YYYY format" << endl;
            }
            else
            {
                // Extract day, month, and year from the deadline string
                int day = stoi(task.deadline.substr(0, 2));
                int month = stoi(task.deadline.substr(3, 2));
                int year = stoi(task.deadline.substr(6, 4));

                // Validate day, month, and year individually
                if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2024)
                {
                    cout << "Invalid date! Please enter a valid date that isn't less than the current one" << endl;
                }
                else
                {
                    // If all validations pass, set validDate flag to true
                    validDate = true;
                }
            }
        }

        // Prompt the user to enter the task priority
        string priorityStr;
        bool validInput = false;
        while (!validInput)
        {
            cout << "Enter the Task Priority (Low, Medium, High): ";
            getline(cin >> ws, priorityStr);

            // Convert input to lowercase
            transform(priorityStr.begin(), priorityStr.end(), priorityStr.begin(), ::tolower);

            // Check if the input is valid
            if (priorityStr == "low" || priorityStr == "medium" || priorityStr == "high")
            {
                validInput = true;
            }
            else
            {
                cout << "Invalid input! Priority must be 'Low', 'Medium', or 'High'" << endl;
            }
        }

        // Convert input priority to TaskPriority enum and assign it to the task object
        transform(priorityStr.begin(), priorityStr.end(), priorityStr.begin(), [](unsigned char c)
                  { return tolower(c); });
        if (priorityStr == "low")
        {
            task.priority = TaskPriority::LOW;
        }
        else if (priorityStr == "medium")
        {
            task.priority = TaskPriority::MEDIUM;
        }
        else if (priorityStr == "high")
        {
            task.priority = TaskPriority::HIGH;
        }

        // Prompt the user to enter the task status
        string statusStr;
        validInput = false; // Resetting the flag for status input validation
        // Loop until valid input is received for status
        while (!validInput)
        {
            cout << "Enter the Task Status (Pending, In Progress, Completed): ";
            getline(is >> ws, statusStr);

            // Convert input to lowercase
            transform(statusStr.begin(), statusStr.end(), statusStr.begin(), ::tolower);

            // Check if the input is valid
            if (statusStr == "pending" || statusStr == "in progress" || statusStr == "completed")
            {
                validInput = true;
            }
            else
            {
                cout << "Invalid input! Status must be 'Pending', 'In Progress', or 'Completed'" << endl;
            }
        }

        // Convert input status to TaskStatus enum and assign it to the task object
        if (statusStr == "pending")
        {
            task.status = TaskStatus::PENDING;
        }
        else if (statusStr == "in progress")
        {
            task.status = TaskStatus::IN_PROGRESS;
        }
        else if (statusStr == "completed")
        {
            task.status = TaskStatus::COMPLETED;
        }
    }
    catch (const exception &ex)
    {
        cerr << "Error: " << ex.what() << endl;             // Print out the error message associated with the caught exception
        is.clear();                                         // Clear the error state of the input stream
        is.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore any remaining characters in the input buffer until a newline character is encountered
    } // End of catch block: Handles any exceptions that occur during input processing

    return is;
}