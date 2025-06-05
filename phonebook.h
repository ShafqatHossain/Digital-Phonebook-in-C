#ifndef PHONEBOOK_H
#define PHONEBOOK_H
#define MAX_NAME 50
#define MAX_PHONE 13
#define MAX_EMAIL 100
#define MAX_USERNAME 20
#define MAX_PASSWORD 20
#define INITIAL_CAPACITY 10
#define MAX_USERS 50

typedef struct {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
} Contact;

typedef struct {
    char username[MAX_USERNAME];
    char passwordHash[MAX_PASSWORD];
    int isAdmin;
} User;


void addContact(Contact **contacts, int *count, int *capacity);
void saveContacts(Contact *contacts, int count);
void loadContacts(Contact **contacts, int *count, int *capacity);
void searchByName(Contact *contacts, int count);
void searchByPhone(Contact *contacts, int count);
int validatePhone(char phone[]);
int validateEmail(char email[]);
void sortContacts(Contact *contacts, int count);
void freeContacts(Contact **contacts);
void deleteContact(Contact **contacts, int *count);


int authenticateUser(User users[], int userCount, char *username, char *password, int *isAdmin);
void loadUsers(User users[], int *userCount);
void saveUsers(User users[], int userCount);
void addUser(User users[], int *userCount);
unsigned long simpleHash(char *str);

void adminPanel(Contact **contacts, int *count, int *capacity, User users[], int *userCount);
void userPanel(Contact **contacts, int *count, int *capacity);

#endif
