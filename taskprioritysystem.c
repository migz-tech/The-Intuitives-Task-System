#include <stdio.h>
#include <string.h>
#define MAX_TASKS 100


// ================= STRUCT ================= // 
typedef struct {
    char name[50];
    int priority;
} Task;

Task tasks[MAX_TASKS];
int taskCount = 0;
char currentUser[50];
// ======================================== //


// ================= HEAP ================= //

// DON'T TOUCH THIS PART UNLESS YOU KNOW WHAT YOU'RE DOING :D
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
// ======================================== //


// ================= TASK ================= //

// 1. add a task
void addTask(char name[], int priority) {
    strcpy(tasks[taskCount].name, name);
    tasks[taskCount].priority = priority;
    heapifyUp(taskCount);
    taskCount++;
}

// 2. view tasks
void displayTasks() {
    if (taskCount == 0) {
        printf("\nNo tasks available.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        printf("\n%s (Priority: %d)\n", tasks[i].name, tasks[i].priority);
    }
}

// 3. process task
void popTask() {
    if (taskCount == 0) {
        printf("\nNo tasks available.\n");
        return;
    }

    printf("\nProcessing: %s (Priority: %d)\n", tasks[0].name, tasks[0].priority);

    tasks[0] = tasks[taskCount - 1];
    taskCount--;
    heapifyDown(0);
}

// 4. edit task
void editTask() {
    if (taskCount == 0) {
        printf("\nNo tasks available.\n");
        return;
    }

    char buffer[20];
    int index;

    printf("\n=== Edit Task ===\n");
    for (int i = 0; i < taskCount; i++) {
        printf("\n%d. %s (Priority: %d)\n", i + 1, tasks[i].name, tasks[i].priority);
    }

    printf("\nSelect task number: ");
    fgets(buffer, sizeof(buffer), stdin);

    if (sscanf(buffer, "%d", &index) != 1 || index < 1 || index > taskCount) {
        printf("\nInvalid selection!\n");
        return;
    }

    index--;

    char newName[50];
    int newPriority;

    printf("\nNew name: ");
    fgets(newName, sizeof(newName), stdin);
    newName[strcspn(newName, "\n")] = '\0';

    printf("\nNew priority: ");
    fgets(buffer, sizeof(buffer), stdin);

    if (sscanf(buffer, "%d", &newPriority) != 1) {
        printf("\nInvalid priority!\n");
        return;
    }

    strcpy(tasks[index].name, newName);
    tasks[index].priority = newPriority;

    heapifyUp(index);
    heapifyDown(index);

    printf("\nTask updated!\n");
}

// 5. Delete Task
void deleteTask() {
    if (taskCount == 0) {
        printf("\nNo tasks available.\n");
        return;
    }

    char buffer[20];
    int index;

    printf("\n=== Delete Task ===\n");
    for (int i = 0; i < taskCount; i++) {
        printf("\n%d. %s (Priority: %d)\n", i + 1, tasks[i].name, tasks[i].priority);
    }

    printf("\nSelect task to delete: ");
    fgets(buffer, sizeof(buffer), stdin);

    if (sscanf(buffer, "%d", &index) != 1 || index < 1 || index > taskCount) {
        printf("\nInvalid choice!\n");
        return;
    }

    index--;

    printf("\nDeleted: %s\n", tasks[index].name);

    tasks[index] = tasks[taskCount - 1];
    taskCount--;

    heapifyUp(index);
    heapifyDown(index);
}

// 6. Add Sorted Display (WITHOUT BREAKING HEAP)
void displaySortedTasks() {
    if (taskCount == 0) {
        printf("\nNo tasks available.\n");
        return;
    }

    // Copy heap
    Task temp[MAX_TASKS];
    int tempCount = taskCount;

    for (int i = 0; i < taskCount; i++) {
        temp[i] = tasks[i];
    }

    printf("\n=== Tasks (Sorted by Priority) ===\n");

    while (tempCount > 0) {
        printf("\n%s (Priority: %d)\n", temp[0].name, temp[0].priority);

        temp[0] = temp[tempCount - 1];
        tempCount--;

        // heapify temp (idk how to operate ts, lowkey just copy the heapify down code here DON'T TOUCH)
        int i = 0;
        while (1) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int largest = i;

            if (left < tempCount && temp[left].priority > temp[largest].priority)
                largest = left;

            if (right < tempCount && temp[right].priority > temp[largest].priority)
                largest = right;

            if (largest == i) break;

            Task t = temp[i];
            temp[i] = temp[largest];
            temp[largest] = t;

            i = largest;
        }
    }
}
// ======================================== //


// ================= FILE ================= //

// Saving process for Choice: 1
void saveTasks() {
    char filename[100];
    sprintf(filename, "data/%s_tasks.txt", currentUser);

    FILE *fp = fopen(filename, "w");
        if (!fp) {
        printf("Error saving file!\n");
    return;
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(fp, "%s %d\n", tasks[i].name, tasks[i].priority);
    }

    fclose(fp);
}

// Loading process for Choice: 2
void loadTasks() {
    printf("Loading tasks...\n");
    taskCount = 0;

    char filename[100];
    sprintf(filename, "data/%s_tasks.txt", currentUser);

    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char name[50];
    int priority;
    char line[100];

    while (fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "%49[^0-9\n] %d", name, &priority) == 2) {
        name[strcspn(name, "\n")] = '\0';
        addTask(name, priority);
        }
    }

    fclose(fp);
}
// ======================================== //


// ================= LOGIN ================= //

// 1. register user
void registerUser() {
    FILE *fp = fopen("data/users.txt", "a");

    char username[50], password[50];

    printf("\n=== Register ===\n");
    printf("\nNo spaces allowed in username or password.\n");
    printf("\nUsername: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    if (strchr(username, ' ') != NULL) {
        printf("\nUsername cannot contain spaces!\n");
        return;
    }

    printf("\n=== Note: Passwords are stored in plain text for simplicity. Don't use real passwords! ===\n");
    printf("\nPassword: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);

    printf("\nRegistered!");
}

// 2. login user
int loginUser() {
    FILE *fp = fopen("data/users.txt", "r");
    if (!fp) return 0;

    char username[50], password[50];
    char fileUser[50], filePass[50];

    printf("\n=== Login ===\n");
    printf("\nUsername: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("\nPassword: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    while (fscanf(fp, "%s %s", fileUser, filePass) != EOF) {
        if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filePass) == 0) {

            strcpy(currentUser, username);
            fclose(fp);
            printf("\nLogin successful!\n");
            return 1;
        }
    }

    fclose(fp);
    printf("\nInvalid login!\n");
    return 0;
}
// ======================================== //


// ================= MAIN ================= //
int main() {
    char buffer[20];
    int choice;
    int loggedIn = 0;
    char name[50];
    int priority;

    // LOGIN LOOP 
    while (!loggedIn) {
        printf("\n================= The Intuitives' Task System =================\n");
        printf("Welcome! Please register or login to continue.\n");
        printf("\n1. Register");
        printf("\n2. Login");
        printf("\nChoice (1-2): ");
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("\nInvalid input! Enter 1 or 2.\n");
            continue;
        }

        if (choice == 1) registerUser();
        else if (choice == 2) loggedIn = loginUser();
        else printf("\nInvalid choice!\n");
    
    }

    printf("\nUser logged in: %s\n", currentUser);
    loadTasks();
    printf("\nTasks loaded: %d\n", taskCount);

    // MAIN MENU
    while (1) {
        printf("\n================= The Intuitives' Task System ==================\n");
        printf("What would you like to do today?\n");
        printf("\n1. Add Task");
        printf("\n2. View Tasks");
        printf("\n3. Process Tasks");
        printf("\n4. Edit Task");
        printf("\n5. Delete Task");
        printf("\n6. View Sorted Tasks");
        printf("\n7. Save & Exit");
        printf("\nChoice (1-7): ");
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("\nInvalid input! Enter 1-7 only.\n");
            continue;
        }

        switch (choice) {
            case 1: {
                char confirm[10];

                while (1) {
                    printf("\nTask name: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';

                    printf("\nPriority: ");
                    fgets(buffer, sizeof(buffer), stdin);

                    if (sscanf(buffer, "%d", &priority) != 1) {
                        printf("Invalid priority!\n");
                        continue;
                    }

                    printf("\nName: %s\nPriority: %d\n", name, priority);
                    printf("Confirm? (y/n): ");
                    fgets(confirm, sizeof(confirm), stdin);

                    if (confirm[0] == 'y' || confirm[0] == 'Y') {
                        addTask(name, priority);
                        printf("\nTask added!\n");
                        break;
                    }
                }
                break;
            }

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
                deleteTask();
                break;

            case 6:
                displaySortedTasks();
                break;

            case 7:
                saveTasks();
                return 0;

            default:
                printf("\nInvalid choice! Please select 1-7.\n");
        }
    }
}