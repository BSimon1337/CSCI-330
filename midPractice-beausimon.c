#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void sortByString(struct Vehicle* vehicles, int size, int order);
void printData(struct Vehicle* vehicles, int size);
int getUserInput();

int main() {
    struct Vehicle* vehicles = NULL;
    int size = 0, choice;

    loadData(&vehicles, &size);

    while (1) {
        choice = getUserInput();

        switch (choice) {
        case 1:
            sortByString(vehicles, size, 1);  // Sort A to Z
            printData(vehicles, size);
            break;
        case 2:
            sortByString(vehicles, size, 0);  // Sort Z to A
            printData(vehicles, size);
            break;
        case 3:
            sortByFloat(vehicles, size, 0);  // Sort high to low
            printData(vehicles, size);
            break;
        case 4:
            sortByFloat(vehicles, size, 1);  // Sort low to high
            printData(vehicles, size);
            break;
        case 5:
            free(vehicles);  // Free memory and exit
            return 0;
        default:
            printf("Invalid choice. Please try again.\\n");
        }
    }
}

// Function for input validation
int getUserInput() {
    int choice;
    char inputBuffer[10];  // Buffer to hold user input

    while (1) {
        // Display menu
        printf("Menu:\n");
        printf("1. Sort data by the string value & print alphabetically (A to Z)\n");
        printf("2. Sort data by the string value & print in reverse alphabetical order (Z to A)\n");
        printf("3. Sort data by the float value & print high to low\n");
        printf("4. Sort data by the float value & print low to high\n");
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
    FILE* file = fopen("midtermPractice.data", "r");
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

void sortByString(struct Vehicle* vehicles, int size, int order) {
    struct Vehicle temp;
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if ((order == 1 && strcmp(vehicles[i].name, vehicles[j].name) > 0) ||
                (order == 0 && strcmp(vehicles[i].name, vehicles[j].name) < 0)) {
                temp = vehicles[i];
                vehicles[i] = vehicles[j];
                vehicles[j] = temp;
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

