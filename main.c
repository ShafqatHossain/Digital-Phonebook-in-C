#include <stdio.h>
#include "phonebook.h"
#include <string.h>

int main() {
    Contact *contacts = NULL;
    int count = 0;
    int capacity = 0;
    User users[MAX_USERS];
    int userCount = 0;

    loadContacts(&contacts, &count, &capacity);
    loadUsers(users, &userCount);

    int choice;
    do {
        printf("\nDigital Phonebook\n");
        printf("1. Admin Panel\n2. User Panel\n3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1 || choice == 2) {
            char username[MAX_USERNAME], password[MAX_PASSWORD];
            int isAdmin = 0, attempts = 3;
            int authenticated = 0;

            while (attempts > 0 && !authenticated) {
                printf("Enter username: ");
                fgets(username, MAX_USERNAME, stdin);
                username[strcspn(username, "\n")] = 0;
                printf("Enter password: ");
                fgets(password, MAX_PASSWORD, stdin);
                password[strcspn(password, "\n")] = 0;

                authenticated = authenticateUser(users, userCount, username, password, &isAdmin);
                if (!authenticated) {
                    attempts--;
                    printf("Invalid credentials. %d attempts remaining.\n", attempts);
                }
            }

            if (!authenticated) {
                printf("Too many failed attempts. Access denied.\n");
                continue;
            }

            if (choice == 1 && !isAdmin) {
                printf("Admin access required!\n");
                continue;
            }

            if (choice == 1) {
                adminPanel(&contacts, &count, &capacity, users, &userCount);
            } else {
                userPanel(&contacts, &count, &capacity);
            }
        } else if (choice == 3) {
            printf("Exiting...\n");
            saveContacts(contacts, count);
            freeContacts(&contacts);
        } else {
            printf("Invalid choice!\n");
        }
    } while (choice != 3);

    return 0;
}
