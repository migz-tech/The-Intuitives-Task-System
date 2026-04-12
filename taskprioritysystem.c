#include <stdio.h>
#include <string.h>

#define MAX_TASKS 100

typedef struct {
    char name[50];
    int priority;
} Task;

Task tasks[MAX_TASKS];
int taskCount = 0;

// ================= HEAP =================
void swap(Task *a, Task *b) {
    Task temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(int index) {
    int parent = (index - 1) / 2;

    while (index > 0 && tasks[index].priority > tasks[parent].priority) {
        swap(&tasks[index], &tasks[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

void heapifyDown(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;

    if (left < taskCount && tasks[left].priority > tasks[largest].priority)
        largest = left;

    if (right < taskCount && tasks[right].priority > tasks[largest].priority)
        largest = right;

    if (largest != index) {
        swap(&tasks[index], &tasks[largest]);
        heapifyDown(largest);
    }
}

// ================= TASK =================
void addTask(char name[], int priority) {
    strcpy(tasks[taskCount].name, name);
    tasks[taskCount].priority = priority;
    heapifyUp(taskCount);
    taskCount++;
}

void editTask() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }

    char buffer[20];
    int index;

    // Show tasks with index
    printf("\n=== Edit Task ===\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. %s (Priority: %d)\n", i + 1, tasks[i].name, tasks[i].priority);
    }

    printf("Select task number to edit: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &index);

    if (index < 1 || index > taskCount) {
        printf("Invalid selection.\n");
        return;
    }

    index--; // convert to 0-based index

    char newName[50];
    int newPriority;

    printf("Enter new name: ");
    fgets(newName, sizeof(newName), stdin);
    newName[strcspn(newName, "\n")] = '\0';

    printf("Enter new priority: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &newPriority);

    // Apply changes
    strcpy(tasks[index].name, newName);
    tasks[index].priority = newPriority;

    // Rebalance heap
    heapifyUp(index);
    heapifyDown(index);

    printf("Task updated successfully!\n");
}

void popTask() {
    if (taskCount == 0) return;

    printf("Processing: %s (Priority: %d)\n", tasks[0].name, tasks[0].priority);

    tasks[0] = tasks[taskCount - 1];
    taskCount--;
    heapifyDown(0);
}

void displayTasks() {
    for (int i = 0; i < taskCount; i++) {
        printf("%s (Priority: %d)\n", tasks[i].name, tasks[i].priority);
    }
}

// ================= FILE =================
void saveTasks() {
    FILE *fp = fopen("data/tasks.txt", "w");

    for (int i = 0; i < taskCount; i++) {
        fprintf(fp, "%s %d\n", tasks[i].name, tasks[i].priority);
    }

    fclose(fp);
}

void loadTasks() {
    FILE *fp = fopen("data/tasks.txt", "r");
    if (!fp) return;

    char name[50];
    int priority;

    printf("Loading: %s %d\n", name, priority);
    while (fscanf(fp, " %[^\n] %d", name, &priority) == 2) {
        printf("Tasks loaded: %d\n", taskCount);
        addTask(name, priority);

        if (taskCount == 0) {
            printf("No tasks available.\n");
        }
    }

    fclose(fp);
}

// ================= LOGIN =================
void registerUser() {
    FILE *fp = fopen("data/users.txt", "a");

    char username[50], password[50];

    printf("\nEnter Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("\nEnter Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);

    printf("\nRegistered!\n");
}

int loginUser() {
    FILE *fp = fopen("data/users.txt", "r");
    if (!fp) return 0;

    char username[50], password[50];
    char fileUser[50], filePass[50];

    int userFound = 0;

    printf("\nUsername: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("\nPassword: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    while (fscanf(fp, "%s %s", fileUser, filePass) != EOF) {
        if (strcmp(username, fileUser) == 0) {
            userFound = 1;

            if (strcmp(password, filePass) == 0) {
                printf("\nLogin successful!\n");
                fclose(fp);
                return 1;
            } else {
                printf("\nIncorrect password!\n");
                fclose(fp);
                return 0;
            }
        }
    }

    fclose(fp);

    if (!userFound) {
        printf("\nUsername not found. Please register first.\n");
        return 0;
    }

    return 0;
}

// ================= MAIN =================
int main() {
    char buffer[20];
    int choice;
    int loggedIn = 0;
    char name[50];
    int priority;

    while (!loggedIn) {
        printf("\n========== Welcome to the Intuitives' Tasks System! ==========");
        printf("\n1. Register");
        printf("\n2. Login");
        printf("\nChoice: ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        int pos;
        if (sscanf(buffer, "%d %n", &choice, &pos) != 1 || buffer[pos] != '\n') {
            printf("Invalid input. Please enter ONLY 1 or 2.\n");
            continue;
            }

        if (choice == 1) {
        registerUser();
        }
        else if (choice == 2) {
            loggedIn = loginUser();
        }
        else {
            printf("Invalid choice. Please enter 1 or 2.\n");
        }
    }

    loadTasks();

    while (1) {
        printf("\n========== Welcome to the Intuitives' Tasks System! ==========\n");
        printf("What do you want to do?\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Process Task\n");
        printf("4. Edit Task\n");
        printf("5. Exit\n");
        printf("Choice: ");
        fgets(buffer, sizeof(buffer), stdin);

        int pos;
        if (sscanf(buffer, "%d %n", &choice, &pos) != 1 || buffer[pos] != '\n') {
        printf("Invalid input. Please enter a number (1-5).\n");
        continue;
        }

        switch (choice) {
            case 1: {
                char confirm[10];

                while (1) {
                    printf("Enter Task name: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';

                    printf("Enter Priority Level (1-10): ");
                    fgets(buffer, sizeof(buffer), stdin);
                    sscanf(buffer, "%d", &priority);

                    // ================= CONFIRMATION =================
                    printf("\nYou entered:\n");
                    printf("Name: %s\nPriority: %d\n", name, priority);

                    printf("Confirm? (y/n): ");
                    fgets(confirm, sizeof(confirm), stdin);

                    if (confirm[0] == 'y' || confirm[0] == 'Y') {
                        addTask(name, priority);
                        printf("Task added!\n");
                        break;
                    } else {
                        printf("Re-enter task details.\n\n");
                    }
                }
                break;}
            case 2:
                displayTasks();
                break;

            case 3:
                popTask();
                break;

            case 4:
                editTask();
                break;

            case 5:
                saveTasks();
                return 0;
        }
    }
}
