#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the size of our Hash Table array (M).
// The performance (O(1) average) depends on a good distribution of keys.
#define TABLE_SIZE 50 

// --- Data Structures ---

// 1. Structure for the core student data
typedef struct Student {
    int id;
    char name[50];
    char grade[5];
} Student;

// 2. Structure for the Hash Table node (used for chaining/collision handling)
typedef struct Node {
    Student student;
    struct Node *next;
} Node;

// The Hash Table: an array of pointers to Nodes (linked lists)
Node *hashTable[TABLE_SIZE];

// --- Core DSA Functions ---

/**
 * Hash function: Calculates the index in the hash table array.
 * This ensures O(1) average access time.
 */
int hash(int id) {
    return id % TABLE_SIZE;
}

/**
 * Creates a new Student node for insertion.
 */
Node* create_node(int id, const char* name, const char* grade) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Error allocating memory for new student node");
        exit(EXIT_FAILURE);
    }
    newNode->student.id = id;
    strncpy(newNode->student.name, name, 49);
    newNode->student.name[49] = '\0'; // Ensure null termination
    strncpy(newNode->student.grade, grade, 4);
    newNode->student.grade[4] = '\0'; // Ensure null termination
    newNode->next = NULL;
    return newNode;
}

/**
 * Adds a new student record. O(1) average time complexity.
 */
void insert_student(int id, const char* name, const char* grade) {
    int index = hash(id);
    Node *head = hashTable[index];
    
    // Check for duplicate ID (collision search)
    Node *current = head;
    while (current != NULL) {
        if (current->student.id == id) {
            printf("\nError: Student ID %d already exists.\n", id);
            return;
        }
        current = current->next;
    }

    // Insert new node at the head of the linked list (chaining)
    Node *newNode = create_node(id, name, grade);
    newNode->next = head;
    hashTable[index] = newNode;
    printf("\nSuccess: Student %s (ID: %d) added.\n", name, id);
}

/**
 * Searches for a student record. O(1) average time complexity.
 */
Node* search_student(int id) {
    int index = hash(id);
    Node *current = hashTable[index];

    // Traverse the linked list at the bucket index
    while (current != NULL) {
        if (current->student.id == id) {
            printf("\n--- Student Found ---\n");
            printf("ID: %d\n", current->student.id);
            printf("Name: %s\n", current->student.name);
            printf("Grade: %s\n", current->student.grade);
            printf("---------------------\n");
            return current; // Student found
        }
        current = current->next;
    }

    printf("\nError: Student ID %d not found.\n", id);
    return NULL; // Student not found
}

/**
 * Removes a student record. O(1) average time complexity.
 */
void delete_student(int id) {
    int index = hash(id);
    Node *current = hashTable[index];
    Node *prev = NULL;

    // Search and remove
    while (current != NULL) {
        if (current->student.id == id) {
            if (prev == NULL) {
                // Deleting the head node of the list
                hashTable[index] = current->next;
            } else {
                // Deleting a node in the middle or end
                prev->next = current->next;
            }
            free(current);
            printf("\nSuccess: Student ID %d removed.\n", id);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("\nError: Student ID %d not found.\n", id);
}

/**
 * Displays all stored student records. O(N) complexity as it iterates over all N students.
 */
void display_all_students() {
    int count = 0;
    printf("\n--- All Student Records ---\n");
    printf("%-5s | %-20s | %-5s\n", "ID", "Name", "Grade");
    printf("--------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = hashTable[i];
        while (current != NULL) {
            printf("%-5d | %-20s | %-5s\n", 
                   current->student.id, 
                   current->student.name, 
                   current->student.grade);
            current = current->next;
            count++;
        }
    }

    if (count == 0) {
        printf("The system contains no student records.\n");
    }
    printf("--------------------------------------\n");
}

// --- Utility Functions ---

void initialize_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

int get_int_input(const char* prompt) {
    int value;
    printf("%s", prompt);
    // Loop until valid integer input is received
    while (scanf("%d", &value) != 1) {
        printf("Invalid input. Please enter a valid number: ");
        // Clear the input buffer
        while (getchar() != '\n');
    }
    // Clear the remaining input buffer
    while (getchar() != '\n');
    return value;
}

void get_string_input(const char* prompt, char* buffer, size_t size) {
    printf("%s", prompt);
    // Read up to size - 1 characters from standard input
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0'; // Handle read error
        return;
    }
    // Remove trailing newline character if present
    buffer[strcspn(buffer, "\n")] = 0; 
}


// --- Main Application Logic ---

void main_menu() {
    int choice;
    int s_id;
    char name[50];
    char grade[5];

    // Initialize all hash table pointers to NULL
    initialize_table(); 

    while (1) {
        printf("\n===== Student Management System (Hash Table) =====");
        printf("\n1. Add New Student (O(1) Avg)");
        printf("\n2. Find Student by ID (O(1) Avg)");
        printf("\n3. Remove Student by ID (O(1) Avg)");
        printf("\n4. Display All Students (O(N))");
        printf("\n5. Exit");
        
        choice = get_int_input("\nEnter your choice (1-5): ");

        switch (choice) {
            case 1:
                printf("\n--- Add Student ---\n");
                s_id = get_int_input("Enter Student ID (e.g., 101): ");
                get_string_input("Enter Student Name: ", name, sizeof(name));
                get_string_input("Enter Student Grade (e.g., A, B+): ", grade, sizeof(grade));
                insert_student(s_id, name, grade);
                break;
            
            case 2:
                printf("\n--- Find Student ---\n");
                s_id = get_int_input("Enter Student ID to find: ");
                search_student(s_id);
                break;
                
            case 3:
                printf("\n--- Remove Student ---\n");
                s_id = get_int_input("Enter Student ID to remove: ");
                delete_student(s_id);
                break;

            case 4:
                display_all_students();
                break;

            case 5:
                printf("\nExiting Student Management System. Goodbye!\n");
                // Cleanup memory (important in C)
                for (int i = 0; i < TABLE_SIZE; i++) {
                    Node *current = hashTable[i];
                    while (current != NULL) {
                        Node *temp = current;
                        current = current->next;
                        free(temp);
                    }
                    hashTable[i] = NULL;
                }
                return;

            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    }
}

int main() {
    main_menu();
    return 0;
}
