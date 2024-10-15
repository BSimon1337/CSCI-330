#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// define the _data struct for storing name and phone number
struct _data {
    char* name;
    long number;
};


// Scan function
int SCAN(FILE** stream) {
    if (*stream == NULL) { //check if the file is not opened
        printf("Error: Unable to open file stream.\n");
            return -1; // Return -1 to indicate an error
    }

    int line_count = 0;
    char ch;

    // loop through the file to count the number of new lines
    while ((ch = fgetc(*stream)) != EOF) {
        if (ch == '\n') {
            line_count++;
        }
    }
    // Reset the file pointer to the beginning of the file
    rewind(*stream);
    return line_count; //return the number of lines
}

struct _data* LOAD(FILE* stream, int size) {
    if (stream == NULL) {
        printf("Error: Invalid file stream.\n");
        return NULL;
    }

    struct _data* BlackBox = (struct _data*)malloc(size * sizeof(struct _data));
    if (BlackBox == NULL) {
        printf("Error: Memory allocation failed for BlackBox.\n");
        return NULL;
    }

    char line[256];  // Fixed buffer size for reading lines
    int index = 0;

    while (fgets(line, sizeof(line), stream) != NULL) {
        char* name = strtok(line, " ");
        char* number = strtok(NULL, " ");

        if (name == NULL || number == NULL) {
            printf("Error: Invalid data format in file.\n");
            free(BlackBox);
            return NULL;
        }

        BlackBox[index].name = (char*)malloc(strlen(name) + 1);
        if (BlackBox[index].name == NULL) {
            printf("Error: Memory allocation failed for name.\n");
            free(BlackBox);
            return NULL;
        }

        strcpy(BlackBox[index].name,name);
        BlackBox[index].number = atol(number);

        index++;
    }

    return BlackBox;
}

// SEARCH funciton: searches for name in the dynamic array
void SEARCH(struct _data* BlackBox, char* name, int size) {
    for (int i = 0; i < size; i++) {
        // Compare the name of the struct with the given name
        if (strcmp(BlackBox[i].name, name) == 0) {
            printf("*******************************************\n");
            printf("The name was found at the %d entry.\n", i + 1);
            printf("*******************************************\n");
            return;
        }
    }
    // if the name is not found print a message
    printf("*******************************************\n");
    printf("The name was NOT found.\n");
    printf("*******************************************\n");
}

void FREE(struct _data* BlackBox, int size) {
    // free the dynamically allocated memory for each name
    for (int i = 0; i < size; i++) {
        free(BlackBox[i].name);
    }
    free(BlackBox);
}

int main(int argc, char** argv) {
    // check if a command-line argument (name to search) is provided
    if (argc < 2) {
        printf("*******************************************\n");
        printf("* You must include a name to search for.  *\n");
        printf("*******************************************\n");
        return 1; // Exit if no name is provided
    }

    FILE* file = fopen("hw5.data.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open the file. \n");
        return 1;
    }

    // Step 1: Scan the file to get the number of lines
    int lines = SCAN(&file);
    if (lines == -1) {
        fclose(file);
        return 1;
    }

    // Step 2: Load the data from the file into a dynamic array
    struct _data* BlackBox = LOAD(file, lines);
    fclose(file); // Close the file after loading the data

    // Step 3: Search for the name provided via command-line argument
    SEARCH(BlackBox, argv[1], lines);

    // Step 4: Free the dynamically allocated memory
    FREE(BlackBox, lines);

    return 0;

}






