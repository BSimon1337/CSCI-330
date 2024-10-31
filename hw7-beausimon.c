#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char* name;
    struct node* next;
};
typedef struct node LINK;

// Function prototypes
int LIST_SEARCH(char name[], LINK* head);
LINK* LIST_INSERT(char name[], LINK* head);
void LIST_DELETE(char name[], LINK** head);
void LIST_DISPLAY(LINK* head);
LINK* LIST_FREE(LINK* head);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("*******************************************\n");
        printf("* You must include a filename to load.    *\n");
        printf("*******************************************\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    LINK* head = NULL;
    char* line = NULL;
    size_t len = 0;

    while (getline(&line, &len, file) != -1) {
        char* name = strtok(line, " \n");
        char* op_code = strtok(NULL, " \n");

        if (name == NULL || op_code == NULL) {
            continue;
        }

        if (*op_code == 'a') {
            if (!LIST_SEARCH(name, head)) {
                head = LIST_INSERT(name, head);
            }
        }
        else if (*op_code == 'd') {
            if (LIST_SEARCH(name, head)) {
                LIST_DELETE(name, &head);
            }
        }
    }

    printf("Final list contents:\n");
    LIST_DISPLAY(head);

    head = LIST_FREE(head);
    free(line);
    fclose(file);

    return 0;
}

// Search function
int LIST_SEARCH(char name[], LINK* head) {
    while (head != NULL) {
        if (strcmp(head->name, name) == 0) {
            return 1;
        }
        head = head->next;
    }
    return 0;
}

// Insert function
LINK* LIST_INSERT(char name[], LINK* head) {
    LINK* new_node = (LINK*)malloc(sizeof(LINK));
    new_node->name = strdup(name);
    new_node->next = NULL;

    if (head == NULL || strcmp(new_node->name, head->name) < 0) {
        new_node->next = head;
        return new_node;
    }

    LINK* current = head;
    while (current->next != NULL && strcmp(current->next->name, new_node->name) < 0) {
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;

    return head;
}

// Delete function
void LIST_DELETE(char name[], LINK** head) {
    LINK* current = *head;
    LINK* previous = NULL;

    while (current != NULL && strcmp(current->name, name) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return;
    }

    if (previous == NULL) {
        *head = current->next;
    }
    else {
        previous->next = current->next;
    }

    free(current->name);
    free(current);
}

// Display function
void LIST_DISPLAY(LINK* head) {
    while (head != NULL) {
        printf("%s\n", head->name);
        head = head->next;
    }
}

// Free function
LINK* LIST_FREE(LINK* head) {
    while (head != NULL) {
        LINK* temp = head;
        head = head->next;
        free(temp->name);
        free(temp);
    }
    return NULL;
}
