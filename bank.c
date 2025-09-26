
// Linux-friendly Banking System in C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// Function prototypes
void checkbalance(char*);
void transfermoney(void);
void display(char*);
void login(void);
void account(void);
void accountcreated(void);
void logout(void);

// Function to get a single character without enter (Linux)
char getch_linux() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON; // disable buffering
    old.c_lflag &= ~ECHO;   // disable echo
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0) perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
    return buf;
}

// Structures
struct pass {
    char username[50];
    int date, month, year;
    char pnumber[15];
    char adharnum[20];
    char fname[20];
    char lname[20];
    char fathname[20];
    char mothname[20];
    char address[50];
    char typeaccount[20];
};

struct money {
    char usernameto[50];
    char userpersonfrom[50];
    long int money1;
};

// Main
int main() {
    int choice;

    printf("WELCOME TO BANK ACCOUNT SYSTEM\n\n");
    printf("**********************************\n");
    printf("DEVELOPER - Naman Kumar\n\n");

    printf("1.... CREATE A BANK ACCOUNT\n");
    printf("2.... ALREADY A USER? SIGN IN\n");
    printf("3.... EXIT\n\n");

    printf("ENTER YOUR CHOICE: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            system("clear");
            account();
            break;
        case 2:
            system("clear");
            login();
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice.\n");
    }

    return 0;
}

// Account creation
void account(void) {
    struct pass u1;
    char password[50];
    int i;

    FILE *fp = fopen("username.txt", "ab");
    if (!fp) {
        fp = fopen("username.txt", "wb");
        if (!fp) { printf("Cannot create user file\n"); return; }
    }

    system("clear");
    printf("!!!!! CREATE ACCOUNT !!!!!\n\n");

    printf("FIRST NAME: "); scanf("%s", u1.fname);
    printf("LAST NAME: "); scanf("%s", u1.lname);
    printf("FATHER'S NAME: "); scanf("%s", u1.fathname);
    printf("MOTHER'S NAME: "); scanf("%s", u1.mothname);
    printf("ADDRESS: "); scanf("%s", u1.address);
    printf("ACCOUNT TYPE: "); scanf("%s", u1.typeaccount);
    printf("DATE OF BIRTH (DD MM YYYY): "); scanf("%d %d %d", &u1.date, &u1.month, &u1.year);
    printf("ADHAR NUMBER: "); scanf("%s", u1.adharnum);
    printf("PHONE NUMBER: "); scanf("%s", u1.pnumber);
    printf("USERNAME: "); scanf("%s", u1.username);

    printf("PASSWORD: ");
    for(i=0;i<50;i++){
        char ch = getch_linux();
        if(ch == '\n') break;
        password[i] = ch;
        printf("*");
    }
    printf("\n");

    fwrite(&u1, sizeof(u1), 1, fp);
    fclose(fp);

    accountcreated();
}

void accountcreated(void){
    printf("\nACCOUNT CREATED SUCCESSFULLY. Press enter to login...");
    getchar(); getchar(); // wait for enter
    login();
}

// Login
void login(void){
    struct pass u1;
    char username[50], password[50];
    int found = 0;
    
    FILE *fp = fopen("username.txt", "rb");
    if(!fp){
        fp = fopen("username.txt", "wb"); // create file if missing
        if(!fp){ printf("Cannot create user file\n"); return; }
        fclose(fp);
        fp = fopen("username.txt", "rb");
    }

    printf("USERNAME: "); scanf("%s", username);
    printf("PASSWORD: ");
    for(int i=0;i<50;i++){
        char ch = getch_linux();
        if(ch=='\n') break;
        password[i] = ch;
        printf("*");
    }
    printf("\n");

    while(fread(&u1,sizeof(u1),1,fp)){
        if(strcmp(u1.username, username)==0){
            found = 1;
            break;
        }
    }
    fclose(fp);

    if(found){
        printf("LOGIN SUCCESSFUL!\nPress enter to continue...");
        getchar(); getchar();
        display(username);
    } else {
        printf("USERNAME NOT FOUND.\n");
    }
}

// Display menu
void display(char username1[]){
    int choice;
    do{
        system("clear");
        printf("HOME - %s\n", username1);
        printf("1.... CHECK BALANCE\n");
        printf("2.... TRANSFER MONEY\n");
        printf("3.... LOG OUT\n");
        printf("4.... EXIT\n");
        printf("ENTER YOUR CHOICE: "); scanf("%d", &choice);

        switch(choice){
            case 1: checkbalance(username1); break;
            case 2: transfermoney(); break;
            case 3: logout(); login(); return;
            case 4: exit(0);
            default: printf("Invalid choice\n");
        }
    }while(1);
}

// Transfer money
void transfermoney(void){
    struct money m1;
    struct pass u1;
    char usernamet[50], usernamep[50];
    int found = 0;

    FILE *fp = fopen("username.txt", "rb");
    if(!fp){
        printf("User file missing\n"); return;
    }

    FILE *fm = fopen("mon.txt", "ab");
    if(!fm){
        fm = fopen("mon.txt", "wb");
        if(!fm){ printf("Cannot create money file\n"); fclose(fp); return; }
    }

    printf("FROM (your username): "); scanf("%s", usernamet);
    printf("TO (recipient username): "); scanf("%s", usernamep);

    while(fread(&u1,sizeof(u1),1,fp)){
        if(strcmp(usernamep,u1.username)==0){
            found = 1;
            strcpy(m1.usernameto, u1.username);
            strcpy(m1.userpersonfrom, usernamet);
            break;
        }
    }

    if(!found){ printf("Recipient not found\n"); fclose(fp); fclose(fm); return; }

    printf("ENTER AMOUNT TO TRANSFER: "); scanf("%ld", &m1.money1);
    fwrite(&m1,sizeof(m1),1,fm);

    fclose(fp);
    fclose(fm);

    printf("Amount transferred successfully!\nPress enter...");
    getchar(); getchar();
}

// Check balance
void checkbalance(char username2[]){
    struct money m1;
    long sum = 0;
    int i=1;

    FILE *fm = fopen("mon.txt", "rb");
    if(!fm){ fm = fopen("mon.txt", "wb"); if(!fm){ printf("Cannot create money file\n"); return; } fclose(fm); fm=fopen("mon.txt","rb"); }

    printf("==== BALANCE DASHBOARD ====\n");
    printf("S.No\tFrom\tAmount\n");

    while(fread(&m1,sizeof(m1),1,fm)){
        if(strcmp(m1.usernameto,username2)==0){
            printf("%d\t%s\t%ld\n", i++, m1.userpersonfrom, m1.money1);
            sum += m1.money1;
        }
    }

    printf("TOTAL AMOUNT: %ld\n", sum);
    fclose(fm);

    printf("Press enter to return...");
    getchar(); getchar();
}

// Logout
void logout(void){
    printf("Logging out... Press enter to continue.");
    getchar(); getchar();
}

