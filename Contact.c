#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"

#define GREEN   "\033[0;32m"
#define RED   "\033[0;31m"
#define YELLOW "\033[0;33m"
#define RESET   "\033[0m"

// INITIALIZE CONTACT
void initialize(AddressBook *addressBook)
{
    FILE *fp = fopen("contacts.txt", "r");
    
    if (fp == NULL) 
    {
        printf("No existing contacts file found. Starting fresh.\n");
        addressBook->contactCount = 0;
        return;
    }

    addressBook->contactCount = 0;
    char line[100];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *ptr;

        if ((ptr = strchr(line, '\n')) != NULL) // remove newline at end
        {
            *ptr = '\0';
        } 

        Contact c;
        // Parse line into name, phone, email
        if (sscanf(line, "%49[^,],%19[^,],%49[^\n]", c.name, c.phone, c.email) == 3) 
        {
            addressBook->contacts[addressBook->contactCount++] = c;
        }
    }

    fclose(fp);
}

// LIST CONTACT
void listContacts(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0) 
    {
        printf("No contacts available.\n");
        return;
    }

    printf("-----------------------------------------------------------------------\n");
    printf("S.No   %-20s           %-15s  %-30s\n", "Name", "Phone", "Email");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        printf("%-6d %-30s %-15s %-30s\n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    printf("-----------------------------------------------------------------------\n");
}

// CREATE CONTACT
int isValidName(const char *name) 
{
    // Check if first character is a space
    if (name[0] == ' ') 
    {
        return 0; // invalid if starts with space
    }

    for (int i = 0; name[i] != '\0'; i++) 
    {
        if ( !( (name[i] >= 'A' && name[i] <= 'Z') ||
               (name[i] >= 'a' && name[i] <= 'z')))
        {
            return 0; //invalid character found
        }
    }
    return 1; //valid name
}

int isValidPhone(const char *phone) 
{
    if (strlen(phone) != 10) 
    return 0;

    for (int i = 0; phone[i] != '\0'; i++) 
    {
        if (!(phone[i] >= '0' && phone[i] <= '9')) 
        return 0;
    }
    return 1;
}

int isDuplicatePhone(AddressBook *addressBook, const char *phone) 
{
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0) 
        {
            return 1; // duplicate found
        }
    }
    return 0; // no duplicate
}

int isValidEmail(const char *email)
{
    // Must contain '@'
    char *at = strchr(email, '@');
    if (!at) 
       return 0;

    // Must contain only one '@'
    if (strchr(at + 1, '@')) 
        return 0;

    // Must contain ".com"
    if (!strstr(email, ".com"))
       return 0;

    // Must have at least one char after '@'
    if (*(at+1) == '\0')
       return 0;

    // Must be all lowercase
    for (int i = 0; email[i] != '\0'; i++) 
    {
        if (email[i] >= 'A' && email[i] <= 'Z') 
            return 0;
    }

    return 1;
}

void createContact(AddressBook *addressBook)
{
    if (addressBook->contactCount >= MAX_CONTACTS)
    {
        printf(RED "AddressBook full!\n" RESET);
        return;
    }

    Contact newContact;
    char temp_name[30], temp_phone[10], temp_email[30];

    // Name with 3 chances
    int attempts = 0, valid = 0;
    while (attempts < 3) 
    {
        printf("Enter name: ");
        scanf(" %[^\n]", temp_name);

        if (isValidName(temp_name)) 
        { 
              // Check duplicate name
            int duplicate = 0;
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, temp_name) == 0)
                {
                    duplicate = 1;
                    break;
                }
            }
            if (duplicate)
            {
                printf(RED "Name already exists. Try again.\n" RESET);
                attempts++;
            }
            else
            {
               valid = 1; 
               break; 
            }
        }
        else 
        { 
            printf(RED "Invalid name. Try again.\n" RESET); 
            attempts++; 
        }
    }
    if (!valid) 
    { 
        printf(RED "Too many invalid attempts.\n" RESET);
        return; 
    }
    strcpy(newContact.name, temp_name);

    // Phone with 3 chances
    attempts = 0; valid = 0;
    while (attempts < 3) 
    {
        printf("Enter phone number: ");
        scanf("%s", temp_phone);

        if (isValidPhone(temp_phone)) 
        {
           if (isDuplicatePhone(addressBook, temp_phone)) 
           {
               printf(RED "Phone number already exists. Try again.\n" RESET);
               attempts++;
           } 
           else
           { 
               valid = 1; 
               break; 
           }
        }
        else 
        { 
            printf(RED "Invalid phone. Try again.\n" RESET); 
            attempts++; 
        }
    }
    if (!valid) 
    { 
        printf(RED "Too many invalid attempts.\n" RESET); 
        return; 
    }
    strcpy(newContact.phone, temp_phone);

    // Email with 3 chances
    attempts = 0; valid = 0;
    while (attempts < 3) 
    {
        printf("Enter email: ");
        scanf("%s", temp_email);

        if (isValidEmail(temp_email)) 
        { 
            // Check duplicate email
            int duplicate = 0;
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].email, temp_email) == 0)
                {
                    duplicate = 1;
                    break;
                }
            }
            if (duplicate)
            {
                printf(RED "Email already exists. Try again.\n" RESET);
                attempts++;
            }
            else
            {
                valid = 1; 
                 break;
            } 
        }
        else 
        { 
            printf(RED "Invalid email. Try again.\n" RESET);
            attempts++; 
        }
    }
    if (!valid) 
    {
        printf(RED "Too many invalid attempts.\n" RESET);
        return; 
    }
    strcpy(newContact.email, temp_email);

    // Store
    addressBook->contacts[addressBook->contactCount++] = newContact;
    printf(GREEN "Contact created successfully!\n" RESET);
}

// SEARCH CONTACT
void PrintContactHeader() 
{
    printf("-------------------------------------------------------------\n");
    printf("%-5s %-20s %-15s %-25s\n", "No.", "Name", "Phone", "Email");
    printf("-------------------------------------------------------------\n");
}

void printWithHighlight(const char *text, const char *search) 
{
    int len = strlen(text);
    int searchLen = strlen(search);

    for (int i = 0; i < len; i++) 
    {
        if (strncasecmp(&text[i], search, searchLen) == 0) 
        {
            printf(YELLOW "%.*s" RESET, searchLen, &text[i]);
            i += searchLen - 1;
        } 
        else 
        {
            putchar(text[i]);
        }
    }
}

void PrintSearchResult(Contact c, int serial, const char *search, int option) 
{
    printf("%-5d ", serial);

    if (option == 1) 
    { 
        // highlight only in name
        printWithHighlight(c.name, search);
        int nameLen = strlen(c.name);
        for (int j = nameLen; j < 20; j++) 
        putchar(' ');
        printf("%-15s %-25s\n", c.phone, c.email);
    }
    else if (option == 2) 
    { 
        // highlight only in phone
        printf("%-20s ", c.name);
        printWithHighlight(c.phone, search);
        int phoneLen = strlen(c.phone);
        for (int j = phoneLen; j < 15; j++) 
        putchar(' ');
        printf("%-25s\n", c.email);
    }
    else if (option == 3) 
    { 
        // highlight only in email
        printf("%-20s %-15s ", c.name, c.phone);
        printWithHighlight(c.email, search);
        int emailLen = strlen(c.email);
        for (int j = emailLen; j < 25; j++) 
        putchar(' ');
        printf("\n");
    }
}

void searchContact(AddressBook *addressBook)
{
    int option;
    char search[50];
    int found = 0, serial = 1;

    printf("Search by:\n1. Name\n2. Phone\n3. Email\nEnter option: ");
    scanf("%d", &option);

    printf("Enter search term: ");
    scanf(" %[^\n]", search);

    PrintContactHeader();

    for (int i=0; i<addressBook->contactCount; i++)
    {
        Contact c = addressBook->contacts[i];
        int match = 0;

        switch (option) 
        {
            case 1: // Search by Name
                if (strstr(c.name, search)) 
                    match = 1; 
                    break;

            case 2: // Search by phone
               if (strstr(c.phone, search))
                   match = 1;
                   break;

            case 3: // Search by Email
                if (strstr(c.email, search))
                   match = 1; 
                   break;

            default:
                printf(RED "Invalid option.\n" RESET);
            return;
        }   
        
        if (match) 
        {
            // print only matching contacts, with sequential serial number
            PrintSearchResult(c, serial++, search, option);
            found = 1;
        }

    }

        if (!found) 
        {
           printf(RED "No matching contacts found.\n" RESET);
        } 
}

//EDIT CONATACT
void editContact(AddressBook *addressBook) 
{
    char searchName[50];
    int attempts = 0;
    int index = -1;

    // allow 3 chances to find contact
    while (attempts < 3) 
    {
        printf("Enter the name of the contact to edit: ");
        scanf(" %[^\n]", searchName);

        for (int i = 0; i < addressBook->contactCount; i++) 
        {
            if (strcmp(addressBook->contacts[i].name, searchName) == 0)
             {
                index = i;
                break;
            }
        }

        if (index != -1) 
            break; // found
        printf(RED "Contact not found. Try again.\n" RESET);
        attempts++;
    }

    if (index == -1) 
    {
        printf(RED "Maximum attempts reached. Returning to main menu.\n" RESET);
        return;
    }

    // Begin editing
    Contact *c = &addressBook->contacts[index];
    char newName[50], newPhone[15], newEmail[50];

    printf("Enter new name: ");
    scanf(" %[^\n]", newName);

    //Function to chech valid name or not
    if (!isValidName(newName)) 
    {
        printf(RED "Invalid name. Returning to main menu.\n" RESET);
        return;
    }

    printf("Enter new phone: ");
    scanf(" %[^\n]", newPhone);

    //Function to chech valid phone number or not
    if (!isValidPhone(newPhone)) 
    {
        printf(RED "Invalid phone. Returning to main menu.\n" RESET);
        return;
    }

    printf("Enter new email: ");
    scanf(" %[^\n]", newEmail);

    //Function to chech valid email or not
    if (!isValidEmail(newEmail)) 
    {
        printf(RED "Invalid email. Returning to main menu.\n" RESET);
        return;
    }

    // Check if same data entered again
    if (strcmp(c->name, newName) == 0 &&
        strcmp(c->phone, newPhone) == 0 &&
        strcmp(c->email, newEmail) == 0) 
    {
        printf(RED "Entered data is same as existing. Edit aborted.\n" RESET);
        return;
    }

    // Check for duplicate phone/email in other contacts
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (i != index) 
        {
            if (strcmp(addressBook->contacts[i].phone, newPhone) == 0) 
            {
                printf(RED "Phone already exists in another contact. Edit aborted.\n" RESET);
                return;
            }
            if (strcmp(addressBook->contacts[i].email, newEmail) == 0) 
            {
                printf(RED "Email already exists in another contact. Edit aborted.\n" RESET);
                return;
            }
        }
    }

    // Update contact
    strcpy(c->name, newName);
    strcpy(c->phone, newPhone);
    strcpy(c->email, newEmail);

    printf(GREEN "Contact updated successfully.\n" RESET);
}

// DELETE CONTACT
void deleteContact(AddressBook *addressBook) 
{
    char deleteName[100];
    int found = -1;
    int attempts = 0;

    while (attempts < 3) 
    {
        printf("Enter name to delete: ");
        scanf(" %[^\n]", deleteName);

        // Search for contact
        for (int i = 0; i < addressBook->contactCount; i++) 
        {
            if (strcmp(addressBook->contacts[i].name, deleteName) == 0) 
            {
                found = i;
                break;
            }
        }

        if (found == -1) 
        {
            printf(RED "Contact not found. Try again!\n" RESET);
            attempts++;
        } 
        else 
        {
            // Shift contacts left
            for (int i = found; i < addressBook->contactCount - 1; i++) 
            {
                addressBook->contacts[i] = addressBook->contacts[i + 1];
            }
            addressBook->contactCount--;
            printf(GREEN "Contact '%s' deleted successfully.\n" RESET, deleteName);
            return; //Exits, goes back to main menu

        }
    }

    printf(RED "Failed to delete contact after 3 attempts.\n" RESET);
}

//SAVE CONTACT
void saveContacts(AddressBook *addressBook)
{
    FILE *fp = fopen("contacts.txt", "w");

    if (fp == NULL) 
    {
        printf(RED "Error: Unable to open contacts.txt for writing.\n" RESET);
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        Contact c = addressBook->contacts[i];
        fprintf(fp, "%s,%s,%s\n", c.name, c.phone, c.email);
    }

    fclose(fp);
    printf(GREEN "Contacts saved successfully.\n" RESET);

}