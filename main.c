#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ACCOUNTS 1000
// Colors
#define RESET         "\x1b[0m"
#define RED           "\x1b[31m"
#define GREEN         "\x1b[32m"
#define YELLOW        "\x1b[33m"
#define BLUE          "\x1b[34m"
#define CYAN          "\x1b[36m"
#define WHITE         "\x1b[37m"
#define hhh            "\e[0;31m"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char accNumber[11];
    char name[50];
    char email[50];
    char mobile[15];
    double balance;
    Date opened;
    char status[20];
    double dailywithdrawal;
    int lastwithdrawdate;
} Account;

/* ===================== GLOBALS ===================== */
Account accounts[MAX_ACCOUNTS];
int accCount = 0;

/* ===================== FUNCTION PROTOTYPES ===================== */
void startSystem();
void initialMenu();
void mainMenu();
int login();
void loadAccounts();
void saveAccounts();
int findAccount(char acc[]);
void printAccount(Account a);
void searchAccount();
void advancedSearch();
void addAccount();
void deleteAccount();
void modifyAccount();
void changeStatus();
void withdrawMoney();
void depositMoney();
void transferMoney();
void deleteMultiple();
void report();
void printSorted();
int isValidEmail(const char *email);
int isValidAccountNumber(const char *acc);
//void getsystemdate(int *d, int *m);
int isValidFloat(const char *str);
int isValidInteger(const char *str);



/* ===================== MAIN ===================== */
int main() {
    startSystem();
    return 0;
}

/* ===================== SYSTEM CONTROL ===================== */
void startSystem() {
    int choice;
    printf( YELLOW "\t\t\t\t   \t  ___________________________________\n" RESET) ;
    printf( hhh   " \t\t\t\t\t\t     BANKING SYSTEM    \t\n" RESET );
    printf( YELLOW "\t\t\t\t   \t  ___________________________________\n" RESET);

    do{
        initialMenu();
            scanf("%d", &choice);

        if (choice == 1) {
            if (login()) {
                loadAccounts();
                mainMenu();
            } else {
                printf(RED "Login failed!\n" RESET);
            }
        } else if (choice == 2) {
            printf("Thanks for using!");
            exit(0);
        }
          else {

            printf(RED "Invalid choice!...Please choose" WHITE "1" RED "or" WHITE "2" RED "only.\n" RESET);
          }

    }while(choice != 2);
}


void initialMenu() {
    printf( CYAN  "\n\t\t\t\t\t\t---   Intial Menu   ---\t\n" RESET );
    printf( GREEN "\n1. " WHITE "LOGIN\n" RED "2. " WHITE "QUIT\n" YELLOW "Choose: " RESET);
}

void mainMenu() {
    int c; printf( CYAN  "\n\t\t\t\t\t\t---   Main Menu   ---\t\n" RESET );

    do {
       //      printf("\n 1.ADD\t\t  2.DELETE\n 3.MODIFY\t  4.SEARCH\n 5.ADV SEARCH\t  6.DELETE MULTIPLE ACCOUNTS\n");
       // printf(" 7.CHANGE STATUS  8.WITHDRAW\n 9.DEPOSIT\t  10.TRANSFER\n");
      //  printf(" 11.REPORT\t  12.PRINT\n 13.SAVE\t  0.QUIT\n\nChoice: ");


printf(CYAN " 1." WHITE " ADD ACCOUNT\t\t"   CYAN  " 2." WHITE " DELETE ACCOUNT\n");
printf(CYAN " 3." WHITE " MODIFY ACCOUNT\t"  CYAN  " 4." WHITE " SEARCH ACCOUNT\n");
printf(CYAN " 5." WHITE " ADVANCED SEARCH\t" CYAN  " 6." WHITE " DELETE MULTIPLE\n");
printf(CYAN " 7." WHITE " CHANGE STATUS\t"   CYAN  " 8." WHITE " WITHDRAW\n");
printf(CYAN " 9." WHITE " DEPOSIT\t\t"       CYAN  "10." WHITE " TRANSFER\n");
printf(CYAN "11." WHITE " REPORT\t\t"        CYAN  "12." WHITE " PRINT ALL\n");
printf(CYAN "13." WHITE " SAVE\t\t"          RED   " 0." RED   " QUIT\n" RESET);
printf(YELLOW "Choice: " RESET);


        scanf("%d", &c);
        switch (c) {
            case 0: printf(GREEN "Saving all changes before exiting...\n" RESET);
                     saveAccounts();
                    printf( "YOU ARE EXITING MAIN MENU.\n"); break;
            case 1: addAccount(); break;
            case 2: deleteAccount(); break;
            case 3: modifyAccount(); break;
            case 4: searchAccount(); break;
            case 5: advancedSearch(); break;
            case 6: deleteMultiple(); break;
            case 7: changeStatus(); break;
            case 8: withdrawMoney(); break;
            case 9: depositMoney(); break;
            case 10: transferMoney(); break;
            case 11: report(); break;
            case 12: printSorted(); break;
            case 13: saveAccounts(); break;
            default: printf(RED "\nInvalid choice!....Please enter another one.\n" RESET);
        }
    } while (c != 0);
}


/* ===================== LOGIN ===================== */
int login() {
    char username[30], password[30];
    char fileuser[30], filepass[30];
    FILE *f;
    printf(YELLOW "Username: " RESET);
    scanf("%s", username);
    printf(YELLOW "Password: " RESET);
    scanf("%s", password);
    f = fopen("users.txt", "r");
    if (f != NULL) {
        while (fscanf(f, "%s %s", fileuser, filepass) != EOF) {
            if (strcmp(username, fileuser) == 0 && strcmp(password, filepass) == 0) {
                fclose(f);
                return 1;
            }
        }
        fclose(f);
    }
    return 0;
}

/* ===================== FILE HANDLING ===================== */

void loadAccounts() {
    FILE *f = fopen("accounts.txt", "r");
    accCount = 0;

    if (f == NULL) {
        printf("No accounts file found. Starting with empty database.\n");
        return;
    }

    char line[1024];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    while (fgets(line, sizeof(line), f)) {

        line[strcspn(line, "\n")] = 0;

        Account temp;

        int items = sscanf(line, "%10[^,],%49[^,],%49[^,],%lf,%14[^,],%d-%d, %19[^,],%d,%lf",
                           temp.accNumber,
                           temp.name,
                           temp.email,
                           &temp.balance,
                           temp.mobile,
                           &temp.opened.month,
                           &temp.opened.year,
                           temp.status,
                           &temp.lastwithdrawdate,
                           &temp.dailywithdrawal);

        if (items >= 8) {
            if (items == 8) {
                temp.lastwithdrawdate = tm.tm_mday;
                temp.dailywithdrawal = 0;
            } else if (items == 9){

            temp.dailywithdrawal = 0;

            }




            if (temp.lastwithdrawdate != tm.tm_mday) {
                temp.dailywithdrawal =0;
                temp.lastwithdrawdate = tm.tm_mday;
            }

            accounts[accCount] = temp;
            accCount++;

            if (accCount >= MAX_ACCOUNTS) {
                printf(BLUE "Maximum number of accounts reached!\n" RESET);
                break;
            }
        }
    }

    fclose(f);
    printf(GREEN "Successfully loaded %d account(s).\n" RESET , accCount);
}





void saveAccounts() {
    FILE *f = fopen("accounts.txt", "w");
    int i;

    if (f == NULL) {
        printf(RED "Error opening file for writing\n" RESET);
        return;
    }

    for (i = 0; i < accCount; i++) {
        fprintf(f,
            "%s,%s,%s,%.2lf,%s,%d-%d, %s,%d,%.2lf\n",
            accounts[i].accNumber,
            accounts[i].name,
            accounts[i].email,
            accounts[i].balance,
            accounts[i].mobile,
            accounts[i].opened.month,
            accounts[i].opened.year,
            accounts[i].status,
            accounts[i].lastwithdrawdate,
            accounts[i].dailywithdrawal
        );
    }

    fclose(f);
    printf( GREEN "All accounts saved successfully!\n" RESET);
}

/* ===================== CORE FUNCTIONS ===================== */
int findAccount(char acc[]) {
    int i;
    for (i = 0; i < accCount; i++) {
        if (strcmp(accounts[i].accNumber, acc) == 0)
            return i;
    }
    return -1;
}

void printAccount(Account a) {
    printf(YELLOW "Account Number" RESET " : %s\n", a.accNumber);
    printf(YELLOW "Name          " RESET " : %s\n", a.name);
    printf(YELLOW "Email         " RESET " : %s\n", a.email);
    printf(YELLOW "Mobile        " RESET " : %s\n", a.mobile);
    printf(YELLOW "Balance       " RESET " : %0.2lf\n", a.balance);
    printf(YELLOW "Date Opened   " RESET " : %d-%d\n", a.opened.month, a.opened.year);
    printf(YELLOW "Status        " RESET " : %s\n", a.status);
}

void searchAccount() {
    char acc[11];
    printf( YELLOW "Enter account number : " RESET);
    scanf("%s", acc);
    if (!isValidAccountNumber(acc)){printf (RED "The number is not valid!" RESET ); return;}
    else{
    int found = findAccount(acc);
    if (found == -1)
        printf(RED "Not Found!..please try again.\n" RESET);

    else
        printAccount(accounts[found]);
}
}
/* ===================== MODIFY ===================== */
void modifyAccount() {
    char acco[11];
    int choice;
    char newName[50];
    char newEmail[100];
    char newPhone[20];


    //loadAccounts();

    printf(YELLOW "Enter account number: " RESET);
    scanf("%s", acco);
    if (!isValidAccountNumber(acco)) {printf (RED "The number is not valid!\n" RESET); return;}
    else{
          int i=findAccount(acco);
          if(i==-1) {printf(RED "Account not found!\n " RESET); return;}
            printf(GREEN "Account found!\n" RESET);
            printf(CYAN "1." RESET "Name\n" CYAN "2." RESET "Email\n" CYAN "3." RESET "Phone\n" YELLOW "Choose: " RESET);
            scanf("%d", &choice);

            if (choice == 1) {
                printf(BLUE "Enter new name : " RESET);
                scanf(" %[^\n]" , newName);
                strcpy(accounts[i].name, newName);
            } else if (choice == 2) {
               printf(BLUE "Enter new email : " RESET);
                scanf("%s", newEmail);
                if (!isValidEmail(newEmail)) {printf (RED "The email is not valid!\n" RESET); return;};
                strcpy(accounts[i].email, newEmail);
            } else if (choice == 3) {
                printf(BLUE "Enter new phone : " RESET);
                scanf("%s", newPhone);
                if (!isValidInteger(newPhone)) {printf (RED "The phone number is not valid!\n" RESET); return;}
                strcpy(accounts[i].mobile, newPhone);
            }


        }





    int cho;
    printf("Save changes? 1.Yes 2.No: ");
    scanf("%d", &cho);
    if (cho == 1){
        saveAccounts();
        printf(GREEN "Changes saved succesfully!\n" RESET);}
   else
   {
       printf(RED "Changes discarded.\n" RESET);
       loadAccounts();
   }

}


void changeStatus() {
    char acco[11];
    char newstatus[20];


   // loadAccounts();

    printf(YELLOW "Please enter account number: " RESET);
    scanf("%s", acco);
    if (!isValidAccountNumber(acco)) {printf (RED "\nInvalid Account Number!\n" RESET); return;}
    else{

     int i=findAccount(acco);
    if(i == -1){printf(RED "Acount not found!\n" RESET); return; }
            printf(GREEN "Account found!\n" RESET);
            printf(YELLOW "Current status: " RESET  "%s\n", accounts[i].status);
            printf(YELLOW "Enter new status (" RESET "active" YELLOW "/" RESET"inactive" YELLOW ") : " RESET);
            scanf("%s", newstatus);
     if (strcmp(newstatus, "active") != 0 &&
    strcmp(newstatus, "inactive") != 0) {
    printf(RED "Invalid status! Only " RESET "'active'" RED "or" RESET "'inactive'" RED "allowed.\n" RESET);

    return;
}
            if (strcmp(accounts[i].status, newstatus) == 0) {
                printf("Account is already %s!\n", accounts[i].status);
            } else {
                strcpy(accounts[i].status, newstatus);
                printf(GREEN "Account status changed successfully!\n" RESET);
            }

        }




    int cho;
    printf("Do you want to save these changes? \n");
    printf("1. Yes \t 2. No\n");
    scanf("%d", &cho);
    if (cho == 1){
        saveAccounts();
        printf(GREEN "Changes saved succesfully!\n" RESET);}
}


void withdrawMoney() {
    char acco[11];

    double amount;

time_t t = time(NULL);
struct tm tm = *localtime(&t);
    //loadAccounts();

    printf( YELLOW "Enter account number: " RESET);
    scanf("%s", acco);
    if (!isValidAccountNumber(acco)) {printf (RED "The number is not valid\n" RESET); return;}
    else{

    int i=findAccount(acco);
          if(i==-1) {printf(RED "Account not found!\n " RESET); return;}
            printf(GREEN "Account found!\n" RESET);

            do{
            printf(YELLOW "Amount: " RESET);
            scanf("%lf", &amount);

            if(amount < 0)
                printf(RED "Amount can't be negative!...Please enter a valid amount\n" RESET);

          }while(amount<0);
            if(accounts[i].lastwithdrawdate == tm.tm_mday && accounts[i].dailywithdrawal >=50000)
            {
                printf(RED "Daily withdrawl limit of 50000 exceeded for today.\n" RESET);
                return;
            }

            if (strcmp(accounts[i].status, "inactive") == 0)
                printf(RED "Account is inactive. Transaction denied.\n" RESET);
            else if (amount > 10000)
                printf(RED "Transaction denied. Maximum withdrawal per transaction is 10000\n" RESET);
            else if ( accounts[i].dailywithdrawal + amount > 50000)
                printf(BLUE "Transaction would exceed daily withdrawal limit of 50000\n" RESET);
            else if (amount > accounts[i].balance)
                printf(RED "Insufficient balance\n" RESET);
            else {
                accounts[i].balance -= amount;
                accounts[i].dailywithdrawal += amount;
                accounts[i].lastwithdrawdate = tm.tm_mday;
                printf(GREEN "Withdrawal successful\n" RESET);

                int cho;
    printf("Do you want to save these changes? \n");
    printf("1. Yes \t 2. No\n");
    scanf("%d", &cho);
    if (cho == 1)
       {

     saveAccounts();
        printf(GREEN "Changes saved succesfully!\n" RESET);}
                char filename[50];
                strcpy(filename, accounts[i].accNumber);
                strcat(filename, ".txt");
                FILE *transFile = fopen(filename, "a");
                if (transFile != NULL) {
                    fprintf(transFile, "Withdrawal: -%.2lf | %d-%d-%d\n", amount, tm.tm_mday,
 tm.tm_mon + 1,
 tm.tm_year + 1900);
                    fclose(transFile);
                }
            }


        }


}



void depositMoney() {
    char acco[11];
    double amount;

time_t t = time(NULL);
struct tm tm = *localtime(&t);

    //loadAccounts();

    printf(YELLOW "Please enter account number: " RESET);
    scanf("%s", acco);
    if (!isValidAccountNumber(acco)) {printf (RED "The number is not valid\n" RESET); return;}
    else {

   int i=findAccount(acco);
          if(i==-1) {printf(RED "Account not found!\n " RESET); return;}
            printf(GREEN "Account found!\n" RESET);

            do{
            printf(YELLOW "Amount: " RESET);
            scanf("%lf", &amount);

            if(amount < 0)
                printf(RED "Amount can't be negative!...Please enter a valid amount\n" RESET);

          }while(amount<0);

            if (strcmp(accounts[i].status, "inactive") == 0)
                printf(RED "Account is inactive. Transaction denied.\n" RESET );
            else if (amount > 10000)
                printf(RED "Transaction denied. Maximum deposit per transaction is 10000\n" RESET);
            else {
                accounts[i].balance += amount;
                printf(GREEN "Deposit successful.\n" RESET);
                int cho;
    printf("Do you want to save these changes? \n");
    printf("1. Yes \t 2. No\n");
    scanf("%d", &cho);
    if (cho == 1)
        {saveAccounts();
        printf(GREEN "Changes saved succesfully!\n");}
                char filename[50];
                strcpy(filename, accounts[i].accNumber);
                strcat(filename, ".txt");
                FILE *transFile = fopen(filename, "a");
                if (transFile != NULL) {
                    fprintf(transFile, "Deposit: +%.2lf | %d-%d-%d\n", amount, tm.tm_mday,
 tm.tm_mon + 1,
 tm.tm_year + 1900);
                    fclose(transFile);
                }
            }


        }




}


void report() {
    char acc[11], fileName[20];
    char lines[100][200];
    int count = 0;
    printf(YELLOW "Enter account number: " RESET);
    scanf("%s", acc);
    if (!isValidAccountNumber(acc)) {printf (RED "The number is not valid\n" RESET); return;}
    else{ int i=findAccount(acc);
          if(i==-1) {printf(RED "Account not found!\n " RESET ); return;}
    strcpy(fileName, acc);
    strcat(fileName, ".txt");
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        printf(BLUE "No transaction history found.\n" RESET);
        return;
    }
    while (fgets(lines[count], 200, f) != NULL) {
        count++;
    }
    fclose(f);
    printf(YELLOW "\nLast transactions:\n" RESET);
    int start;
    if (count > 5)
        start = count - 5;
    else
        start = 0;
    for (int i = start; i < count; i++) {
        printf("%s", lines[i]);
    }
}
}
void transferMoney() {
    char s[11], r[11];
    char fileS[20], fileR[20];
    double amt;
    time_t t = time(NULL);
struct tm tm = *localtime(&t);


    //loadAccounts();

    printf(YELLOW "Sender: "RESET );
    scanf("%s", s);


        if (!isValidAccountNumber(s)) {printf (RED "The number is not valid!\n" RESET); return;}
        int i=findAccount(s);
          if(i==-1) {printf(RED "Account not found!\n " RESET); return;}
        else if(strcmp(accounts[i].status, "inactive") == 0)
                printf(RED "Account is inactive. Transaction denied.\n" RESET);
        else {


        printf(YELLOW "Receiver: " RESET);
        scanf("%s", r);

        if (!isValidAccountNumber(r)) {printf (RED "The number is not valid!\n" RESET); return;}
        int j=findAccount(r);
          if(j==-1) {printf(RED "Account not found!\n " RESET); return;}
        else if (strcmp(accounts[j].status, "inactive") == 0)
                printf(RED "Account is inactive. Transaction denied.\n" RESET);
        else{




          do{
            printf(YELLOW "Amount: " RESET);
            scanf("%lf", &amt);

            if(amt < 0)
                printf(RED "Amount can't be negative!...Please enter a valid amount\n" RESET);

          }while(amt<0);
            if (accounts[i].balance < amt) {
                printf(RED "Insufficient balance\n" RESET);
                return;
            }

            accounts[i].balance -= amt;
            accounts[j].balance += amt;

            strcpy(fileS, s);
            strcat(fileS, ".txt");
            strcpy(fileR, r);
            strcat(fileR, ".txt");

            FILE *fs = fopen(fileS, "a");
            FILE *fr = fopen(fileR, "a");

            if (fs && fr) {


                fprintf(fs, "Transfer Sent: -%.2lf to %s | Balance: %.2lf | %d-%d-%d\n", amt, r, accounts[i].balance, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
                fprintf(fr, "Transfer Received: +%.2lf from %s | Balance: %.2lf | %d-%d-%d\n", amt, s, accounts[j].balance, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);


    saveAccounts();
    printf( GREEN "Transfer successful and saved!\n" RESET);


                fclose(fs);
                fclose(fr);


            }
        }
}
}
void swap(Account *a, Account *b) {
    Account temp = *a;
    *a = *b;
    *b = temp;
}

void sortByName() {
    for (int i = 0; i < accCount - 1; i++) {
        for (int j = i + 1; j < accCount; j++) {
            if (strcmp(accounts[i].name, accounts[j].name) > 0) {
                swap(&accounts[i], &accounts[j]);
            }
        }
    }
}

void sortByBalance() {
    for (int i = 0; i < accCount - 1; i++) {
        for (int j = i + 1; j < accCount; j++) {
            if (accounts[i].balance > accounts[j].balance) {
                swap(&accounts[i], &accounts[j]);
            }
        }
    }
}

void sortByDate() {
    for (int i = 0; i < accCount - 1; i++) {
        for (int j = i + 1; j < accCount; j++) {
            if (accounts[i].opened.year > accounts[j].opened.year || (accounts[i].opened.year == accounts[j].opened.year && accounts[i].opened.month > accounts[j].opened.month)) {
                swap(&accounts[i], &accounts[j]);
            }
        }
    }
}

void sortByStatus() {
    for (int i = 0; i < accCount - 1; i++) {
        for (int j = i + 1; j < accCount; j++) {
            if (strcmp(accounts[i].status, accounts[j].status) > 0) {
                swap(&accounts[i], &accounts[j]);
            }
        }
    }
}

void printSorted() {
    int choice;
    printf(YELLOW "\nSort by :\n" RESET);
    printf(CYAN "1." RESET "Name\n");
    printf(CYAN "2." RESET "Balance\n");
    printf(CYAN "3." RESET "Date Opened\n");
    printf(CYAN "4." RESET "Status\n");
    printf("Choice: ");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            sortByName();
            break;
        case 2:
            sortByBalance();
            break;
        case 3:
            sortByDate();
            break;
        case 4:
            sortByStatus();
            break;
        default:
            printf(RED "Invalid choice\n" RESET);
            return;
    }
    printf(CYAN "\n===== ACCOUNTS =====\n" RESET);
    for (int i = 0; i < accCount; i++) {
        printAccount(accounts[i]);
        printf(CYAN "--------------------\n" RESET);
    }
}

void advancedSearch() {
    char keyword[50];
    int found = 0;
    printf(YELLOW "Enter The Keyword : " RESET);
    scanf(" %[^\n]", keyword);
    for (int i = 0; i < accCount; i++) {
        if (strstr(accounts[i].name, keyword)) {
            printAccount(accounts[i]);
            found = 1;
        }
    }
    if (!found)
        printf(RED "No matches\n" RESET);
}

void addAccount() {
    time_t t;
    struct tm *tm_info;
    Account Added;
     if(accCount >= MAX_ACCOUNTS){
        printf(RED "Maximum number of accounts.\n" RESET);
        return;
     }
    printf(YELLOW "Enter Account number: " RESET);
    scanf("%s", Added.accNumber);
    if (!isValidAccountNumber(Added.accNumber))
       {printf (RED "The number is not valid!\n" RESET); return;}

    else {
    getchar();

    if (findAccount(Added.accNumber) != -1) {
        printf(BLUE "Account Number already exists!\n" RESET);
        return;
    }

    printf(YELLOW "Enter Name      : "RESET ) ;
    scanf(" %[^\n]", Added.name);
    printf(YELLOW "Enter Email     : " RESET);
    scanf("%s", Added.email);
    if(!isValidEmail(Added.email))
    {
        printf(RED"Invalid email!\n" RESET); return;
    }


    printf( YELLOW "Enter Mobile    : " RESET);
    scanf("%s", Added.mobile);
    if(!isValidInteger(Added.mobile))
    {
        printf(RED "Invalid phone number!\n" RESET); return;
    }
   do {printf(YELLOW "Initial balance : " RESET);
    scanf("%lf", &Added.balance);
    if(Added.balance < 0 )
    {
        printf(RED "Invalid balance!...Please try again.\n" RESET);
    }
   }while(Added.balance<0);
    t = time(NULL);
    tm_info = localtime(&t);
    Added.opened.month = tm_info->tm_mon + 1;
    Added.opened.year = tm_info->tm_year + 1900;
Added.lastwithdrawdate = tm_info->tm_mday;
Added.dailywithdrawal = 0;
    strcpy(Added.status, "active");

    accounts[accCount] = Added;
    accCount++;
int cho;
    printf("Do you want to save these changes? \n");
    printf("1. Yes \t 2. No\n");
    scanf("%d", &cho);

    if (cho == 1){
        saveAccounts();
    printf(GREEN "Account added successfully\n" RESET);}

}
}

void deleteAccount() {
    char acco[11];

    //loadAccounts();

    printf(YELLOW "Enter account number: " RESET);
    scanf("%s", acco);
    if (!isValidAccountNumber(acco)) {printf (RED"The number is not valid!\n" RESET); return;}
    else{

    int found = findAccount(acco);
    if (found == -1) {
        printf( RED "Not Found!\n" RESET);
        return;
    }

    if (accounts[found].balance != 0) {
        printf(BLUE "Balance not zero\n" RESET);
        return;
    }

    for (int i = found; i < accCount - 1; i++)
        accounts[i] = accounts[i + 1];

    accCount--;
    int cho;
    printf("Do you want to save these changes? \n");
    printf("1. Yes \t 2. No\n");
    scanf("%d", &cho);
    if (cho == 1){
        saveAccounts();
    printf(RED "The account is deleted!\n" RESET);}




}
}
/*void getsystemdate(int *d, int *m) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    *d = tm.tm_mday;
    *m = tm.tm_mon + 1;
}*/

void deleteMultiple() {
    int choice;
    int deleted = 0;

    printf(YELLOW "\nDELETE MULTIPLE ACCOUNTS\n" RESET);
    printf(CYAN "1." RESET "By Date (MM-YYYY)\n");
    printf(CYAN "2." RESET "Inactive > 90 days & balance = 0\n");
    printf(YELLOW "Choose: " RESET);
    scanf("%d", &choice);

    if (choice == 1) {
        int y, m;
        int found = 0;

        printf(YELLOW "Enter date (MM-YYYY): " RESET);
        scanf("%d-%d", &m, &y);

        for (int i = 0; i < accCount; ) {
            if (accounts[i].opened.year == y &&
                accounts[i].opened.month == m ) {

                found = 1;
                for (int j = i; j < accCount - 1; j++) {
                    accounts[j] = accounts[j + 1];
                }
                accCount--;
                deleted++;
            } else {
                i++;
            }
        }

        if (!found) {
            printf(RED "No account created on this date.\n" RESET);
            return;
        }

        printf(GREEN "Completed: %d account(s) deleted.\n" RESET, deleted);
        saveAccounts();
    }

    else if (choice == 2) {
        time_t t = time(NULL);
        struct tm *now = localtime(&t);

       int todaymonths =
        (now->tm_year + 1900) * 12+
        (now->tm_mon + 1);

        int found = 0;

        for (int i = 0; i < accCount; ) {
            if (strcmp(accounts[i].status, "inactive")==0 && accounts[i].balance == 0) {

                   int openedMonths =
                accounts[i].opened.year * 12 +
                accounts[i].opened.month;

            int diff = todaymonths - openedMonths;

                if (diff > 3) {
                    found = 1;
                    for (int j = i; j < accCount - 1; j++) {
                        accounts[j] = accounts[j + 1];
                    }
                    accCount--;
                    deleted++;
                    continue;
                }
            }
            i++;
        }

        if (!found) {
            printf(BLUE "No inactive accounts older than 90 days or with zero balance.\n" RESET);
            return;
        }

        printf(GREEN "Completed: %d account(s) deleted.\n" RESET, deleted);
        saveAccounts();
    }

    else {
        printf(RED "Invalid choice.\n" RESET);
    }
}

int isValidAccountNumber(const char *acc) {
    if (strlen(acc) != 10)
        return 0;
    for (int i = 0; i < 10; i++) {
        if (!isdigit(acc[i]))
            return 0;
    }
    return 1;
}
int isValidEmail(const char *email) {
    int atFound = 0, dotFound = 0;

    if (email == NULL || strlen(email) < 5)
        return 0;

    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == ' ')
            return 0;

        if (email[i] == '@')
            atFound = 1;

        if (atFound && email[i] == '.')
            dotFound = 1;
    }

    return atFound && dotFound;
}
int isValidInteger(const char *str) {
    if (str == NULL || strlen(str) == 0)
        return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}
int isValidFloat(const char *str) {
    int dotCount = 0;

    if (str == NULL || strlen(str) == 0)
        return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dotCount++;
            if (dotCount > 1)
                return 0;
        }
        else if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}
