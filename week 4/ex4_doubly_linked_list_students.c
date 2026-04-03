/*
Name: Kirtika
Roll No: 2023A7PS0219U
Course: CS F211 - Data Structures & Algorithms
Lab Exercise 4: Doubly Linked List
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 100
#define BRANCH_LEN 16

typedef struct StudentNode {
    char name[NAME_LEN];
    int id;
    char branch[BRANCH_LEN];
    float cgpa;
    struct StudentNode *prev;
    struct StudentNode *next;
} StudentNode;

static void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard */
    }
}

static void to_uppercase(char *text) {
    for (size_t i = 0; text[i] != '\0'; i++) {
        text[i] = (char)toupper((unsigned char)text[i]);
    }
}

static int is_valid_branch(const char *branch) {
    static const char *valid_branches[] = {"CS", "BT", "CE", "CHE", "EEE"};

    for (size_t i = 0; i < sizeof(valid_branches) / sizeof(valid_branches[0]); i++) {
        if (strcmp(branch, valid_branches[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static StudentNode *create_student(const char *name, int id, const char *branch, float cgpa) {
    StudentNode *node = (StudentNode *)malloc(sizeof(StudentNode));
    if (node == NULL) {
        return NULL;
    }

    strncpy(node->name, name, NAME_LEN - 1);
    node->name[NAME_LEN - 1] = '\0';
    node->id = id;
    strncpy(node->branch, branch, BRANCH_LEN - 1);
    node->branch[BRANCH_LEN - 1] = '\0';
    node->cgpa = cgpa;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

static int add_student_sorted(StudentNode **head, const char *name, int id, const char *branch, float cgpa) {
    StudentNode *current = *head;
    StudentNode *previous = NULL;

    while (current != NULL && current->id < id) {
        previous = current;
        current = current->next;
    }

    if (current != NULL && current->id == id) {
        return 0;
    }

    StudentNode *new_node = create_student(name, id, branch, cgpa);
    if (new_node == NULL) {
        return -1;
    }

    new_node->prev = previous;
    new_node->next = current;

    if (previous != NULL) {
        previous->next = new_node;
    } else {
        *head = new_node;
    }

    if (current != NULL) {
        current->prev = new_node;
    }

    return 1;
}

static int delete_student(StudentNode **head, int id) {
    StudentNode *current = *head;

    while (current != NULL && current->id != id) {
        current = current->next;
    }

    if (current == NULL) {
        return 0;
    }

    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        *head = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    }

    free(current);
    return 1;
}

static StudentNode *search_student(StudentNode *head, int id) {
    StudentNode *current = head;

    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

static void print_student(const StudentNode *student) {
    printf("Name   : %s\n", student->name);
    printf("IDNO   : %d\n", student->id);
    printf("Branch : %s\n", student->branch);
    printf("CGPA   : %.2f\n", student->cgpa);
}

static void display_descending(StudentNode *head) {
    if (head == NULL) {
        printf("No student records available.\n");
        return;
    }

    StudentNode *tail = head;
    while (tail->next != NULL) {
        tail = tail->next;
    }

    printf("\nAll Students' details in descending order of ID:\n");
    printf("%-20s %-10s %-10s %-10s\n", "Name", "IDNO", "Branch", "CGPA");
    printf("----------------------------------------------------------\n");

    while (tail != NULL) {
        printf("%-20s %-10d %-10s %-10.2f\n", tail->name, tail->id, tail->branch, tail->cgpa);
        tail = tail->prev;
    }
}

static void show_best_performer(StudentNode *head) {
    if (head == NULL) {
        printf("No student records available.\n");
        return;
    }

    StudentNode *best = head;
    StudentNode *current = head->next;

    while (current != NULL) {
        if (current->cgpa > best->cgpa) {
            best = current;
        }
        current = current->next;
    }

    printf("\nStudent with highest CGPA:\n");
    print_student(best);
}

static void free_list(StudentNode *head) {
    StudentNode *current = head;
    while (current != NULL) {
        StudentNode *next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    StudentNode *head = NULL;
    int choice;

    do {
        printf("\nOptions:\n");
        printf("1. Add student\n");
        printf("2. Delete student\n");
        printf("3. Search student\n");
        printf("4. Display all students (descending order)\n");
        printf("5. Best performer\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number from 1 to 6.\n");
            clear_input_buffer();
            continue;
        }

        if (choice == 1) {
            char name[NAME_LEN];
            int id;
            char branch[BRANCH_LEN];
            float cgpa;

            printf("Enter name, idno, branch & cgpa: ");
            if (scanf("%99s %d %15s %f", name, &id, branch, &cgpa) != 4) {
                printf("Invalid input format.\n");
                clear_input_buffer();
                continue;
            }

            to_uppercase(branch);
            if (!is_valid_branch(branch)) {
                printf("Invalid branch. Use one of: CS, BT, CE, CHE, EEE.\n");
                continue;
            }

            int status = add_student_sorted(&head, name, id, branch, cgpa);
            if (status == 1) {
                printf("Addition Success!\n");
            } else if (status == 0) {
                printf("Student ID already exists.\n");
            } else {
                printf("Memory allocation failed.\n");
            }
        } else if (choice == 2) {
            int id;
            printf("Enter IDNO of student to delete: ");
            if (scanf("%d", &id) != 1) {
                printf("Invalid ID input.\n");
                clear_input_buffer();
                continue;
            }

            if (delete_student(&head, id)) {
                printf("Deletion Success!\n");
            } else {
                printf("Student with ID %d not found.\n", id);
            }
        } else if (choice == 3) {
            int id;
            printf("Enter IDNO of student to search: ");
            if (scanf("%d", &id) != 1) {
                printf("Invalid ID input.\n");
                clear_input_buffer();
                continue;
            }

            StudentNode *result = search_student(head, id);
            if (result != NULL) {
                printf("\nStudent details are:\n");
                print_student(result);
            } else {
                printf("Student with ID %d not found.\n", id);
            }
        } else if (choice == 4) {
            display_descending(head);
        } else if (choice == 5) {
            show_best_performer(head);
        } else if (choice == 6) {
            printf("Bye Bye!\n");
        } else {
            printf("Invalid choice. Please select 1 to 6.\n");
        }
    } while (choice != 6);

    free_list(head);
    return 0;
}
