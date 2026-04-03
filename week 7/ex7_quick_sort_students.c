/*
Name: Kirtika
Roll No: 2023A7PS0219U
Course: CS F211 - Data Structures & Algorithms
Lab Exercise 7: Quick Sort on Student Records
*/

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define MAX_STUDENTS 200
#define NAME_LEN 64

typedef struct {
    char name[NAME_LEN];
    int id;
    float cgpa;
} Student;

static long long current_time_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec;
}

static void print_records(const Student arr[], int n) {
    printf("%-5s %-20s %-10s %-10s\n", "No.", "Name", "ID", "CGPA");
    printf("------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-5d %-20s %-10d %-10.2f\n", i + 1, arr[i].name, arr[i].id, arr[i].cgpa);
    }
}

static void print_partition_stage(const Student arr[], int n, int low, int high, float pivot, int pivot_index) {
    printf("\nAfter partition [%d..%d], pivot = %.2f at index %d\n", low, high, pivot, pivot_index);
    for (int i = 0; i < n; i++) {
        printf("%s(%.2f)", arr[i].name, arr[i].cgpa);
        if (i != n - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

static void swap_students(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

static int partition(Student arr[], int low, int high, int total_size) {
    float pivot = arr[high].cgpa;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].cgpa <= pivot) {
            i++;
            swap_students(&arr[i], &arr[j]);
        }
    }

    swap_students(&arr[i + 1], &arr[high]);
    int pivot_index = i + 1;

    print_partition_stage(arr, total_size, low, high, pivot, pivot_index);

    return pivot_index;
}

static void quick_sort(Student arr[], int low, int high, int total_size) {
    if (low < high) {
        int pi = partition(arr, low, high, total_size);
        quick_sort(arr, low, pi - 1, total_size);
        quick_sort(arr, pi + 1, high, total_size);
    }
}

int main(void) {
    Student students[MAX_STUDENTS];
    int n;

    printf("Enter number of student records (at least 10, max %d): ", MAX_STUDENTS);
    if (scanf("%d", &n) != 1 || n < 10 || n > MAX_STUDENTS) {
        printf("Invalid number of records.\n");
        return 1;
    }

    printf("Enter each record as: name id cgpa\n");
    for (int i = 0; i < n; i++) {
        printf("Record %d: ", i + 1);
        if (scanf("%63s %d %f", students[i].name, &students[i].id, &students[i].cgpa) != 3) {
            printf("Invalid input format.\n");
            return 1;
        }
    }

    printf("\nUnsorted student records:\n");
    print_records(students, n);

    long long start = current_time_us();
    quick_sort(students, 0, n - 1, n);
    long long elapsed = current_time_us() - start;

    printf("\nSorted student records (ascending by CGPA):\n");
    print_records(students, n);

    printf("\nQuick sort execution time: %lld micro-secs\n", elapsed);
    return 0;
}
