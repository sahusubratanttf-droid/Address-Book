#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[MAX_CONTACTS]; //contacts array of 100 where in each address i can store contact(info) of each person saved
    int contactCount;
} AddressBook;

int isValidName(const char *name);
int isValidPhone(const char *phone);
int isValidEmail(const char *email);
int isDuplicatePhone(AddressBook *addressBook, const char *phone);
void createContact(AddressBook *addressBook); //addressbook is a variable where contact is stored

void PrintContactHeader();
void PrintSearchResult(Contact c, int serial, const char *search, int option);
void printWithHighlight(const char *text, const char *search);  
void searchContact(AddressBook *addressBook); //Search contact

void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveContacts(AddressBook *AddressBook);
void Exit(AddressBook *addressBook);

#endif