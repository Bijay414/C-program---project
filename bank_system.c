#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define the user structure
struct user {
    int phone;
    char account[100];
    char password[100];
    float balance;
};

// Function to generate a random account number
void generateAccountNumber(char *account) {
    for (int i = 0; i < 10; i++) {
        account[i] = '0' + rand() % 10;
    }
    account[10] = '\0';
}

// Clear input buffer
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    struct user us, t;
    int ch, flag = 0; 
    char password[100], accountno[100];
    int phonenumber, choice;
    float amount;
    FILE *fp, *tempFile;

    srand(time(0)); // Seed random number generator

    printf("What do you want to do?\n");
    printf("1. Register An Account\n");
    printf("2. Log In An Account\n");
    printf("Your choice: ");
    if (scanf("%d", &ch) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return 1;
    }
    clearInputBuffer();

    if (ch == 1) {
        // Register a new account
        fp = fopen("account.txt", "a"); // Open file in append mode
        if (fp == NULL) {
            printf("Error opening file!\n");
            return 1;
        }
        generateAccountNumber(us.account); // Generate random account number
        printf("Your generated account number is: %s\n", us.account);
        printf("Enter your phone number: ");
        if (scanf("%d", &us.phone) != 1) {
            printf("Invalid input!\n");
            clearInputBuffer();
            fclose(fp);
            return 1;
        }
        clearInputBuffer();
        printf("Enter your new password: ");
        if (scanf("%99s", us.password) != 1) {
            printf("Invalid input!\n");
            clearInputBuffer();
            fclose(fp);
            return 1;
        }
        clearInputBuffer();
        us.balance = 0; // Initialize balance to 0
        fprintf(fp, "%s\t%d\t%s\t%.2f\n", us.account, us.phone, us.password, us.balance); // Write account details to file
        fclose(fp); // Close file
        printf("Your account has been successfully created.\n");
    } 
    else if (ch == 2) {
        // Log in to an existing account
        fp = fopen("account.txt", "r"); // Open file in read mode
        tempFile = fopen("temp.txt", "w"); // Open temporary file in write mode
        if (fp == NULL || tempFile == NULL) {
            printf("Error opening file!\n");
            if (fp != NULL) fclose(fp);
            if (tempFile != NULL) fclose(tempFile);
            return 1;
        }
        printf("Enter your phone number: ");
        if (scanf("%d", &phonenumber) != 1) {
            printf("Invalid input!\n");
            clearInputBuffer();
            fclose(fp);
            fclose(tempFile);
            return 1;
        }
        clearInputBuffer();
        printf("Enter your password: ");
        if (scanf("%99s", password) != 1) {
            printf("Invalid input!\n");
            clearInputBuffer();
            fclose(fp);
            fclose(tempFile);
            return 1;
        }
        clearInputBuffer();

        // Read account details from file
        while (fscanf(fp, "%99s%d%99s%f", us.account, &us.phone, us.password, &us.balance) != EOF) {
            if (us.phone == phonenumber && strcmp(password, us.password) == 0) {
                // If phone number and password match
                printf("\t\tCUSTOMER ACCOUNT BANKING MANAGEMENT SYSTEM\n");
                printf("1. Check Balance\n");
                printf("2. Deposit An Amount\n");
                printf("3. Withdraw\n");
                printf("4. Transfer Balance\n");
                printf("5. Change Password\n");
                printf("Your Choice: ");
                if (scanf("%d", &choice) != 1) {
                    printf("Invalid input!\n");
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();
                switch (choice) {
                    case 1:
                        printf("Your current balance is %.2f\n", us.balance);
                        break;
                    case 2:
                        printf("Enter the amount for deposit: ");
                        if (scanf("%f", &amount) != 1) {
                            printf("Invalid input!\n");
                            clearInputBuffer();
                            break;
                        }
                        clearInputBuffer();
                        us.balance += amount;
                        printf("Amount deposited successfully.\n");
                        break;
                    case 3:
                        printf("Enter the amount for withdrawal: ");
                        if (scanf("%f", &amount) != 1) {
                            printf("Invalid input!\n");
                            clearInputBuffer();
                            break;
                        }
                        clearInputBuffer();
                        if (amount > us.balance) {
                            printf("Insufficient balance!\n");
                        } else {
                            us.balance -= amount;
                            printf("You have withdrawn Rs. %.2f\n", amount);
                        }
                        break;
                    case 4:
                        // Transfer balance
                        printf("Enter account number to transfer amount: ");
                        if (scanf("%99s", accountno) != 1) {
                            printf("Invalid input!\n");
                            clearInputBuffer();
                            break;
                        }
                        clearInputBuffer();
                        rewind(fp); // Reset file pointer to the beginning
                        while (fscanf(fp, "%99s%d%99s%f", t.account, &t.phone, t.password, &t.balance) != EOF) {
                            if (strcmp(accountno, t.account) == 0) {
                                printf("Enter amount to transfer: ");
                                if (scanf("%f", &amount) != 1) {
                                    printf("Invalid input!\n");
                                    clearInputBuffer();
                                    break;
                                }
                                clearInputBuffer();
                                if (amount > us.balance) {
                                    printf("Insufficient balance!\n");
                                } else {
                                    t.balance += amount;
                                    us.balance -= amount;
                                    printf("Transfer successful.\n");
                                }
                            }
                            fprintf(tempFile, "%s\t%d\t%s\t%.2f\n", t.account, t.phone, t.password, t.balance); // Write updated account details to temporary file
                        }
                        break;
                    case 5:
                        // Change password
                        printf("Enter your new password: ");
                        if (scanf("%99s", password) != 1) {
                            printf("Invalid input!\n");
                            clearInputBuffer();
                            break;
                        }
                        clearInputBuffer();
                        strcpy(us.password, password);
                        printf("Password changed successfully.\n");
                        break;
                    default:
                        printf("Invalid choice.\n");
                }
                flag = 1; // Set flag to indicate successful login
            }
            fprintf(tempFile, "%s\t%d\t%s\t%.2f\n", us.account, us.phone, us.password, us.balance); // Write account details to temporary file
        }
        fclose(fp);
        fclose(tempFile); 
        remove("account.txt"); 
        rename("temp.txt", "account.txt"); 

        if (flag == 0) {
            printf("Wrong Password or Account Not Found!\n");
        }
    }
    return 0;
}