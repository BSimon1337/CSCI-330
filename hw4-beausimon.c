#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>
#include <stdlib.h>

// Structure to hold the data from the file
struct Vehicle {
    char name[20];
    float engine;
    int price;
    char color[20];
};

// Function declarations
void loadData(struct Vehicle** vehicles, int* size);
void sortByFloat(struct Vehicle* vehicles, int size, int order);
void sortByInt(struct Vehicle* vehicles, int size, int order);
void printData(struct Vehicle* vehicles, int size);
int getUserInput();  // New function for input validation

int main() {
    struct Vehicle* vehicles = NULL;  // Dynamic memory allocation for the vehicles array
    int size = 0, choice;

    loadData(&vehicles, &size);  // Memory allocation is handled within this function

    while (1) {
        choice = getUserInput();  // Validate user input

        switch (choice) {
        case 1:
            sortByFloat(vehicles, size, 0);  // 0 for high to low
            printData(vehicles, size);
            break;
        case 2:
            sortByFloat(vehicles, size, 1);  // 1 for low to high
            printData(vehicles, size);
            break;
        case 3:
            sortByInt(vehicles, size, 0);    // 0 for high to low
            printData(vehicles, size);
            break;
        case 4:
            sortByInt(vehicles, size, 1);    // 1 for low to high
            printData(vehicles, size);
            break;
        case 5:
            free(vehicles);  // Freeing dynamically allocated memory before exit
            return 0;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Function for input validation
int getUserInput() {
    int choice;
    char inputBuffer[10];  // Buffer to hold user input

    while (1) {
        // Display menu
        printf("Menu:\n");
        printf("1. Sort by float (high to low)\n");
        printf("2. Sort by float (low to high)\n");
        printf("3. Sort by int (high to low)\n");
        printf("4. Sort by int (low to high)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL) {
            if (sscanf(inputBuffer, "%d", &choice) != 1 || choice < 1 || choice > 5) {
                printf("Invalid input! Please enter a number between 1 and 5.\n");
                continue;  // Ask for input again if invalid
            }
            break;
        }
    }
    return choice;
}

// Function to load data from the file
void loadData(struct Vehicle** vehicles, int* size) {
    FILE* file = fopen("hw4.data.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }


    // Check if file is empty
    fseek(file, 0, SEEK_END);  // Move to the end of the file
    long fileSize = ftell(file);  // Get the position (file size)
    fseek(file, 0, SEEK_SET);  // Move back to the beginning
    if (fileSize == 0) {
        printf("The file is empty!\n");
        fclose(file);
        exit(1);
    }

    int capacity = 10;  // Initial capacity for the dynamic array
    *vehicles = (struct Vehicle*)calloc(capacity, sizeof(struct Vehicle));  // Dynamically allocate memory

    if (*vehicles == NULL) {
        printf("Initial memory allocation failed!\n");
        fclose(file);
        exit(1);
    }

    *size = 0;
    while (fscanf(file, "%19s %f %d %19s", (*vehicles)[*size].name, &(*vehicles)[*size].engine,
        &(*vehicles)[*size].price, (*vehicles)[*size].color) != EOF) {
        (*size)++;

        // Ensure not writing out of bounds
        if (*size >= capacity) {
            capacity *= 2;  // Double the capacity if needed
            struct Vehicle* temp = (struct Vehicle*)realloc(*vehicles, capacity * sizeof(struct Vehicle));

            if (temp == NULL) {
                printf("Memory reallocation failed!\n");
                free(*vehicles);  // Free the original block of memory to avoid memory leak
                fclose(file);
                exit(1);
            }

            *vehicles = temp;  // Assign the newly allocated memory block to the original pointer
        }
    }

    fclose(file);
}

// Function to sort by float value (engine size) using bubble sort
void sortByFloat(struct Vehicle* vehicles, int size, int order) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if ((order == 0 && vehicles[j].engine < vehicles[j + 1].engine) ||
                (order == 1 && vehicles[j].engine > vehicles[j + 1].engine)) {
                struct Vehicle temp = vehicles[j];
                vehicles[j] = vehicles[j + 1];
                vehicles[j + 1] = temp;
            }
        }
    }
}

// Function to sort by int value (price) using bubble sort
void sortByInt(struct Vehicle* vehicles, int size, int order) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if ((order == 0 && vehicles[j].price < vehicles[j + 1].price) ||
                (order == 1 && vehicles[j].price > vehicles[j + 1].price)) {
                struct Vehicle temp = vehicles[j];
                vehicles[j] = vehicles[j + 1];
                vehicles[j + 1] = temp;
            }
        }
    }
}

// Function to print the data
void printData(struct Vehicle* vehicles, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s %.1f %d %s\n", vehicles[i].name, vehicles[i].engine, vehicles[i].price, vehicles[i].color);
    }
}
