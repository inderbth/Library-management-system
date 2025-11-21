#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[50];
    char author[50];
    int quantity;

    // Added Issue Details
    char issuedToName[50];
    char issuedToRoll[20];
    char issuedToClass[20];
    int isIssued;  // 1 = issued, 0 = available
};

void addBook();
void displayBooks();
void searchBook();
void issueBook();
void returnBook();

FILE *fp;

int main() {
    int choice;

    while (1) {
        printf("\n========== Library Management System ==========\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book by ID\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: exit(0);
            default: printf("Invalid Choice! Try Again.\n");
        }
    }
    return 0;
}

void addBook() {
    struct Book bk;
    fp = fopen("library.txt", "ab");

    printf("\nEnter Book ID: ");
    scanf("%d", &bk.id);

    printf("Enter Title: ");
    getchar();
    fgets(bk.title, 50, stdin);
    bk.title[strcspn(bk.title, "\n")] = '\0';

    printf("Enter Author: ");
    fgets(bk.author, 50, stdin);
    bk.author[strcspn(bk.author, "\n")] = '\0';

    printf("Enter Quantity: ");
    scanf("%d", &bk.quantity);

    // initialize issue details
    bk.isIssued = 0;
    strcpy(bk.issuedToName, "");
    strcpy(bk.issuedToRoll, "");
    strcpy(bk.issuedToClass, "");

    fwrite(&bk, sizeof(bk), 1, fp);
    fclose(fp);

    printf("Book Added Successfully!\n");
}

void displayBooks() {
    struct Book bk;
    fp = fopen("library.txt", "rb");

    if (fp == NULL) {
        printf("No books found!\n");
        return;
    }

    printf("\n--- All Books ---\n");
    while (fread(&bk, sizeof(bk), 1, fp)) {
        printf("\nID: %d\n", bk.id);
        printf("Title: %s\n", bk.title);
        printf("Author: %s\n", bk.author);
        printf("Quantity: %d\n", bk.quantity);

        if (bk.isIssued == 1) {
            printf("Status: Issued\n");
            printf("Issued To: %s\n", bk.issuedToName);
            printf("Roll/ID: %s\n", bk.issuedToRoll);
            printf("Class: %s\n", bk.issuedToClass);
        } else {
            printf("Status: Available\n");
        }
    }

    fclose(fp);
}

void searchBook() {
    struct Book bk;
    int id, found = 0;

    fp = fopen("library.txt", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Book ID to Search: ");
    scanf("%d", &id);

    while (fread(&bk, sizeof(bk), 1, fp)) {
        if (bk.id == id) {
            printf("\nBook Found!\n");
            printf("ID: %d\n", bk.id);
            printf("Title: %s\n", bk.title);
            printf("Author: %s\n", bk.author);
            printf("Quantity: %d\n", bk.quantity);

            if (bk.isIssued == 1) {
                printf("Status: Issued\n");
                printf("Issued To: %s\n", bk.issuedToName);
                printf("Roll/ID: %s\n", bk.issuedToRoll);
                printf("Class: %s\n", bk.issuedToClass);
            } else {
                printf("Status: Available\n");
            }

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Book not found!\n");

    fclose(fp);
}

void issueBook() {
    struct Book bk;
    int id, found = 0;

    fp = fopen("library.txt", "rb+");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Book ID to Issue: ");
    scanf("%d", &id);

    while (fread(&bk, sizeof(bk), 1, fp)) {
        if (bk.id == id) {
            found = 1;

            if (bk.quantity > 0 && bk.isIssued == 0) {
                bk.quantity--;
                bk.isIssued = 1;

                getchar();
                printf("Enter Student Name: ");
                fgets(bk.issuedToName, 50, stdin);
                bk.issuedToName[strcspn(bk.issuedToName, "\n")] = '\0';

                printf("Enter Student Roll/ID: ");
                fgets(bk.issuedToRoll, 20, stdin);
                bk.issuedToRoll[strcspn(bk.issuedToRoll, "\n")] = '\0';

                printf("Enter Class: ");
                fgets(bk.issuedToClass, 20, stdin);
                bk.issuedToClass[strcspn(bk.issuedToClass, "\n")] = '\0';

                fseek(fp, -sizeof(bk), SEEK_CUR);
                fwrite(&bk, sizeof(bk), 1, fp);

                printf("Book Issued Successfully!\n");
            } 
            else {
                printf("Book Already Issued or Out of Stock!\n");
            }
            break;
        }
    }

    if (!found)
        printf("Book not found!\n");

    fclose(fp);
}

void returnBook() {
    struct Book bk;
    int id, found = 0;

    fp = fopen("library.txt", "rb+");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Book ID to Return: ");
    scanf("%d", &id);

    while (fread(&bk, sizeof(bk), 1, fp)) {
        if (bk.id == id) {
            found = 1;

            if (bk.isIssued == 1) {
                bk.quantity++;
                bk.isIssued = 0;

                strcpy(bk.issuedToName, "");
                strcpy(bk.issuedToRoll, "");
                strcpy(bk.issuedToClass, "");

                fseek(fp, -sizeof(bk), SEEK_CUR);
                fwrite(&bk, sizeof(bk), 1, fp);

                printf("Book Returned Successfully!\n");
            } 
            else {
                printf("This Book Was Not Issued!\n");
            }
            break;
        }
    }

    if (!found)
        printf("Book not found!\n");

    fclose(fp);
}
