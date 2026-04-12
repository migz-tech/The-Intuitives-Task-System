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

    while (fscanf(fp, " %[^\n] %d", name, &priority) != EOF) {
        addTask(name, priority);
    }

    fclose(fp);
}

// ================= LOGIN =================
void registerUser() {
    FILE *fp = fopen("data/users.txt", "a");

    char username[50], password[50];

    printf("Enter Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);

    printf("Registered!\n");
}

int loginUser() {
    FILE *fp = fopen("data/users.txt", "r");
    if (!fp) return 0;

    char username[50], password[50];
    char fileUser[50], filePass[50];

    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    while (fscanf(fp, "%s %s", fileUser, filePass) != EOF) {
        if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filePass) == 0) {
            printf("Login successful!\n");
            fclose(fp);
            return 1;
        }   if (strcmp(username, fileUser) != 0 &&
            strcmp(password, filePass) == 0) {
            printf("Incorrect username!\n");
            fclose(fp);
            return 1;
    }   if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filePass) != 0) {
            printf("Incorrect password!\n");
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
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
        printf("\n1. Register\n2. Login\nChoice: ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        if (choice == 1) registerUser();
        else if (choice == 2) loggedIn = loginUser();
    }

    loadTasks();

    while (1) {
        printf("\n========== Welcome to the Intuitives' Tasks System! ==========");
        printf("\n1. Add Task\n2. View\n3. Process\n4. Exit\nChoice: ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        switch (choice) {
            case 1:
                printf("Task name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';

                printf("Priority: ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &priority);

                addTask(name, priority);
                break;

            case 2:
                displayTasks();
                break;

            case 3:
                popTask();
                break;

            case 4:
                saveTasks();
                return 0;
        }
    }
}
