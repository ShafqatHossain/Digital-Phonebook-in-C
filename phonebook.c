#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "phonebook.h"


void addContact(Contact **contacts, int *count, int *capacity) {
    if (*count >= *capacity) {
        *capacity = (*capacity == 0) ? INITIAL_CAPACITY : *capacity * 2;
        *contacts = (Contact *)realloc(*contacts, *capacity * sizeof(Contact));
        if (!*contacts) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }
    Contact newContact;
    printf("Enter name: ");
    fgets(newContact.name, MAX_NAME, stdin);
    newContact.name[strcspn(newContact.name, "\n")] = 0;
    if (strlen(newContact.name) == 0) {
        printf("Name cannot be empty.\n");
        return;
    }
    printf("Enter phone number: ");
    fgets(newContact.phone, MAX_PHONE, stdin);
    newContact.phone[strcspn(newContact.phone, "\n")] = 0;
    printf("Enter email: ");
    fgets(newContact.email, MAX_EMAIL, stdin);
    newContact.email[strcspn(newContact.email, "\n")] = 0;

    if (!validatePhone(newContact.phone) || !validateEmail(newContact.email)) {
        printf("Invalid phone or email. Contact not added.\n");
        return;
    }

    (*contacts)[*count] = newContact;
    (*count)++;
    printf("Contact added successfully!\n");
}

void saveContacts(Contact *contacts, int count) {
    FILE *file = fopen("phonebook.txt", "w");
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s\n", contacts[i].name, contacts[i].phone, contacts[i].email);
    }
    fclose(file);
}

void loadContacts(Contact **contacts, int *count, int *capacity) {
    FILE *file = fopen("phonebook.txt", "r");
    if (!file) {
        return;
    }
    *count = 0;
    *capacity = INITIAL_CAPACITY;
    *contacts = (Contact *)malloc(*capacity * sizeof(Contact));
    if (!*contacts) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    Contact temp;
    while (fscanf(file, "%[^,],%[^,],%[^\n]\n",
           temp.name, temp.phone, temp.email) == 3) {
        if (*count >= *capacity) {
            *capacity *= 2;
            *contacts = (Contact *)realloc(*contacts, *capacity * sizeof(Contact));
            if (!*contacts) {
                printf("Memory allocation failed!\n");
                exit(1);
            }
        }
        (*contacts)[*count] = temp;
        (*count)++;
    }
    fclose(file);
}


void searchByName(Contact *contacts, int count) {
    if (count == 0) {
        printf("Phonebook is empty.\n");
        return;
    }
    char searchName[MAX_NAME];
    printf("Enter name to search: ");
    fgets(searchName, MAX_NAME, stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    int found = 0;
    printf("\nSearch Results:\n");
    printf("----------------------------------------\n");
    for (int i = 0; i < count; i++) {
        if (strcasecmp(contacts[i].name, searchName) == 0) {
            printf("Name: %s\nPhone: %s\nEmail: %s\n",
                   contacts[i].name, contacts[i].phone, contacts[i].email);
            printf("----------------------------------------\n");
            found = 1;
        }
    }
    if (!found) {
        printf("No contacts found with name '%s'.\n", searchName);
    }
}

void searchByPhone(Contact *contacts, int count) {
    if (count == 0) {
        printf("Phonebook is empty.\n");
        return;
    }
    char searchPhone[MAX_PHONE];
    printf("Enter phone number to search: ");
    fgets(searchPhone, MAX_PHONE, stdin);
    searchPhone[strcspn(searchPhone, "\n")] = 0;

    int found = 0;
    printf("\nSearch Results:\n");
    printf("----------------------------------------\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(contacts[i].phone, searchPhone) == 0) {
            printf("Name: %s\nPhone: %s\nEmail: %s\n",
                   contacts[i].name, contacts[i].phone, contacts[i].email);
            printf("----------------------------------------\n");
            found = 1;
        }
    }
    if (!found) {
        printf("No contacts found with phone '%s'.\n", searchPhone);
    }
}


int validatePhone(char phone[]) {
    int len = strlen(phone);
    if (len == 0 || len > 12) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

int validateEmail(char email[]) {
    if (strlen(email) == 0) return 0;
    char *domains[] = {"@gmail.com", "@yahoo.com", "@outlook.com"};
    int valid = 0;
    for (int i = 0; i < 3; i++) {
        char *domainPos = strstr(email, domains[i]);
        if (domainPos && strlen(domainPos) == strlen(domains[i])) {
            valid = 1;
            break;
        }
    }
    return valid;
}

void sortContacts(Contact *contacts, int count) {
    if (count == 0) {
        printf("Phonebook is empty.\n");
        return;
    }
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcasecmp(contacts[j].name, contacts[j + 1].name) > 0) {
                Contact temp = contacts[j];
                contacts[j] = contacts[j + 1];
                contacts[j + 1] = temp;
            }
        }
    }
    saveContacts(contacts, count);
    printf("Contacts sorted and saved.\n");
}

unsigned long simpleHash(char *str) {
    unsigned long hash = 0;
    for (int i = 0; str[i]; i++) {
        hash += str[i];
    }
    return hash;
}

void loadUsers(User users[], int *userCount) {
    FILE *file = fopen("users.txt", "r");
    if (!file) {

        if (*userCount < MAX_USERS) {
            strcpy(users[*userCount].username, "admin");
            unsigned long hash = simpleHash("admin123");
            snprintf(users[*userCount].passwordHash, MAX_PASSWORD, "%lu", hash);
            users[*userCount].isAdmin = 1;
            (*userCount)++;
            saveUsers(users, *userCount);
        }
        return;
    }
    *userCount = 0;
    while (*userCount < MAX_USERS && fscanf(file, "%[^,],%[^,],%d\n",
           users[*userCount].username, users[*userCount].passwordHash,
           &users[*userCount].isAdmin) == 3) {
        (*userCount)++;
    }
    fclose(file);
}

void saveUsers(User users[], int userCount) {
    FILE *file = fopen("users.txt", "w");
    if (!file) {
        printf("Error opening users file!\n");
        return;
    }
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s,%s,%d\n",
                users[i].username, users[i].passwordHash, users[i].isAdmin);
    }
    fclose(file);
}

int authenticateUser(User users[], int userCount, char *username, char *password, int *isAdmin) {
    if (strlen(username) == 0 || strlen(password) == 0) return 0;
    unsigned long hash = simpleHash(password);
    char hashStr[MAX_PASSWORD];
    snprintf(hashStr, MAX_PASSWORD, "%lu", hash);
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].passwordHash, hashStr) == 0) {
            *isAdmin = users[i].isAdmin;
            return 1;
        }
    }
    return 0;
}

void addUser(User users[], int *userCount) {
    if (*userCount >= MAX_USERS) {
        printf("User limit reached!\n");
        return;
    }
    User newUser;
    printf("Enter new username: ");
    fgets(newUser.username, MAX_USERNAME, stdin);
    newUser.username[strcspn(newUser.username, "\n")] = 0;
    if (strlen(newUser.username) == 0) {
        printf("Username cannot be empty.\n");
        return;
    }


    for (int i = 0; i < *userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("Username already exists!\n");
            return;
        }
    }

    printf("Enter password: ");
    char password[MAX_PASSWORD];
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = 0;
    if (strlen(password) == 0) {
        printf("Password cannot be empty.\n");
        return;
    }

    printf("Is this an admin? (1 for yes, 0 for no): ");
    scanf("%d", &newUser.isAdmin);
    getchar();
    if (newUser.isAdmin != 0 && newUser.isAdmin != 1) {
        printf("Invalid role. Use 1 for admin, 0 for user.\n");
        return;
    }

    unsigned long hash = simpleHash(password);
    snprintf(newUser.passwordHash, MAX_PASSWORD, "%lu", hash);

    users[*userCount] = newUser;
    (*userCount)++;
    saveUsers(users, *userCount);
    printf("User added successfully!\n");
}

void deleteContact(Contact **contacts, int *count) {
    if (*count == 0) {
        printf("Phonebook is empty.\n");
        return;
    }
    char searchPhone[MAX_PHONE];
    printf("Enter phone number of contact to delete: ");
    fgets(searchPhone, MAX_PHONE, stdin);
    searchPhone[strcspn(searchPhone, "\n")] = 0;

    int foundIndex = -1;
    for (int i = 0; i < *count; i++) {
        if (strcmp((*contacts)[i].phone, searchPhone) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("No contact found with phone '%s'.\n", searchPhone);
        return;
    }


    for (int i = foundIndex; i < *count - 1; i++) {
        (*contacts)[i] = (*contacts)[i + 1];
    }
    (*count)--;


    saveContacts(*contacts, *count);
    printf("Contact deleted successfully!\n");
}

void adminPanel(Contact **contacts, int *count, int *capacity, User users[], int *userCount) {
    int choice;
    do {
        printf("\nAdmin Panel\n");
        printf("1. Add Contact\n2. Search by Name\n3. Search by Phone\n");
        printf("4. Sort and Save\n5. View All Contacts\n6. Add New User\n");
        printf("7. Delete Contact\n8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                addContact(contacts, count, capacity);
                break;
            case 2:
                searchByName(*contacts, *count);
                break;
            case 3:
                searchByPhone(*contacts, *count);
                break;
            case 4:
                sortContacts(*contacts, *count);
                break;
            case 5:
                if (*count == 0) {
                    printf("Phonebook is empty.\n");
                } else {
                    printf("\nAll Contacts:\n");
                    printf("----------------------------------------\n");
                    for (int i = 0; i < *count; i++) {
                        printf("Name: %s\nPhone: %s\nEmail: %s\n",
                               (*contacts)[i].name, (*contacts)[i].phone, (*contacts)[i].email);
                        printf("----------------------------------------\n");
                    }
                }
                break;
            case 6:
                addUser(users, userCount);
                break;
            case 7:
                deleteContact(contacts, count);
                break;
            case 8:
                printf("Exiting Admin Panel...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 8);
}

void userPanel(Contact **contacts, int *count, int *capacity) {
    int choice;
    do {
        printf("\nUser Panel\n");
        printf("1. Add Contact\n2. Search by Name\n3. Search by Phone\n4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                addContact(contacts, count, capacity);
                break;
            case 2:
                searchByName(*contacts, *count);
                break;
            case 3:
                searchByPhone(*contacts, *count);
                break;
            case 4:
                printf("Exiting User Panel...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 4);
}

void freeContacts(Contact **contacts) {
    if (*contacts) {
        free(*contacts);
        *contacts = NULL;
    }
}
