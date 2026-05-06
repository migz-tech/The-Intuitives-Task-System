#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_TASKS 100


// ================= STRUCT ================= // 
typedef struct {
    char name[50];
    int priority;
    char dueDate[20];
} Task;

void archiveTask(Task t);

Task tasks[MAX_TASKS];
int taskCount = 0;
char currentUser[50];
// ======================================== //

// ================= HEAP ================= //
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
void addTask(char name[], int priority, char dueDate[]) {
    if (taskCount >= MAX_TASKS) {
        system("cls");
        printf("Task list is full!\n");
        return;
    }

    strcpy(tasks[taskCount].name, name);
    tasks[taskCount].priority = priority;
    strcpy(tasks[taskCount].dueDate, dueDate); // NEW
    heapifyUp(taskCount);
    taskCount++;
}

// 2. view tasks (Sorted)
void displaySortedTasks() {
    if (taskCount == 0) {
        system("cls");
        printf("\nNo tasks available.\n");
        return;
    }

    // Copy heap
    Task temp[MAX_TASKS];
    int tempCount = taskCount;

    for (int i = 0; i < taskCount; i++) {
        temp[i] = tasks[i];
    }

    system("cls");
    printf("\n=== Tasks ===\n");

    while (tempCount > 0) {
        printf("\n%s (Priority: %d) (Due Date: %s)\n", temp[0].name, temp[0].priority, temp[0].dueDate);

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

void popTask() {
    if (taskCount == 0) {
        system("cls");
        printf("\nNo tasks available.\n");
        return;
    }

    Task temp[MAX_TASKS];
    int map[MAX_TASKS]; // maps sorted index → real index
    int tempCount = taskCount;

    // Copy tasks + track original index
    for (int i = 0; i < taskCount; i++) {
        temp[i] = tasks[i];
        map[i] = i;
    }

    printf("\n=== Process Task ===\n");

    int displayIndex = 0;

    // Display sorted WITHOUT destroying original
    while (tempCount > 0) {
        printf("\n%d. %s (Priority: %d) (Due: %s)\n", displayIndex + 1, temp[0].name, temp[0].priority, temp[0].dueDate);

        // Remove root (like heap pop)
        temp[0] = temp[tempCount - 1];
        map[0] = map[tempCount - 1];
        tempCount--;

        // Heapify down
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

            // swap temp
            Task t = temp[i];
            temp[i] = temp[largest];
            temp[largest] = t;

            // swap mapping too
            int m = map[i];
            map[i] = map[largest];
            map[largest] = m;

            i = largest;
        }

        displayIndex++;
    }

    char buffer[20];
    int choice;

    printf("\nEnter task number to process: ");
    fgets(buffer, sizeof(buffer), stdin);

    if (sscanf(buffer, "%d", &choice) != 1 || choice < 1 || choice > taskCount) {
        printf("\nInvalid choice!\n");
        return;
    }

    int realIndex = map[choice - 1]; // map back to real index in heap

    printf("\nConfirm processing: %s (y/n): ", tasks[realIndex].name);
    fgets(buffer, sizeof(buffer), stdin);

    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        system("cls");
        printf("\nProcessing cancelled.\n");
        return;
    }

    // Process
    system("cls");
    printf("\nProcessing: %s (Priority: %d) (Due: %s)\n",
           tasks[realIndex].name,
           tasks[realIndex].priority,
           tasks[realIndex].dueDate);

    printf("Process Complete! Archiving...\n");
    archiveTask(tasks[realIndex]);

    // Remove from heap
    tasks[realIndex] = tasks[taskCount - 1];
    taskCount--;

    if (realIndex < taskCount) {
        heapifyUp(realIndex);
        heapifyDown(realIndex);
    }

    printf("\nTask archived successfully!\n");
}

void editTask() {
    if (taskCount == 0) {
        system("cls");
        printf("\nNo tasks available.\n");
        return;
    }

    char buffer[20];
    int choice;

    // Create temp heap + index mapping
    Task temp[MAX_TASKS];
    int indexMap[MAX_TASKS];
    int tempCount = taskCount;

    for (int i = 0; i < taskCount; i++) {
        temp[i] = tasks[i];
        indexMap[i] = i;
    }
    
    printf("\n=== Edit Task ===\n");

    int displayIndex = 0;

    while (tempCount > 0) {
        printf("\n%d. %s (Priority: %d) (Due: %s)\n",
               displayIndex + 1,
               temp[0].name,
               temp[0].priority,
               temp[0].dueDate);

        // Move root to end
        temp[0] = temp[tempCount - 1];
        indexMap[0] = indexMap[tempCount - 1];
        tempCount--;

        // Heapify temp
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

            // swap temp
            Task t = temp[i];
            temp[i] = temp[largest];
            temp[largest] = t;

            // swap index map
            int tmp = indexMap[i];
            indexMap[i] = indexMap[largest];
            indexMap[largest] = tmp;

            i = largest;
        }

        displayIndex++;
    }

    printf("\nSelect task number: ");
    fgets(buffer, sizeof(buffer), stdin);

    if (sscanf(buffer, "%d", &choice) != 1 || choice < 1 || choice > taskCount) {
        system("cls");
        printf("\nInvalid selection!\n");
        return;
    }

    int index = indexMap[choice - 1]; // 🔥 map back to real task

    printf("\nConfirm editing: %s (y/n)? ", tasks[index].name);
    fgets(buffer, sizeof(buffer), stdin);

    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        system("cls");
        printf("\nEdit cancelled.\n");
        return;
    }

    char newName[50];
    int newPriority;
    char newDueDate[50];

    system("cls");
    printf("\n=== Edit Task ===\n");
    printf("\nNew name: ");
    fgets(newName, sizeof(newName), stdin);
    newName[strcspn(newName, "\n")] = '\0';

    printf("\nNew priority: ");
    fgets(buffer, sizeof(buffer), stdin);

    if (sscanf(buffer, "%d", &newPriority) != 1) {
        printf("\nInvalid priority!\n");
        return;
    }

    printf("\nNew Due Date (YYYY-MM-DD): ");
    fgets(newDueDate, sizeof(newDueDate), stdin);
    newDueDate[strcspn(newDueDate, "\n")] = '\0';

    printf("\nName: %s\nPriority: %d\nDue Date: %s\n",
           newName, newPriority, newDueDate);

    printf("Confirm? (y/n): ");
    fgets(buffer, sizeof(buffer), stdin);

    if (buffer[0] == 'y' || buffer[0] == 'Y') {
        strcpy(tasks[index].name, newName);
        tasks[index].priority = newPriority;
        strcpy(tasks[index].dueDate, newDueDate);

        heapifyUp(index);
        heapifyDown(index);

        system("cls");
        printf("\nTask updated! (name: %s, priority: %d, due date: %s)\n", newName, newPriority, newDueDate);
    } else {
        system("cls");
        printf("\nEdit cancelled.\n");
    }
}

// 5. Delete Task
void deleteTask() {
    if (taskCount == 0) {
        system("cls");
        printf("\nNo tasks available.\n");
        return;
    }

    char buffer[20];
    int index;

    printf("\n=== Delete Task ===\n");
    for (int i = 0; i < taskCount; i++) {
        printf("\n%d. %s (Priority: %d) (Due Date: %s)\n", i + 1, tasks[i].name, tasks[i].priority, tasks[i].dueDate);
    }

    printf("\nSelect task to delete: ");
    fgets(buffer, sizeof(buffer), stdin);

    if (sscanf(buffer, "%d", &index) != 1 || index < 1 || index > taskCount) {
        system("cls");
        printf("\nInvalid choice!\n");
        return;
    }

    printf("\nConfirm deleting %s? (y/n): ", tasks[index - 1].name);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != 'y' && buffer[0] != 'Y') {
        system("cls");
        printf("\nDeletion cancelled.\n");
        return;
    }

    index--;

    system("cls");
    printf("\nDeleted: %s (Priority: %d) (Due Date: %s)\n", tasks[index].name, tasks[index].priority, tasks[index].dueDate);

    tasks[index] = tasks[taskCount - 1];
    taskCount--;

    // Heapify to restore order
    if (index < taskCount) {
        heapifyUp(index);
        heapifyDown(index);
    }
}

// 6. View Archive
void viewArchive() {
    char filename[100];
    sprintf(filename, "data/%s_archive.txt", currentUser);

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        system("cls");
        printf("\nNo archived tasks.\n");
        return;
    }

    Task temp[MAX_TASKS];
    int tempCount = 0;

    char line[100];
    char name[50], dueDate[20];
    int priority;

    // Load Archive into temp array
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%49[^|]|%d|%19[^\n]", name, &priority, dueDate) == 3) {
            strcpy(temp[tempCount].name, name);
            temp[tempCount].priority = priority;
            strcpy(temp[tempCount].dueDate, dueDate);
            tempCount++;
        }
    }

    fclose(fp);

    if (tempCount == 0) {
        printf("\nNo archived tasks.\n");
        return;
    }

    // Building Heap
    for (int i = (tempCount / 2) - 1; i >= 0; i--) {
        int parent = i;

        while (1) {
            int left = 2 * parent + 1;
            int right = 2 * parent + 2;
            int largest = parent;

            if (left < tempCount && temp[left].priority > temp[largest].priority)
                largest = left;

            if (right < tempCount && temp[right].priority > temp[largest].priority)
                largest = right;

            if (largest == parent) break;

            Task t = temp[parent];
            temp[parent] = temp[largest];
            temp[largest] = t;

            parent = largest;
        }
    }

    system("cls");
    printf("\n=== Archived Tasks ===\n");

    while (tempCount > 0) {
        printf("\n%s (Priority: %d) (Due: %s)\n",
               temp[0].name,
               temp[0].priority,
               temp[0].dueDate);

        temp[0] = temp[tempCount - 1];
        tempCount--;

        // heapify down
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
        fprintf(fp, "%s|%d|%s\n", tasks[i].name, tasks[i].priority, tasks[i].dueDate);
    }

    fclose(fp);
}

// Loading process for Choice: 2
void loadTasks() {
    printf("Loading tasks...");
    taskCount = 0;

    char filename[100];
    sprintf(filename, "data/%s_tasks.txt", currentUser);

    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char name[50];
    int priority;
    char dueDate[50];
    char line[100];

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%49[^|]|%d|%19[^\n]", name, &priority, dueDate) == 3) {
            addTask(name, priority, dueDate);
        }  
    }

    fclose(fp);
}

// Archiving process for Choice: 3
void archiveTask(Task t) {
    char filename[100];
    sprintf(filename, "data/%s_archive.txt", currentUser);

    FILE *fp = fopen(filename, "a");
    if (!fp) {
        system("cls");
        printf("Error saving archive!\n");
        return;
    }

    fprintf(fp, "%s|%d|%s\n", t.name, t.priority, t.dueDate);
    fclose(fp);
}

// ======================================== //


// ================= LOGIN ================= //

// 1. register user
void registerUser() {
    FILE *fp = fopen("data/users.txt", "a");

        if (!fp) {
        printf("Error opening users file!\n");
        return;
    }

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

    fprintf(fp, "%s|%s\n", username, password);
    fclose(fp);

    printf("\nRegistered!");
}

// 2. login user
int loginUser() {
    FILE *fp = fopen("data/users.txt", "r");
    if (!fp) {
        printf("Error opening users file!\n");
        return 0;
    }

    char username[50], password[50];
    char fileUser[50], filePass[50];

    printf("\n=== Login ===\n");
    printf("\nUsername: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("\nPassword: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    while (fscanf(fp, "%[^|]|%[^|\n]\n", fileUser, filePass) != EOF) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
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
        printf("\n============== The Intuitives' Task Priority System ==============\n");
        printf("\nWelcome! Please register or login to continue.");
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

    system("cls");
    printf("\nUser logged in: %s\n", currentUser);
    loadTasks();
    printf("Tasks loaded: %d\n", taskCount);

    // MAIN MENU
    while (1) {
        printf("\n============== The Intuitives' Task Priority System ==============\n");
        printf("\nWhat would you like to do today?\n");
        printf("\n1. Add Task");
        printf("\n2. View Tasks");
        printf("\n3. Process Tasks");
        printf("\n4. Edit Task");
        printf("\n5. Delete Task");
        printf("\n6. View Archive");
        printf("\n7. Save & Exit\n");
        printf("\nNOTE: Make sure to save before exiting, or your changes will be lost!\n");
        printf("\nChoice (1-7): ");
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("\nInvalid input! Enter 1-7 only.\n");
            continue;
        }

        switch (choice) {
            case 1: {
                char confirm[10];
                char dueDate[20];

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

                    printf("\nDue Date (YYYY-MM-DD): ");
                    fgets(dueDate, sizeof(dueDate), stdin);
                    dueDate[strcspn(dueDate, "\n")] = '\0';

                    printf("\nName: %s\nPriority: %d\nDue Date: %s\n", name, priority, dueDate);
                    printf("Confirm? (y/n): ");
                    fgets(confirm, sizeof(confirm), stdin);

                    if (confirm[0] == 'y' || confirm[0] == 'Y') {
                        system("cls");
                        addTask(name, priority, dueDate);
                        printf("\nTask added! (name: %s, priority: %d, due date: %s)\n", name, priority, dueDate);
                        break;
                    }
                }
                break;
            }

            case 2:
                displaySortedTasks();
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
                viewArchive();
                break;

            case 7:
                saveTasks();
                return 0;

            default:
                system("cls");
                printf("\nInvalid choice! Please select 1-7.\n");
        }
    }
}