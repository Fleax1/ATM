#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            &r.id,
            &u.id,
            &u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void updateAccInfos(struct User u) {
    int accountNumber;
    struct Record r;
    FILE *ptr;
    char userName[50];
    int option;

    ptr = fopen(RECORDS, "r+");

    system("clear");
    printf("\t\t====== Update Account Information =====\n");

    printf("Enter the account number to update (-1 to go back): ");
    scanf("%d", &accountNumber);

    if (accountNumber == -1) {
        fclose(ptr);
        mainMenu(u); // Retour à la page précédente (menu principal)
        return;
    }

    while (getAccountFromFile(ptr, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNumber) {
            // Account found, prompt for new information
            printf("\nEnter the updated country: ");
            scanf("%s", r.country);
            printf("Enter the updated phone number: ");
            scanf("%d", &r.phone);
            printf("Enter the updated account type: ");
            scanf("%s", r.accountType);

            // Move the file pointer back to the beginning of the record
            fseek(ptr, -sizeof(struct Record), SEEK_CUR);

            // Write the updated record to the file
            saveAccountToFile(ptr, u, r);

            fclose(ptr);
            success(u);
            return;
        }
    }

    fclose(ptr);

    system("clear");
    printf("\n✖ Account not found!\n");

invalid:
    printf("\nEnter 1 to try again, 2 to go back to the previous page, or 3 to exit: ");
    scanf("%d", &option);

    if (option == 1) {
        updateAccInfos(u); // Retry
    } else if (option == 2) {
        mainMenu(u); // Retour à la page précédente (menu principal)
    } else if (option == 3) {
        exit(0); // Quitter le programme
    } else {
        printf("Insert a valid option!\n");
        goto invalid;
    }
}

